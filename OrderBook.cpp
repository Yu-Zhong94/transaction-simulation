#include "OrderBook.h"
#include "CSVReader.h"
#include <map>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <cmath>

/** construct, reading a csv data file */
OrderBook::OrderBook(std::string filename)
{
    orders = CSVReader::readCSV(filename);
}

/** return vector of all know products in the dataset*/
std::vector<std::string> OrderBook::getKnownProducts()
{
    std::vector<std::string> products;

    std::map<std::string,bool> prodMap;

    for (OrderBookEntry& e : orders)
    {
        prodMap[e.product] = true;
    }
    
    // now flatten the map to a vector of strings
    for (auto const& e : prodMap)
    {
        products.push_back(e.first);
    }

    return products;
}


/** return vector of Orders according to the sent filters*/
std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type, 
                                        std::string product, 
                                        std::string timestamp)
{
    std::vector<OrderBookEntry> orders_sub;
    for (OrderBookEntry& e : orders)
    {
        if (e.orderType == type && 
            e.product == product && 
            e.timestamp == timestamp )
            {
                orders_sub.push_back(e);
            }
    }
    return orders_sub;
}

//get max amount among orders to buy or sell, pass amount and price to advisorbot
void OrderBook::getMaxAmount(std::vector<OrderBookEntry>& entries, OrderBookEntry& bot)
{
    bot.amount = entries[0].amount;
    for (OrderBookEntry& e : entries)
    {
        if (e.price > bot.amount)
        bot.amount = e.amount;
        bot.price = e.price;
    }
}

// get the highest price
double OrderBook::getHighPrice(std::vector<OrderBookEntry>& orders)
{
    // double max = orders[0].price;
    // for (OrderBookEntry& e : orders)
    // {
    //     if (e.price > max)max = e.price;
    // }
    // return max;
    std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByPriceDesc);
    return orders[0].price;
}

// get the lowest price
double OrderBook::getLowPrice(std::vector<OrderBookEntry>& orders)
{
    // double min = orders[0].price;
    // for (OrderBookEntry& e : orders)
    // {
    //     if (e.price < min)min = e.price;
    // }
    // return min;

    std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByPriceAsc);
    return orders[0].price;
}

// the mean value
double OrderBook::getMeanPrice(std::vector<OrderBookEntry>& orders)
{
    double mean = orders[0].price;
    double sum = orders[0].price;

    for (OrderBookEntry& e : orders)
    {
        sum += e.price;
    }
    mean = sum / orders.size();
    return mean;
}

// compute average value
double OrderBook::getavg(std::vector<double>& passedNum) 
{
    double sum = std::accumulate(passedNum.begin(), passedNum.end(), 0.0);
    double mean = sum / passedNum.size();
    return mean;
}

// caculate ATR 
// ATR = Average ( True Range, n )
// True Range = Max of ( High - Low ), ( High -PreviousClose ), ( PreviousClose - Low )
// Average = Simple, Exponential, Weighted, and Triangular
// n = Time period

// caculate bid‐ask spread 
// at the same timestamps , the ETH/BTC spread = lowest ask price - highest bid price
double OrderBook::getSpread(std::vector<OrderBookEntry>& entries_ask, std::vector<OrderBookEntry>& entries_bid)
{
    double low_ask = OrderBook::getLowPrice(entries_ask);
    double high_bid = OrderBook::getHighPrice(entries_bid);
    double spread = std::abs(low_ask - high_bid);
    return spread;
}



std::string OrderBook::getEarliestTime()
{
    // std::string earliest_timestamp = orders[0].timestamp;
    // for (OrderBookEntry& e : orders)
    // {
    //     if (e.timestamp < earliest_timestamp)
    //     {
    //         earliest_timestamp = e.timestamp;
    //         break;
    //     }
    // }
    // return earliest_timestamp;
    int low = 0;
    int high = orders.size() - 1;
    while(low < high)
    {
        int pivot = low + (high - low) / 2;
        if(orders[pivot].timestamp < orders[high].timestamp)
        {
            high = pivot;
        } else {
            low = pivot + 1;
        }
    }
    return orders[low].timestamp;
}



