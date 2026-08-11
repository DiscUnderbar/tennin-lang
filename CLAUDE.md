# CLAUDE.md

Behavioral guidelines to reduce common LLM coding mistakes. Merge with project-specific instructions as needed.

**Tradeoff:** These guidelines bias toward caution over speed. For trivial tasks, use judgment.

## 1. Think Before Coding

**Don't assume. Don't hide confusion. Surface tradeoffs.**

Before implementing:
- State your assumptions explicitly. If uncertain, ask.
- If multiple interpretations exist, present them - don't pick silently.
- If a simpler approach exists, say so. Push back when warranted.
- If something is unclear, stop. Name what's confusing. Ask.

## 2. Simplicity First

**Minimum code that solves the problem. Nothing speculative.**

- No features beyond what was asked.
- No abstractions for single-use code.
- No "flexibility" or "configurability" that wasn't requested.
- No error handling for impossible scenarios.
- If you write 200 lines and it could be 50, rewrite it.

Ask yourself: "Would a senior engineer say this is overcomplicated?" If yes, simplify.

## 3. Surgical Changes

**Touch only what you must. Clean up only your own mess.**

When editing existing code:
- Don't "improve" adjacent code, comments, or formatting.
- Don't refactor things that aren't broken.
- Match existing style, even if you'd do it differently.
- If you notice unrelated dead code, mention it - don't delete it.

When your changes create orphans:
- Remove imports/variables/functions that YOUR changes made unused.
- Don't remove pre-existing dead code unless asked.

The test: Every changed line should trace directly to the user's request.

## 4. Goal-Driven Execution

**Define success criteria. Loop until verified.**

Transform tasks into verifiable goals:
- "Add validation" → "Write tests for invalid inputs, then make them pass"
- "Fix the bug" → "Write a test that reproduces it, then make it pass"
- "Refactor X" → "Ensure tests pass before and after"

For multi-step tasks, state a brief plan:
```
1. [Step] → verify: [check]
2. [Step] → verify: [check]
3. [Step] → verify: [check]
```

Strong success criteria let you loop independently. Weak criteria ("make it work") require constant clarification.

---

**These guidelines are working if:** fewer unnecessary changes in diffs, fewer rewrites due to overcomplication, and clarifying questions come before implementation rather than after mistakes.

# 기획 관련

- 모든 기록은 까먹을 수 있다. 그러므로 세세한 것까지 `tennin-spec.md` 에 저장한다. 하나가 빠지는 순간 완성도가 낮아지고 지적할 부분이 많아진다.
- "왜"가 가장 중요하다. 특히 다른 사람이 반박할 만한 부분에는 반드시 왜 그렇게 했는지 적는다. 처음 보는 사람이 납득할 수 있어야 한다.
- "왜"를 문서 안에 녹여내야 한다. 갑자기 **왜** 이렇게 되어있는것보단. 그냥 "참고로 이거 이렇게 해야지 zevra 랑 쫌더 비슷해 져서 이렇게 했음" 이라고 해야지 그냥 "**왜**: 이 zevra 의 호완성 때문인데 이 Tennin 은 zevring 에서 지원하고 zevra 개발자들..." 이건 굳이 양을 늘린거지 "왜" 를 잘 적은게 아니다.
- 특별한 부분 (장단점이 확실히 구분되거나, 두 마리 토끼를 잡거나, 무언가를 포기했을 때) 은 실재할 때만 적는다. 단점을 적자마자 우회로로 해소하지 않는다.
- 이유에는 네 종류가 있다: 제약 / 도메인 / 고통 / 철학. 철학 하나로만 서 있는 결정은 재검토 후보다.
- 딴 일 하다 떠오른 것은 `IDEAS.md`, 모르는 것은 `UNKNOWN.md`, 미룬 것은 `TODO.md`, 바뀐 것은 `CHANGELOG.md` (날짜 / 전 → 후 / 왜 / 무엇을 버렸나) 에 적는다.
- 우리가 만든 것을 비판적으로 생각해 본다. 결정은 사람이 한다.
- 봐뀐건 CHANGELOG.md 에 저장하지 직접 문서에 봐뀐거라고 남겨놓으면 않된다. 그 봐뀐거에 "왜" 진짜 이걸 보면 알수 있는 "왜" 가 있을때만 넣어놓는다.
