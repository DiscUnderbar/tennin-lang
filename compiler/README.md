# CTennin — Tennin compiler (v0.2)

A first, deliberately tiny **native** compiler for Tennin. It exists to answer one
question honestly: *does any of this actually run, or is it jewels on paper?*

Right now the answer is: **Tennin programs with variables, integer arithmetic, and
printing compile through LLVM to a native binary and run.**

## Pipeline

```
hello.tenn ──(CTennin.cpp)──▶ LLVM IR (.ll) ──(lower.py / llvmlite)──▶ object (.o) ──(g++)──▶ native exe
```

- `CTennin.cpp` — the compiler proper: lexer → parser → **textual LLVM IR** emitter. No LLVM linking; it just prints `.ll`. This is the "LLVM IR 직접" path without the LLVM C++ API dependency.
- `lower.py` — uses **llvmlite** (bundled LLVM) to parse/verify the IR and emit a native object file.
- `g++` — links the object into an executable.
- `build.sh` — runs all three steps and prints the generated IR.

## Build & run

```bash
bash build.sh examples/hello.tenn /tmp/hello
/tmp/hello        # -> Hello, World

bash build.sh examples/arith.tenn /tmp/arith
/tmp/arith        # -> 14 / 30 / done
```

Requires: `g++`, `python3`, and `llvmlite` (`pip install llvmlite --break-system-packages`).

## Supported subset (v0.2)

Everything else is rejected with a line-numbered error (so it never *silently* pretends to work):

- `using tenn;` — parsed, ignored.
- `#main` — job marker; makes the following `func` the entry point.
- `func NAME ():( <stmts> ):();` — func decl. Params/return slots must be empty `()` for now.
- `var NAME :: int = <expr>;` — integer variable declaration (`const` also parses).
- `<expr>` — int literals, variables, `+ - * /`, parentheses, usual precedence.
- `print(<expr>)` → `printf %d`; `print("...")` / `Line.addLine("...")` → `puts`.
- `@... ` line comments; string literals with `\n \t \\ \"` escapes.

Checked errors: unknown builtin, undefined variable, redeclaration, non-`int` type — all line-numbered.

## What this proves (and what it does NOT)

Proves: the unusual `{}:()` / slot / `#job` surface lexes and parses, and the toolchain
(IR → native) is real and reproducible.

Does **not** yet touch the parts the design bets the most on — and that are the actual
risk (see `../data.md`, `../UNKOWN.md`):

- 원칙 0 in earnest: a **user-defined** control structure via 국/소국 injection.
- `expand` (compile-time execution) and zero-cost control flow.
- 타입 매직 (type-directed evaluation), Yulist, arenas / memory safety.

Those are the next slices. This v0.2 is the floor, not the claim.
