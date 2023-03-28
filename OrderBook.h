#pragma once
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string>
#include <vector>

class OrderBook
{
    public:
    /** construct, reading a csv data file */
        OrderBook(std::string filename);
    /** return vector of all know products in the dataset*/
        std::vector<std::string> getKnownProducts();
    /** return vector of Orders according to the sent filters*/
        std::vector<OrderBookEntry> getOrders(OrderBookType type, 
                                              std::string product, 
                                              std::string timestamp);

        /** returns the earliest time in the orderbook*/
        std::string getEarliestTime();
        /** returns the next time after the 
         * sent time in the orderbook  
         * If there is no next timestamp, wraps around to the start
         * */
        std::string getNextTime(std::string timestamp);
        std::string getLastTime(std::string timestamp);
        // std::vector<std::string> getPassedTime(std::string timestamp);
        std::vector<std::string> getsettedTime(std::vector<std::string>& timeLine, size_t timeover);
        void insertOrder(OrderBookEntry& order);
        OrderBookType getStrategy(   std::vector <OrderBookEntry>& entries,
                                        std::vector<std::string>& timeLine, 
                                        OrderBookType orderType, 
                                        std::string product,
                                        size_t timeover  );
        std::vector<OrderBookEntry> matchAsksToBids(std::string product, std::string timestamp);
        static void getMaxAmount(std::vector<OrderBookEntry>& entries, OrderBookEntry& bot);
        static double getHighPrice(std::vector<OrderBookEntry>& orders);
        static double getLowPrice(std::vector<OrderBookEntry>& orders);
        static double getMeanPrice(std::vector<OrderBookEntry>& orders);
        double predictMin(std::vector<std::string>& timeLine, OrderBookType orderType, std::string product, size_t timeover);
        double predictMax(std::vector<std::string>& timeLine, OrderBookType orderType, std::string product, size_t timeover);
        double getAvg(std::vector<std::string>& timeLine, OrderBookType orderType, std::string product, size_t timeover);

        static double getSpread(std::vector<OrderBookEntry>& entries_ask, std::vector<OrderBookEntry>& entries_bid);
        static double getavg(std::vector<double>& passedNum);
        static double getProfit(std::vector <OrderBookEntry>& sales,std::vector <OrderBookEntry>& entries);

    private:
        std::vector<OrderBookEntry> orders;


};
