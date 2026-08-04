# Interpreter.py

```python
import math, time, random, threading
from nodes import *
from lexer import Lexer
from parser import Parser

# ── 제어 흐름 ─────────────────────────────────────────
class Break(Exception): pass
class Return(Exception):
    def __init__(self, v, stop=True): self.v = v; self.stop = stop
class LmnopError(Exception):
    def __init__(self, m): super().__init__(f'오류: {m}')

# ── 내장 타입 ─────────────────────────────────────────
class LList:
    def __init__(self, e): self.e = list(e)
    def __repr__(self): return '[' + ', '.join(lrepr(x) for x in self.e) + ']'

class LDict:
    def __init__(self, p): self.d = dict(p)
    def __repr__(self): return '{' + ', '.join(f'{k}:{lrepr(v)}' for k,v in self.d.items()) + '}'

class LFunc:
    def __init__(self, name, params, body, has_get, env):
        self.name=name; self.params=params; self.body=body
        self.has_get=has_get; self.env=env; self.always={}
    def __repr__(self): return f'<함수 {self.name}>'

class LSled:
    def __init__(self, t, name): self.t=t; self.name=name; self._stop=threading.Event(); self.unerror=False
    def Stop(self): self._stop.set()
    def Play(self): self._stop.clear()
    def stopped(self): return self._stop.is_set()
    def __repr__(self): return f'<스레드 {self.name}>'

def lrepr(v):
    if v is None: return 'null'
    if isinstance(v, bool): return 'true' if v else 'false'
    if isinstance(v, float) and v == int(v): return str(int(v))
    return str(v)

def truthy(v):
    if v is None: return False
    if isinstance(v, bool): return v
    if isinstance(v, (int,float)): return v != 0
    if isinstance(v, str): return len(v) > 0
    return True

# ── When 컨텍스트 ─────────────────────────────────────
class When:
    def __init__(self): self.n=0; self.total=None; self.start=None; self.end=None; self.step=None; self.elapsed=0.0; self.cur=None

# ── 환경 ──────────────────────────────────────────────
class Env:
    def __init__(self, p=None): self.v={}; self.p=p
    def get(self, k):
        if k in self.v: return self.v[k]
        if self.p: return self.p.get(k)
        raise LmnopError(f'정의되지 않은 변수: {k}')
    def set(self, k, v):
        if k in self.v: self.v[k]=v; return
        if self.p and self.p.has(k): self.p.set(k,v); return
        self.v[k]=v
    def define(self, k, v): self.v[k]=v
    def has(self, k): return k in self.v or (bool(self.p) and self.p.has(k))

# ── 인터프리터 ────────────────────────────────────────
class Interpreter:
    def __init__(self):
        self.g = Env()
        self.when = []
        self.g.define('printer', '__printer__')
        self.g.define('When', '__When__')

    def run(self, src):
        from lexer import Lexer
        from parser import Parser
        toks = Lexer(src).tokenize()
        ast = Parser(toks).parse()
        env = Env(self.g)
        try: self.body(ast.body, env)
        except Break: pass
        except Return: pass

    def body(self, stmts, env):
        for s in stmts:
            if s is not None: self.stmt(s, env)

    def stmt(self, s, env):
        if isinstance(s, ImportStmt):   self.do_import(s, env)
        elif isinstance(s, VarDecl):
            v = self.ev(s.val, env)
            if s.typ: v = self.typecheck(v, s.typ)
            if s.kind == 'local': env.define(s.name, v)
            else: env.set(s.name, v)
        elif isinstance(s, Assign):     self.do_assign(s.target, self.ev(s.val, env), env)
        elif isinstance(s, MultiAssign):
            v = self.ev(s.val, env)
            vals = v if isinstance(v, (list,tuple)) else [v]
            for t,val in zip(s.targets, vals): self.do_assign(t, val, env)
        elif isinstance(s, FuncDecl):
            fn = LFunc(s.name, s.params, s.body, s.has_get, env)
            env.set(s.name, fn)
        elif isinstance(s, IfStmt):     self.do_if(s, env)
        elif isinstance(s, WhileStmt):  self.do_while(s, env)
        elif isinstance(s, ForStmt):    self.do_for(s, env)
        elif isinstance(s, ForInStmt):  self.do_forin(s, env)
        elif isinstance(s, RepeatStmt): self.do_repeat(s, env)
        elif isinstance(s, TimeStmt):   self.do_time(s, env)
        elif isinstance(s, CooldownStmt): self.do_cooldown(s, env)
        elif isinstance(s, MaybeStmt):  self.do_maybe(s, env)
        elif isinstance(s, WaitStmt):   self.do_wait(s, env)
        elif isinstance(s, ReturnStmt):
            vals = [self.ev(v, env) for v in s.vals]
            raise Return(vals[0] if len(vals)==1 else vals, stop=True)
        elif isinstance(s, UnReturnStmt):
            vals = [self.ev(v, env) for v in s.vals]
            raise Return(vals[0] if len(vals)==1 else vals, stop=False)
        elif isinstance(s, BreakStmt):  raise Break()
        elif isinstance(s, ErrorStmt):
            msg = self.ev(s.msg, env)
            stop = self.ev(s.stop, env) if not isinstance(s.stop, bool) else s.stop
            if stop: raise LmnopError(str(msg))
            else: print(f'[오류] {msg}')
        else: self.ev(s, env)

    def do_import(self, s, env):
        n = s.name
        svc = None
        if n == 'MathService':
            svc = {'log':math.log,'sin':math.sin,'cos':math.cos,'tan':math.tan,
                   'asin':math.asin,'acos':math.acos,'atan':math.atan,
                   'abs':abs,'floor':math.floor,'ceil':math.ceil,'round':round,
                   'max':max,'min':min,
                   'random':lambda a,b:random.randint(int(a),int(b))}
        elif n == 'ValueService':  svc = {'Change':'__VC__'}
        elif n == 'InputService':  svc = {'Input':'__Input__','KeyDown':'__KD__','KeyUp':'__KU__','Click':'__Click__'}
        elif n == 'TimeService':   svc = {'time':'__time__','tick':'__tick__'}
        elif n == 'NetworkService':svc = {'__type__':'Network','__events__':{}}
        if svc: env.set(n, svc)

    def typecheck(self, v, t):
        m = {'int':(int,float),'float':float,'str':str,'bool':bool}
        e = m.get(t)
        if e and not isinstance(v, e): raise LmnopError(f'타입 오류: {t} 필요, {type(v).__name__} 발견')
        if t == 'int' and isinstance(v, float): return int(v)
        return v

    def do_assign(self, target, val, env):
        if isinstance(target, Id): env.set(target.name, val)
        elif isinstance(target, Attr):
            obj = self.ev(target.obj, env)
            if isinstance(obj, LDict): obj.d[target.attr] = val
            elif isinstance(obj, dict): obj[target.attr] = val
        elif isinstance(target, Idx):
            obj = self.ev(target.obj, env)
            idx = self.ev(target.idx, env)
            if isinstance(obj, LList): obj.e[int(idx)] = val
            elif isinstance(obj, LDict): obj.d[idx] = val

    def iv(self, ivars, local, env):
        for name, expr in ivars: local.define(name, self.ev(expr, env))

    def do_if(self, s, env):
        for cond, ivars, body in s.branches:
            if truthy(self.ev(cond, env)):
                loc = Env(env); self.iv(ivars, loc, env)
                self.body(body, loc); return
        if s.else_body is not None: self.body(s.else_body, Env(env))

    def do_while(self, s, env):
        w = When(); self.when.append(w)
        try:
            while truthy(self.ev(s.cond, env)):
                w.n += 1; loc = Env(env); self.iv(s.iv, loc, env)
                try: self.body(s.body, loc)
                except Break: break
        finally: self.when.pop()

    def do_for(self, s, env):
        w = When(); self.when.append(w)
        try:
            if isinstance(s.spec, tuple):
                st, en, sp = [self.ev(x, env) for x in s.spec]
                w.start=st; w.end=en; w.step=sp; i=st
                while (sp>0 and i<=en) or (sp<0 and i>=en):
                    w.n+=1; loc=Env(env); self.iv(s.iv,loc,env)
                    try: self.body(s.body, loc)
                    except Break: break
                    i+=sp
            else:
                cnt = int(self.ev(s.spec, env)); w.total=cnt
                for i in range(1, cnt+1):
                    w.n=i; loc=Env(env); self.iv(s.iv,loc,env)
                    try: self.body(s.body, loc)
                    except Break: break
        finally: self.when.pop()

    def do_forin(self, s, env):
        it = self.ev(s.it, env)
        items = it.e if isinstance(it, LList) else list(it)
        w = When(); w.total=len(items); self.when.append(w)
        try:
            for i, item in enumerate(items):
                w.n=i+1; w.cur=item
                loc=Env(env); loc.define(s.var, item); self.iv(s.iv,loc,env)
                try: self.body(s.body, loc)
                except Break: break
        finally: self.when.pop()

    def do_repeat(self, s, env):
        w = When(); self.when.append(w)
        try:
            while True:
                w.n+=1; loc=Env(env); self.iv(s.iv,loc,env)
                try: self.body(s.body, loc)
                except Break: break
                if not truthy(self.ev(s.cond, env)): break
        finally: self.when.pop()

    def do_time(self, s, env):
        dur = float(self.ev(s.dur, env)); rate = float(self.ev(s.rate, env))
        interval = 1.0/rate if rate>0 else 0.1; t0=time.time()
        w = When(); self.when.append(w)
        try:
            while True:
                el = time.time()-t0
                if el >= dur: break
                w.n+=1; w.elapsed=el; loc=Env(env); self.iv(s.iv,loc,env)
                try: self.body(s.body, loc)
                except Break: break
                time.sleep(interval)
        finally: self.when.pop()

    def do_cooldown(self, s, env):
        if not truthy(self.ev(s.cond, env)): return
        loc=Env(env); self.iv(s.iv,loc,env)
        try: self.body(s.body, loc)
        except Break: pass

    def do_maybe(self, s, env):
        raw = self.ev(s.cond, env)
        ok = raw[0] if isinstance(raw,tuple) else truthy(raw)
        pv = raw[1] if isinstance(raw,tuple) else raw
        if not ok: return
        loc=Env(env)
        if s.param: loc.define(s.param, pv)
        self.iv(s.iv, loc, env)
        try: self.body(s.body, loc)
        except Break: raise

    def do_wait(self, s, env):
        while True:
            v = self.ev(s.cond, env)
            if (v[0] if isinstance(v,tuple) else truthy(v)): break
            time.sleep(0.05)

    # ── 표현식 평가 ───────────────────────────────────
    def ev(self, n, env):
        if isinstance(n, Num):    return n.v
        if isinstance(n, Str):    return n.v
        if isinstance(n, Bool):   return n.v
        if isinstance(n, Null):   return None
        if isinstance(n, GetNull):return None
        if isinstance(n, GetPi):  return math.pi
        if isinstance(n, GetE):   return math.e
        if isinstance(n, FStr):   return self.fstr(n, env)
        if isinstance(n, ListLit):return LList([self.ev(e,env) for e in n.elems])
        if isinstance(n, DictLit):return LDict([(k,self.ev(v,env)) for k,v in n.pairs])
        if isinstance(n, Id):     return env.get(n.name)

        if isinstance(n, Attr):
            obj = self.ev(n.obj, env)
            return self.getattr_(obj, n.attr, env)

        if isinstance(n, Idx):
            obj=self.ev(n.obj,env); idx=self.ev(n.idx,env)
            if isinstance(obj,LList): return obj.e[int(idx)]
            if isinstance(obj,LDict): return obj.d[idx]
            raise LmnopError('인덱싱 불가')

        if isinstance(n, Call):
            # When() 특수처리
            if isinstance(n.fn, Id) and n.fn.name == 'When':
                return self.when[-1].n if self.when else 0
            fn = self.ev(n.fn, env)
            args = [self.ev(a,env) if not isinstance(a,AnonFunc)
                    else LFunc('<익명>',a.params,a.body,a.has_get,env)
                    for a in n.args]
            return self.call(fn, args, env)

        if isinstance(n, BinOp):  return self.binop(n, env)
        if isinstance(n, UnOp):   return self.unop(n, env)
        if isinstance(n, Concat):
            return lrepr(self.ev(n.l,env)) + lrepr(self.ev(n.r,env))
        if isinstance(n, Pipe):
            v=self.ev(n.v,env)
            for fn in n.fns: v=self.call(self.ev(fn,env),[v],env)
            return v
        if isinstance(n, EventExpr):
            v=self.ev(n.cond,env); return (truthy(v), v)
        if isinstance(n, AnonFunc):
            return LFunc('<익명>',n.params,n.body,n.has_get,env)
        if isinstance(n, MethodCall):
            return self.method(n, env)
        raise LmnopError(f'알 수 없는 노드: {type(n).__name__}')

    def fstr(self, n, env):
        res = ''
        for kind, part in n.parts:
            if kind == 's': res += part
            else:
                toks = Lexer(part).tokenize()
                expr = Parser(toks).expr()
                res += lrepr(self.ev(expr, env))
        return res

    def binop(self, n, env):
        op = n.op
        if op == 'and':
            l=self.ev(n.l,env); return l if not truthy(l) else self.ev(n.r,env)
        if op == 'or':
            l=self.ev(n.l,env); return l if truthy(l) else self.ev(n.r,env)
        l=self.ev(n.l,env); r=self.ev(n.r,env)
        if op=='+': return l+r
        if op=='-': return l-r
        if op=='*': return l*r
        if op=='/': return l/r
        if op=='%': return l%r
        if op=='^': return l**r
        if op=='==': return l==r
        if op=='/=': return l!=r
        if op=='<':  return l<r
        if op=='>':  return l>r
        if op=='<=': return l<=r
        if op=='>=': return l>=r
        raise LmnopError(f'알 수 없는 연산자: {op}')

    def unop(self, n, env):
        v=self.ev(n.v,env)
        if n.op=='-': return -v
        if n.op=='~': return math.sqrt(v)
        if n.op=='not': return not truthy(v)

    def getattr_(self, obj, attr, env):
        if obj == '__printer__': return ('__printer__', attr)
        if obj == '__When__':
            w = self.when[-1] if self.when else When()
            m = {'left': lambda: (w.total-w.n if w.total else None),
                 'time': lambda: w.elapsed,
                 'start': lambda: w.start,
                 'end': lambda: w.end,
                 'step': lambda: w.step,
                 'value': lambda: w.cur}
            if attr in m: return m[attr]
        if isinstance(obj, dict):
            if attr in obj: return obj[attr]
            raise LmnopError(f'서비스 속성 없음: {attr}')
        if isinstance(obj, LList):
            m={'add':obj.e.append,'remove':lambda i:obj.e.pop(int(i)),
               'get':lambda i:obj.e[int(i)],'len':lambda:len(obj.e),
               'has':lambda v:v in obj.e,'clear':obj.e.clear}
            if attr in m: return m[attr]
        if isinstance(obj, LDict):
            m={'get':lambda k:obj.d.get(k),'set':lambda k,v:obj.d.update({k:v}),
               'has':lambda k:k in obj.d,'remove':lambda k:obj.d.pop(k,None),
               'keys':lambda:LList(list(obj.d.keys()))}
            if attr in m: return m[attr]
        if isinstance(obj, LSled):
            if attr=='Stop': obj.Stop(); return None
            if attr=='Play': obj.Play(); return None
            if attr=='UnError': obj.unerror=True; return None
        if isinstance(obj, str) and obj == '__Get__':
            return ('__Get__', attr)
        raise LmnopError(f'속성 없음: {type(obj).__name__}.{attr}')

    def call(self, fn, args, env):
        if callable(fn): return fn(*args)
        if isinstance(fn, LFunc): return self.callfn(fn, args)
        if isinstance(fn, tuple) and fn[0]=='__printer__': return self.printer(fn[1], args)
        raise LmnopError(f'호출 불가: {type(fn).__name__}')

    def printer(self, m, args):
        if m == 'Print': print(lrepr(args[0]) if args else '')
        elif m == 'Clear': print('\033[2J\033[H', end='')
        return None

    def callfn(self, fn, args):
        loc = Env(fn.env)
        for i,p in enumerate(fn.params): loc.define(p, args[i] if i<len(args) else None)
        if 'start' in fn.always:
            try: self.body(fn.always['start'], Env(loc))
            except: pass
        try:
            self.body(fn.body, loc)
        except Return as r:
            if 'return' in fn.always:
                try: self.body(fn.always['return'], Env(loc))
                except: pass
            return r.v
        except LmnopError:
            if 'error' in fn.always:
                try: self.body(fn.always['error'], Env(loc))
                except: pass
            raise
        except Break: pass
        if 'end' in fn.always:
            try: self.body(fn.always['end'], Env(loc))
            except: pass
        return None

    def method(self, n, env):
        m = n.method

        # Get.함수:Play 등
        if isinstance(n.obj, Attr) and isinstance(n.obj.obj, Id) and n.obj.obj.name == '__Get__':
            fn_name = n.obj.attr
            if fn_name == 'Sled':
                args = [self.ev(a,env) for a in n.args]
                sl = args[0] if args else None
                sled = sl if isinstance(sl, LSled) else LSled(threading.current_thread(), 'main')
                if m=='Stop': sled.Stop(); return None
                if m=='Play': sled.Play(); return None
                if m=='UnError': sled.unerror=True; return None
                return sled
            fn = env.get(fn_name) if env.has(fn_name) else None
            if fn is None: raise LmnopError(f'함수 없음: {fn_name}')
            return self.dispatch(fn, m, n.args, env)

        obj = self.ev(n.obj, env)

        # printer
        if obj == '__printer__':
            args=[self.ev(a,env) for a in n.args]
            return self.printer(m, args)

        # 서비스 dict
        if isinstance(obj, dict):
            fn = obj.get(m)
            if callable(fn):
                args=[self.ev(a,env) for a in n.args]
                return fn(*args)

        # LList / LDict
        if isinstance(obj, (LList, LDict)):
            fn = self.getattr_(obj, m, env)
            if callable(fn):
                args=[self.ev(a,env) for a in n.args]
                return fn(*args)

        # Always 등록
        if m.startswith('Always.'):
            event = m[len('Always.'):]
            if isinstance(obj, LFunc) and n.args:
                a = n.args[0]
                obj.always[event] = a.body if isinstance(a, AnonFunc) else []
            return None

        # LSled
        if isinstance(obj, LSled):
            if m=='Stop': obj.Stop(); return None
            if m=='Play': obj.Play(); return None
            if m=='UnError': obj.unerror=True; return None

        # When.left() 등 callable
        if callable(obj): return obj()

        # LFunc
        if isinstance(obj, LFunc):
            return self.dispatch(obj, m, n.args, env)

        raise LmnopError(f'메서드 호출 불가: {type(obj).__name__}:{m}')

    def dispatch(self, fn, m, arg_nodes, env):
        def ev_args():
            return [self.ev(a,env) if not isinstance(a,AnonFunc)
                    else LFunc('<익명>',a.params,a.body,a.has_get,env)
                    for a in arg_nodes]
        if m == 'Play':     return self.callfn(fn, ev_args())
        if m == 'Run':      return self.callfn(fn, [])
        if m == 'Stop':     return None
        if m == 'SledPlay': return self.sledplay(fn, ev_args())
        raise LmnopError(f'알 수 없는 메서드: {m}')

    def sledplay(self, fn, args):
        sl = LSled(None, fn.name)
        def run():
            try: self.callfn(fn, args)
            except Exception as e:
                if not sl.unerror: print(f'[스레드 오류] {e}')
        t = threading.Thread(target=run, daemon=True); t.start()
        sl.t = t; return sl
```

