/**
 * positionservice.hpp
 * Defines the data types and Service for positions.
 *
 * @author Breman Thuraisingham
 */
//  Created by Jiaxi(Jessie) Wang on 2016/12/5.
#ifndef POSITION_SERVICE_HPP
#define POSITION_SERVICE_HPP

#include <string>
#include <map>
#include "soa.hpp"
#include "tradebookingservice.hpp"
#include "products.hpp"

using namespace std;

/**
 * Position class in a particular book.
 * Type T is the product type.
 */
template<typename T>
class Position
{

public:

  // ctor for a position
  Position(const T &_product);
    Position();

  // Get the product
  const T& GetProduct() const;

  // Get the position quantity
  long GetPosition(string &book);

  // Get the aggregate position
  long GetAggregatePosition();
    
    void AddPosition(string &book,long quantitiy);

private:
  T product;
  map<string,long> positions;

};

template<typename T>
Position<T>::Position(){
}
//***********************************************************************************
// Bond Position Service, Derived from Position Service class.
//***********************************************************************************
class BondPositionService : public Service<string,Position <Bond>>{
public:
    // virtual functions inherented from Service:

    Position<Bond>& GetData(string key);
    // The callback that a Connector should invoke for any new or updated data
    virtual void OnMessage(Position<Bond> &data);
    // Add a listener to the Service for callbacks on add, remove, and update events
    // for data to the Service.
    virtual void AddListener(ServiceListener<Position<Bond>> &listener);
    // Get all listeners on the Service.

    const vector< ServiceListener<Position<Bond>>*>& GetListeners() const;
    
    // Add a trade to the service
    void AddTrade(const Trade<Bond> &trade);
    
private:
    map<string,Position<Bond>> positionBook;
    vector<ServiceListener<Position<Bond>>*>  listeners;
};





// Impletations for BondPositionService class


Position<Bond>& BondPositionService::GetData(string key){
    return positionBook[key];
}

void BondPositionService::OnMessage(Position<Bond> &data){
    string key = data.GetProduct().GetProductId();
    positionBook[key] = data;
}
void BondPositionService::AddListener(ServiceListener<Position<Bond>> &listener){
    listeners.push_back(&listener);
}


const vector<ServiceListener<Position<Bond>>*>& BondPositionService::GetListeners() const{
    return listeners;
}




// Add a trade to the service
void BondPositionService :: AddTrade( const Trade<Bond> &trade){
    
    string key  = trade.GetProduct().GetProductId();
    long quantity = trade.GetQuantity();
    if(trade.GetSide() == SELL){
        quantity = -quantity;
    }
    string tradebook  = trade.GetBook();
    //cout <<  << endl;
    positionBook[key].AddPosition(tradebook, quantity);
    
    // call RiskService
    for(int i = 0; i < listeners.size(); i++){
        //cout << "call risk service via listener" << endl;
        listeners[i]->ProcessAdd(positionBook[key]);
    }
}








//***********************************************************************************
// Position Service listener( from TradeBookingService )
//***********************************************************************************

class TradeBookingPositionListener : public ServiceListener<Trade<Bond>>{
private:
    BondPositionService &bondPositionService;
public:
    virtual void ProcessAdd(Trade<Bond> & data){
        //cout << "data goes to Position Service via listener " << endl;
        bondPositionService.AddTrade(data);
    }
     TradeBookingPositionListener(BondPositionService & source);
};


TradeBookingPositionListener::TradeBookingPositionListener(BondPositionService & source):bondPositionService(source)
{
}




// Impletations for Position class


template<typename T>
Position<T>::Position(const T &_product) :
  product(_product)
{
}

template<typename T>
const T& Position<T>::GetProduct() const
{
  return product;
}

template<typename T>
long Position<T>::GetPosition(string &book)
{
  return positions[book];
}

template<typename T>
long Position<T>::GetAggregatePosition()
{
  // No-op implementation - should be filled out for implementations
    long sum = positions["TRSY1"] + positions["TRSY2"] + positions["TRSY3"];
    return sum;
}




template<typename T>
void Position<T>::AddPosition(string &book, long quantity)
{
    map<string, long>::iterator itr;
    for (itr = positions.begin(); itr != positions.end(); itr++)
    {
        if (itr->first == book)
        {
            positions[book] += quantity;
            break;
        }
    }
    if (itr == positions.end())
    {
        positions[book] = quantity;
    }
}
#endif
