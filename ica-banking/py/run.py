"""Runs ../cases.txt against solution.Bank.  python3 run.py [cases.txt] [--solution other.py]"""
import importlib.util
import pathlib
import sys

HERE = pathlib.Path(__file__).parent
OPS = {  # op -> argument converters, in order (first is always the timestamp)
    "create_account": (int, str),
    "top_up": (int, str, int),
    "consume": (int, str, int),
    "top_spenders": (int, int),
    "transfer": (int, str, str, int),
    "accept_transfer": (int, str, str),
    "merge_accounts": (int, str, str),
    "get_balance": (int, str, int),
}


def fmt(v):
    if v is None:
        return "null"
    if isinstance(v, bool):
        return "true" if v else "false"
    if isinstance(v, list):
        return "[" + ",".join(v) + "]"
    return str(v)


def load(path):
    spec = importlib.util.spec_from_file_location("sol", path)
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod.Bank


def main(argv):
    cases = HERE.parent / "cases.txt"
    sol = HERE / "solution.py"
    args = list(argv)
    if "--solution" in args:
        i = args.index("--solution")
        sol = pathlib.Path(args[i + 1])
        del args[i:i + 2]
    if args:
        cases = pathlib.Path(args[0])
    Bank = load(sol)
    bank, passed, failed = Bank(), 0, 0
    for n, raw in enumerate(open(cases), 1):
        line = raw.split("#", 1)[0].strip()
        if not line:
            continue
        lhs, _, exp = line.partition("=>")
        toks = lhs.split()
        op = toks[0]
        if op == "reset":
            bank = Bank()
            continue
        conv = OPS[op]
        call_args = [c(t) for c, t in zip(conv, toks[1:])]
        try:
            got = fmt(getattr(bank, op)(*call_args))
        except Exception as e:  # keep going so later levels still report
            got = f"<{type(e).__name__}: {e}>"
        if got == exp.strip():
            passed += 1
        else:
            failed += 1
            print(f"FAIL line {n}: {lhs.strip()} => got {got}, expected {exp.strip()}")
    print(f"{passed} passed, {failed} failed")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
