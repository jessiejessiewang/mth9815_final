//
//  algoexecutionservice.h
//  Final_Project
//
//  Created by Jessie Wang on 2016/12/17.
//  Copyright © 2016年 Jessie Wang. All rights reserved.
//

#ifndef algoexecutionservice_h
#define algoexecutionservice_h

#include "executionorder.hpp"
#include "products.hpp"
#include "soa.hpp"
#include <map>
#include <vector>
#include "orderbook.hpp"

template <typename T>
class AlgoExecution
{
public:
    AlgoExecution(ExecutionOrder<T> & exe);
    
    AlgoExecution();
    
    ExecutionOrder<T>* GetExecutionOrder() const
    {
        return executionOrder;
    }
private:
    ExecutionOrder<T>* executionOrder;
    
};

template<typename T>
AlgoExecution<T>::AlgoExecution(ExecutionOrder<T> & exe):executionOrder(&exe)
{
}

template<typename T>
AlgoExecution<T>::AlgoExecution(){
    
}

//*************************************************************************************************
// Bond ALgo Execution Service: be keyed on product identifier with value an AlgoExecution object *
//*************************************************************************************************

class BondAlgoExecutionService : public Service < string, AlgoExecution < Bond > >
{
public:
    
    virtual AlgoExecution<Bond>& GetData(string productId) override;
    
    virtual void OnMessage(AlgoExecution<Bond> & data) override;
    
    virtual void AddListener(ServiceListener<AlgoExecution<Bond>> & listener) override;
    
    virtual const vector<ServiceListener<AlgoExecution<Bond>>*>& GetListeners() const override;
    
private:
    
    map < string , AlgoExecution < Bond >> AlgoExecutionBook;
    vector < ServiceListener< AlgoExecution<Bond>>*> listeners;
};


AlgoExecution<Bond>& BondAlgoExecutionService::GetData(string productId)
{
    return AlgoExecutionBook[productId];
}

void BondAlgoExecutionService :: OnMessage(AlgoExecution<Bond> & data)
{
    string key =data.GetExecutionOrder()->GetProduct().GetProductId();
    AlgoExecutionBook[key] = data;
    for(int i = 0; i < listeners.size(); i++){
        listeners[i]->ProcessAdd(data);
    }
}

void BondAlgoExecutionService::AddListener(ServiceListener<AlgoExecution<Bond>> &listener)
{
    listeners.push_back(&listener);
}

const vector<ServiceListener<AlgoExecution<Bond>>*>& BondAlgoExecutionService::GetListeners() const
{
    
    return listeners;
}




#endif /* algoexecutionservice_h */
