# ICA banking ledger (four levels)

Industry-coding-assessment style: one class, methods added level by level, each level's tests build on the last.
Reconstructed from public descriptions of a widely used assessment task; the exact rules below are this repo's own.
90 minutes for all four levels; expect to finish two or three.

All timestamps are integers in milliseconds and strictly increase across calls. Amounts are positive integers.
Every method first applies any transfer expirations that are due (see Level 3).

## Level 1 — accounts

- `create_account(ts, id) -> bool`: `false` if the account already exists.
- `top_up(ts, id, amount) -> int | null`: adds `amount`, returns the new balance; `null` if no such account.
- `consume(ts, id, amount) -> int | null`: subtracts `amount`, returns the new balance; `null` if no such account or balance < amount.

## Level 2 — ranking

- `top_spenders(ts, n) -> list[str]`: up to `n` accounts as `"id(total)"`, where `total` is the value that has left the account through `consume` plus outgoing transfers that were accepted. Pending or expired transfers do not count. Sort by total descending, then id ascending. Accounts with total 0 are included. `n = 0` returns `[]`.

## Level 3 — transfers with expiry

- `transfer(ts, src, dst, amount) -> str | null`: `null` if `src == dst`, either account is missing, or `src` balance < amount. Otherwise the amount leaves `src` immediately (held), a pending transfer is created, and its id is returned: `"transfer" + k`, where `k` counts successful transfers so far (`transfer1`, `transfer2`, ...). Failed calls do not consume an id.
- `accept_transfer(ts, id, transfer_id) -> bool`: `false` if the transfer is unknown, not pending, or `id` is not its destination. On success the amount is credited to the destination and counts toward the source's spend total.
- Expiry: a pending transfer expires when any call arrives with `ts >= created_at + 86_400_000` (24 h). The held amount returns to the source, recorded in the source's balance history at `created_at + 86_400_000`. A transfer accepted at exactly `created_at + 86_400_000` has already expired.

## Level 4 — merges and history

- `merge_accounts(ts, id1, id2) -> bool`: `false` if `id1 == id2` or either is missing. Otherwise `id2` merges into `id1`: balances add, spend totals add, pending transfers that name `id2` as source or destination now name `id1`, and `id1` inherits `id2`'s balance history. `id2` no longer exists.
- `get_balance(ts, id, time_at) -> int | null`: the balance of `id` right after all operations with timestamp `<= time_at`. `null` if `id` does not exist now or did not exist at `time_at`. For a `time_at` before a merge, the result includes the balance the merged-in account had at that time. Held (pending) amounts are not part of a balance.

## Edge cases the tests hit

- Duplicate create, operations on missing accounts, overdraw.
- Ties in `top_spenders`; `n` larger than the account count; `n = 0`.
- Transfer to self, to a missing account, exceeding balance; accepting from the wrong account; accepting twice; accepting after expiry; the exact expiry boundary.
- Merge with self or a missing account; operating on the merged-away id; balances before, at and after the merge; a pending transfer whose source or destination was merged away; expiry after the source was merged away.
