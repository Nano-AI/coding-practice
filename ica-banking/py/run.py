"""Runs ../cases.txt against solution.Bank, one section per "# ..." header line.
Usage: python3 run.py [cases.txt] [--level N] [--solution other.py]
"""
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

TTY = sys.stdout.isatty()


def paint(code, s):
    return f"\033[{code}m{s}\033[0m" if TTY else s


green, red, bold, dim = (lambda s: paint(32, s)), (lambda s: paint(31, s)), (lambda s: paint(1, s)), (lambda s: paint(2, s))


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


def matches(header, level):
    if not level:
        return True
    toks = header.split()
    return len(toks) >= 2 and toks[0] == "Level" and toks[1] == level


def close_section(sec):
    line = f"{sec['pass']}/{sec['pass'] + sec['fail']} passed"
    print((red("  ✗ " + line) if sec["fail"] else green("  ✓ " + line)) + "\n")


def main(argv):
    args = list(argv)
    cases, sol, level = HERE.parent / "cases.txt", HERE / "solution.py", ""
    for flag in ("--solution", "--level"):
        if flag in args:
            i = args.index(flag)
            val = args[i + 1]
            del args[i:i + 2]
            if flag == "--solution":
                sol = pathlib.Path(val)
            else:
                level = val
    if args:
        cases = pathlib.Path(args[0])
    Bank = load(sol)
    bank, secs, active = Bank(), [], not level
    for n, raw in enumerate(open(cases), 1):
        t = raw.strip()
        if not t:
            continue
        if t.startswith("#"):
            h = t[1:].strip()
            if not h:
                continue
            active = matches(h, level)
            if active:
                if secs:
                    close_section(secs[-1])
                secs.append({"name": h, "pass": 0, "fail": 0})
                print(bold(h))
            continue
        if not active:
            continue
        line = raw.split("#", 1)[0].strip()
        lhs, _, exp = line.partition("=>")
        lhs, exp = lhs.strip(), exp.strip()
        toks = lhs.split()
        op = toks[0]
        if op == "reset":
            bank = Bank()
            continue
        if not secs:
            secs.append({"name": "(no header)", "pass": 0, "fail": 0})
            print(bold("(no header)"))
        call_args = [c(tk) for c, tk in zip(OPS[op], toks[1:])]
        try:
            got = fmt(getattr(bank, op)(*call_args))
        except Exception as e:  # keep going so later sections still report
            got = f"<{type(e).__name__}: {e}>"
        if got == exp:
            secs[-1]["pass"] += 1
            print(green("  ✓ ") + dim(f"{lhs} → {exp}"))
        else:
            secs[-1]["fail"] += 1
            print(red("  ✗ ") + f"line {n}: {lhs} → got {red(got)}, expected {green(exp)}")
    if not secs:
        print(f"no section matching Level {level}", file=sys.stderr)
        return 2
    close_section(secs[-1])
    print(bold("Summary"))
    fails = 0
    for s in secs:
        print(f"  {s['name']:<44} {s['pass']:>3}/{s['pass'] + s['fail']:<3} {red('✗') if s['fail'] else green('✓')}")
        fails += s["fail"]
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
