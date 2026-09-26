// Runs ../cases.txt against Solution in solution.h, one section per "# ..." header line.
// Usage: ./test [cases.txt] [level]      e.g. ./test ../cases.txt 2      (or: make test L=2)
#include "solution.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <unistd.h>
#include <vector>

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

static const bool TTY = isatty(1);
static std::string paint(const char* code, const std::string& s) { return TTY ? std::string("\033[") + code + "m" + s + "\033[0m" : s; }
static std::string green(const std::string& s) { return paint("32", s); }
static std::string red(const std::string& s) { return paint("31", s); }
static std::string bold(const std::string& s) { return paint("1", s); }
static std::string dim(const std::string& s) { return paint("2", s); }

struct Section { std::string name; int pass = 0, fail = 0; };

static bool matches(const std::string& header, const std::string& level) {
    if (level.empty()) return true;
    std::istringstream ss(header);
    std::string w, n;
    ss >> w >> n;
    return w == "Level" && n == level;
}
static void close_section(const Section& s) {
    std::string line = std::to_string(s.pass) + "/" + std::to_string(s.pass + s.fail) + " passed";
    std::cout << (s.fail ? red("  ✗ " + line) : green("  ✓ " + line)) << "\n\n";
}

int main(int argc, char** argv) {
    std::string path = argc > 1 ? argv[1] : "../cases.txt";
    std::string level = argc > 2 ? argv[2] : "";
    std::ifstream in(path);
    if (!in) { std::cerr << "cannot open " << path << "\n"; return 2; }
    auto bank = std::make_unique<Solution>();
    std::vector<Section> secs;
    bool active = level.empty();
    std::string line;
    int lineno = 0;
    while (std::getline(in, line)) {
        ++lineno;
        std::string t = trim(line);
        if (t.empty()) continue;
        if (t[0] == '#') {
            std::string h = trim(t.substr(1));
            if (h.empty()) continue;
            active = matches(h, level);
            if (active) {
                if (!secs.empty()) close_section(secs.back());
                secs.push_back({h});
                std::cout << bold(h) << "\n";
            }
            continue;
        }
        if (!active) continue;
        if (auto hsh = line.find('#'); hsh != std::string::npos) line = line.substr(0, hsh);
        auto arrow = line.find("=>");
        std::string lhs = trim(arrow == std::string::npos ? line : line.substr(0, arrow));
        std::string expected = arrow == std::string::npos ? "" : trim(line.substr(arrow + 2));
        std::istringstream ss(lhs);
        std::string op;
        if (!(ss >> op)) continue;
        if (op == "reset") { bank = std::make_unique<Solution>(); continue; }
        if (secs.empty()) { secs.push_back({"(no header)"}); std::cout << bold("(no header)") << "\n"; }
        long long ts; ss >> ts;
        std::string got;
        if (op == "ping") { got = fmt(bank->ping(ts)); }
        else { std::cerr << "line " << lineno << ": unknown op " << op << "\n"; return 2; }
        if (got == expected) {
            ++secs.back().pass;
            std::cout << green("  ✓ ") << dim(lhs + " → " + expected) << "\n";
        } else {
            ++secs.back().fail;
            std::cout << red("  ✗ ") << "line " << lineno << ": " << lhs << " → got " << red(got) << ", expected " << green(expected) << "\n";
        }
    }
    if (secs.empty()) { std::cerr << "no section matching Level " << level << "\n"; return 2; }
    close_section(secs.back());
    int fails = 0;
    std::cout << bold("Summary") << "\n";
    for (const auto& s : secs) {
        std::printf("  %-44s %3d/%-3d %s\n", s.name.c_str(), s.pass, s.pass + s.fail, s.fail ? red("✗").c_str() : green("✓").c_str());
        fails += s.fail;
    }
    return fails ? 1 : 0;
}
