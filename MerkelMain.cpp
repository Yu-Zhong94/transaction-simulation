#include "MerkelMain.h"
#include <iostream>
#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "CSVReader.h"
#include "CmdEntry.h"

MerkelMain::MerkelMain()
{

}

void MerkelMain::init()
{
    currentTime = orderBook.getEarliestTime();
    timeLine.push_back(currentTime);
    // passedTime = orderBook.getPassedTime(currentTime);
    wallet.insertCurrency("BTC", 10);

    while(true)
    {
        getUserOption();
    }
}


void MerkelMain::printHelp()
{
    std::cout << " The available commands are help, help <cmd>, prod, min, max, avg, predict, time, step, spread, wallet, makeask, makebid, enter, advice" << std::endl;
}
void MerkelMain::printHelpprod()
{
    std::cout << " prod -> ETH/BTC,DOGE/BTC,etc "<< std::endl;
}
void MerkelMain::printHelpmin()
{
    std::cout << " min ETH/BTC ask -> The min ask for ETH/BTC "<< std::endl;
}
void MerkelMain::printHelpmax()
{
    std::cout << " max ETH/BTC ask -> The max ask for ETH/BTC "<< std::endl;
}
void MerkelMain::printHelpavg()
{
    std::cout << " avg ETH/BTC bid 10 -> average ETH/BTC bid over last 10 time steps "<< std::endl;
}
void MerkelMain::printHelppredict()
{
    std::cout << " predict max ETH/BTC bid -> The average ETH/BTC ask price over the last 10 timesteps "<< std::endl;
}
void MerkelMain::printHelptime()
{
    std::cout << " time -> 2020/03/17 17:01:24"<< std::endl;
}
void MerkelMain::printHelpstep()
{
    std::cout << " step ->  now at 2020/03/17 17:01:30 "<< std::endl;
}
// spread, wallet, makeask, makebid, enter, advice
void MerkelMain::printHelpspread()
{
    std::cout << " spread ETH/BT ->  Spread for ETH/BTC : 5.3e-07 "<< std::endl;
}
void MerkelMain::printHelpwallet()
{
    std::cout << " wallet ->  BTC : 10.000000 "<< std::endl;
}
void MerkelMain::printHelpmakeask()
{
    std::cout << " makeask ->  Make an ask - enter the order: product,price,amount, eg  ETH/BTC,200,0.5 "<< std::endl;
    std::cout << " ETH/BTC,200,0.5 ->\n  Wallet::canFulfillOrder ETH : 0.5 \n Wallet has insufficient funds . "<< std::endl;
}
void MerkelMain::printHelpmakebid()
{
    std::cout << " makebid ->  Make a bid - enter the order: product,price,amount, eg  ETH/BTC,200,0.5 "<< std::endl;
    std::cout << " ETH/BTC,200,0.5 ->\n  Wallet::canFulfillOrder BTC : 100 \n Wallet has insufficient funds . "<< std::endl;
}
void MerkelMain::printHelpenter()
{
    std::cout << " enter ->\n  You can ask BTC/USDT \n You can bid DOGE/BTC \n You can bid ETH/BTC "<< std::endl;
    std::cout << " enter advice -> Advisorbot: You can ask BTC/USDT "<< std::endl;
}
void MerkelMain::printHelpadvice()
{
    std::cout << " advice ->\n Wallet::canFulfillOrder BTC : 0.00052289 \n Advisorbot recommand ask : BTC/USDT,9493.81,0.00052289 "<< std::endl;
    std::cout << " auto advice ->\n Going to next time frame. \n Going to next time frame \n Wallet::canFulfillOrder BTC : 0.003 \n Advisorbot recommand ask : BTC/USDT,9498.53,0.003 "<< std::endl;
}
void MerkelMain::printProd()
{
    for (std::string const& p : orderBook.getKnownProducts())
    {
        std::cout << p << "  ";
    }
    std::cout << "\n"<< std::endl;
}

void MerkelMain::printMax()
{
    std::vector<OrderBookEntry> entries = orderBook.getOrders(orderType, product, currentTime);
    std::cout << "Max " 
                << OrderBookEntry::enumToString(orderType)
                << " price on "<< product
                <<" : "<< OrderBook::getHighPrice(entries) << std::endl;
}
void MerkelMain::printMin()
{
    std::vector<OrderBookEntry> entries = orderBook.getOrders(orderType, product, currentTime);
    std::cout << "Min " 
                << OrderBookEntry::enumToString(orderType)
                << " price on "<< product
                <<" : "<< OrderBook::getLowPrice(entries) << std::endl;
}

