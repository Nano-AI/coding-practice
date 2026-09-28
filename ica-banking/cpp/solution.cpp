#include "solution.h"

bool Bank::create_account(long long ts, const std::string& id) { 
  if (this->balance.find(id) != this->balance.end()) {
    return false;
  }

  this->balance[id] = 0;
  const std::string* id_c = &this->balance.find(id)->first;
  auto it = this->order.emplace(0, id_c);
  this->balance_to_order[id_c] = it;

  this->return_expired_transfers(ts);
  return true;
}

std::optional<long long> Bank::update_balance(long long ts, const std::string &id, long long amount) {
  if (this->balance.find(id) == this->balance.end()) {
    return std::nullopt;
  }

  auto id_c = &this->balance.find(id)->first;

  if (this->balance[id] + amount < 0) {
    return std::nullopt;
  }

  this->balance[id] += amount;

  if (amount < 0) {
    this->consumption[id_c] -= amount;
  }

  this->order.erase(this->balance_to_order[id_c]);
  auto it = this->order.emplace(this->consumption[id_c], id_c);
  this->balance_to_order[id_c] = it;

  return this->balance[id];
}

std::optional<long long> Bank::top_up(long long ts, const std::string& id, long long amount) { 
  this->return_expired_transfers(ts);
  return this->update_balance(ts, id, amount);
}

std::optional<long long> Bank::consume(long long ts, const std::string& id, long long amount) { 
  this->return_expired_transfers(ts);
  return this->update_balance(ts, id, -amount);
}

std::vector<std::string> Bank::top_spenders(long long ts, int n) {
  this->return_expired_transfers(ts);
  std::vector<std::string> out;
  out.reserve(n);

  auto it = order.begin();
  int i = 0;

  while (it != order.end() && i < n) {
    out.push_back(*it->second + '(' + std::to_string(it->first) + ')');
    ++i;
    ++it;
  }

  return out;
}


std::optional<std::string> Bank::transfer(long long ts, const std::string& src, const std::string& dst, long long amount) {
  this->return_expired_transfers(ts);
  if (src == dst || this->balance.find(src) == this->balance.end() || this->balance.find(dst) == this->balance.end()) {
    return std::nullopt;
  }

  auto a = this->consume(ts, src, amount);
  if (a == std::nullopt) {
    return std::nullopt;
  }

  auto src_id = &this->balance.find(src)->first;
  auto dst_id = &this->balance.find(dst)->first;

  std::string transfer_id = "transfer" + std::to_string(++transfer_counter);

  this->transfers.emplace(transfer_id, Transfer{ts + DAY, src_id, dst_id, amount});
  this->transfer_order.push_back(transfer_id);
  // this->transfers[transfer_id].iter = this->transfer_order.begin();
  // this->transfer_order.push(this->transfers.find(transfer_id));

  return this->transfers.find(transfer_id)->first;
}

bool Bank::accept_transfer(long long ts, const std::string& id, const std::string& transfer_id) {
  this->return_expired_transfers(ts);
  if (this->transfers.find(transfer_id) == this->transfers.end()) {
    return false;
  }
  auto t = this->transfers[transfer_id];
  if (t.expiry < ts) {
    return false;
  }
  if (id != *t.dst) {
    return false;
  }
  this->top_up(ts, *t.dst, t.amount);
  this->transfers.erase(transfer_id);
  return true;
}

void Bank::return_expired_transfers(long long ts) {
  while (!this->transfer_order.empty()) {
    auto first = this->transfer_order.front();
    auto curr_transfer = this->transfers.find(first);

    if (curr_transfer != this->transfers.end()) {
      auto transfer = this->transfers.find(first)->second;
      if (transfer.expiry > ts) break;

      auto transfer_id = this->transfers.find(first)->first;

      if (transfers.find(transfer_id) != transfers.end()) {
        this->update_balance(ts, *transfer.src, transfer.amount);
        this->consumption[transfer.src] -= transfer.amount;
      }
      this->transfers.erase(transfer_id);
    }

    this->transfer_order.pop_front();
  }
}
