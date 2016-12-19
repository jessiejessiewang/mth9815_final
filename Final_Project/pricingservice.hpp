/**
 * pricingservice.hpp
 * Defines the data types and Service for internal prices.
 *
 * @author Breman Thuraisingham
 */
//  Created by Jiaxi(Jessie) Wang on 2016/12/5.
#ifndef PRICING_SERVICE_HPP
#define PRICING_SERVICE_HPP

#include <string>
#include "soa.hpp"
#include "products.hpp"
#include "AlgoStream.hpp"

/**
 * A price object consisting of mid and bid/offer spread.
 * Type T is the product type.
 */
template<typename T>
class Price
{

public:

  // ctor for a price
  Price(T _product, double _mid, double _bidOfferSpread);
    Price();

  // Get the product
  T GetProduct() const;

  // Get the mid price
  double GetMid() const;

  // Get the bid/offer spread around the mid
  double GetBidOfferSpread() const;

   // Price<T>& operator=(Price<T>&);
private:
  const T product;
  double mid;
  double bidOfferSpread;

};


template<typename T>
Price<T>::Price(){
    
}



/**
 * Pricing Service managing mid prices and bid/offers.
 * Keyed on product identifier.
 * Type T is the product type.
 */
template<typename T>
class PricingService : public Service<string,Price <T> >
{
};


// Implemetations for PricingService
template<typename T>
Price<T>::Price(T _product, double _mid, double _bidOfferSpread) :
product(_product)
{
    mid = _mid;
    bidOfferSpread = _bidOfferSpread;
}
template<typename T>
T Price<T>::GetProduct() const
{
    return product;
}

template<typename T>
double Price<T>::GetMid() const
{
    return mid;
}

template<typename T>
double Price<T>::GetBidOfferSpread() const
{
    return bidOfferSpread;
}
/*
template<typename T>
Price<T>& Price<T>::operator=(Price<T> &source)
{
    product = source.product;
    mid = source.mid;
    bidOfferSpread = source.bidOfferSpread;
    return * this;
}
*/


//*****************************************************************************
// * Bond Pricing Service managing mid prices and bid/offers for bond products.
// * Keyed on product identifier.
//*****************************************************************************
class BondPricingService : public PricingService<Bond>
{
private:
    map<string,Price<Bond>> PriceBook;
    
    vector<ServiceListener<Price<Bond>>*> listeners;
    
public:
    virtual Price<Bond>& GetData(string Key) override;
    
    virtual void OnMessage(Price<Bond>& source) override;
    
    virtual void AddListener(ServiceListener<Price<Bond>>& listener) override;
    
    virtual const vector< ServiceListener<Price<Bond>>*>& GetListeners() const override;

    
    BondPricingService& operator=(BondPricingService&);
    
};



// Bond Pricing Connector, reading data from "prices.txt"
class BondPricingConnector : public Connector<Price<Bond>>
{
private:
    BondPricingService & BondPricing;
    
public:
    BondPricingConnector(string K, BondPricingService& source);
    
    virtual void Publish(Price<Bond> &data);
    
};




// Implementations for Bond Pricing Connector
BondPricingConnector::BondPricingConnector(string address,BondPricingService& source):BondPricing(source)
{
    ifstream file2(address);
    string value[3];
    int itr = 6000000;
    while (itr-- )
    {
        getline(file2,value[0],',');
        getline(file2,value[1],',');
        getline(file2,value[2]);
        
        int size = value[1].size();
        if(value[1][size-1] == '+')
            value[1][size-1] = '4';
        double price = (double)(value[1][size-1]-'0')/256.0 + (double)(value[1][size-2]-'0')/32.0 + 10.0 * (double)(value[1][size-3]-'0')/32.0 + (double)(value[1][size-5]-'0') + 10.0 * double(value[1][size-6]-'0') ;
        if(size == 7)
            price = price + 100.0;
        double gap = (double)(value[2][value[2].size()-1]-'0')/256.0;
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
        //cout << "look here !!!!!!!!!!!!!!!!" << endl;
        //cout << value[0] <<endl;
        //cout << CUSIP << endl;
        Bond Mybond(value[0],CUSIP,"T",0.2,*maturity);
        Price<Bond> Myprice(Mybond,price,gap);
        //cout << value[0] << value [1] << endl;
        BondPricing.OnMessage(Myprice);
    }
}


void BondPricingConnector::Publish(Price<Bond> &data){
    //cout << " Get txt file" << endl;
}





// Implementations for Bond Pricing Service

Price<Bond>& BondPricingService :: GetData(string Key)
{
    return PriceBook[Key];
}

void BondPricingService :: OnMessage(Price<Bond> & source)
{
    string key = source.GetProduct().GetProductId();
    //cout << "inmessage" << endl;
    //PriceBook[key] = source;
    for(int i = 0; i < listeners.size(); i++){
        listeners[i]->ProcessAdd(source);
    }
}

void BondPricingService::AddListener(ServiceListener<Price<Bond>>& listener)
{
    listeners.push_back(&listener);
}


const vector<ServiceListener<Price<Bond>>*>& BondPricingService :: GetListeners() const
{
    return listeners;
}




#endif
