//
//  executionorder.h
//  Final_Project
//
//  Created by Jessie Wang on 2016/12/18.
//  Copyright © 2016年 Jessie Wang. All rights reserved.
//

#ifndef executionorder_h
#define executionorder_h

#include "products.hpp"
#include "orderbook.hpp"


enum OrderType { FOK, IOC, MARKET, LIMIT, STOP };

enum Market { BROKERTEC, ESPEED, CME };


//An execution order that can be placed on an exchange.
//Type T is the product type
template<typename T>
class ExecutionOrder
{
public:
    
    // ctor for an order
    ExecutionOrder(const T &_product, PricingSide _side, string _orderId, OrderType _orderType, double _price, double _visibleQuantity, double _hiddenQuantity, string _parentOrderId, bool _isChildOrder);
    ExecutionOrder();
    // Get the product
    const T& GetProduct() const;
    
    // Get the order ID
    const string& GetOrderId() const;
    
    // Get the order type on this order
    OrderType GetOrderType() const;
    
    // Get the price on this order
    double GetPrice() const;
    
    // Get the visible quantity on this order
    long GetVisibleQuantity() const;
    
    // Get the hidden quantity
    long GetHiddenQuantity() const;
    
    // Get the parent order ID
    const string& GetParentOrderId() const;
    
    // Is child order?
    bool IsChildOrder() const;
    
private:
    T product;
    PricingSide side;
    string orderId;
    OrderType orderType;
    double price;
    double visibleQuantity;
    double hiddenQuantity;
    string parentOrderId;
    bool isChildOrder;
    
};

template<typename T>
ExecutionOrder<T>::ExecutionOrder()
{
    
}

template<typename T>
ExecutionOrder<T>::ExecutionOrder(const T &_product, PricingSide _side, string _orderId, OrderType _orderType, double _price, double _visibleQuantity, double _hiddenQuantity, string _parentOrderId, bool _isChildOrder) :
product(_product)
{
    side = _side;
    orderId = _orderId;
    orderType = _orderType;
    price = _price;
    visibleQuantity = _visibleQuantity;
    hiddenQuantity = _hiddenQuantity;
    parentOrderId = _parentOrderId;
    isChildOrder = _isChildOrder;
}

template<typename T>
const T& ExecutionOrder<T>::GetProduct() const
{
    return product;
}

template<typename T>
const string& ExecutionOrder<T>::GetOrderId() const
{
    return orderId;
}

template<typename T>
OrderType ExecutionOrder<T>::GetOrderType() const
{
    return orderType;
}

template<typename T>
double ExecutionOrder<T>::GetPrice() const
{
    return price;
}

template<typename T>
long ExecutionOrder<T>::GetVisibleQuantity() const
{
    return visibleQuantity;
}

template<typename T>
long ExecutionOrder<T>::GetHiddenQuantity() const
{
    return hiddenQuantity;
}

template<typename T>
const string& ExecutionOrder<T>::GetParentOrderId() const
{
    return parentOrderId;
}

template<typename T>
bool ExecutionOrder<T>::IsChildOrder() const
{
    return isChildOrder;
}


#endif /* executionorder_h */
