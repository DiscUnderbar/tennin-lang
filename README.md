# The Tennin Programming Language

A language that expresses everything through real functions `():{}`, countries, and functions. Calling it a functional language would be a bit of a stretch — it introduces two new concepts\
0IOP and ROP, which are explained in the [design document](tennin-spec.ko.md).

- **Form 1** `"aa"; "bb"` — Bare values. The values are passed as arguments. (Shorthand for `("aa"; "bb"):{}`.)
- **Form 2** `("aa"; "bb")` — Values wrapped in parentheses, separated by `;`. Passed as arguments.
- **Form 2-2** `{body}` — Body only, with no arguments. This is `():{body}` with the empty argument list `()` dropped. Countries that take no arguments, such as `else` and `expand`, belong here.
- **Form 3 (base form)** `(args):{body}` — `()` holds the arguments, `{}` holds the body. This is the base form of a real function. The parameter and return-type slots are omitted by default (parameters are injected as small countries, and there is no notion of returning by default).

- Syntax: [tennin-spec](tennin-spec.en.md)

Hello, World!:

```tennin
using tenn;

#main func helloWorld ():(
    Line.addLine("Hello, World");
):();
```

-- *tenn*
