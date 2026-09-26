# oa-prep

Problems that recur in online assessments, kept as runnable specs so they can be re-solved cold.
One folder per problem. No company names anywhere in this repo; the vault's pipeline note maps problems to firms.

```
<problem>/
  PROBLEM.md      the spec, levels, edge cases
  cases.txt       sequential test cases, one Bank/Solution instance per `reset`
  py/solution.py  solve here in Python        python3 py/run.py
  cpp/solution.h  solve here in C++           make -C cpp test   (AddressSanitizer + UBSan on)
  .reference/     a reference solution used only to validate cases.txt. Do not open before solving.
TEMPLATE/         copy to start a new problem
```

## Workflow

1. `cp -r TEMPLATE <new-problem>`; write PROBLEM.md, then cases.txt (write the cases before the code).
2. Solve in `py/solution.py` or `cpp/solution.h`. Run until green.
3. Re-solving later: move the old file to `attempts/YYYY-MM-DD.<ext>` first, then solve blank.

## cases.txt format

```
# comment
reset                              # fresh instance
op arg1 arg2 ... => expected       # args are space-separated, first arg is the timestamp
```
Expected values: `true` / `false`, an integer, a string, `null`, or a list as `[a,b,c]` (`[]` when empty).
Cases run in order on one instance until the next `reset`, so timestamps only need to increase within a block.

## Adding an op to a harness

Python: add the op and its argument converters to `OPS` in `py/run.py`.
C++: add an `else if (op == "...")` branch in `cpp/test.cpp`.
