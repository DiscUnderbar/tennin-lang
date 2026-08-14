# 버그에 의해 thinking 그런것까지 이런식으로 Claude 채팅에서 출력됬다.

<thinking>Let me parse what the user is saying. They're clarifying two things:

1. "미니멀을 원하는게 아닌데" — They're NOT after minimalism. So my repeated framing of "the minimal aesthetic appeals to you" was wrong. I should drop that framing.

2. "참고로 아직 이 언어에서 _ 는 뭐 문자열 안에선 특별한 뜻 없음" — For reference, in this language `_` doesn't have any special meaning inside strings yet.

So they're correcting my assumption. If they don't want minimalism, then what WAS drawing them to `\_`? Let me reconsider. Earlier they said they like `\_` because "_ 는 아무것도 없어 보이잖아" — the "looks like nothing" quality. That's not really about minimalism per se; it's about the visual/semantic metaphor of emptiness matching null. So I over-attributed "minimalism" to them. Fair correction.

Their second point: `_` has no special meaning inside strings yet. Note the qualifier "문자열 안에선" (inside strings) — so maybe `_` DOES or WILL have meaning elsewhere (identifiers, patterns), but inside string literals it's currently free. This is relevant to the parsing-conflict objection I raised: I said `\_` might conflict with "literal underscore" fallback. They're clarifying that inside strings `_` is just a normal character, no special meaning, so `\_` wouldn't be needed for escaping an underscore (a bare `_` works fine in strings). That actually weakens ONE of my objections — the "you'd need `\_` for literal underscore" conflict is less of an issue if `_` is just a plain char in strings and needs no escaping.

