/**
 * historicaldataservice.hpp
 * historicaldataservice.hpp
 *
 * @author Breman Thuraisingham
 * Defines the data types and Service for historical data.
 *
 * @author Breman Thuraisingham
 */
//  Created by Jiaxi(Jessie) Wang on 2016/12/5.
#ifndef HISTORICAL_DATA_SERVICE_HPP
#define HISTORICAL_DATA_SERVICE_HPP
#include <fstream>
#include "products.hpp"
#include "soa.hpp"
#include "riskservice.hpp"

/**
 * Service for processing and persisting historical data to a persistent store.
 * Keyed on some persistent key.
 * Type T is the data type to persist.
 */
template<typename T>
class HistoricalDataService : Service<string,T>
{

public:

  // Persist data to a store
  void PersistData(string persistKey, const T& data) = 0;
    

};

template<typename T>
class BondHistoricalDataService : public Service<string,T>
{

public:
    T& GetData(string key) {
        return HistoricalBond[key];
    }
    
    void OnMessage(T &data) {}
    
    void AddListener(ServiceListener<T> &listener){}
    
    const vector< ServiceListener<T> *>& GetListeners() const{
        return listeners;
    }
private:
    vector<ServiceListener<T>*> listeners;
    
    map<string,T> HistoricalBond;
};



class BondHistoricalDataInquiryServiceListener : public ServiceListener<Inquiry<Bond>>
{
public:
    BondHistoricalDataInquiryServiceListener(BondHistoricalDataService<Inquiry<Bond> > &);
    
    void ProcessAdd(Inquiry<Bond> & data);
    
private:
    BondHistoricalDataService<Inquiry<Bond> > & bondHistoricalDataService;
};


BondHistoricalDataInquiryServiceListener::BondHistoricalDataInquiryServiceListener(BondHistoricalDataService<Inquiry<Bond> > & s):bondHistoricalDataService(s)
{
}

void BondHistoricalDataInquiryServiceListener::ProcessAdd(Inquiry<Bond> & data)
{
    ofstream outfile("/Users/Jessie/Desktop/hw3/Final_Project/Final_Project/historicalInquiries.txt",ios::app);
    outfile << data.GetInquiryId() <<", "<< data.GetProduct().GetProductId() <<", "<< data.GetState()<< ", "<< data.GetQuantity() << endl;
    //cout << data.GetInquiryId() <<", "<< data.GetProduct().GetProductId() <<", "<< data.GetState()<< ", "<< data.GetQuantity() << endl;
    //string key = data.GetProduct().GetProductId();
    //bondHistoricalDataService.HistoricalBond[key] = data;
}




class BondHistoricalDataExecutionListener : public ServiceListener<ExecutionOrder<Bond> >
{
public:
    BondHistoricalDataExecutionListener(BondHistoricalDataService<ExecutionOrder<Bond>> &);
    
    void ProcessAdd(ExecutionOrder<Bond> & data);
    
private:
    BondHistoricalDataService<ExecutionOrder<Bond>> & bondHistoricalDataService;
};

BondHistoricalDataExecutionListener::BondHistoricalDataExecutionListener(BondHistoricalDataService<ExecutionOrder<Bond>> & s):bondHistoricalDataService(s)
{
}

void BondHistoricalDataExecutionListener::ProcessAdd(ExecutionOrder<Bond> & data)
{
    ofstream outfile("/Users/Jessie/Desktop/hw3/Final_Project/Final_Project/historicalExecution.txt",ios::app);
       outfile << data.GetOrderId() << data.GetProduct().GetProductId() << data.GetOrderType() << data.GetVisibleQuantity() << data.GetPrice() << data.GetParentOrderId() << endl;
    //cout << data.GetOrderId() << data.GetProduct().GetProductId() << data.GetOrderType() << data.GetVisibleQuantity() << data.GetPrice() << data.GetParentOrderId() << endl;
    //string key = data.GetProduct().GetProductId();
    //bondHistoricalDataService.HistoricalBond[key] = data;
}


class BondHistoricalDataPositionListener : public ServiceListener<Position<Bond>>
{
public:
    BondHistoricalDataPositionListener(BondHistoricalDataService<Position<Bond>> &);
    
    void ProcessAdd(Position<Bond>& data);
    
private:
    BondHistoricalDataService<Position<Bond>> & bondHistoricalDataService;
};


BondHistoricalDataPositionListener::BondHistoricalDataPositionListener(BondHistoricalDataService<Position<Bond>> & s):bondHistoricalDataService(s)
{
}

void BondHistoricalDataPositionListener::ProcessAdd(Position<Bond>& data)
{
    ofstream outfile("/Users/Jessie/Desktop/hw3/Final_Project/Final_Project/historicalPosition.txt",ios::app);
    outfile <<  data.GetProduct() << data.GetAggregatePosition() << endl;
    //string key = data.GetProduct().GetProductId();
    //bondHistoricalDataService.HistoricalBond[key] = data;
}


class BondHistoricalDataRiskListener : public ServiceListener<PV01<Bond>>
{
public:
    BondHistoricalDataRiskListener(BondHistoricalDataService<PV01<Bond>> &);
    
    virtual void ProcessAdd(PV01 <Bond> & data) override;
    
private:
    BondHistoricalDataService<PV01<Bond>> & bondHistoricalDataService;
    
};

BondHistoricalDataRiskListener::BondHistoricalDataRiskListener(BondHistoricalDataService<PV01<Bond>> & s):bondHistoricalDataService(s)
{
}

void BondHistoricalDataRiskListener::ProcessAdd(PV01 <Bond> & data)
{
    ofstream outfile("/Users/Jessie/Desktop/hw3/Final_Project/Final_Project/historicalRisk.txt",ios::app);
    outfile << data.GetProduct() << data.GetPV01() << data.GetQuantity() << endl;
    //cout << data.GetProduct().GetProductId() <<", "<< data.GetPV01() <<", " << data.GetQuantity() << endl;
    //string key = data.GetProduct().GetProductId();
    //bondHistoricalDataService.HistoricalBond[key] = data;
}


class BondHistoricalDataStreamingListener : public ServiceListener<PriceStream<Bond> >
{
public:
    BondHistoricalDataStreamingListener(BondHistoricalDataService<PriceStream<Bond> > &);
    
    void ProcessAdd(PriceStream<Bond> & data);
    
private:
    BondHistoricalDataService<PriceStream<Bond> > & bondHistoricalDataService;
};

BondHistoricalDataStreamingListener::BondHistoricalDataStreamingListener(BondHistoricalDataService<PriceStream<Bond> >& s):bondHistoricalDataService(s)
{
}

void BondHistoricalDataStreamingListener::ProcessAdd(PriceStream<Bond> & data)
{
    ofstream outfile("/Users/Jessie/Desktop/hw3/Final_Project/Final_Project/historicalStreaming.txt",ios::app);
    outfile <<data.GetProduct().GetProductId() <<", " << data.GetBidOrder().GetPrice()<<", " << data.GetBidOrder().GetVisibleQuantity() <<", " << data.GetOfferOrder().GetPrice() <<", " << data.GetOfferOrder().GetVisibleQuantity()<< endl;
    //cout << data.GetProduct().GetProductId() <<", " << data.GetBidOrder().GetPrice()<<", " << data.GetBidOrder().GetVisibleQuantity() <<", " << data.GetOfferOrder().GetPrice() <<", " << data.GetOfferOrder().GetVisibleQuantity()<< endl;
    //string key = data.GetProduct().GetProductId();
    //bondHistoricalDataService.HistoricalBond[key] = data;
}





#endif
