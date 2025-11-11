#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <string>

struct CoinData {
  int price;
  float price_change_percentage;
  std::string symbol;
};

CoinData fetch_coin_data();

#endif