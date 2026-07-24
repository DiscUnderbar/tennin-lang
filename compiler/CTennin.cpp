// CTennin — minimal Tennin compiler (v0.2)
// Pipeline: Tennin source -> lex -> parse -> emit textual LLVM IR (stdout)
//
// Supported subset (v0.2 = v0.1 + variables/int/arithmetic):
//   using tenn;                         (parsed, ignored)
//   #main                               (job: marks the following func as entry)
//   func NAME ():( <stmts> ):();        (func decl; params/return slots empty for now)
//   var NAME :: int = <expr>;           (int variable declaration)
//   print(<expr>);  print("...");       (int -> printf %d, string -> puts)
//   Line.addLine(<expr> | "...");
//   <expr> := int literals, variables, + - * /, parentheses (usual precedence)
//   @... line comment
//
// Anything else is rejected with a line-numbered error (never a silent "works").

#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <cstdio>

using std::string;
using std::vector;

[[noreturn]] static void fail(int line, const string& msg) {
    std::cerr << "CTennin: error (line " << line << "): " << msg << "\n";
    std::exit(1);
}

// ---------- lexer ----------
enum class Tk { Ident, Num, Str, Punct, Hash, End };
struct Token { Tk kind; string text; int line; };

struct Lexer {
    const string& src; size_t i = 0; int line = 1;
    explicit Lexer(const string& s) : src(s) {}

    vector<Token> run() {
        vector<Token> out;
        while (i < src.size()) {
            char c = src[i];
            if (c == '\n') { line++; i++; continue; }
            if (std::isspace((unsigned char)c)) { i++; continue; }
            if (c == '@') { while (i < src.size() && src[i] != '\n') i++; continue; }
            if (c == '"') { out.push_back(readString()); continue; }
            if (c == '#') { i++; out.push_back({Tk::Hash, "#", line}); continue; }
            if (std::isdigit((unsigned char)c)) { out.push_back(readNumber()); continue; }
            if (std::isalpha((unsigned char)c) || c == '_') { out.push_back(readIdent()); continue; }
            if (c == ':') {                       // ':' or '::'
                if (i + 1 < src.size() && src[i + 1] == ':') { out.push_back({Tk::Punct, "::", line}); i += 2; }
                else { out.push_back({Tk::Punct, ":", line}); i++; }
                continue;
            }
            if (string("{}();.,+-*/=").find(c) != string::npos) {
                out.push_back({Tk::Punct, string(1, c), line}); i++; continue;
            }
            fail(line, string("unexpected character '") + c + "'");
        }
        out.push_back({Tk::End, "", line});
        return out;
    }
    Token readIdent() { int ln = line; size_t s = i;
        while (i < src.size() && (std::isalnum((unsigned char)src[i]) || src[i] == '_')) i++;
        return {Tk::Ident, src.substr(s, i - s), ln}; }
    Token readNumber() { int ln = line; size_t s = i;
        while (i < src.size() && std::isdigit((unsigned char)src[i])) i++;
        return {Tk::Num, src.substr(s, i - s), ln}; }
    Token readString() { int ln = line; i++; string v;
        while (i < src.size() && src[i] != '"') {
            char c = src[i++];
            if (c == '\\' && i < src.size()) { char e = src[i++];
                switch (e) { case 'n': v += '\n'; break; case 't': v += '\t'; break;
                             case '\\': v += '\\'; break; case '"': v += '"'; break; default: v += e; } }
            else { if (c == '\n') line++; v += c; } }
        if (i >= src.size()) fail(ln, "unterminated string literal");
        i++; return {Tk::Str, v, ln}; }
};

// ---------- AST ----------
struct Expr {
    enum Kind { Int, Var, Bin } kind;
    long ival = 0;      // Int
    string name;        // Var
    char op = 0;        // Bin: + - * /
    Expr* l = nullptr; Expr* r = nullptr;
    int line = 0;
};
struct Arg { bool isStr; string str; Expr* expr; };
struct Stmt {
    enum Kind { Call, VarDecl } kind;
    string callee; vector<Arg> args;      // Call
    string varName; Expr* init = nullptr; // VarDecl
    int line = 0;
};
struct Func { string name; bool isMain = false; vector<Stmt> body; int line = 0; };
struct Program { vector<Func> funcs; };

// ---------- parser ----------
struct Parser {
    vector<Token> toks; size_t p = 0;
    explicit Parser(vector<Token> t) : toks(std::move(t)) {}

    const Token& peek() const { return toks[p]; }
    const Token& advance() { return toks[p++]; }
    bool isPunct(const string& s) const { return peek().kind == Tk::Punct && peek().text == s; }
    bool isIdent(const string& s) const { return peek().kind == Tk::Ident && peek().text == s; }
    void expectPunct(const string& s) {
        if (!isPunct(s)) fail(peek().line, "expected '" + s + "' but got '" + peek().text + "'");
        advance();
    }