void MerkelMain::printSpread()
{
    std::vector<OrderBookEntry> entries_ask = orderBook.getOrders(OrderBookType::ask, product, currentTime);
    std::vector<OrderBookEntry> entries_bid = orderBook.getOrders(OrderBookType::bid, product, currentTime);

    std::cout << "Spread for " << product << " : "<<OrderBook::getSpread(entries_ask, entries_bid) << std::endl;
}
void MerkelMain::checkTime()
{
    size_t Line;
    Line = timeLine.size();
    if (timeover > Line) 
    {
        std::cout << "Your setting timeover is over actual passed time." << std::endl;
        std::cout << "Adviserbot set passed timestamps to "<< Line << std::endl;
        timeover = Line;
    } 
}
//print average
void MerkelMain::printAvg()
{
    double passedAvgAvg;
    std::cout << "Your setting timeover is "<< timeover << std::endl;
    checkTime();
    passedAvgAvg = orderBook.getAvg(timeLine, orderType, product, timeover);
    std::cout <<"For last " << timeover << " timestamps, "
                <<OrderBookEntry::enumToString(orderType) << " "
                << product 
                << " average : "<< passedAvgAvg << std::endl;
}

// compute moving average to predict min
void MerkelMain::printPredictmax()
{
    double passedMaxAvg;
    timeover = 10;
    std::cout << "Deflaut setting timeover is "<< timeover << std::endl;
    checkTime();
    passedMaxAvg = orderBook.predictMax(timeLine, orderType, product, timeover);
    std::cout << "Predict "<<OrderBookEntry::enumToString(orderType)  
                << " "<<  product 
                << " max : "<< passedMaxAvg 
                <<" ; in last "<< timeover <<" timestamps"<< std::endl;
}

// compute moving average to predict min
void MerkelMain::printPredictmin()
{
    double passedMinAvg;
    timeover = 10;
    std::cout << "Deflaut setting timeover is "<< timeover << std::endl;
    checkTime();
    passedMinAvg = orderBook.predictMin(timeLine, orderType, product, timeover);
    std::cout << "Predict "<<OrderBookEntry::enumToString(orderType)  
                << " "<<  product 
                << " min : "<< passedMinAvg 
                <<" ; in last "<< timeover <<" timestamps"<< std::endl;
}

void MerkelMain::printProfit()
{
    std::vector<OrderBookEntry> entries_bidsale;
    std::vector<OrderBookEntry> entries_bid;
    entries_bidsale = orderBook.getOrders(OrderBookType::bidsale ,product, lastTime);
    entries_bid = orderBook.getOrders(OrderBookType::bid,product,currentTime);
    std::cout << "Bid Profit: "<< OrderBook::getProfit(entries_bidsale, entries_bid) << std::endl;

    std::vector<OrderBookEntry> entries_asksale;
    std::vector<OrderBookEntry> entries_ask;
    entries_asksale = orderBook.getOrders(OrderBookType::asksale ,product, lastTime);
    entries_ask = orderBook.getOrders(OrderBookType::ask,product,currentTime);
    std::cout << "Ask Profit: "<< OrderBook::getProfit(entries_asksale, entries_ask) << std::endl;
}

void MerkelMain::printTime()
{
    std::cout <<"Current time is "<< currentTime << std::endl;
}
void MerkelMain::printStep()
{
    gotoNextTimeframe();
    std::cout <<"Now at "<< currentTime << std::endl;
}

void MerkelMain::enterAsk()
{
    std::cout << "Make an ask - enter the order: product,price,amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterAsk Bad input! " << input << std::endl;
    }
    else {
        try {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2], 
                currentTime, 
                tokens[0], 
                OrderBookType::ask 
            );
            obe.username = "simuser";
            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
            }
            else {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        }catch (const std::exception& e)
        {
            std::cout << " MerkelMain::enterAsk Bad input " << std::endl;
        }   
    }
}

