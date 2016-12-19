
//  Created by Jiaxi(Jessie) Wang on 2016/12/5.
#ifndef EXECUTION_SERVICE_HPP
#define EXECUTION_SERVICE_HPP

#include "executionorder.hpp"



//Service for executing orders on an exchange.
//Keyed on product identifier.
//Type T is the product type.

template<typename T>
class ExecutionService : public Service<string,ExecutionOrder <T> >
{
public:

  // Execute an order on a market
  virtual void ExecuteOrder(AlgoExecution<T>& data) = 0;

};


//*******************************
//Bond Execution Service
//*******************************
class BondExecutionService : public ExecutionService<Bond>
{
public:
    // virtual functions inherented from Service:
    // Get data on our service given a key
    virtual ExecutionOrder<Bond>& GetData(string key) override;
    
    // The callback that a Connector should invoke for any new or updated data
    virtual void OnMessage(ExecutionOrder<Bond> &data) override;
    // Add a listener to the Service for callbacks on add, remove, and update events
    // for data to the Service.
    virtual void AddListener(ServiceListener<ExecutionOrder<Bond>> &listener) override;
    // Get all listeners on the Service.
    virtual const vector< ServiceListener<ExecutionOrder<Bond>>*>& GetListeners() const override;

    virtual void ExecuteOrder(AlgoExecution<Bond>& data);
private:
    map<string, ExecutionOrder<Bond>> executionOrderbook;
    vector<ServiceListener<ExecutionOrder<Bond>>*> listeners;
};



ExecutionOrder<Bond>& BondExecutionService::GetData(string  key)
{
    return executionOrderbook[key];
}

void BondExecutionService::OnMessage(ExecutionOrder<Bond> &data)
{
    string key = data.GetProduct().GetProductId();
    executionOrderbook[key] = data;
}

void BondExecutionService::AddListener(ServiceListener<ExecutionOrder<Bond>> &listener)
{
    listeners.push_back(&listener);
}

const vector< ServiceListener<ExecutionOrder<Bond>> *>&  BondExecutionService::GetListeners() const
{
    return listeners;
}

void  BondExecutionService::ExecuteOrder(AlgoExecution<Bond>& data)
{
    std::cout << "Execution the Order" << endl;
}



#endif