    Program parse() {
        Program prog; bool pendingMain = false;
        while (peek().kind != Tk::End) {
            if (peek().kind == Tk::Hash) { advance();
                if (peek().kind != Tk::Ident) fail(peek().line, "expected job name after '#'");
                string job = advance().text;
                if (job == "main") pendingMain = true;
                else fail(peek().line, "unknown job '#" + job + "' (only #main supported)");
                continue; }
            if (isIdent("using")) { while (peek().kind != Tk::End && !isPunct(";")) advance();
                                    if (isPunct(";")) advance(); continue; }
            if (isIdent("func")) { Func f = parseFunc();
                f.isMain = pendingMain || f.name == "main"; pendingMain = false;
                prog.funcs.push_back(std::move(f)); continue; }
            fail(peek().line, "expected 'func', '#main', or 'using' at top level, got '" + peek().text + "'");
        }
        return prog;
    }

    Func parseFunc() {
        Func f; f.line = peek().line; advance(); // 'func'
        if (peek().kind != Tk::Ident) fail(peek().line, "expected function name after 'func'");
        f.name = advance().text;
        expectPunct("(");
        if (!isPunct(")")) fail(peek().line, "v0 supports only empty params '()' for now");
        expectPunct(")");
        expectPunct(":"); expectPunct("(");
        while (!isPunct(")")) {
            if (peek().kind == Tk::End) fail(f.line, "unterminated function body");
            f.body.push_back(parseStmt());
        }
        expectPunct(")");
        if (isPunct(":")) { advance(); expectPunct("(");
            if (!isPunct(")")) fail(peek().line, "v0 supports only empty return '()' for now");
            expectPunct(")"); }
        expectPunct(";");
        return f;
    }

    Stmt parseStmt() {
        if (isIdent("var") || isIdent("const")) return parseVarDecl();
        return parseCall();
    }

    Stmt parseVarDecl() {
        Stmt s; s.kind = Stmt::VarDecl; s.line = peek().line;
        advance(); // var / const
        if (peek().kind != Tk::Ident) fail(peek().line, "expected variable name");
        s.varName = advance().text;
        expectPunct("::");
        if (peek().kind != Tk::Ident) fail(peek().line, "expected a type after '::'");
        string ty = advance().text;
        if (ty != "int") fail(peek().line, "v0 supports only 'int' variables (got '" + ty + "')");
        expectPunct("=");
        s.init = parseExpr();
        expectPunct(";");
        return s;
    }

    Stmt parseCall() {
        Stmt s; s.kind = Stmt::Call; s.line = peek().line;
        if (peek().kind != Tk::Ident) fail(peek().line, "expected a statement (var decl or call)");
        string name = advance().text;
        while (isPunct(".")) { advance();
            if (peek().kind != Tk::Ident) fail(peek().line, "expected name after '.'");
            name += "." + advance().text; }
        s.callee = name;
        expectPunct("(");
        if (!isPunct(")")) {
            s.args.push_back(parseArg());
            while (isPunct(",")) { advance(); s.args.push_back(parseArg()); }
        }
        expectPunct(")"); expectPunct(";");
        return s;
    }

    Arg parseArg() {
        if (peek().kind == Tk::Str) { Arg a; a.isStr = true; a.str = advance().text; a.expr = nullptr; return a; }
        Arg a; a.isStr = false; a.str = ""; a.expr = parseExpr(); return a;
    }

    // expression: + - (low)  ->  * / (high)  ->  primary
    Expr* parseExpr() { return parseAdd(); }
    Expr* parseAdd() {
        Expr* l = parseMul();
        while (isPunct("+") || isPunct("-")) { char op = advance().text[0];
            Expr* r = parseMul(); l = new Expr{Expr::Bin, 0, "", op, l, r, l->line}; }
        return l;
    }
    Expr* parseMul() {
        Expr* l = parsePrimary();
        while (isPunct("*") || isPunct("/")) { char op = advance().text[0];
            Expr* r = parsePrimary(); l = new Expr{Expr::Bin, 0, "", op, l, r, l->line}; }
        return l;
    }
    Expr* parsePrimary() {
        if (peek().kind == Tk::Num) { const Token& t = advance();
            return new Expr{Expr::Int, std::stol(t.text), "", 0, nullptr, nullptr, t.line}; }
        if (isPunct("(")) { advance(); Expr* e = parseExpr(); expectPunct(")"); return e; }
        if (peek().kind == Tk::Ident) { const Token& t = advance();
            return new Expr{Expr::Var, 0, t.text, 0, nullptr, nullptr, t.line}; }
        fail(peek().line, "expected a number, variable, or '(' in expression, got '" + peek().text + "'");
    }
};

