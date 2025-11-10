#ifndef NETWORK_H
#define NETWORK_H

#include <vector>

struct BtcData {
  int price;
  float price_change_percentage;
};

BtcData fetch_btc_data();

#endif