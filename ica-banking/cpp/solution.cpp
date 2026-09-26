#include "solution.h"

bool Bank::create_account(long long ts, const std::string& id) { 
  if (this->balance.find(id) != this->balance.end()) {
    return false;
  }

  this->balance[id] = 0;
  const std::string* id_c = &this->balance.find(id)->first;
  auto it = this->order.emplace(0, id_c);
  this->balance_to_order[id_c] = it;

  return true;
}

std::optional<long long> Bank::top_up(long long ts, const std::string& id, long long amount) { 
  if (this->balance.find(id) == this->balance.end()) {
    return std::nullopt;
  }
  this->balance[id] += amount;
  this->consumption[id] += amount;
  return this->balance[id];
}

std::optional<long long> Bank::consume(long long ts, const std::string& id, long long amount) { 
  if (this->balance.find(id) == this->balance.end()) {
    return std::nullopt;
  }
  this->balance[id] -= amount;
  this->consumption[id] += amount;
  return this->balance[id];
}

std::vector<std::string> Bank::top_spenders(long long ts, int n) {
  auto it = order.rbegin();
  size_t i = 0;
}

