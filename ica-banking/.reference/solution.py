"""Reference used to validate cases.txt. Not for reading before you have solved it."""
DAY = 86_400_000


class Bank:
    def __init__(self):
        self.acc = {}        # id -> dict(bal, spent, created, hist=[(ts, bal)], inherited=[(merge_ts, acc_dict)])
        self.transfers = {}  # tid -> dict(src, dst, amount, created, state)
        self.n = 0

    def _set(self, aid, ts, bal):
        a = self.acc[aid]
        a["bal"] = bal
        a["hist"].append((ts, bal))

    def _expire(self, ts):
        due = [t for t in self.transfers.values() if t["state"] == "pending" and ts >= t["created"] + DAY]
        for t in sorted(due, key=lambda t: t["created"]):
            t["state"] = "expired"
            self._set(t["src"], t["created"] + DAY, self.acc[t["src"]]["bal"] + t["amount"])

    def create_account(self, ts, id):
        self._expire(ts)
        if id in self.acc:
            return False
        self.acc[id] = {"bal": 0, "spent": 0, "created": ts, "hist": [(ts, 0)], "inherited": []}
        return True

    def top_up(self, ts, id, amount):
        self._expire(ts)
        if id not in self.acc:
            return None
        self._set(id, ts, self.acc[id]["bal"] + amount)
        return self.acc[id]["bal"]

    def consume(self, ts, id, amount):
        self._expire(ts)
        if id not in self.acc or self.acc[id]["bal"] < amount:
            return None
        self._set(id, ts, self.acc[id]["bal"] - amount)
        self.acc[id]["spent"] += amount
        return self.acc[id]["bal"]

    def top_spenders(self, ts, n):
        self._expire(ts)
        rows = sorted(self.acc.items(), key=lambda kv: (-kv[1]["spent"], kv[0]))[:n]
        return [f"{k}({v['spent']})" for k, v in rows]

    def transfer(self, ts, src, dst, amount):
        self._expire(ts)
        if src == dst or src not in self.acc or dst not in self.acc or self.acc[src]["bal"] < amount:
            return None
        self.n += 1
        tid = f"transfer{self.n}"
        self._set(src, ts, self.acc[src]["bal"] - amount)
        self.transfers[tid] = {"src": src, "dst": dst, "amount": amount, "created": ts, "state": "pending"}
        return tid

    def accept_transfer(self, ts, id, transfer_id):
        self._expire(ts)
        t = self.transfers.get(transfer_id)
        if not t or t["state"] != "pending" or t["dst"] != id or id not in self.acc:
            return False
        t["state"] = "accepted"
        self._set(id, ts, self.acc[id]["bal"] + t["amount"])
        self.acc[t["src"]]["spent"] += t["amount"]
        return True

    def merge_accounts(self, ts, id1, id2):
        self._expire(ts)
        if id1 == id2 or id1 not in self.acc or id2 not in self.acc:
            return False
        a2 = self.acc.pop(id2)
        self._set(id1, ts, self.acc[id1]["bal"] + a2["bal"])
        self.acc[id1]["spent"] += a2["spent"]
        self.acc[id1]["inherited"].append((ts, a2))
        for t in self.transfers.values():
            if t["state"] == "pending":
                if t["src"] == id2:
                    t["src"] = id1
                if t["dst"] == id2:
                    t["dst"] = id1
        return True

    def _bal_at(self, a, t):
        bal = None
        for ts, b in a["hist"]:
            if ts <= t:
                bal = b
            else:
                break
        if bal is None:
            return None
        for merge_ts, other in a["inherited"]:
            if t < merge_ts and t >= other["created"]:
                ob = self._bal_at(other, t)
                if ob is not None:
                    bal += ob
        return bal

    def get_balance(self, ts, id, time_at):
        self._expire(ts)
        if id not in self.acc or time_at < self.acc[id]["created"]:
            return None
        return self._bal_at(self.acc[id], time_at)
