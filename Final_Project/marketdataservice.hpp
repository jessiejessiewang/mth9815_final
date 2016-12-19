/**
 * marketdataservice.hpp
 * Defines the data types and Service for order book market data.
 *
 * @author Breman Thuraisingham
 */
//  Created by Jiaxi(Jessie) Wang on 2016/12/5.
#ifndef MARKET_DATA_SERVICE_HPP
#define MARKET_DATA_SERVICE_HPP

#include <string>
#include <vector>
#include <fstream>
#include "products.hpp"
#include "soa.hpp"
#include "algoexecutionservice.hpp"
#include "orderbook.hpp"



//Market Data Service which distributes market data
//Keyed on product identifier.
//Type T is the product type.

template<typename T>
class MarketDataService : public Service<string,OrderBook <T> >
{

public:

  // Get the best bid/offer order
  virtual const BidOffer& GetBestBidOffer(const string &productId) = 0;

  // Aggregate the order book
  virtual const OrderBook<T>& AggregateDepth(const string &productId) = 0;

};

//*****************************************
// Bond Market Data Service * Keyed on product identifier.
//*****************************************
class BondMarketDataService : public MarketDataService < Bond >
{
public:
    //virtual functions, from Service class
    virtual void OnMessage(OrderBook<Bond> & data) override;
    
    virtual OrderBook<Bond>& GetData(string key) override;
    
    virtual void AddListener(ServiceListener<OrderBook<Bond>>& listener) override;
    
    virtual const vector< ServiceListener<OrderBook<Bond>>*>& GetListeners() const override;
    // Get the best bid/offer order
    virtual const BidOffer& GetBestBidOffer(const string &productId);
    
    // Aggregate the order book
    virtual const OrderBook<Bond>& AggregateDepth(const string &productId);
    // send order to BondExecutionService
    void ExecuteOrder(OrderBook <Bond>&);
private:
    map <string, OrderBook < Bond>>  orderBook;
    vector<ServiceListener < OrderBook < Bond > > * > listeners;
};


// Bond Market Data Connector

class BondMarketDataConnector : public Connector < OrderBook < Bond >>
{
public:
    
    BondMarketDataConnector(string K, BondMarketDataService & source);
    
    virtual void Publish(OrderBook<Bond> & data) override;
    
private:
    
    BondMarketDataService & bondMarketDataService;
    
};


void BondMarketDataConnector::Publish(OrderBook<Bond>& data)
{
    
}








//implementations for BondMarketDataService

void BondMarketDataService :: OnMessage( OrderBook<Bond> & data)
{
    string key = data.GetProduct().GetProductId();
    orderBook[key] = data;
    for(int i = 0; i < listeners.size(); i++){
        listeners[i]->ProcessAdd(data);
    }
    //cout<<orderBook[key];
}

OrderBook<Bond>& BondMarketDataService :: GetData(string Key)
{
    return orderBook[Key];
}

void BondMarketDataService :: AddListener(ServiceListener<OrderBook<Bond>>& listener)
{
    listeners.push_back(&listener);
}

const vector<ServiceListener< OrderBook < Bond > > * >& BondMarketDataService::GetListeners() const
{
    return listeners;
}

// Get the best bid/offer order
const BidOffer& BondMarketDataService :: GetBestBidOffer(const string &productId)
{
    Order Bid = orderBook [productId].GetBidStack()[0];
    Order Offer = orderBook [productId].GetOfferStack()[0];
    BidOffer bidoffer (Bid, Offer);
    return bidoffer;
}

// Aggregate the order book
const OrderBook<Bond>& BondMarketDataService :: AggregateDepth(const string &productId)
{
    return orderBook[productId];
}


void BondMarketDataService::ExecuteOrder(OrderBook <Bond>&)
{
    std::cout << "Execution " << endl;
}




BondMarketDataConnector :: BondMarketDataConnector(string address,BondMarketDataService & source):bondMarketDataService(source)
{
    ifstream file1(address);
    string value[22];
    int itr = 6000000;
    while (itr-- )
    {
        for(int i = 0; i < 21; i++)
        {
            getline(file1,value[i],',');
        }
        getline(file1,value[21]);
        
        double bidofferprice[10];
        long bidoffersize[10];
        
        for(int i =0 ; i < 10; i++)
        {
            int size = value[i+2].size();
            if(value[i+2][size-1] == '+')
                value[i+2][size-1] = '4';
            bidofferprice[i] = (double)(value[i+2][size-1]-'0')/256.0+(double)(value[i+2][size-2]-'0')/32.0 + 10.0 * (double)(value[i+2][size-3]-'0')/32.0+(double)(value[i+2][size-5]-'0')+10.0 * double(value[2+1][size-6]-'0') ;
            if(size == 7)
                bidofferprice[i] += 100.0;
            bidoffersize[i] = (5-i)*1000000*3;
        }
        
        date * maturity;
        if(value[0] == "912828U40")
        {
            maturity = new date(2017,Dec,1);
        }
        else if(value[0] == "912828U73")
        {
            value[0] = "912828U32";
            maturity = new date(2018,Dec,1);
        }
        else if(value[0] == "912828U65")
        {
            maturity = new date(2020,Dec,1);
        }
        else if(value[0] == "912828U57")
        {
            maturity = new date(2022,Dec,1);
        }
        else if(value[0] == "912828U24")
        {
            maturity = new date(2025,Dec,1);
        }
        else if(value[0] == "912810RU4")
        {
            maturity = new date(2045,Dec,1);
        }
        Bond Mybond(value[0],CUSIP,"T",0.2, *maturity);
        vector<Order> Bidbook;
        vector<Order> Offerbook;
        for(int i = 0; i < 5; i++)
        {
            Order Bid(bidofferprice[i], bidoffersize[i], BID);
            Order Offer(bidofferprice[9-i], bidoffersize[9 - i], OFFER);
            Bidbook.push_back(Bid);
            Offerbook.push_back(Offer);
        }
        OrderBook < Bond> MyOrderBook(Mybond, Bidbook, Offerbook);
        //cout<<value[0] <<value[2]<<endl;
        bondMarketDataService.OnMessage(MyOrderBook);
    }
}


#endif
