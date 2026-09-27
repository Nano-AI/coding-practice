# oa-prep

Problems that recur in online assessments, kept as runnable specs so they can be re-solved cold.
One folder per problem. No company names anywhere in this repo; the vault's pipeline note maps problems to firms.

```
<problem>/
  PROBLEM.md      the spec, levels, edge cases
  cases.txt       sequential test cases, one Bank/Solution instance per `reset`
  py/solution.py  solve here in Python        python3 py/run.py            one level: python3 py/run.py --level 2
  cpp/solution.h  solve here in C++           make -C cpp test             one level: make -C cpp test L=2
                  (a solution.cpp next to solution.h is linked automatically; sanitizers on)
  .reference/     a reference solution used only to validate cases.txt. Do not open before solving.
TEMPLATE/         copy to start a new problem
cse333/           course head-start exercises (own Makefile + test.c each, not the cases.txt harness)
```

## Workflow

1. `cp -r TEMPLATE <new-problem>`; write PROBLEM.md, then cases.txt (write the cases before the code).
2. Solve in `py/solution.py` or `cpp/solution.h`. Run until green.
3. Re-solving later: move the old file to `attempts/YYYY-MM-DD.<ext>` first, then solve blank.

## cases.txt format

```
# Level 1            <- a comment line starting a block is a section header; "Level N" enables --level N / L=N
reset                              # fresh instance
op arg1 arg2 ... => expected       # args are space-separated, first arg is the timestamp
```
Expected values: `true` / `false`, an integer, a string, `null`, or a list as `[a,b,c]` (`[]` when empty).
Cases run in order on one instance until the next `reset`, so timestamps only need to increase within a block.

## Adding an op to a harness

Python: add the op and its argument converters to `OPS` in `py/run.py`.
C++: add an `else if (op == "...")` branch in `cpp/test.cpp`.
