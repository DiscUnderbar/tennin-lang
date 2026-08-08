**7/22/2026 12:17 AM** lmnop 의 역사 lmnop abc 의 ErrScript 와 M.G.S (Make Games Script) 그리고 lmnop def, lmnop hijk, lmnop qrs, lmnop tuv, lmnop wxyz 가 끝나고 tennin 으로 봐뀌었다.
**7/22/2026 10:42 PM** 작업 폴더의 위치가 lmnop 폴더에서 tennin 폴더로 이동됬고, 이 언어 파일 확인자 -- *lmp* 를 거이다 -- *tenn* 으로 봐꿨으며 일부는 남겨뒀다.

**7/23/2026** 이전 "lmnop 언어 설계 피드백 요청" 채팅에서 결정·논의됐지만 spec 에 빠져 있던 것 4 개를 tennin-spec.md 에 반영. 전 → 후: (없음 → 있음).
- **실행 vs 참조 규칙** (`값이 나오는 경로` 뒤) — `f ()` = 실행 / `f` = 참조. 왜: 실행을 기호가 아니라 뒤따르는 인자 함수 유무(구조)로 드러내 무모호·익숙함·기호 절약을 동시에. 이로써 이전에 논의만 하던 `;;`/`\` 실행 표시 방안을 버림.
- **다메커니즘 func 문법 스케치** (`함수` 섹션, 미확정 라벨) — at countrys/country/grade/at slot, in smallCountrys/in slot/at manual. at=인자 매크로, in=본문 소국(위치로 갈림).
- **`expression` 타입** (정보 지향 섹션) — 수식을 boolean 이 아니라 `.calculation`/`.result` 로 보존해서 받기, change 예시. 비용 옵트인.
- **재디스패치 원시요소** (`함수` 섹션) — special/Feature Absorption 의 기반. 받은 콜백을 이름표 블록으로 재디스패치(`call.target`/`call.args`/`f.block`). 범용 리플렉션이라 특례 아님.
버린 것: 없음(순수 추가). 세부 문법은 대부분 미정으로 표시.

**7/23/2026** class 제거 (Luau 모델). 전 → 후: `### 클래스`(class/new/extends/self::template 문법) → `### 클래스 없음 — Yulist 로 대체`. 왜: class 와 Yulist 둘 다 "필드+동작 묶은 구조적 타입" 이라 #10(중복 금지) 위반. Luau 가 table+metatable 로 객체를 표현하듯 Tennin 은 Yulist+Interception 으로. 메서드=Yulist 슬롯의 함수-값, 생성=create 훅 + would[T].complete(), 타입=이름붙인 Yulist. **버린 것: `class`/`new`/`extends`/`self :: template` 문법** (Yulist 로 재구축 필요). 함께: would[T] 메커니즘 확정(`.complete()`→T, 왜=단계적으로 형태를 갖춰가는 값의 생성 원시요소) + "타입도 값(typeof[T]) / 별도 comptime 없음, 타입 매직이 처리" 명시. 상속·self·Yulist 정의·생성 문법은 미정(→ TODO).

**7/23/2026** 위 두 결정 후 문서 전체 일괄 갱신. 전 → 후: 남아 있던 `class`/`클래스` 예시·언급을 Yulist(구조적 타입)로 교체 (naming 예시 `class Point`→`func moveTo`, 파일 형태·소국·job·모듈·main·take 문법의 "클래스"→"Yulist", 함수 이름 예시 `if, class`→`if, while` 등). expand 섹션의 "값만 남는다 (comptime/상수계산)" → "(상수계산 — expand 의 한 결과)". 유지한 것: Java 의 class·C++ 구현부(`enum class`)·Zig comptime 은 외부 참조라 그대로. 버린 것: 없음(표현 정리).

**7/24/2026** `template` 타입 제거. 전 → 후: 타입 표의 `template`("생성 시 처음 self 의 틀") → 삭제. 왜: would[T] 를 추가할 때 겹치는 template 을 같이 없앴어야 했는데 빠뜨렸다 — 생성 중 미완성 값은 would[T] 로 일원화한다. 버린 것: `template` 타입 (would[T] 가 대체). would 의 왜 문구에서 'template→instance' 표현도 '미완성→완성' 으로 정정.