# Lexer.py

```python
import re

# 토큰 타입
TK = type('TK', (), {k: k for k in [
    'NUM', 'STR', 'FSTR', 'BOOL', 'NULL', 'ID',
    'VALUE', 'LOCALVALUE', 'FUNCTION', 'GET', 'RETURN', 'UNRETURN',
    'IF', 'EIF', 'ELSE', 'END', 'WHILE', 'FOR', 'REPEAT', 'TIME',
    'COOLDOWN', 'BREAK', 'IMPORT', 'LIST', 'DICT',
    'MAYBE', 'EVENT', 'WAIT', 'IN', 'THEN', 'AND', 'OR', 'NOT', 'ERROR',
    'PLUS', 'MINUS', 'STAR', 'SLASH', 'PCT', 'CARET', 'TILDE',
    'PIPE', 'CONCAT', 'EQ', 'NEQ', 'LT', 'GT', 'LTE', 'GTE',
    'ASSIGN', 'DOTDOT', 'DOT', 'COMMA', 'COLON', 'SEMI',
    'LP', 'RP', 'LB', 'RB', 'LC', 'RC', 'DLP',
    'NL', 'EOF',
]})()

KEYWORDS = {
    'true': TK.BOOL, 'false': TK.BOOL, 'null': TK.NULL,
    'Value': TK.VALUE, 'LocalValue': TK.LOCALVALUE,
    'Function': TK.FUNCTION, 'Get': TK.GET,
    'Return': TK.RETURN, 'UnReturn': TK.UNRETURN,
    'if': TK.IF, 'eif': TK.EIF, 'else': TK.ELSE, 'end': TK.END,
    'While': TK.WHILE, 'For': TK.FOR, 'Repeat': TK.REPEAT,
    'Time': TK.TIME, 'Cooldown': TK.COOLDOWN,
    'break': TK.BREAK, 'import': TK.IMPORT,
    'List': TK.LIST, 'Dict': TK.DICT,
    'Maybe': TK.MAYBE, 'Event': TK.EVENT, 'Wait': TK.WAIT,
    'in': TK.IN, 'then': TK.THEN,
    'and': TK.AND, 'or': TK.OR, 'not': TK.NOT,
    'Error': TK.ERROR,
}

class Tok:
    def __init__(self, t, v, ln): self.t = t; self.v = v; self.ln = ln
    def __repr__(self): return f'{self.t}({self.v!r})'

class LexError(Exception):
    def __init__(self, msg, ln): super().__init__(f'[줄 {ln}] 렉서 오류: {msg}')

class Lexer:
    def __init__(self, src):
        self.s = src; self.i = 0; self.ln = 1

    def c(self): return self.s[self.i] if self.i < len(self.s) else ''
    def p(self, n=1): return self.s[self.i+n] if self.i+n < len(self.s) else ''
    def adv(self):
        ch = self.s[self.i]; self.i += 1
        if ch == '\n': self.ln += 1
        return ch

    def tokenize(self):
        toks = []
        while self.i < len(self.s):
            # 공백 스킵 (개행 제외)
            while self.c() in ' \t\r': self.adv()
            if self.i >= len(self.s): break
            ch = self.c(); ln = self.ln

            # 개행
            if ch == '\n':
                self.adv()
                if not toks or toks[-1].t != TK.NL:
                    toks.append(Tok(TK.NL, '\n', ln))
                continue

            # 블록 주석 (! ... !)
            if ch == '(' and self.p() == '!':
                self.adv(); self.adv()
                while self.i < len(self.s):
                    if self.c() == '!' and self.p() == ')':
                        self.adv(); self.adv(); break
                    self.adv()
                continue

            # 한 줄 주석 !
            if ch == '!':
                while self.i < len(self.s) and self.c() != '\n': self.adv()
                continue

            # 인라인 주석 /내용/ 또는 /= 또는 /
            if ch == '/':
                if self.p() == '=':
                    self.adv(); self.adv(); toks.append(Tok(TK.NEQ, '/=', ln)); continue
                # 인라인 주석 시도
                saved = self.i
                self.adv()
                buf = ''
                found = False
                while self.i < len(self.s) and self.c() != '\n':
                    if self.c() == '/': self.adv(); found = True; break
                    buf += self.adv()
                if found: continue
                self.i = saved; self.adv()
                toks.append(Tok(TK.SLASH, '/', ln)); continue

            # f-string
            if ch == 'f' and self.p() in ('"', "'"):
                toks.append(self._fstr(ln)); continue

            # 문자열
            if ch in ('"', "'"): toks.append(self._str(ch, ln)); continue

            # 숫자
            if ch.isdigit(): toks.append(self._num(ln)); continue

            # 식별자
            if ch.isalpha() or ch == '_': toks.append(self._id(ln)); continue

            # 두 글자
            two = ch + self.p()
            if two == '::': self.adv(); self.adv(); toks.append(Tok(TK.CONCAT, '::', ln)); continue
            if two == '>>': self.adv(); self.adv(); toks.append(Tok(TK.PIPE, '>>', ln)); continue
            if two == '==': self.adv(); self.adv(); toks.append(Tok(TK.EQ, '==', ln)); continue
            if two == '<=': self.adv(); self.adv(); toks.append(Tok(TK.LTE, '<=', ln)); continue
            if two == '>=': self.adv(); self.adv(); toks.append(Tok(TK.GTE, '>=', ln)); continue
            if two == '..': self.adv(); self.adv(); toks.append(Tok(TK.DOTDOT, '..', ln)); continue
            if two == '((': self.adv(); self.adv(); toks.append(Tok(TK.DLP, '((', ln)); continue

            # 한 글자
            m = {'+':TK.PLUS,'-':TK.MINUS,'*':TK.STAR,'%':TK.PCT,
                 '^':TK.CARET,'~':TK.TILDE,'=':TK.ASSIGN,
                 '<':TK.LT,'>':TK.GT,'.':TK.DOT,',':TK.COMMA,
                 ':':TK.COLON,';':TK.SEMI,
                 '(':TK.LP,')':TK.RP,'[':TK.LB,']':TK.RB,'{':TK.LC,'}':TK.RC}
            if ch in m: toks.append(Tok(m[ch], ch, ln)); self.adv(); continue

            raise LexError(f'모르는 문자: {ch!r}', ln)

        toks.append(Tok(TK.EOF, None, self.ln))
        return toks

    def _str(self, q, ln):
        self.adv(); s = ''
        while self.i < len(self.s):
            ch = self.c()
            if ch == q: self.adv(); return Tok(TK.STR, s, ln)
            if ch == '\\': self.adv(); e=self.adv(); s+=({'n':'\n','t':'\t','\\':'\\',q:q}).get(e,e)
            else: s += self.adv()
        raise LexError('문자열이 닫히지 않음', ln)

    def _fstr(self, ln):
        self.adv(); q = self.adv(); parts = []; s = ''
        while self.i < len(self.s):
            ch = self.c()
            if ch == q:
                self.adv()
                if s: parts.append(('s', s))
                return Tok(TK.FSTR, parts, ln)
            if ch == '{':
                self.adv()
                if s: parts.append(('s', s)); s = ''
                expr = ''; d = 1
                while self.i < len(self.s):
                    c = self.c()
                    if c == '{': d += 1
                    if c == '}': d -= 1
                    if d == 0: self.adv(); break
                    expr += self.adv()
                parts.append(('e', expr))
            else: s += self.adv()
        raise LexError('f-string이 닫히지 않음', ln)

    def _num(self, ln):
        s = ''
        while self.i < len(self.s) and (self.c().isdigit() or (self.c()=='.' and self.p()!='.')):
            s += self.adv()
        return Tok(TK.NUM, float(s) if '.' in s else int(s), ln)

    def _id(self, ln):
        s = ''
        while self.i < len(self.s) and (self.c().isalnum() or self.c() == '_'): s += self.adv()
        if s in KEYWORDS:
            v = (s == 'true') if s in ('true','false') else (None if s=='null' else s)
            return Tok(KEYWORDS[s], v, ln)
        return Tok(TK.ID, s, ln)
```

