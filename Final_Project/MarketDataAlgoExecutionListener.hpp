//
//  MarketDataAlgoExecutionListener.h
//  Final_Project
//
//  Created by Jessie Wang on 2016/12/18.
//  Copyright © 2016年 Jessie Wang. All rights reserved.
//

#ifndef MarketDataAlgoExecutionListener_h
#define MarketDataAlgoExecutionListener_h

#include "marketdataservice.hpp"
#include "algoexecutionservice.hpp"



//***********************************************************************************
//Bond Algo Execution from Market Data, listener.
//***********************************************************************************



class MarketDataAlgoExecutionListener : public ServiceListener<OrderBook<Bond>>
{
private:
    BondAlgoExecutionService & bondAlgoExecutionService;
    
public:
    virtual void ProcessAdd(OrderBook<Bond>& data);
    
    MarketDataAlgoExecutionListener(BondAlgoExecutionService &);
    
};





MarketDataAlgoExecutionListener::MarketDataAlgoExecutionListener(BondAlgoExecutionService& s):bondAlgoExecutionService(s)
{
}


void MarketDataAlgoExecutionListener::ProcessAdd(OrderBook<Bond>& data)
{
    
    ofstream outfile("/Users/Jessie/Desktop/hw3/Final_Project/Final_Project/historicalExecution.txt",ios::app);
    outfile  << data.GetProduct().GetProductId() << endl;
    //cout << data.GetProduct().GetProductId() << endl;
    //ExecutionOrder<Bond> MyExe(data.GetProduct());
    //bondAlgoExecutionService.OnMessage(data);
}



#endif /* MarketDataAlgoExecutionListener_h */
