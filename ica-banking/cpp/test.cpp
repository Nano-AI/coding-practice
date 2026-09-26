// Runs ../cases.txt against Bank in solution.h. Usage: ./test [cases.txt]
#include "solution.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

static std::string trim(const std::string& s) {
    auto b = s.find_first_not_of(" \t\r\n"), e = s.find_last_not_of(" \t\r\n");
    return b == std::string::npos ? "" : s.substr(b, e - b + 1);
}
static std::string fmt(bool b) { return b ? "true" : "false"; }
static std::string fmt(const std::optional<long long>& v) { return v ? std::to_string(*v) : "null"; }
static std::string fmt(const std::optional<std::string>& v) { return v ? *v : "null"; }
static std::string fmt(const std::vector<std::string>& v) {
    std::string s = "[";
    for (size_t i = 0; i < v.size(); ++i) { if (i) s += ","; s += v[i]; }
    return s + "]";
}

int main(int argc, char** argv) {
    std::string path = argc > 1 ? argv[1] : "../cases.txt";
    std::ifstream in(path);
    if (!in) { std::cerr << "cannot open " << path << "\n"; return 2; }
    auto bank = std::make_unique<Bank>();
    std::string line;
    int lineno = 0, pass = 0, fail = 0;
    while (std::getline(in, line)) {
        ++lineno;
        if (auto h = line.find('#'); h != std::string::npos) line = line.substr(0, h);
        auto arrow = line.find("=>");
        std::string lhs = arrow == std::string::npos ? line : line.substr(0, arrow);
        std::string expected = arrow == std::string::npos ? "" : trim(line.substr(arrow + 2));
        std::istringstream ss(lhs);
        std::string op;
        if (!(ss >> op)) continue;
        if (op == "reset") { bank = std::make_unique<Bank>(); continue; }
        long long ts; ss >> ts;
        std::string got;
        if (op == "create_account") { std::string id; ss >> id; got = fmt(bank->create_account(ts, id)); }
        else if (op == "top_up") { std::string id; long long amt; ss >> id >> amt; got = fmt(bank->top_up(ts, id, amt)); }
        else if (op == "consume") { std::string id; long long amt; ss >> id >> amt; got = fmt(bank->consume(ts, id, amt)); }
        else if (op == "top_spenders") { int n; ss >> n; got = fmt(bank->top_spenders(ts, n)); }
        else if (op == "transfer") { std::string s, d; long long amt; ss >> s >> d >> amt; got = fmt(bank->transfer(ts, s, d, amt)); }
        else if (op == "accept_transfer") { std::string id, tid; ss >> id >> tid; got = fmt(bank->accept_transfer(ts, id, tid)); }
        else if (op == "merge_accounts") { std::string a, b; ss >> a >> b; got = fmt(bank->merge_accounts(ts, a, b)); }
        else if (op == "get_balance") { std::string id; long long at; ss >> id >> at; got = fmt(bank->get_balance(ts, id, at)); }
        else { std::cerr << "line " << lineno << ": unknown op " << op << "\n"; return 2; }
        if (got == expected) ++pass;
        else { ++fail; std::cout << "FAIL line " << lineno << ": " << trim(lhs) << " => got " << got << ", expected " << expected << "\n"; }
    }
    std::cout << pass << " passed, " << fail << " failed\n";
    return fail ? 1 : 0;
}
