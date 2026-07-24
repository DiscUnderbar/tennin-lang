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


-- *lmp*
-- *tenn*
