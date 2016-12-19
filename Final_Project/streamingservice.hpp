/**
 * streamingservice.hpp
 * Defines the data types and Service for price streams.
 *
 * @author Breman Thuraisingham
 */
//  Created by Jiaxi(Jessie) Wang on 2016/12/5.
#ifndef STREAMING_SERVICE_HPP
#define STREAMING_SERVICE_HPP

#include "soa.hpp"
#include "orderbook.hpp"

/**
 * A price stream order with price and quantity (visible and hidden)
 */
class PriceStreamOrder
{

public:

  // ctor for an order
  PriceStreamOrder(double _price, long _visibleQuantity, long _hiddenQuantity, PricingSide _side);
    PriceStreamOrder();
  // The side on this order
  PricingSide GetSide() const;

  // Get the price on this order
  double GetPrice() const;

  // Get the visible quantity on this order
  long GetVisibleQuantity() const;

  // Get the hidden quantity on this order
  long GetHiddenQuantity() const;

private:
  double price;
  long visibleQuantity;
  long hiddenQuantity;
  PricingSide side;

};

PriceStreamOrder::PriceStreamOrder(){
    
}


/**
 * Price Stream with a two-way market.
 * Type T is the product type.
 */
template<typename T>
class PriceStream
{

public:

  // ctor
  PriceStream(const T &_product, const PriceStreamOrder &_bidOrder, const PriceStreamOrder &_offerOrder);
    PriceStream();
    PriceStream(const PriceStream<T> &);

  // Get the product
  const T& GetProduct() const;

  // Get the bid order
  const PriceStreamOrder& GetBidOrder() const;

  // Get the offer order
  const PriceStreamOrder& GetOfferOrder() const;

private:
  T product;
  PriceStreamOrder bidOrder;
  PriceStreamOrder offerOrder;

};

template<typename T>
PriceStream<T>::PriceStream()
{
}



/**
 * Streaming service to publish two-way prices.
 * Keyed on product identifier.
 * Type T is the product type.
 */
template<typename T>
class StreamingService : public Service<string,PriceStream <T> >
{

public:

  // Publish two-way prices
  void PublishPrice(const PriceStream<T>& priceStream) = 0;

};


class BondStreamingService:public Service<string, PriceStream<Bond>>
{
    
private:
    
    map<string, PriceStream<Bond>> priceStream;
    vector<ServiceListener<PriceStream<Bond>>*> listeners;
    
public:
    
    //virtual functions
    PriceStream<Bond>& GetData(string  key);
    void OnMessage(PriceStream<Bond> &data) ;
    void AddListener(ServiceListener<PriceStream<Bond>> &listener) ;
    const vector< ServiceListener<PriceStream<Bond>> *>& GetListeners() const;
    

    //other functions
    void PublishPrice(const PriceStream<Bond>& priceStream);
    
};


PriceStream<Bond>& BondStreamingService::GetData(string key)
{
    return priceStream[key];
}

void BondStreamingService::OnMessage(PriceStream<Bond> &data)
{
    string key = data.GetProduct().GetProductId();
    priceStream[key] = data;
    for(int i = 0; i < listeners.size(); i++){
        listeners[i]->ProcessAdd(data);
    }
}

void BondStreamingService::AddListener(ServiceListener<PriceStream<Bond>> &listener)
{
    listeners.push_back(&listener);
}

const vector< ServiceListener<PriceStream<Bond>> *>& BondStreamingService::GetListeners() const
{
    return listeners;
}

void BondStreamingService::PublishPrice(const PriceStream<Bond>& priceStream){
    //cout << "Publishing Price " << endl;
}






template<typename T>
PriceStream<T>::PriceStream(const PriceStream<T>& source):product(source.product),bidOrder(source.bidOrder),offerOrder(source.offerOrder)
{
    
}

PriceStreamOrder::PriceStreamOrder(double _price, long _visibleQuantity, long _hiddenQuantity, PricingSide _side)
{
  price = _price;
  visibleQuantity = _visibleQuantity;
  hiddenQuantity = _hiddenQuantity;
  side = _side;
}

double PriceStreamOrder::GetPrice() const
{
  return price;
}

long PriceStreamOrder::GetVisibleQuantity() const
{
  return visibleQuantity;
}

long PriceStreamOrder::GetHiddenQuantity() const
{
  return hiddenQuantity;
}

template<typename T>
PriceStream<T>::PriceStream(const T &_product, const PriceStreamOrder &_bidOrder, const PriceStreamOrder &_offerOrder) :
  product(_product), bidOrder(_bidOrder), offerOrder(_offerOrder)
{
}

template<typename T>
const T& PriceStream<T>::GetProduct() const
{
  return product;
}

template<typename T>
const PriceStreamOrder& PriceStream<T>::GetBidOrder() const
{
  return bidOrder;
}

template<typename T>
const PriceStreamOrder& PriceStream<T>::GetOfferOrder() const
{
  return offerOrder;
}

#endif
