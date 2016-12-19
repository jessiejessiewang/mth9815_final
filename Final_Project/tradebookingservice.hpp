/**
 * tradebookingservice.hpp
 * Defines the data types and Service for trade booking.
 *
 * @author Breman Thuraisingham
 */
//  Created by Jiaxi(Jessie) Wang on 2016/12/5.

#ifndef TRADE_BOOKING_SERVICE_HPP
#define TRADE_BOOKING_SERVICE_HPP

#include <string>
#include <vector>
#include <map>
#include "soa.hpp"
#include "products.hpp"
#include <fstream>
using namespace std;


// Trade sides
enum Side { BUY, SELL };

/**
 * Trade object with a price, side, and quantity on a particular book.
 * Type T is the product type.
 */
template<typename T>
class Trade
{

public:

  // ctor for a trade
  Trade(const T &_product, string _tradeId, string _book, long _quantity, Side _side);
    Trade();
  // Get the product
  const T& GetProduct() const;

  // Get the trade ID
  const string& GetTradeId() const;

  // Get the book
  const string& GetBook() const;

  // Get the quantity
  long GetQuantity() const;

  // Get the side
  Side GetSide() const;
    
private:
  T product;
  string tradeId;
  string book;
  long quantity;
  Side side;

};


template<typename T>
Trade<T>::Trade(){
    
}

/**
 * Trade Booking Service to book trades to a particular book.
 * Keyed on product identifier.
 * Type T is the product type.
 */



template<typename T>
class TradeBookingService : public Service<string,Trade <T> >
{

public:
  // Book the trade
  //virtual void BookTrade(const Trade<T> &trade) = 0;

};

//***********************************************************************************
/*BondTradeBookingService
 This should read data from trades.txt. 
 The BondTradeBookingService should be linked to a BondPositionService via a ServiceListener and send all trades there via the AddTrade() method 
 */
//***********************************************************************************



class BondTradeBookingService : public Service<string,Trade <Bond> >
{
public:
    // virtual functions inherented from Service:
    // Get data on our service given a key
    virtual Trade<Bond>& GetData(string key);
    
    // The callback that a Connector should invoke for any new or updated data
    virtual void OnMessage(Trade<Bond> &data);
    
    // Add a listener to the Service for callbacks on add, remove, and update events
    // for data to the Service.
    virtual void AddListener(ServiceListener<Trade<Bond>> &listener);
    
    // Get all listeners on the Service.
    virtual const vector< ServiceListener<Trade<Bond>>*>& GetListeners() const;
    
    // Book the trade: book one trade from the "trades.txt" file
   // virtual void BookTrade(const Trade<Bond> &trade);
    
    BondTradeBookingService();
    BondTradeBookingService(const BondTradeBookingService& );
    
private:
    map<string,Trade<Bond>> tradingBook;
    vector<ServiceListener<Trade<Bond>>*>  listeners;
};

BondTradeBookingService::BondTradeBookingService()
{
    //cout << "construct BondTradeBookingService() "<< endl;
}

BondTradeBookingService::BondTradeBookingService(const BondTradeBookingService& source):tradingBook(source.tradingBook),listeners(source.listeners)
{
}



//***********************************************************************************
// BondTradeBookingConnector: read data from the "trades.txt" and call the onMessage function for each traing data
//***********************************************************************************
class BondTradeBookingConnector : public Connector<Trade<Bond>>
{
private:
    BondTradeBookingService & BondTradeBooking;
public:
    // constructor
    BondTradeBookingConnector(string K, BondTradeBookingService & source);
    // other functions
    virtual void Publish(Trade<Bond> & data) override;
};



void BondTradeBookingConnector::Publish(Trade<Bond>& data)
{
}



//Implementations for Class Trade

template<typename T>
Trade<T>::Trade(const T &_product, string _tradeId, string _book, long _quantity, Side _side) :
  product(_product)
{
  tradeId = _tradeId;
  book = _book;
  quantity = _quantity;
  side = _side;
}

template<typename T>
const T& Trade<T>::GetProduct() const
{
  return product;
}

template<typename T>
const string& Trade<T>::GetTradeId() const
{
  return tradeId;
}

template<typename T>
const string& Trade<T>::GetBook() const
{
  return book;
}

template<typename T>
long Trade<T>::GetQuantity() const
{
  return quantity;
}

template<typename T>
Side Trade<T>::GetSide() const
{
  return side;
}


//Implementations for Class BondTradeBookingService

Trade<Bond>& BondTradeBookingService :: GetData(string key){
    return tradingBook[key];
}

void BondTradeBookingService::OnMessage(Trade<Bond> &data){
    //string key = data.GetProduct().GetProductId();
    //tradingBook[key] = data;
    //cout << "tradebooking Service call listeners" << endl;
    for(int i = 0; i < listeners.size(); i++){
        listeners[i]->ProcessAdd(data);
    }
}
void BondTradeBookingService::AddListener(ServiceListener<Trade<Bond>> &listener){
    listeners.push_back(&listener);
}
const vector<ServiceListener<Trade<Bond>>*>& BondTradeBookingService::GetListeners() const{
    return listeners;
}



// Implementation for BondTradeBookingConnector
// string K is the address of "trades.txt"
BondTradeBookingConnector::BondTradeBookingConnector(string address,BondTradeBookingService & source):BondTradeBooking(source)
{
    ifstream file1(address);
    string value[5];
    int itr = 60;
    while (itr-- )
    {
        for(int i = 0; i < 4; i++)
        {
            getline(file1,value[i],',');
        }
        getline(file1,value[4]);
        date * maturity;
        if(value[0] == "912828U40")
        {
            maturity = new date(2017,Dec,1);
        }
        else if(value[0] == "912828U32")
        {
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
        Bond Mybond(value[0],CUSIP,"T", 0.2, *maturity);
        Side Myside = SELL;
        if(value[4] == "BUY")
        {
            Myside = BUY;
        }
        Trade<Bond> MyTrade(Mybond, value[1], value[2], long(stoi(value[3])), Myside);
        //cout << MyTrade.GetQuantity() << MyTrade.GetBook() << MyTrade.GetProduct() << endl;
        //cout << "read data from trades.txt "<< endl;
        BondTradeBooking.OnMessage(MyTrade);
    }
}



#endif