//get next timestamp
std::string OrderBook::getNextTime(std::string timestamp)
{
    std::string next_timestamp = "";
    for (OrderBookEntry& e : orders)
    {
        if (e.timestamp > timestamp) 
        {
            next_timestamp = e.timestamp;
            break;
        }
    }
    if (next_timestamp == "")
    {
        next_timestamp = orders[0].timestamp;
    }

    return next_timestamp;
}


// //get passed timestamps
// std::vector<std::string> OrderBook::getPassedTime(std::string currentTime)
// {
//     std::vector<std::string> timeLine;
//     timeLine.push_back(currentTime);
//     return timeLine;
// }

//get setted timestamps

std::vector<std::string> OrderBook::getsettedTime(std::vector<std::string>& timeLine, size_t timeover)
{
    std::vector<std::string> set_timeLine;
    size_t i = 0;
    for (std::vector<std::string>::reverse_iterator it = timeLine.rbegin(); it != timeLine.rend(); ++it,++i)
    {
        if (i < timeover) set_timeLine.push_back(*it);
    }
    return set_timeLine;
}

void OrderBook::insertOrder(OrderBookEntry& order)
{
    orders.push_back(order);
    std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestamp);
}

std::vector<OrderBookEntry> OrderBook::matchAsksToBids(std::string product, std::string timestamp)
{
    std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask, 
                                                 product, 
                                                 timestamp);
    std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid, 
                                                 product, 
                                                    timestamp);

    std::vector<OrderBookEntry> sales; 
    if (asks.size() == 0 || bids.size() == 0)
    {
        std::cout << " OrderBook::matchAsksToBids no bids or asks" << std::endl;
        return sales;
    }

    std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);
    std::sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDesc);
    // std::cout << "max ask " << asks[asks.size()-1].price << std::endl;
    // std::cout << "min ask " << asks[0].price << std::endl;
    // std::cout << "max bid " << bids[0].price << std::endl;
    // std::cout << "min bid " << bids[bids.size()-1].price << std::endl;
    
    for (OrderBookEntry& ask : asks)
    {
        for (OrderBookEntry& bid : bids)
        {
            if (bid.price >= ask.price)
            {
                OrderBookEntry sale{ask.price, 0, timestamp, product, OrderBookType::asksale};
                if (bid.username == "simuser")
                {
                    sale.username = "simuser";
                    sale.orderType = OrderBookType::bidsale;
                }
                if (ask.username == "simuser")
                {
                    sale.username = "simuser";
                    sale.orderType =  OrderBookType::asksale;
                }
            
                if (bid.amount == ask.amount)
                {
                    sale.amount = ask.amount;
                    sales.push_back(sale);
                    bid.amount = 0;
                    break;
                }
                if (bid.amount > ask.amount)
                {
                    sale.amount = ask.amount;
                    sales.push_back(sale);
                    bid.amount =  bid.amount - ask.amount;
                    break;
                }

                if (bid.amount < ask.amount && 
                   bid.amount > 0)
                {
                    sale.amount = bid.amount;
                    sales.push_back(sale);
                    ask.amount = ask.amount - bid.amount;
                    bid.amount = 0;
                    continue;
                }
            }
        }
    }
    return sales;             
}

double OrderBook::getAvg(std::vector<std::string>& timeLine, OrderBookType orderType, std::string product, size_t timeover)
{
    std::vector<double> passedAvg;
    std::vector<OrderBookEntry> entries;
    std::vector<std::string> set_timeLine;
    set_timeLine = getsettedTime(timeLine,timeover);

    //compute a vector of average, through a reverse iterator to timeline
    size_t i = 0;
    for (std::string& timestamp : set_timeLine)
    {
        entries = getOrders(orderType, product, timestamp);
        double p = getMeanPrice(entries);
        passedAvg.push_back(p);
    }
    double passedAvgAvg = OrderBook::getavg(passedAvg);
    return passedAvgAvg;
}