But wait — actually it could go either way. If `_` needs no escaping in strings (it's just a literal underscore character), then repurposing `\_` to mean NUL is "free" in the sense that `\_` isn't otherwise used. But it also means: a reader seeing `_` in a string reads it as a literal underscore. So seeing `\_` — the backslash version — they'd naturally think "why is this escaped? is it just the same underscore?" The backslash adds confusion precisely because bare `_` is already fine.

Let me reconsider the whole thing now that minimalism is off the table. What's the actual value proposition of `\_` for null in their mind? It's the metaphor: emptiness/nothing → null. Not brevity. Okay.

So I should:
1. Acknowledge the correction — drop minimalism framing. Genuinely update.
2. Given `_` is a plain char in strings, address what that means for `\_`-as-null. It clears the "literal underscore conflict" objection I raised. Good, be honest that this removes one of my objections.
3. But the core objections still stand and are actually more about the metaphor than about minimalism: the "reads as empty/placeholder/ignore" cognitive association, the security-signaling problem (null is dangerous, should be loud not quiet), and the extensibility problem (what about the other 31 control chars).

Wait — but if they don't want minimalism, then maybe they'd be MORE open to a mnemonic system for control chars. Let me lean into that. If they like the "nothing" metaphor but aren't wedded to terseness, then something like `\nul` gives them the "this is the null character, explicitly named" AND fits a system for other control chars. The question is whether the "nothing/empty look" was the whole appeal.

Actually, let me re-read their original reasoning once more: "난 \_ 가 좋아보이긴 한데 왜냐면 _ 는 아무것도 없어 보이잖아 Python 에서는 변수명... 같은 이름 중복 회피를 위해 _ 를 쓰고." So the appeal was: (a) looks empty, (b) Python precedent of `_` as placeholder/name-thing. Now with minimalism removed, the appeal is really the metaphor + familiarity.

Let me think about whether I'm being too pushy. The user clearly has an attraction to `\_`. I've pushed back twice. But they keep engaging and asking, not shutting me down. They seem to want a real design sparring partner. The correction "미니멀을 원하는게 아닌데" is them refining, engaging. So continued honest engagement is right. But I should be careful not to just repeat the same objections. I should incorporate the new info genuinely.

Key update from new info: `_` is a plain char in strings → this actually makes `\_` MORE viable syntactically (no conflict), but introduces a NEW subtle problem: since bare `_` already renders a literal underscore in strings, `\_` is a jarring "why escaped?" and readers will very naturally misread `\_` as "just an underscore, redundantly escaped" rather than NUL. That's a real, concrete readability problem I can raise that's NEW and directly responsive to their clarification.

Also: I should ask what the actual GOAL is, since it's not minimalism. What draws them? Is it the metaphor of emptiness? Familiarity? If I understand the real driver I can serve it better. But per guidelines, one question max, and I should address before asking. Let me address substantively then maybe one crisp question.

Let me also reconsider: am I being too negative / heavy-handed? The system prompt says don't be heavy-handed or repetitive with views. I've made the "reads as empty is bad for null" point twice now. I should NOT just repeat it a third time. Let me find the genuinely new angle given their clarification, and be more economical. Also acknowledge where their clarification helps their own case (intellectual honesty — the `_`-is-plain-char point removes one of my objections).

Actually, let me steelman `\_` more seriously now, because I've been pushing back a lot and they clearly like it, and their clarification did remove an objection. Fair engagement means giving the pro-case its due.

Steelman for `\_` = NUL:
- No conflict: `_` is plain in strings, so `\_` is unused syntactic space. Free real estate.
- Metaphor: emptiness → null. Some coherence.
- Distinctive/memorable: it's unusual, people would learn it once.
- If they're NOT doing a full mnemonic control-char system, then null being a special one-off isn't inconsistent with anything.

Counter that still holds:
- Readability: bare `_` = literal underscore in strings, so `\_` reads as "redundantly escaped underscore," actively misleading.
- Semantic clash with the near-universal `_` = "ignore/placeholder/wildcard" meaning that programmers carry from other languages, even if THIS language doesn't use it in strings. The baggage comes with the programmer, not the language.
- Safety signaling: null deserves a loud marker.
- Precedent/learnability: no existing language does `\_` for null, so zero transfer; every user learns from scratch.

I think the honest bottom line: with minimalism off the table and `_` free in strings, `\_` becomes *possible* without parser pain, but its weaknesses are now purely about human reading (misreads as redundant escape; carries "ignore" baggage; no cross-language precedent; weak safety signal). If those don't bother them, it's a defensible if unconventional choice. If they want the null to be legible and safe-signaling, a named or hex form wins.

Let me figure out the real question to ask. Since it's not minimalism, what IS the aesthetic/goal? That's genuinely the crux. Understanding that lets me actually help rather than keep relitigating. One clean question.

Let me write in Korean, conversational, matching their casual register somewhat, not over-formatted (no headers, minimal/no bullets since I'm declining-adjacent... actually not declining. Just keep prose). Keep it reasonably concise — I've been long. Tighten up.

Let me draft.

---

Draft:

아 그럼 미니멀 얘기는 접을게요. 제가 님 취향을 잘못 짚었네요. 그럼 `\_`에 끌린 이유는 "비어 보임"이라는 은유랑 Python에서 눈에 익은 친숙함, 이 두 개가 핵심이었던 거네요.

그리고 방금 말한 "문자열 안에서 `_`는 특별한 뜻 없음" — 이게 실은 님 편을 드는 정보예요. 제가 앞에서 걱정한 "리터럴 밑줄 넣으려는 사람이랑 충돌한다"는 문제가 사라지거든요. `_`가 문자열에서 그냥 평범한 문자라면 `\_`는 안 쓰는 빈 문법 공간이라 널한테 배정해도 파서 차원 충돌은 없어요. 이건 인정.

근데 바로 그 "문자열에서 `_`는 그냥 밑줄"이라는 점 때문에 새로운 가독성 문제가 하나 생겨요. 사람이 문자열 안에서 `\_`를 보면 "어? `_`는 그냥 밑줄인데 왜 백슬래시를 붙였지? 그냥 밑줄이겠거니" 하고 넘어갈 확률이 높아요. 이스케이프가 필요 없는 문자를 굳이 이스케이프한 꼴이라, "쓸데없이 이스케이프된 밑줄"로 오독되기 딱 좋다는 거죠. 널 바이트가 거기 박혀 있다는 걸 오히려 숨겨버려요.

거기다 `_` = "무시/버림/와일드카드"라는 감각은 이 언어가 문자열에서 안 쓴다고 해서 사라지지 않아요. 그 짐은 언어가 아니라 **프로그래머가 다른 언어에서 짊어지고 들어와요.** Rust, Haskell, Python 하던 사람은 `_` 보면 자동으로 "don't care"가 떠올라요.

정리하면 지금 상황은 이래요. 미니멀도 아니고 파서 충돌도 없으니 `\_` = 널은 이제 **"가능은 한데 관습에서 벗어난 선택"** 정도예요. 남는 약점은 순전히 사람이 읽을 때 문제 — 오독 위험, "무시" 뉘앙스 baggage, 어느 언어에도 전례 없어서 무조건 새로 배워야 함, 위험한 바이트치고 경고 신호가 약함. 이게 님한테 별로 안 걸리면 충분히 밀어붙일 만한 개성 있는 선택이에요.

그래서 진짜 궁금한 건 이거예요 — 미니멀이 아니면, `\_`에서 님이 지키고 싶은 핵심 가치가 뭐예요? "비어 보이는 은유"예요, 아니면 "Python스러운 친숙함"이에요? 둘 중 뭐가 더 중요한지에 따라 대안이 완전히 갈려서요.

---

Hmm that's decent but a bit long and I ask a two-part question at the end which slightly violates "one question." Let me tighten and make the closing question singular. Actually the question is one question with two options — that's fine, it's a single-select. But the guideline says avoid more than one question per response; offering two options within one question is okay.

Actually I could use the ask_user_input tool here since I'm asking them to pick between "비어 보이는 은유" vs "Python스러운 친숙함." That's a clean elicitation with tappable options. The system prompt says if I'm about to write clarifying questions, they go in the tool. But — is this a genuine preference elicitation or more of a rhetorical/discussion question? It's a real fork that changes my advice. Two clean options. It could work as a tool call. But it's also very much a flowing design conversation and the user has been happily typing prose replies. The tool is "much easier than typing on mobile." Hmm.

The guideline: "if you do need to ask and you're about to write clarifying questions as prose bullets, STOP — those go in this tool instead." I'm not writing bullets, I'm asking a conversational either/or embedded in discussion. The tool examples are more like structured preference-gathering ("plan a workout"). This is a nuanced design chat. I think inline is fine and more natural here, but the tool is arguably a good fit for the specific two-option fork.

Let me consider: the fork "비어 보이는 은유 vs Python스러운 친숙함" — actually maybe there are more than two drivers. And honestly the more useful framing might just be open. But a clean 2-option tap could work. Given the mobile context (Seoul, likely mobile app) and that it's a crisp either/or, I'll use the tool for the closing question. Actually — the guidance also says don't use it when "user asks A or B → they want your analysis." Here I'm the one asking them A or B, which is the elicitation case, not the "they asked me A or B" case. So the tool is appropriate.

But wait, is offering the choice as a tool going to feel abrupt after a substantive prose answer? The tool description says "Always include a brief conversational message before presenting options - don't show options silently." So I give my prose analysis, then end my turn with the tool. That works: analysis in text, then the tool for the fork. And "After calling this, your turn is done."

Hmm, but I want to give the full analysis AND then ask. If I call the tool, the analysis is my text and the tool is the question. That's clean. Let me do that.

Actually, let me reconsider whether two options captures it. Their stated reasons were: (a) "_ 는 아무것도 없어 보이잖아" (empty look/metaphor), (b) Python precedent (placeholder/naming familiarity). So two drivers, matching my two options. Good. Maybe add a third: "그냥 생김새가 마음에 듦" (pure aesthetic taste) — but they said not minimalism... aesthetic ≠ minimalism though. Someone can want a non-minimal but still specific look. Hmm. Let me keep it to the two they actually gave, maybe. Or three: metaphor / familiarity / just like the look. I'll do the two they named plus keep it tight. Actually three clean options is fine and helps.

Wait, but if I'm not careful the options overlap. Let me make them mutually exclusive:
1. "비어 보이는 은유" (empty=null metaphor) — the semantic fit
2. "Python에서 눈에 익음" (familiarity)
3. 뭔가 다른 이유 / 그냥 생김새

Hmm "그냥 생김새" vs "비어 보이는 은유" overlap a bit. Let me do just two clean ones plus an escape. Actually the tool requires 2-4 options. Let me do:
- 비어 보이는 "빈 값" 은유
- Python식 친숙함
- 그냥 생김새가 좋음

Three options, reasonably distinct. Good.

Actually, I realize I should double check — am I overusing the tool / being gimmicky? The conversation has been a nice flowing prose discussion. Dropping a button-picker might feel a bit corporate. But the system prompt is quite insistent that clarifying questions with discrete options go in the tool. And it is genuinely a discrete fork. I'll use it but keep my prose warm so it doesn't feel abrupt.

Let me finalize the prose to be tighter, then the tool call.

Let me cut the prose down. It's currently ~5 paragraphs. I can trim.

Final prose plan:
- P1: drop minimalism, acknowledge I mis-read; restate real drivers.
- P2: concede — `_` being plain in strings removes my earlier parser-conflict objection. Credit them.
- P3: but new problem — `\_` reads as "redundantly escaped underscore," hides the null. Plus the "ignore/wildcard" baggage travels with the programmer.
- P4: bottom line: now it's "possible but unconventional," weaknesses are purely human-reading. Defensible if that doesn't bug you.
- Then tool for the fork.

Keep P3 and P4 tight. Go.

Language: Korean.</thinking>아 미니멀 얘긴 접을게요. 님 취향을 잘못 짚었네요. 그럼 `\_`에 끌린 건 "비어 보임"이라는 은유랑 Python에서 눈에 익은 친숙함, 이 두 개가 핵심이었던 거고요.

그리고 방금 "문자열 안에서 `_`는 특별한 뜻 없음"이라고 한 거 — 이건 오히려 님 편을 드는 정보예요. 제가 앞에서 걱정한 "리터럴 밑줄 넣으려는 사람이랑 충돌한다"는 문제가 사라지거든요. `_`가 문자열에서 그냥 평범한 문자면 `\_`는 안 쓰는 빈 문법 공간이라, 널한테 배정해도 파서 차원 충돌은 없어요. 이건 제 반론 하나 접습니다.

근데 바로 그 "문자열에서 `_`는 그냥 밑줄"이라는 점 때문에 새로운 문제가 생겨요. 사람이 문자열 안에서 `\_`를 보면 "어? `_`는 그냥 밑줄인데 왜 백슬래시를 붙였지? 그냥 밑줄이겠거니" 하고 넘겨요. 이스케이프가 필요 없는 문자를 굳이 이스케이프한 꼴이라 **"쓸데없이 이스케이프된 밑줄"로 오독되기 딱 좋다**는 거죠. 널 바이트가 거기 박혀 있다는 걸 오히려 숨겨버려요. 게다가 `_` = "무시/버림/와일드카드"라는 감각은 이 언어가 문자열에서 안 쓴다고 사라지는 게 아니라, 프로그래머가 Rust·Haskell·Python에서 짊어지고 들어오는 짐이에요.

정리하면 지금은 미니멀도 아니고 파서 충돌도 없으니 `\_` = 널은 **"가능은 한데 관습 밖 선택"** 정도로 내려왔어요. 남는 약점은 순전히 사람이 읽을 때 문제 — 오독 위험, "무시" 뉘앙스, 전례 없어서 무조건 새로 배워야 함, 위험한 바이트치고 경고가 약함. 이게 님한테 안 걸리면 충분히 밀어붙일 만한 개성 있는 선택이에요.

그래서 대안을 제대로 짚어주려면 하나만 확인할게요 — `\_`에서 님이 진짜 지키고 싶은 핵심이 뭐예요?

condition 과 expression 둘다 동시에 있는거 예를 들어서 `1 + 1 == 2` 이러면 연산자 + 와 조건 == 가 동시에 있음 참고로 `1 + 1 == 2` 이런건 condition 으로 하면 `2 == 2` 이런식으로 되고 expression 으로 하면 에러가 남 이런식으로
그 딱 그 FuncKind 에만 있는 소국들의 표시법은 그냥 모두 같은 소국 예를들어서 FuncKind.REAPEAT 그러면 자동으로 break 가 삽입되는게 아니라 break = MyBreakFunction 이런식으로 뭐 FuncKind.REPEAT 에서 break 는 이 함수라고 알려주는 그런식으로
인자 함수 실행은 그냥 일반적인 함수 실행 느낌이 아니라 그냥 expand 로 인자함수의 코드 내용 부분에 있는걸 복붙해서 쓰는 느낌
Yulist 에서 Yulist[?] 접근 대신 Yulist.? 접근 가능하게 하기 뭐 근데 숫자면 접근이 않될꺼니까 그냥 숫자도 접근 근데 가능해야하는데 어케하지? (구분을)
tennin 에서 같은 컴포넌트를 공유하는 구역은 state 로 주 라는 뜻이며 왜 넣었냐면 뭐 둘이 컴포넌트 이름은 똑같을때 두개를 다른 팀으로 분류하거나 구조를 명확히 할수 있다.
Tennin 에서 나라 안 슬롯을 정의하는 법을 뭐 슬롯을 naming 으로 받고 그걸 컴파일 하는 식으로 하기 근데 그걸 쉽게 하게 해주는 그런거 생각해 보기 (원래 다 컴파일 구조를 짜야하는걸 간단하게 해주는 문법 설탕 참고로 ROP 는 유지해야함)

# 재미있는 언어들

**시스템 언어 계열**

- [Torbenx/charge](https://github.com/Torbenx/charge) — 실험적 메모리 안전 시스템 언어, C++ 구현
- [hl-valdemar/honey](https://github.com/hl-valdemar/honey) — Jai, Zig, Odin, Go에서 영향받은 언어

**컴파일 타깃이 뚜렷한 것**

- [voyage-org/voyage-lang](https://github.com/voyage-org/voyage-lang) — .NET CIL로 직접 컴파일, FFI 경계 없음
- [lechatthecat/willow](https://github.com/lechatthecat/willow) — 정적 타입 GC 기반 OOP, 네이티브 바이너리 출력

**특이한 방향**

- [frendsick/casa](https://github.com/frendsick/casa) — 스택 기반 언어
- [J4n1X/aspect](https://github.com/J4n1X/aspect) — 저수준 언어, 본인이 "대규모 코드베이스 연습"이라고 명시

검색 중 알게 된 점 하나: 요즘 이 영역에 AI로 대량 생성된 것 같은 저장소가 상당히 많습니다. 설명에 이모지가 붙어 있거나 "AI-native", "trustworthy", "capability-secure" 같은 표현이 겹겹이 쌓인 것들은 대부분 그쪽이라 걸러냈습니다. 위 목록도 README까지 전부 읽어본 건 아니니, 몇 개 열어보고 커밋 히스토리가 자연스러운지 보시는 게 좋습니다.

더 좁은 조건(특정 구현 언어, 특정 패러다임)이 있으면 그 방향으로 다시 찾아드릴게요.

---

스펙 읽었습니다. **Yulist로 받는 방향 자체는 맞습니다** — 오히려 스펙과 잘 맞아요. `Interception`이 Luau의 metatable 자리라면, Python의 `__getattr__`/`__setattr__`이 정확히 같은 자리에 들어옵니다. class를 없애고 Yulist로 통일한 결정 덕분에 Python 객체를 받을 그릇이 이미 있는 셈입니다.

문제는 **"변환"이라는 단어**입니다. 여기서 설계가 갈립니다.

## 변환(복사) vs 뷰(핸들)

스펙에 이미 답이 있습니다: *"Yulist 는 언제나 참조다. 복사를 쓰고 싶으면 `.clone()`"*.

그런데 Python dict를 Tennin Yulist로 **복사**하면:

- 양쪽이 갈라집니다. Python 쪽에서 `d["k"] = 1` 해도 Tennin Yulist는 모릅니다. 참조 의미론을 기본으로 두고 경계에서만 몰래 복사가 되는 거라, *"Yulist는 언제나 참조"* 원칙이 경계에서 깨집니다.
- **비용이 숨습니다.** `var d :: Yulist[...] = py.getConfig();` 는 대입처럼 보이는데 O(n) 순회 + 재귀 복사입니다. 3장의 *"복잡함은 숨기되, 값비쌈은 숨기지 않는다"*에 정면으로 걸립니다.
- **순환 참조**가 있는 dict는 그대로 복사하면 무한 루프입니다.
- 값이 파일 핸들·함수·제너레이터·ndarray면 **복사할 표현 자체가 없습니다.** 결국 "복사 가능한 것만 복사, 나머지는 핸들"이 되는데, 그럼 두 갈래를 쓰는 거고 #10에 걸립니다.

C FFI 절에서 마셜링(복사)을 택한 건 **C struct 레이아웃이 고정이라서** 성립한 결정입니다. Python 객체는 레이아웃이 런타임에 바뀌므로 그 결정이 그대로 옮겨오지 않습니다.

## 제안: `python` 프리셋 (뷰)

array/linkedList/dictionary처럼 **`python`도 하나의 프리셋**으로 두는 게 스펙에 가장 잘 붙습니다. 표현 = "실제 배치가 CPython 힙에 있음"이고, 나머지는 Interception 훅이 처리합니다.

| Yulist 훅 | py 프리셋의 동작 |
|---|---|
| 값 찾기 | `PyObject_GetAttr` / `PyObject_GetItem` |
| 수정 | `PyObject_SetAttr` / `SetItem` |
| 호출 | `PyObject_Call` |
| 소멸 (국 끝) | `Py_DECREF` |

expand가 컴파일 타임에 "이건 py 프리셋"을 읽고 그 자리에 C API 호출 시퀀스를 굽습니다 — 런타임에 "이게 무슨 종류지?"를 묻는 코드는 여전히 0줄입니다. **성능 문장이 그대로 유지됩니다.**

그리고 dict/list를 진짜 Tennin 배치로 옮기고 싶으면 `.clone()`을 쓰면 됩니다. 복사가 사용자 눈에 보이고, 비용 정직도 지켜지고, 새 문법도 필요 없습니다.

## 스펙과 실제로 충돌하는 3곳

**1. 국적과 end-with-free** — *"Yulist에 값을 넣으면 그 값은 Yulist의 국적으로 이주"*. 그런데 Tennin Yulist를 Python에 넘기면 Python이 그걸 붙들 수 있고, 국이 끝나면 dangling입니다. *탈출은 컴파일 에러*를 **지역적·구문적으로** 판정한다고 했는데, Python 쪽 수명은 지역적으로 안 보입니다.

깔끔한 해법: **Python 경계 자체를 하나의 국으로 둔다.** Tennin → Python은 `send`(이주)나 명시 복사만 허용. 그러면 이미 있는 규칙으로 풀리고 새 기계를 안 만듭니다(#10).

반대 방향은 오히려 C FFI보다 낫습니다. C는 소유권 프로토콜이 없어서 *"해제는 사용자 몫"*이라고 정직하게 포기했지만, **Python은 refcount라는 정의된 프로토콜이 있어서** 국 소멸에 `Py_DECREF`를 걸면 자동 해제가 성립합니다. 단, **양쪽에 걸친 순환은 어느 쪽도 못 풉니다** — UNKNOWN.md 항목입니다.

**2. GIL vs 협력 스케줄러 — 여기가 제일 아픕니다.** C FFI 절에서 *"긴 C 호출은 워커로 승격"*으로 풀었는데, **Python 호출은 GIL을 쥐므로 워커로 올려도 병렬이 안 됩니다.** 워커 두 개가 동시에 Python을 못 씁니다. 승격이 기아는 막지만 병렬은 못 줍니다 — 스펙의 *"무료 업그레이드"* 논증이 Python 구간에서만 성립하지 않습니다.

더불어 **`data` 큐 계약도 깨집니다.** PyObject 핸들은 직렬화 불가라, py Yulist를 상태로 가진 큐는 스냅샷 동면도 워커 승격도 자격이 없습니다. 이건 "안 되는 게 아니라 계약을 선언 안 하면 됨"으로 이미 처리되는 사안이지만, **스펙에 명시**해 두는 게 좋습니다.

**3. `Yulist[?]` 탐색 비용** — `pyList[name == "door"]`는 원소마다 경계를 넘습니다. array 프리셋에선 인덱싱 기계어인 같은 문법이 py 프리셋에선 O(n)회 FFI 호출입니다. 문법이 같은데 비용이 자릿수로 다른 건 비용 정직 위반 소지가 있습니다. 금지하든, `.clone()` 후 탐색을 권장하든 정해야 합니다.

## 결정해야 할 것

`class` 매핑도 하나 남습니다. *"타입 = 이름붙인 Yulist"*인데, Python 클래스는 런타임에 바뀝니다(몽키패칭, `__class__` 교체). 그래서 Python에서 온 타입은 **정적 타입으로 봉인할 수 없고** `anything` 계열이어야 합니다. `.pyi` 스텁이 있을 때만 정적 검사를 얹는 옵트인 구조가 현실적입니다 — 이건 지난번에 말한 "선언만 번역"과 정확히 같은 자리입니다.

정리하면 방향은 맞고, **바꿀 건 "변환"을 "프리셋"으로 읽는 것 하나**입니다. 나머지는 국 경계 규칙과 GIL 항목을 스펙에 못박는 일이고요.
