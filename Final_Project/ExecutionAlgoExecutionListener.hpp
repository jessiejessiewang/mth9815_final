//
//  ExecutionAlgoExecutionListener.h
//  Final_Project
//
//  Created by Jessie Wang on 2016/12/18.
//  Copyright © 2016年 Jessie Wang. All rights reserved.
//

#ifndef ExecutionAlgoExecutionListener_h
#define ExecutionAlgoExecutionListener_h

#include "algoexecutionservice.hpp"
#include "executionservice.hpp"


//***********************************************************************************
//Bond Algo Execution Service from Execution Service, listener.
//***********************************************************************************

class ExecutionAlgoExecutionListener: public ServiceListener<AlgoExecution<Bond>>
{
public:
    virtual void ProcessAdd(AlgoExecution<Bond>& data);
    
    ExecutionAlgoExecutionListener(BondExecutionService &);
    
private:
    BondExecutionService & bondExecutionService;
};


void ExecutionAlgoExecutionListener::ProcessAdd(AlgoExecution<Bond>& data)
{
    bondExecutionService.ExecuteOrder(data);
}

ExecutionAlgoExecutionListener::ExecutionAlgoExecutionListener(BondExecutionService& s):bondExecutionService(s)
{
    
}


#endif /* ExecutionAlgoExecutionListener_h */