# lmnop.py

```python
#!/usr/bin/env python3
"""
lmnop 인터프리터
  python lmnop.py 파일.lmnop   파일 실행
  python lmnop.py              REPL 모드
"""
import sys
from interpreter import Interpreter, LmnopError
from lexer import LexError
from parser import ParseError

def run(path):
    with open(path, encoding='utf-8') as f: src = f.read()
    interp = Interpreter()
    try: interp.run(src)
    except (LexError, ParseError, LmnopError) as e: print(e); sys.exit(1)
    except Exception as e: print(f'내부 오류: {e}'); sys.exit(1)

def repl():
    print('lmnop REPL  (빈 줄=실행, exit=종료)')
    interp = Interpreter(); buf = []
    while True:
        try: line = input('... ' if buf else '>>> ')
        except (EOFError, KeyboardInterrupt): print(); break
        if line.strip() == 'exit': break
        if line.strip() == '' and buf:
            try: interp.run('\n'.join(buf))
            except (LexError, ParseError, LmnopError) as e: print(e)
            except Exception as e: print(f'내부 오류: {e}')
            buf = []
        else: buf.append(line)

if __name__ == '__main__':
    if len(sys.argv) >= 2: run(sys.argv[1])
    else: repl()
```

# Nodes.py

```python
# 모든 AST 노드를 간결하게 정의

def node(name, *fields):
    def __init__(self, *args):
        for f, a in zip(fields, args): setattr(self, f, a)
    def __repr__(self):
        vals = ', '.join(f'{f}={getattr(self,f)!r}' for f in fields)
        return f'{name}({vals})'
    return type(name, (), {'__init__': __init__, '__repr__': __repr__})

# 리터럴
Num     = node('Num', 'v')
Str     = node('Str', 'v')
Bool    = node('Bool', 'v')
Null    = node('Null')
FStr    = node('FStr', 'parts')
ListLit = node('ListLit', 'elems')
DictLit = node('DictLit', 'pairs')

# 식별자/접근
Id      = node('Id', 'name')
GetPi   = node('GetPi')
GetE    = node('GetE')
GetNull = node('GetNull')
Attr    = node('Attr', 'obj', 'attr')
Idx     = node('Idx', 'obj', 'idx')
Call    = node('Call', 'fn', 'args')

# 연산
BinOp   = node('BinOp', 'op', 'l', 'r')
UnOp    = node('UnOp', 'op', 'v')
Concat  = node('Concat', 'l', 'r')
Pipe    = node('Pipe', 'v', 'fns')

# 변수
VarDecl = node('VarDecl', 'kind', 'name', 'val', 'typ')
Assign  = node('Assign', 'target', 'val')
MultiAssign = node('MultiAssign', 'targets', 'val')

# 제어
IfStmt      = node('IfStmt', 'branches', 'else_body')
WhileStmt   = node('WhileStmt', 'cond', 'iv', 'body')
ForStmt     = node('ForStmt', 'spec', 'iv', 'body')
ForInStmt   = node('ForInStmt', 'it', 'var', 'iv', 'body')
RepeatStmt  = node('RepeatStmt', 'cond', 'iv', 'body')
TimeStmt    = node('TimeStmt', 'dur', 'rate', 'iv', 'body')
CooldownStmt= node('CooldownStmt', 'cond', 'iv', 'body')
MaybeStmt   = node('MaybeStmt', 'cond', 'param', 'iv', 'body')
WaitStmt    = node('WaitStmt', 'cond')
BreakStmt   = node('BreakStmt')

# 함수
FuncDecl    = node('FuncDecl', 'name', 'params', 'body', 'has_get')
AnonFunc    = node('AnonFunc', 'params', 'body', 'has_get')
ReturnStmt  = node('ReturnStmt', 'vals')
UnReturnStmt= node('UnReturnStmt', 'vals')
MethodCall  = node('MethodCall', 'obj', 'method', 'args')

# 기타
EventExpr   = node('EventExpr', 'cond')
ErrorStmt   = node('ErrorStmt', 'msg', 'stop')
ImportStmt  = node('ImportStmt', 'name')
Program     = node('Program', 'body')
```

