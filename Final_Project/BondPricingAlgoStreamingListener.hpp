//
//  BondPricingAlgoStreamingListener.h
//  Final_Project
//
//  Created by Jessie Wang on 2016/12/18.
//  Copyright © 2016年 Jessie Wang. All rights reserved.
//

#ifndef BondPricingAlgoStreamingListener_h
#define BondPricingAlgoStreamingListener_h


#include "algostreamingservice.hpp"
#include "pricingservice.hpp"

class BondPricingAlgoStreamingListener : public ServiceListener<Price<Bond>>
{
public:
    
    BondPricingAlgoStreamingListener(BondAlgoStreamingService &);
    
    void ProcessAdd(Price<Bond>& data);
    
private:
    
    BondAlgoStreamingService & bondAlgoStreamingService;
    
};



BondPricingAlgoStreamingListener::BondPricingAlgoStreamingListener(BondAlgoStreamingService & s):bondAlgoStreamingService(s)
{
}

void BondPricingAlgoStreamingListener::ProcessAdd(Price<Bond>& data)
{
    PriceStreamOrder bidOrder(data.GetMid() - 0.5*data.GetBidOfferSpread(), 1000000, 20000, BID);
    PriceStreamOrder offerOrder(data.GetMid() + 0.5*data.GetBidOfferSpread(), 2000000,10000, OFFER);
    PriceStream<Bond> Mypricestream(data.GetProduct(), bidOrder, offerOrder);
    AlgoStream<Bond> Myalgostream(Mypricestream);
    //cout << "listener" << bidOrder.GetPrice() << endl;
    bondAlgoStreamingService.OnMessage(Myalgostream);
}


#endif /* BondPricingAlgoStreamingListener_h */