void MerkelMain::enterBid()
{
    std::cout << "Make an bid - enter the order: product,price,amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterBid Bad input! " << input << std::endl;
    }
    else {
        try {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2], 
                currentTime, 
                tokens[0], 
                OrderBookType::bid 
            );
            obe.username = "simuser";

            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
            }
            else {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        }catch (const std::exception& e)
        {
            std::cout << " MerkelMain::enterBid Bad input " << std::endl;
        }   
    }
}
// command enter to see what product user can bid or ask
void MerkelMain::enterAskorBid()
{
    std::vector<std::string> product = orderBook.getKnownProducts();
    std::vector<OrderBookEntry> bots = wallet.checkProduct(product);
    for(auto &bot : bots)
        std::cout <<"You can " << OrderBookEntry::enumToString(bot.orderType)<< " " << bot.product << std::endl;
}

// command enter advice to print out Advisorbot's advice on bid or ask
void MerkelMain::enterAskorBid_Advisor()
{
    std::vector<OrderBookEntry> advisorbots = enterAdvisor();
    if (!advisorbots.empty())
    {
        for(auto &bot : advisorbots)
        {
            std::cout <<"Advisorbot: You can " 
                        << OrderBookEntry::enumToString(bot.orderType)
                        << " " << bot.product << std::endl;
        }
    } else 
    {
        std::cout << "Advisorbot : There is no advice on bid or ask." << std::endl;
    }

}

// command advice to see what user are advised on product, ordertype, price, amount.
void MerkelMain::printAdvice()
{
    std::vector<OrderBookEntry> advisor = enterAdvisor();
    std::vector<OrderBookEntry> advisorbots = enterAdvisor(advisor);
    if (!advisorbots.empty())
    {
        for (auto &bot: advisorbots)
        {
            std::cout <<"Advisorbot recommand "
                        << OrderBookEntry::enumToString(bot.orderType)<< " : " 
                        << bot.product <<","
                        << bot.price <<","
                        << bot.amount << std::endl;
        }
    } else 
    {
        std::cout << "Advisorbot : There is no advice on product, ordertype, price, amount." << std::endl;
    }

}

// command auto advise to give advice in auto running mode
void MerkelMain::printAdvice_auto()
{
    do
    {
        std::vector<OrderBookEntry> askorbid_Advisor = enterAdvisor();
        std::vector<OrderBookEntry> advisorbots = enterAdvisor(askorbid_Advisor);

        if (!advisorbots.empty())
        {
            for (auto &bot: advisorbots)
            {
                std::cout <<"Advisorbot recommand "
                            << OrderBookEntry::enumToString(bot.orderType)<< " : " 
                            << bot.product <<","
                            << bot.price <<","
                            << bot.amount << std::endl;
            }
            break;
        }
        gotoNextTimeframe();
    } while (true);
}

void MerkelMain::printWallet()
{
    std::cout << wallet.toString() << std::endl;
}

void MerkelMain::gotoNextTimeframe()
{
    std::cout << "Going to next time frame. " << std::endl;
    for (std::string p : orderBook.getKnownProducts())
    {
        // std::cout << "matching " << p << std::endl;
        std::vector<OrderBookEntry> sales =  orderBook.matchAsksToBids(p, currentTime);
        // std::cout << "Sales: " << sales.size() << std::endl;
        for (OrderBookEntry& sale : sales)
        {
            std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl; 
            if (sale.username == "simuser"||sale.username == "advisorbot")
            {
                // update the wallet
                wallet.processSale(sale);
            }
        }
    }
    
    currentTime = orderBook.getNextTime(currentTime);
    timeLine.push_back(currentTime);
}

// command auto to give advice in auto running mode
void MerkelMain::autoExchange()
{
    do
    {
        std::vector<OrderBookEntry> askorbid_Advisor = enterAdvisor();
        std::vector<OrderBookEntry> advisorbots = enterAdvisor(askorbid_Advisor);

        if (!advisorbots.empty())
        {
            for (auto &bot: advisorbots)
            {
                orderBook.insertOrder(bot);
            }
        }
        gotoNextTimeframe();
    } while (true);
}