// ---------- codegen (textual LLVM IR) ----------
static string irEscape(const string& s, size_t& byteLen) {
    string r;
    for (unsigned char c : s) {
        if (c == '\\' || c == '"' || c < 0x20 || c >= 0x7f) {
            char buf[5]; std::snprintf(buf, sizeof buf, "\\%02X", c); r += buf;
        } else r += (char)c;
    }
    r += "\\00"; byteLen = s.size() + 1; return r;
}

struct Codegen {
    std::ostringstream globals, body;
    int strCount = 0, tmp = 0;
    bool usePrintf = false;
    std::set<string> declared;

    string ntemp() { return "%t" + std::to_string(tmp++); }

    string addString(const string& s, size_t& len) {
        string esc = irEscape(s, len);
        string name = "@.str" + std::to_string(strCount++);
        globals << name << " = private unnamed_addr constant [" << len
                << " x i8] c\"" << esc << "\"\n";
        return name;
    }

    // returns an i32 operand (immediate like "14" or a register like "%t3")
    string emitExpr(const Expr* e) {
        switch (e->kind) {
            case Expr::Int: return std::to_string(e->ival);
            case Expr::Var: {
                if (!declared.count(e->name)) fail(e->line, "undefined variable '" + e->name + "'");
                string t = ntemp();
                body << "  " << t << " = load i32, ptr %v." << e->name << "\n";
                return t;
            }
            case Expr::Bin: {
                string lv = emitExpr(e->l), rv = emitExpr(e->r);
                const char* opc = e->op == '+' ? "add" : e->op == '-' ? "sub"
                                : e->op == '*' ? "mul" : "sdiv";
                string t = ntemp();
                body << "  " << t << " = " << opc << " i32 " << lv << ", " << rv << "\n";
                return t;
            }
        }
        return "0";
    }

    void emitStmt(const Stmt& s) {
        if (s.kind == Stmt::VarDecl) {
            if (declared.count(s.varName)) fail(s.line, "variable '" + s.varName + "' redeclared");
            body << "  %v." << s.varName << " = alloca i32\n";
            string v = emitExpr(s.init);
            body << "  store i32 " << v << ", ptr %v." << s.varName << "\n";
            declared.insert(s.varName);
            return;
        }
        // Call
        if (s.callee != "print" && s.callee != "Line.addLine")
            fail(s.line, "unknown function '" + s.callee + "' (v0 builtins: print, Line.addLine)");
        if (s.args.size() != 1)
            fail(s.line, "'" + s.callee + "' expects exactly one argument in v0");
        const Arg& a = s.args[0];
        if (a.isStr) {
            size_t len; string g = addString(a.str, len);
            string p = ntemp();
            body << "  " << p << " = getelementptr inbounds [" << len
                 << " x i8], ptr " << g << ", i64 0, i64 0\n";
            string r = ntemp();
            body << "  " << r << " = call i32 @puts(ptr " << p << ")\n";
        } else {
            usePrintf = true;
            string v = emitExpr(a.expr);
            string f = ntemp();
            body << "  " << f << " = getelementptr inbounds [4 x i8], ptr @.fmtint, i64 0, i64 0\n";
            string r = ntemp();
            body << "  " << r << " = call i32 (ptr, ...) @printf(ptr " << f << ", i32 " << v << ")\n";
        }
    }

    string emit(const Program& prog) {
        const Func* mainF = nullptr;
        for (const auto& f : prog.funcs) if (f.isMain) { mainF = &f; break; }
        if (!mainF) fail(1, "no entry function (mark one with #main or name it 'main')");

        for (const auto& s : mainF->body) emitStmt(s);

        std::ostringstream out;
        out << "; CTennin v0.2 generated LLVM IR\n";
        out << globals.str();
        if (usePrintf)
            out << "@.fmtint = private unnamed_addr constant [4 x i8] c\"%d\\0A\\00\"\n";
        out << "\ndeclare i32 @puts(ptr)\n";
        if (usePrintf) out << "declare i32 @printf(ptr, ...)\n";
        out << "\ndefine i32 @main() {\nentry:\n";
        out << body.str();
        out << "  ret i32 0\n}\n";
        return out.str();
    }
};

// ---------- driver ----------
int main(int argc, char** argv) {
    if (argc < 2) { std::cerr << "usage: CTennin <file.tenn>\n"; return 2; }
    std::ifstream in(argv[1]);
    if (!in) { std::cerr << "CTennin: cannot open " << argv[1] << "\n"; return 2; }
    std::stringstream ss; ss << in.rdbuf();
    string src = ss.str();

    Lexer lex(src);
    Parser parser(lex.run());
    Program prog = parser.parse();
    Codegen cg;
    std::cout << cg.emit(prog);
    return 0;
}
