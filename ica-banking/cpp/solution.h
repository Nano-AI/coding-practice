// Solve here. Spec: ../PROBLEM.md. Run: make test  (builds with -fsanitize=address,undefined)
#pragma once
#include <map>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
#include <set>
#include <queue>

constexpr long long DAY = 86'400'000;
using ll_string = std::pair<long long, const std::string*>;

class Compare {
public:
    bool operator()(const ll_string &a, const ll_string &b) const {
        if (a.first == b.first) {
            return a.second < b.second;
        }
        return a.first > b.first;
    }
};

using balance_map = std::unordered_map<std::string, long long>;
using balance_order = std::multiset<ll_string, Compare>;

struct Transfer {
    long long expiry = 0;
    const std::string* src = nullptr;
    const std::string* dst = nullptr;
    long long amount = 0;
};

using transfers_map = std::unordered_map<std::string, Transfer>;

class Bank {
public:
    // Level 1
    bool create_account(long long ts, const std::string& id);
    std::optional<long long> top_up(long long ts, const std::string& id, long long amount);
    std::optional<long long> consume(long long ts, const std::string& id, long long amount);
    // Level 2
    std::vector<std::string> top_spenders(long long ts, int n);
    // Level 3
    std::optional<std::string> transfer(long long ts, const std::string& src, const std::string& dst, long long amount);
    bool accept_transfer(long long ts, const std::string& id, const std::string& transfer_id);
    // Level 4
    bool merge_accounts(long long ts, const std::string& id1, const std::string& id2) { return false; }
    std::optional<long long> get_balance(long long ts, const std::string& id, long long time_at) { return std::nullopt; }
private:
    long long transfer_counter = 0;
    balance_map balance;
    std::unordered_map<const std::string*, long long> consumption;
    balance_order order;
    std::unordered_map<const std::string*, balance_order::iterator> balance_to_order;
    transfers_map transfers;
    std::deque<std::string> transfer_order;

    std::optional<long long> update_balance(long long ts, const std::string &id, long long amount);
    void return_expired_transfers(long long ts);
};
