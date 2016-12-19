//
//  BondStreamingAlgoStreamingListener.h
//  Final_Project
//
//  Created by Jessie Wang on 2016/12/18.
//  Copyright © 2016年 Jessie Wang. All rights reserved.
//

#ifndef BondStreamingAlgoStreamingListener_h
#define BondStreamingAlgoStreamingListener_h
#include "algostreamingservice.hpp"
#include "streamingservice.hpp"

class BondStreamingAlgoStreamingListener : public ServiceListener<AlgoStream<Bond>>
{
public:
    BondStreamingAlgoStreamingListener(BondStreamingService &);
    
    void ProcessAdd(AlgoStream<Bond>& data);
    
private:
    BondStreamingService & bondStreamingService;
    
};

BondStreamingAlgoStreamingListener::BondStreamingAlgoStreamingListener(BondStreamingService& s):bondStreamingService(s)
{
}

void BondStreamingAlgoStreamingListener::ProcessAdd(AlgoStream<Bond>& data)
{
    PriceStream<Bond> MyPriceStream = data.GetPriceStream();
    bondStreamingService.PublishPrice(MyPriceStream);
    bondStreamingService.OnMessage(MyPriceStream);
    
}







#endif /* BondStreamingAlgoStreamingListener_h */