double OrderBook::predictMin(std::vector<std::string>& timeLine, OrderBookType orderType, std::string product, size_t timeover)
{
    std::vector<double> passedMin;
    std::vector<OrderBookEntry> entries;
    std::vector<std::string> set_timeLine;
    set_timeLine = getsettedTime(timeLine,timeover);

    //compute a vector of average, through a reverse iterator to timeline
    size_t i = 0;
    for (std::string& timestamp : set_timeLine)
    {
        entries = getOrders(orderType, product, timestamp);
        double p = getLowPrice(entries);
        passedMin.push_back(p);
    }
    double passedMinAvg = OrderBook::getavg(passedMin);
    return passedMinAvg;
}

double OrderBook::predictMax(std::vector<std::string>& timeLine, OrderBookType orderType, std::string product, size_t timeover)
{
    std::vector<double> passedMax;
    std::vector<OrderBookEntry> entries;
    std::vector<std::string> set_timeLine;
    set_timeLine = getsettedTime(timeLine,timeover);

    //compute a vector of average, through a reverse iterator to timeline
    size_t i = 0;
    for (std::string& timestamp : set_timeLine)
    {
        entries = getOrders(orderType, product, timestamp);
        double p = getHighPrice(entries);
        passedMax.push_back(p);
    }
    double passedMaxAvg = OrderBook::getavg(passedMax);
    return passedMaxAvg;
}
// construct model to predict exchange rates
// three variables: the Purchasing Power Parity (PPP) index, Basis SWAP Libor (estimated interest rates), and historical volatility
// error fitting: the crossover of two exponentially weighted moving averages (EWMA) of different length of the EUR/USD exchange rates



// trading Strategy with the Predicted Values
// 1. Carry trading is referred to buying a currency that pays a high interest rate compared to a low interest rate currency, and selling the currency that pays a lower rate compared to a higher interest rate currency.
// 2. Valuation rises from the long-run currency tendencies to reach their “fair value”, or the rational and unbiased estimate of the potential market price of an asset.

// If the strategies predict that prices were to go up the next day,  the  system  will  buy  the  maximum  number  of  Bitcoins  it  can at the current price level or not do anything if is unable to buy any more Bitcoins. If the strategies predict that prices were to  go  down  the  following  day,  the  system  will  sell  all  the  Bitcoins at the current price level or not do anything if there is none left.
OrderBookType OrderBook::getStrategy(   std::vector<OrderBookEntry>& entries,
                                        std::vector<std::string>& timeLine, 
                                        OrderBookType orderType, 
                                        std::string product,
                                        size_t timeover  )
{
    OrderBookType ordertype;
    //caculate price in current timestamp
    double priceMin = getLowPrice(entries);
    double priceMax = getHighPrice(entries);
    double priceMid = getMeanPrice(entries);
    //caculate predicted price for next timestamp
    double price_predictedMin = predictMin(timeLine, orderType, product, timeover);
    double price_predictedMax = predictMax(timeLine, orderType, product, timeover);
    double price_predictedMid = getAvg(timeLine, orderType, product, timeover);
    if (price_predictedMid - priceMid > 0.0 && price_predictedMax - priceMax > 0.0 && price_predictedMin - priceMin > 0.0)
    {
        //set buy
        ordertype = OrderBookType::bid;
    } 
    if (priceMid - price_predictedMid > 0.0 && priceMax - price_predictedMax > 0.0 && priceMin - price_predictedMin > 0.0)
    {
        //set sell
        ordertype = OrderBookType::ask;
    }
    return ordertype;
}

// calculate predicted profit
// profit_predicted = product you buy or sell price * amount - next timestamp predicted price * amount

double OrderBook::getProfit(std::vector <OrderBookEntry>& sales,std::vector <OrderBookEntry>& entries)
{
    //set price in middle
    double price = (getLowPrice(entries)+getHighPrice(entries))/2;

    //count sum of dealed amount
    double total_amount;
    for(auto &e:sales)
    {
        total_amount += e.amount;
    }

    //caculate total deal
    std::vector<double> total;
    for(const auto &e:sales)
    {
        double t = e.price * e.amount;
        total.push_back(t);
    }
    double total_sales = std::accumulate(total.begin(), total.end(), 0.0);

    //make a divide with setting price
    double profit = price * total_amount - total_sales;
    return profit;
}
