#include "solution.h"

bool Bank::create_account(long long ts, const std::string& id) { 
  if (this->balance.find(id) == this->balance.end()) {
    return false;
  }
  this->balance[id] = 0;
  return true;
}

std::optional<long long> Bank::top_up(long long ts, const std::string& id, long long amount) { 
  return std::nullopt; 
}

std::optional<long long> Bank::consume(long long ts, const std::string& id, long long amount) { 
  return std::nullopt; 
}

