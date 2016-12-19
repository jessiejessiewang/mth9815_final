//
//  main.cpp
//  Final_Project
//
//  Created by Jiaxi(Jessie) Wang on 2016/12/5.
//
//  This file is designed to generate data and cout in the trades.txt. Create 10 trades for each security (so a total of 60 trades across all 6 securities) in the file with the relevant trade attributes. Positions should be across books TRSY1, TRSY2, and TRSY3.



#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <string>
#include "stdlib.h"
#include "string.h"
#include "products.hpp"
#include "tradebookingservice.hpp"
#include "positionservice.hpp"
#include "riskservice.hpp"
#include "pricingservice.hpp"
#include "marketdataservice.hpp"
#include "algoexecutionservice.hpp"
#include "executionservice.hpp"
#include "algostreamingservice.hpp"
#include "inquiryservice.hpp"
#include "historicaldataservice.hpp"
#include "MarketDataAlgoExecutionListener.hpp"
#include "ExecutionAlgoExecutionListener.hpp"
#include "streamingservice.hpp"
#include "BondPricingAlgoStreamingListener.hpp"
#include "BondStreamingAlgoStreamingListener.hpp"

using namespace std;

struct data{
    string CUSIP;
    int TradeId;
    string bookno;
    int quantity;
    string s_side;
};


int main() {
    
    string trades = "/Users/Jessie/Desktop/hw3/Final_Project/Final_Project/trades.txt";
    string prices = "/Users/Jessie/Desktop/hw3/Final_Project/Final_Project/prices.txt";
    string marketdata = "/Users/Jessie/Desktop/hw3/Final_Project/Final_Project/marketdata.txt";
    string inquries = "/Users/Jessie/Desktop/hw3/Final_Project/Final_Project/inquiries.txt";

    cout << "BookingService" << endl;
    BondTradeBookingService bondTradeBookingService;
    BondPositionService bondPositionService;
    TradeBookingPositionListener tradeBookingPositionListener(bondPositionService);
    bondTradeBookingService.AddListener(tradeBookingPositionListener);
    BondRiskService bondRiskService;
    BondRiskPositionListener bondRiskPositionListener(bondRiskService);
    bondPositionService.AddListener(bondRiskPositionListener);
    BondHistoricalDataService<PV01<Bond>> bondHistoricalDataServicePV01;
    BondHistoricalDataRiskListener bondHistoricalDataRiskListener(bondHistoricalDataServicePV01);
    bondRiskService.AddListener(bondHistoricalDataRiskListener);
    BondTradeBookingConnector bondTradeBookingConnector(trades, bondTradeBookingService);
    cout << "FinishBookingService" << endl;

    cout << "InquiryService" << endl;
    BondInquiryService bondInquiryService;
    BondInquiryConnector2 bondInquiryConnector2;
    bondInquiryService.AddConnector(bondInquiryConnector2);
    BondHistoricalDataService<Inquiry<Bond>> bondHistoricalDataServiceInquiry;
    BondHistoricalDataInquiryServiceListener bondHistoricalDataInquiryServiceListener(bondHistoricalDataServiceInquiry);
    bondInquiryService.AddListener(bondHistoricalDataInquiryServiceListener);
    BondInquiryConnector bondInquiryConnector(inquries,bondInquiryService);
    
    cout << "FinishInquiryService" << endl;
    
    cout << "MarketService" << endl;
    BondMarketDataService bondMarketDataService;
    BondAlgoExecutionService bondAlgoExecutionService;
    MarketDataAlgoExecutionListener marketDataAlgoExecutionListener(bondAlgoExecutionService);
    bondMarketDataService.AddListener(marketDataAlgoExecutionListener);

    BondExecutionService bondExecutionService;
    ExecutionAlgoExecutionListener executionAlgoExecutionListener(bondExecutionService);
    bondAlgoExecutionService.AddListener(executionAlgoExecutionListener);
    BondHistoricalDataService<ExecutionOrder<Bond>> bondHistoricalDataServiceExecutionOrder;
    BondHistoricalDataExecutionListener bondHistoricalDataExecutionListener(bondHistoricalDataServiceExecutionOrder);
    bondExecutionService.AddListener(bondHistoricalDataExecutionListener);
    BondMarketDataConnector bondMarketDataConnector(marketdata,bondMarketDataService);
 
    cout << "FinishMarketService" << endl;
 
    
    cout << "PricingService" << endl;
    BondPricingService bondPricingService;
    BondAlgoStreamingService bondAlgoStreamingService;
    BondPricingAlgoStreamingListener bondPricingAlgoStreamingListener(bondAlgoStreamingService);
    bondPricingService.AddListener(bondPricingAlgoStreamingListener);
    BondStreamingService bondStreamingService;
    BondStreamingAlgoStreamingListener bondStreamingAlgoStreamingListener(bondStreamingService);
    bondAlgoStreamingService.AddListener(bondStreamingAlgoStreamingListener);
    BondHistoricalDataService<PriceStream<Bond>> bondHistoricalDataServicePriceStream;
    BondHistoricalDataStreamingListener bondHistoricalDataStreamingListener(bondHistoricalDataServicePriceStream);
    bondStreamingService.AddListener(bondHistoricalDataStreamingListener);
    BondPricingConnector bondPricingConnector(prices,bondPricingService);
    
    cout << "FinishPricingService" << endl;
    
    
    
     // this is used to create "trades.txt"
    /*
    vector<string> SaveCUSIP = {"912828U40",
        "912828U32",
        "912828U65",
        "912828U57",
        "912828U24",
        "912810RU4"};
    vector<string> Savebookno = {"TRSY1", "TRSY2", "TRSY3"};
    vector<string> SaveSide = {"SELL", "BUY"};
    
    ofstream outfile;
    outfile.open("/Users/Jessie/Desktop/hw3/Final_Project/Final_Project/trades.txt");
    
    // print the headline
    // outfile << "CUSIP,TradeId,Bookno,Quantity,Side" << endl;
    
    // generate all the data needed
    
    for(int i = 0; i < 6; i++){
        for(int j = 0; j <10; j++){
            data input = {SaveCUSIP[i],
                i * 10 + j + 1,
                Savebookno[rand() % 3],
                rand() % 10000 + rand()%100,
                SaveSide[rand() % 2]};
            outfile << input.CUSIP << ",T" << input.TradeId << "," << input.bookno << "," << input.quantity << "," << input.s_side << endl;
        }
    }
    outfile.close();
    */
    cout << "Finish!" << endl;
    return 0;
};
