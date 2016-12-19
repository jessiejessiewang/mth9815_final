//
//  bondalgostreamingservice.h
//  Final_Project
//
//  Created by Jessie Wang on 2016/12/17.
//  Copyright © 2016年 Jessie Wang. All rights reserved.
//

#ifndef bondalgostreamingservice_h
#define bondalgostreamingservice_h

#include "streamingservice.hpp"
#include "pricingservice.hpp"
#include "AlgoStream.hpp"


class BondAlgoStreamingService : public Service<string,AlgoStream<Bond>>
{
public:
    //virtual functions
    AlgoStream<Bond>& GetData(string productId) override;
    
    void OnMessage(AlgoStream<Bond> & data) override;
    
    void AddListener(ServiceListener<AlgoStream<Bond>> & listener) override;
    
    const vector<ServiceListener<AlgoStream<Bond>>*>& GetListeners() const override;
    

private:
    map<string,AlgoStream<Bond>> algoStreamBook;
    
    vector<ServiceListener<AlgoStream<Bond>>*> listeners;
};





AlgoStream<Bond>& BondAlgoStreamingService::GetData(string productId)
{
    return algoStreamBook[productId];
}

void BondAlgoStreamingService::OnMessage(AlgoStream<Bond> & data)
{
    string key = data.GetPriceStream().GetProduct().GetProductId();
    algoStreamBook[key] = data;
    //cout << "OnMessage" << endl;
    for(int i = 0; i < listeners.size(); i++){
        listeners[i]->ProcessAdd(data);
    }
}

void BondAlgoStreamingService::AddListener(ServiceListener<AlgoStream<Bond>> & listener)
{
    listeners.push_back(&listener);
}

const vector<ServiceListener<AlgoStream<Bond>>*>& BondAlgoStreamingService::GetListeners() const
{
    return listeners;
}





#endif /* bondalgostreamingservice_h */



