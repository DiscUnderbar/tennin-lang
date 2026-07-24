#!/usr/bin/env python3
# lower.py — turn textual LLVM IR into a native object file using llvmlite (LLVM).
# usage: python3 lower.py input.ll output.o
import sys
import llvmlite.binding as llvm

def main():
    if len(sys.argv) != 3:
        print("usage: lower.py input.ll output.o", file=sys.stderr)
        sys.exit(2)
    ir = open(sys.argv[1]).read()

    # llvmlite 22 removed the generic initialize(); the native-target/asmprinter
    # registrations are still required before selecting a target.
    llvm.initialize_native_target()
    llvm.initialize_native_asmprinter()

    mod = llvm.parse_assembly(ir)   # parses + validates IR (raises on bad IR)
    mod.verify()

    target = llvm.Target.from_default_triple()
    tm = target.create_target_machine(reloc="pic", codemodel="default")
    mod.triple = tm.triple
    mod.data_layout = str(tm.target_data)

    obj = tm.emit_object(mod)
    with open(sys.argv[2], "wb") as f:
        f.write(obj)

if __name__ == "__main__":
    main()