**7/24/2026** `### C FFI` 섹션 신설 (메모리 관리 섹션 뒤). 전 → 후: SM 섹션의 "C FFI 는 마셜링 — TODO" 한 줄 → 정식 섹션. 왜: Tennin 의 SM·국·협력 스케줄러·expand 모델이 C 경계를 다르게 만든다. 네 입장: ① 경계 명시(안전 보장 종료, 원칙 3) ② 마셜링 복사 + expand 컴파일타임 생성(아레나 포인터 노출 금지 제약) ③ C 소유 메모리는 불투명 핸들·수동 해제(정직한 포기 — FFI 너머 누수는 사용자 몫) ④ 블로킹 C 호출은 워커 승격 재사용(협력 스케줄러 기아 방지, #10). 세부 문법·타입 매핑·콜백·ABI 등은 미정(→ TODO). 버린 것: 없음(추가).

**7/24/2026** Yulist 상속 결정 — 상속 없음, 합성(composition). 전 → 후: `클래스 없음` 섹션의 "상속(extends) — Luau __index 체인 후보, 미정" → "상속 대신 합성 — 결정". 왜: 상속은 구조가 명확하다는 장점이 있지만 어쩔 때 코드를 오히려 더 복잡하게 만든다 — Tennin 목적(구현 복잡도 낮추기)에 맞춰 합성을 택함. 예: Point2D·Point3D 가 reset 을 똑같이 쓰면 reset 을 컴포넌트로 빼서 둘 다에 넣는 식. 포기(정직): 상속의 '구조 한눈에 보임' 이점은 내려놓음(합성은 동작이 어느 컴포넌트에서 왔는지 추적 필요). 버린 것: 상속 흉내(Luau metatable __index 체인 후보). 컴포넌트 포함/위임 문법·이름충돌 해소는 미정.


**7/24/2026** 아이디어 메모 일괄 반영 (spec). 추가: (타입매직) condition vs expression 층 구분 — `1+1==2` 를 condition 으로 받으면 `2==2`, expression 으로 받으면 에러(expression=순수 산술 층). (expand) 인자 함수 실행 = expand 인라인(복붙, 호출 아님). (FuncKind) 전용 소국은 자동삽입 아니라 명시 매핑 `break = MyBreakFunction`. (Yulist) `.키` 멤버식 점 접근 추가(숫자 키 구분은 UNKNOWN). (합성) `state` = 같은 컴포넌트 공유 구역(이름충돌·팀 구분). (내장함수) 반복문(while/repeat)·switch·이벤트시스템(emit/when/give — give 값이 emit 반환)·기타 골격 추가, 이벤트↔액터메시징 #10 경계는 TODO. UNKNOWN 에 ⑤ Yulist 숫자 점접근 구분 ⑥ 국 슬롯정의 문법설탕(ROP 유지) ⑦ 이벤트/메시징 경계 파킹. 버린 것: 없음(추가·정정).

**7/24/2026** 이벤트 시스템(emit/when/give) 제거. 전 → 후: 내장함수 `#### 이벤트 시스템` 서브섹션 → 삭제(한 줄 사유만 남김). 왜: 1-D 액터 메시징(send/ask/on/receive)의 요청-응답과 겹쳐 #10(중복 금지) 위반 — 신호·요청-응답은 메시징 하나로 일원화. 버린 것: emit/when/give. UNKNOWN ⑦(이벤트/메시징 경계)도 해소되어 제거.


**7/24/2026** None-Native(인터프리터 모드) 결정 + spec 섹션 신설. 전 → 후: TODO 의 `@none-native` 재검토 후보 → 결정. `using tenn-none-native;` 로 인터프리터 opt-in(native 가 기본, 정체성 반영 — Luau 는 반대로 인터프리터 기본+`--!native`). 왜: 워크로드별 선택(시작 빠름 vs 실행 빠름) — 도메인 근거(반복개발·REPL·DSL 임베드). 안 갈라지는 아키텍처: 프론트(lex→parse→expand)는 공유, 백엔드만 분기(LLVM native / 인터프리터가 post-expand 코어 IR 해석) — Luau 가 바이트코드 하나를 원천으로 두는 규율과 같음. 정직한 비용: 인터프리터는 실행 느림, 백엔드 둘 유지(대신 레퍼런스 인터프리터 = native 검증 오라클). 참고: 초기 평가에서 "expand 와 충돌·결을 거스름"이라 반대했으나 Luau 선례로 철회 — expand 를 공용 프론트에 두면 해소됨. 미정: 코어 IR 표기, none-native 의 FFI·워커·자동양보 정합, 부분 native.


**7/24/2026** Interception 문법 확정 (스텁 → 완성). 전 → 후: `interception name {}:(target):( when set {value} )` 스텁 → `interception NAME ( when 상황 {인자}:(내용); … )`. 결정: (1) **target 제거** — 독립 선언, 대상은 부착 시점에 정해짐(재사용·선언/적용 분리). (2) `when 상황` = 메타테이블식 훅, 상황은 고정목록 아님(연산이 emit, when이 받음 — 내부 모델), 다루는 상황만 가로채고 나머지 통과(표현 지향·비용 정직). (3) **부착은 job**(`#이름`) — 기존 job 데코레이터 모델(ref가 = 바꾸듯)과 정합, Yulist 프리셋도 같은 훅 묶음. 값 내는 훅은 return. 버린 것: target 파라미터. 미정(→ UNKNOWN⑦): 상황별 인자 계약, set 키 유무, 중첩 순서. + data.md #11 통찰(FuncKind = 자연어의 품사).

**7/28/2026** 인자함수를 아주 간단히 정리했다. 문법 섹션에 인자함수 개념 전체 정리 섹션에서 볼수 있다.

**7/24/2026** 탈출 모델 level → label 변경. 전 → 후: `return/break/continue` 가 숫자 `level`(몇 국 바깥) → **라벨**(이름으로 목적지 국). 대상에 naming 으로 라벨(`while outer {..}`, `func find result {..}`), 탈출은 **`to` 국**으로 가리킴(`break to outer;`, `return (값) to result;`). `to` 없으면 가장 가까운 대상(기존 기본값 유지). `to` 는 셋이 공유하는 국. 왜: level 은 국 추가·삭제 시 숫자가 어긋나는 실재 단점이 있었음 — 라벨은 리팩터 안전·가독 우위, 원칙 3(명시)도 유지(선례 Java/Rust/Kotlin). 버린 것: 숫자 level·`level = N` 인자. 연동 수정: 국 실행순서 주석, 내장함수 교차참조, FuncKind 규칙("label 이 가리키는 국의 kind 를 읽음").


**7/24/2026** 모듈 완성 + 팀 + for 제거. (1) **모듈**: 전 → 후: TODO 스텁 → 확정. id 는 `setId ID;`(naming, 없으면 파일명), 모듈이 내주는 건 **`#main` 이 가리키는 것**(main 이 함수·변수·Yulist 뭐든 되는 이유가 이것), `take 아이디 as 이름` 으로 그 #main 을 가져옴. `#main` 함수의 FuncKind 가 **MANIF(선언문)** 이면 함수 대신 **그 선언문이 반환하는 것**을 내줌. `Module` 은 내장 자료 구조(생성 `module 이름();`), setId 없으면 Module 타입을 #main 으로 두고 그 이름이 id. `#module.모듈이름` job 으로 함수를 모듈에 소속시킴(`Pointer.newPoint()`). `import` 는 Module 안의 한 객체를 가져오는 기능으로 역할 확정. 버린 것: 기존 `#export`/`#module` 잡 아이디어, "import 없으면 전부 가져옴" 규칙. (2) **팀**: 같은 컴포넌트를 공유하는 단위 = 팀, 자료구조 간 관계를 명확히 하려고 도입, 어디서나 공유하는 컴포넌트는 `#public`. 팀 선언 문법은 아직 없음. (3) **for 제거**: while 과 하는 일이 같아 #10 위반 — 순회는 나중에 `forEach` 류 Yulist 순회 함수로. 버린 것: `for` 문. + TODO 에 "소국을 인자함수 맨 위에 명시" 추가, UNKNOWN⑦ 에 직접 emit 허용 여부·훅 return 흐름 추가.


**7/24/2026** kind 규칙 추가: **같은 FuncKind 는 같은 형식(슬롯 모양)을 유지한다.** 왜: FuncKind = 자연어의 품사(data.md #11)이므로 같은 품사끼리 활용 형식이 같아야 한다(영어가 명사에 `to` 라는 같은 형식을 쓰듯). 하나를 배우면 그 품사 전체가 읽히고, 커스텀 제어구조·프로젝트 방언이 "새 언어"가 아니라 "새 어휘"가 되는 근거. 어긋나면 컴파일 에러(판정 세부는 func 문법 때). data.md #11 에도 실질적 귀결로 반영. 함께: 809줄의 "표준 라이브러리(`tenn`)" 오기 정정 — `tenn` 은 라이브러리 이름이 아니라 `using` 이 받는 언어·모드 세팅. (표준 라이브러리는 §1 "범위는 거의 모든 것을 지원" 한 줄만 있고, 1-E `tenn --library` 는 라이브러리 관리 CLI 이지 표준 라이브러리 서술이 아니다.)


**7/24/2026** (복구) 모듈 섹션을 새로 쓰면서 기존 내용을 확인하지 않고 덮어써 날아간 것들 복원. (1) **"표준 라이브러리는 take 하지 않아도 된다"** 복원 — 왜: take 를 강제하면 Python 처럼 코드 흐름을 끊고 맨 윗줄로 올라가 import 를 추가해야 한다(점프 싫어함 — 정보 굽기의 기각 사유와 동일). §1 에는 한 줄 요약 + 1-C 모듈로 링크. (2) "모듈을 take 했을 때 자동 실행되는 코드 표시 job" TODO 복원. (3) None-Native 중복 정리 — 기존 `### None-Native`(`using none-native;`) 초안이 남은 채 새 `## None-Native (인터프리터 모드)`(`using tenn-none-native;`)를 따로 만들어 문법이 어긋나 있었음 → 옛 초안 삭제, 확정본으로 일원화. 교훈: 섹션을 다시 쓸 때는 기존 내용을 먼저 읽고 병합할 것.

**8/*/2026** SubStackTurner 이라는 표준 라이브러리와 여러가지 라이브러리 추가

-- *lmp*
-- *tenn*