// let advisorbot give their advise on buy or sell
std::vector<OrderBookEntry> MerkelMain::enterAdvisor()
{
    std::vector<OrderBookEntry> advisorbots;
    timeover = 10;
    std::vector<std::string> product = orderBook.getKnownProducts();
    std::vector<OrderBookEntry> bots = wallet.checkProduct(product);
    for (auto &bot : bots)
    {
        std::vector<OrderBookEntry> entries = orderBook.getOrders(bot.orderType, bot.product, currentTime);
        // check whether this product can buy or sell
        OrderBookType botcheck = orderBook.getStrategy(entries,timeLine, bot.orderType, bot.product, timeover);
        // std::cout <<"botcheck"<< OrderBookEntry::enumToString(botcheck) << std::endl;
        if (OrderBookEntry::enumToString(bot.orderType) == OrderBookEntry::enumToString(botcheck))
        {
            bot.username = "advisorbot";
            advisorbots.push_back(bot);
        }
    }
    return advisorbots;
}
// let advisorbot give their advise on price and amount based on strategy and wallet
std::vector<OrderBookEntry> MerkelMain::enterAdvisor(std::vector<OrderBookEntry> bots)
{
    std::vector<OrderBookEntry> advisorbots;
    std::vector<OrderBookEntry> askorbid_Advisor = enterAdvisor();
    for (OrderBookEntry& bot : bots)
    {
        if (bot.orderType == OrderBookType::bid)
        {
            // find largest amount in ask amount
            std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask, bot.product, currentTime);
            OrderBook::getMaxAmount(entries, bot);
        }
        if (bot.orderType == OrderBookType::ask)
        {
            // find largest amount in ask amount
            std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::bid, bot.product, currentTime);
            OrderBook::getMaxAmount(entries, bot);
        }
        if (wallet.canFulfillOrder(bot))
        {
            advisorbots.push_back(bot);
        }
    }
    return advisorbots;
}
void MerkelMain::getUserOption()
{
    std::string input;
    std::getline(std::cin, input);
    std::string operfind;
    std::string oper;
    std::string order;

    std::vector<std::string> tokens = CSVReader::tokenise(input, ' ');
    if ( tokens.size() > 4 )
    {
        std::cout << "MerkelMain::getUserOption Bad input! " << input << std::endl;
    }
    else {
        try {
            oper = CmdEntry::stringToOperatorType(tokens,operatorset);
            operfind = CmdEntry::findOperation(oper,operFunction);
        }catch (const std::exception& e)
        {
            std::cout << " MerkelMain::getUserOption Bad input " << std::endl;
        }
        try {
            productset = orderBook.getKnownProducts();
            product = CmdEntry::findProduct(tokens,productset);
        }catch (const std::exception& e)
        {
            std::cout << " MerkelMain::getUserOption Bad input " << std::endl;
        }
        try {
            order = CmdEntry::findOrderBookType(tokens, orderbookset);
            orderType = OrderBookEntry::stringToOrderBookType(order);
        }catch (const std::exception& e)
        {
            std::cout << " MerkelMain::getUserOption Bad input " << std::endl;
        }   
        try {
            timeover = CmdEntry::findTimeover(tokens,passedTime);

            setFunction(operfind);
        }catch (const std::exception& e)
        {
            std::cout << " MerkelMain::getUserOption Bad input " << std::endl;
        }   
    }
}
void MerkelMain::setFunction(std::string s)
{
		if (s == "help") printHelp();
        if (s == "helpprod") printHelpprod();
        if (s == "helpavg") printHelpavg();
        if (s == "helpmax") printHelpmax();
        if (s == "helpmin") printHelpmin();
        if (s == "helppredict") printHelppredict();
        if (s == "helptime") printHelptime();
        if (s == "helpstep") printHelpstep();

        if (s == "helpwallet") printHelpwallet();
        if (s == "helpmakeask") printHelpmakeask();
        if (s == "helpmakebid") printHelpmakebid();
        if (s == "helpspread") printHelpspread();
        if (s == "helpenter") printHelpenter();
        if (s == "helpadvice") printHelpadvice();

        if (s == "prod") printProd();
        if (s == "avg") printAvg();
        if (s == "max") printMax();
        if (s == "min") printMin();
        if (s == "predictmax") printPredictmax();
        if (s == "predictmin") printPredictmin();
        if (s == "time") printTime();
        if (s == "step") printStep();
        if (s == "spread") printSpread();
        if (s == "profit") printProfit();
        if (s == "wallet") printWallet();
        if (s == "auto") autoExchange();
        if (s == "enter") enterAskorBid();
        if (s == "advice") printAdvice();
        if (s == "autoadvice") printAdvice_auto();
        if (s == "enteradvice") enterAskorBid_Advisor();
        if (s == "makeask") enterAsk();
        if (s == "makebid") enterBid();
}
