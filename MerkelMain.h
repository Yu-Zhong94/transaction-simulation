#pragma once

#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"


class MerkelMain
{
    public:
        MerkelMain();
        /** Call this to start the sim */
        void init();
        

    private: 
        void printHelp();
        void printHelpprod(); 
        void printHelpavg(); 
        void printHelpmax();
        void printHelpmin(); 
        void printHelpspread();
        void printHelppredict();
        void printHelptime(); 
        void printHelpstep();
        void printHelpwallet();
        void printHelpmakeask();
        void printHelpmakebid();
        void printHelpenter();
        void printHelpadvice();

        void printMin();
        void printMax();
        void printAvg();
        std::vector<double> getPassedAvg();
        std::vector<double> getPassedmax();
        std::vector<double> getPassedmin();

        void printSpread();

        void printProd(); 
        void checkTime();
        void printPredictmax();
        void printPredictmin();
        void printTime();
        void printStep();
        void enterAsk();
        void enterBid();
        std::vector<OrderBookEntry> enterAdvisor();
        std::vector<OrderBookEntry> enterAdvisor(std::vector<OrderBookEntry> advisorbots);
        void enterAskorBid_Advisor();
        void enterAskorBid();
        void printAdvice();
        void printProfit();
        
        void printWallet();
        void gotoNextTimeframe();
        void printAdvice_auto();
        void autoExchange();
        void getUserOption();
        void setFunction(std::string operfind);

        size_t passedTime;
        std::vector<std::string> timeLine;
        std::string lastTime;
        std::string product;
        OrderBookType orderType;
        size_t timeover=10;
        std::string currentTime;
        std::vector<std::string> productset;
        std::vector<std::string> operatorset = {"help","prod","avg","max","min","predict","time","step","enter","auto","spread","profit","advice","makeask","makebid","wallet"};
        std::vector<std::string> operFunction = {"help","helpprod","helpavg","helpmax","helpmin","helpspread","helppredict","helptime","helpstep","helpwallet","helpmakeask","helpmakebid","helpspread","helpenter","helpadvice","prod","avg","max","min","predictmax","predictmin","time","step","enter","auto","spread","profit","advice","autoadvice","enteradvice","makeask","makebid","wallet"};
        std::vector<std::string> orderbookset = {"bid","ask"};
        std::vector<OrderBookEntry> sales;
        
    //    OrderBook orderBook{"20200317.csv"};
	    OrderBook orderBook{"20200601.csv"};
        Wallet wallet;

};
