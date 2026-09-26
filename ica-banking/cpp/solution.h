// Solve here. Spec: ../PROBLEM.md. Run: make test  (builds with -fsanitize=address,undefined)
#pragma once
#include <map>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

constexpr long long DAY = 86'400'000;

class Bank {
public:
    // Level 1
    bool create_account(long long ts, const std::string& id) { return false; }
    std::optional<long long> top_up(long long ts, const std::string& id, long long amount) { return std::nullopt; }
    std::optional<long long> consume(long long ts, const std::string& id, long long amount) { return std::nullopt; }
    // Level 2
    std::vector<std::string> top_spenders(long long ts, int n) { return {}; }
    // Level 3
    std::optional<std::string> transfer(long long ts, const std::string& src, const std::string& dst, long long amount) { return std::nullopt; }
    bool accept_transfer(long long ts, const std::string& id, const std::string& transfer_id) { return false; }
    // Level 4
    bool merge_accounts(long long ts, const std::string& id1, const std::string& id2) { return false; }
    std::optional<long long> get_balance(long long ts, const std::string& id, long long time_at) { return std::nullopt; }
};
