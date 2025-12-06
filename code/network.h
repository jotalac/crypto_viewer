#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <string>

#define GRAPH_POINTS 30
#define DOWNSAMPLE_GRAPH 10 //285 / 50

struct CoinData {
  float price;
  float price_change_percentage;
  float ath_price;
  float ath_percentage;
  std::string symbol;
};

struct GraphData {
  float min_price;
  float max_price;
  float price_history[GRAPH_POINTS];
  int data_count;

  //time fetch data
  unsigned long last_fetch = 0;
};

CoinData fetch_coin_data();

GraphData fetch_graph_data();



#endif