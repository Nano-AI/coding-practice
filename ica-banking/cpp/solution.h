// Solve here. Spec: ../PROBLEM.md. Run: make test  (builds with -fsanitize=address,undefined)
#pragma once
#include <map>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
#include <set>

constexpr long long DAY = 86'400'000;
using user_id = std::string;

class Bank {
public:
    // Level 1
    bool create_account(long long ts, const std::string& id);
    std::optional<long long> top_up(long long ts, const std::string& id, long long amount);
    std::optional<long long> consume(long long ts, const std::string& id, long long amount);
    // Level 2
    std::vector<std::string> top_spenders(long long ts, int n);
    // Level 3
    std::optional<std::string> transfer(long long ts, const std::string& src, const std::string& dst, long long amount) { return std::nullopt; }
    bool accept_transfer(long long ts, const std::string& id, const std::string& transfer_id) { return false; }
    // Level 4
    bool merge_accounts(long long ts, const std::string& id1, const std::string& id2) { return false; }
    std::optional<long long> get_balance(long long ts, const std::string& id, long long time_at) { return std::nullopt; }
private:
    using balance_map = std::unordered_map<std::string, long long>;
    using balance_order = std::multiset<std::pair<long long, const std::string*>>;

    balance_map balance;
    balance_map consumption;
    balance_order order;
    std::unordered_map<const std::string*, balance_order::iterator> balance_to_order;
};
