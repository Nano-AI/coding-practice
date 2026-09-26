"""Solve here. Spec: ../PROBLEM.md. Run: python3 run.py"""

DAY = 86_400_000


class Bank:
    def __init__(self):
        pass

    # Level 1
    def create_account(self, ts: int, id: str) -> bool:
        raise NotImplementedError

    def top_up(self, ts: int, id: str, amount: int):
        raise NotImplementedError

    def consume(self, ts: int, id: str, amount: int):
        raise NotImplementedError

    # Level 2
    def top_spenders(self, ts: int, n: int) -> list:
        raise NotImplementedError

    # Level 3
    def transfer(self, ts: int, src: str, dst: str, amount: int):
        raise NotImplementedError

    def accept_transfer(self, ts: int, id: str, transfer_id: str) -> bool:
        raise NotImplementedError

    # Level 4
    def merge_accounts(self, ts: int, id1: str, id2: str) -> bool:
        raise NotImplementedError

    def get_balance(self, ts: int, id: str, time_at: int):
        raise NotImplementedError