# Parser.py

```python
from lexer import TK, Tok
from nodes import *

class ParseError(Exception):
    def __init__(self, msg, ln): super().__init__(f'[줄 {ln}] 파서 오류: {msg}')

class Parser:
    def __init__(self, toks):
        self.toks = toks; self.i = 0

    def c(self): return self.toks[self.i]
    def p(self, n=1): return self.toks[min(self.i+n, len(self.toks)-1)]
    def adv(self): t=self.toks[self.i]; self.i=min(self.i+1,len(self.toks)-1); return t
    def check(self, *ts): return self.c().t in ts
    def match(self, *ts):
        if self.check(*ts): return self.adv()
    def expect(self, t, msg=''):
        if self.c().t != t: raise ParseError(msg or f'{t} 필요, {self.c().t}({self.c().v!r}) 발견', self.c().ln)
        return self.adv()
    def skip(self):
        while self.check(TK.NL): self.adv()
    def eat(self):
        while self.check(TK.NL): self.adv()

    def parse(self):
        body = self.body(TK.EOF)
        return Program(body)

    def body(self, *stops):
        stmts = []
        while True:
            self.skip()
            if self.check(TK.EOF, *stops): break
            s = self.stmt()
            if s is not None: stmts.append(s)
        return stmts

    def stmt(self):
        t = self.c().t
        if t == TK.NL: self.adv(); return None
        if t == TK.IMPORT:     return self.s_import()
        if t == TK.VALUE:      return self.s_var('value')
        if t == TK.LOCALVALUE: return self.s_var('local')
        if t == TK.LIST:       return self.s_listdict('list')
        if t == TK.DICT:       return self.s_listdict('dict')
        if t == TK.FUNCTION:   return self.s_func()
        if t == TK.IF:         return self.s_if()
        if t == TK.WHILE:      return self.s_while()
        if t == TK.FOR:        return self.s_for()
        if t == TK.REPEAT:     return self.s_repeat()
        if t == TK.TIME:       return self.s_time()
        if t == TK.COOLDOWN:   return self.s_cooldown()
        if t == TK.MAYBE:      return self.s_maybe()
        if t == TK.WAIT:       return self.s_wait()
        if t == TK.RETURN:     return self.s_return()
        if t == TK.UNRETURN:   return self.s_unreturn()
        if t == TK.BREAK:      self.adv(); self.eat(); return BreakStmt()
        if t == TK.ERROR:      return self.s_error()
        return self.s_expr()

    def s_import(self):
        self.adv()
        name = self.expect(TK.ID, 'import 뒤 이름').v
        self.eat(); return ImportStmt(name)

    def s_var(self, kind):
        self.adv()
        self.expect(TK.COLON)
        name = self.expect(TK.ID, '변수 이름').v
        self.expect(TK.ASSIGN)
        val = self.expr()
        # 타입 지정: val<int> — _compare가 BinOp('<', val, Id('int'))로 파싱했을 경우 처리
        typ = None
        if isinstance(val, BinOp) and val.op == '<' and isinstance(val.r, Id) and self.check(TK.GT):
            self.adv(); typ = val.r.name; val = val.l
        self.eat(); return VarDecl(kind, name, val, typ)

    def s_listdict(self, kind):
        self.adv()
        self.expect(TK.COLON)
        name = self.expect(TK.ID).v
        self.expect(TK.ASSIGN)
        val = self.expr()
        self.eat(); return VarDecl('value', name, val, None)

    def s_func(self):
        self.adv()
        self.expect(TK.COLON)
        has_get = False
        if self.check(TK.GET): self.adv(); self.expect(TK.DOT); has_get = True
        name = self.expect(TK.ID, '함수 이름').v
        self.expect(TK.ASSIGN)
        self.expect(TK.LP)
        params = self.params()
        self.expect(TK.RP)
        self.eat()
        body = self.body(TK.END)
        self.expect(TK.END); self.eat()
        return FuncDecl(name, params, body, has_get)

    def params(self):
        ps = []
        while self.check(TK.ID): ps.append(self.adv().v); self.match(TK.COMMA)
        return ps

    def s_if(self):
        self.adv()
        cond = self.expr(); iv = self.inline_vars(); self.eat()
        body = self.body(TK.EIF, TK.ELSE, TK.END)
        branches = [(cond, iv, body)]
        while self.check(TK.EIF):
            self.adv()
            cond = self.expr(); iv = self.inline_vars(); self.eat()
            body = self.body(TK.EIF, TK.ELSE, TK.END)
            branches.append((cond, iv, body))
        else_body = None
        if self.match(TK.ELSE): self.eat(); else_body = self.body(TK.END)
        self.expect(TK.END); self.eat()
        return IfStmt(branches, else_body)

    def s_while(self):
        self.adv()
        cond = self.expr(); iv = self.inline_vars(); self.eat()
        body = self.body(TK.END)
        self.expect(TK.END); self.eat()
        return WhileStmt(cond, iv, body)

    def s_for(self):
        self.adv()
        # For 리스트 in 변수; 패턴 확인
        if self.check(TK.ID) and self.p().t == TK.IN:
            it = self.adv().v; self.adv()
            var = self.expect(TK.ID, '값 변수').v
            iv = self.inline_vars(); self.eat()
            body = self.body(TK.END)
            self.expect(TK.END); self.eat()
            return ForInStmt(Id(it), var, iv, body)
        start = self.expr()
        if self.check(TK.DOTDOT):
            self.adv(); end = self.expr()
            step = Num(1)
            if self.check(TK.DOTDOT): self.adv(); step = self.expr()
            iv = self.inline_vars(); self.eat()
            body = self.body(TK.END)
            self.expect(TK.END); self.eat()
            return ForStmt((start, end, step), iv, body)
        iv = self.inline_vars(); self.eat()
        body = self.body(TK.END)
        self.expect(TK.END); self.eat()
        return ForStmt(start, iv, body)

    def s_repeat(self):
        self.adv()
        cond = self.expr(); iv = self.inline_vars(); self.eat()
        body = self.body(TK.END)
        self.expect(TK.END); self.eat()
        return RepeatStmt(cond, iv, body)

    def s_time(self):
        self.adv()
        dur = self.expr(); self.expect(TK.COMMA); rate = self.expr()
        iv = self.inline_vars(); self.eat()
        body = self.body(TK.END)
        self.expect(TK.END); self.eat()
        return TimeStmt(dur, rate, iv, body)

    def s_cooldown(self):
        self.adv()
        cond = self.expr(); iv = self.inline_vars(); self.eat()
        body = self.body(TK.END)
        self.expect(TK.END); self.eat()
        return CooldownStmt(cond, iv, body)

    def s_maybe(self):
        self.adv()
        cond = self.expr()
        param = None
        if self.match(TK.THEN): param = self.expect(TK.ID, 'then 뒤 변수').v
        iv = self.inline_vars(); self.eat()
        body = self.body(TK.END)
        self.expect(TK.END); self.eat()
        return MaybeStmt(cond, param, iv, body)

    def s_wait(self):
        self.adv(); self.expect(TK.LP)
        cond = self.expr(); self.expect(TK.RP); self.eat()
        return WaitStmt(cond)

    def s_return(self):
        self.adv()
        vals = [self.expr()]
        while self.match(TK.COMMA): vals.append(self.expr())
        self.eat(); return ReturnStmt(vals)

    def s_unreturn(self):
        self.adv()
        vals = [self.expr()]
        while self.match(TK.COMMA): vals.append(self.expr())
        self.eat(); return UnReturnStmt(vals)

    def s_error(self):
        self.adv(); self.expect(TK.LP)
        msg = self.expr(); stop = True
        if self.match(TK.COMMA): stop = self.expr()
        self.expect(TK.RP); self.eat()
        return ErrorStmt(msg, stop)

    def inline_vars(self):
        iv = []
        if not self.match(TK.SEMI): return iv
        while self.check(TK.ID):
            name = self.adv().v; self.expect(TK.ASSIGN)
            val = self.expr(); iv.append((name, val))
            if not self.match(TK.COMMA): break
        return iv

    def s_expr(self):
        e = self.expr()
        if self.match(TK.ASSIGN):
            val = self.expr(); self.eat(); return Assign(e, val)
        if self.check(TK.COMMA) and isinstance(e, Id):
            targets = [e]
            while self.match(TK.COMMA): targets.append(self.expr())
            self.expect(TK.ASSIGN); val = self.expr(); self.eat()
            return MultiAssign(targets, val)
        self.eat(); return e

    # ── 표현식 ────────────────────────────────────────
    def expr(self): return self.pipe()

    def pipe(self):
        v = self.or_()
        if self.check(TK.PIPE):
            fns = []
            while self.match(TK.PIPE): fns.append(self.postfix())
            return Pipe(v, fns)
        return v

    def or_(self):
        l = self.and_()
        while self.check(TK.OR): self.adv(); r=self.and_(); l=BinOp('or',l,r)
        return l

    def and_(self):
        l = self.not_()
        while self.check(TK.AND): self.adv(); r=self.not_(); l=BinOp('and',l,r)
        return l

    def not_(self):
        if self.check(TK.NOT):
            self.adv(); self.expect(TK.LP)
            v = self.expr(); self.expect(TK.RP)
            return UnOp('not', v)
        return self.cmp()

    def cmp(self):
        l = self.concat()
        OPS = {TK.EQ:'==', TK.NEQ:'/=', TK.LTE:'<=', TK.GTE:'>='}
        while True:
            if self.c().t in OPS:
                op = OPS[self.c().t]; self.adv(); r=self.concat(); l=BinOp(op,l,r)
            elif self.check(TK.LT):
                # <ID> 패턴이면 타입지정 → BinOp로 남기고 break
                if self.p().t == TK.ID and self.p(2).t == TK.GT:
                    self.adv(); r=self.concat(); l=BinOp('<',l,r); break
                self.adv(); r=self.concat(); l=BinOp('<',l,r)
            elif self.check(TK.GT):
                self.adv(); r=self.concat(); l=BinOp('>',l,r)
            else: break
        return l

    def concat(self):
        l = self.add()
        while self.match(TK.CONCAT): r=self.add(); l=Concat(l,r)
        return l

    def add(self):
        l = self.mul()
        while self.check(TK.PLUS, TK.MINUS): op=self.adv().v; r=self.mul(); l=BinOp(op,l,r)
        return l

    def mul(self):
        l = self.unary()
        while self.check(TK.STAR, TK.SLASH, TK.PCT): op=self.adv().v; r=self.unary(); l=BinOp(op,l,r)
        return l

    def unary(self):
        if self.check(TK.MINUS): self.adv(); return UnOp('-', self.unary())
        if self.check(TK.TILDE): self.adv(); return UnOp('~', self.unary())
        return self.power()

    def power(self):
        l = self.postfix()
        if self.match(TK.CARET): return BinOp('^', l, self.unary())
        return l

    def postfix(self):
        n = self.primary()
        while True:
            if self.check(TK.DOT):
                self.adv(); attr=self.expect(TK.ID).v; n=Attr(n, attr)
            elif self.check(TK.LB):
                self.adv(); idx=self.expr(); self.expect(TK.RB); n=Idx(n,idx)
            elif self.check(TK.LP):
                self.adv()
                args=[]
                while not self.check(TK.RP, TK.EOF):
                    args.append(self.arg()); self.match(TK.COMMA)
                self.expect(TK.RP); n=Call(n, args)
            elif self.check(TK.COLON):
                self.adv()
                parts=[self.expect(TK.ID,'메서드 이름').v]
                while self.check(TK.DOT): self.adv(); parts.append(self.expect(TK.ID).v)
                method='.'.join(parts)
                args=[]
                if self.check(TK.LP):
                    self.adv()
                    while not self.check(TK.RP, TK.EOF):
                        args.append(self.arg()); self.match(TK.COMMA)
                    self.expect(TK.RP)
                n=MethodCall(n, method, args)
            else: break
        return n

    def arg(self):
        # 익명 함수: (( params body ) 또는 Get.((
        has_get = False
        if self.check(TK.GET) and self.p().t == TK.DOT and self.p(2).t == TK.DLP:
            self.adv(); self.adv(); has_get = True
        if self.check(TK.DLP):
            self.adv()
            params=[]
            while self.check(TK.ID): params.append(self.adv().v); self.match(TK.COMMA)
            self.skip()
            body=self.body(TK.RP); self.expect(TK.RP)
            return AnonFunc(params, body, has_get)
        return self.expr()

    def primary(self):
        t = self.c()
        if t.t == TK.NUM:  self.adv(); return Num(t.v)
        if t.t == TK.STR:  self.adv(); return Str(t.v)
        if t.t == TK.BOOL: self.adv(); return Bool(t.v)
        if t.t == TK.NULL: self.adv(); return Null()
        if t.t == TK.FSTR: self.adv(); return FStr(t.v)

        if t.t == TK.GET:
            self.adv(); self.expect(TK.DOT)
            name = self.expect(TK.ID, 'Get 뒤 이름').v
            if name == 'pi':   return GetPi()
            if name == 'e':    return GetE()
            if name == 'null': return GetNull()
            # Get.Sled, Get.함수이름 등 → Attr(Id('__Get__'), name)
            return Attr(Id('__Get__'), name)

        if t.t == TK.EVENT:
            self.adv(); self.expect(TK.LP)
            cond=self.expr(); self.expect(TK.RP)
            return EventExpr(cond)

        if t.t == TK.LB:
            self.adv(); elems=[]
            while not self.check(TK.RB, TK.EOF):
                elems.append(self.expr()); self.match(TK.COMMA)
            self.expect(TK.RB); return ListLit(elems)

        if t.t == TK.LC:
            self.adv(); pairs=[]
            while not self.check(TK.RC, TK.EOF):
                key=self.expect(TK.ID,'딕셔너리 키').v
                self.expect(TK.COLON)
                val=self.expr(); pairs.append((key,val)); self.match(TK.COMMA)
            self.expect(TK.RC); return DictLit(pairs)

        if t.t == TK.LP:
            self.adv(); e=self.expr(); self.expect(TK.RP); return e

        if t.t == TK.ID:
            self.adv(); return Id(t.v)

        raise ParseError(f'예상치 못한 토큰: {t.t}({t.v!r})', t.ln)
```
