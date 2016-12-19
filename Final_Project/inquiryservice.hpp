/**
 * inquiryservice.hpp
 * Defines the data types and Service for customer inquiries.
 *
 * @author Breman Thuraisingham
 */
//  Created by Jiaxi(Jessie) Wang on 2016/12/5.


#ifndef INQUIRY_SERVICE_HPP
#define INQUIRY_SERVICE_HPP

#include "soa.hpp"
#include "tradebookingservice.hpp"
#include <iostream>
#include <fstream>

// Various inqyury states
enum InquiryState { RECEIVED, QUOTED, DONE, REJECTED, CUSTOMER_REJECTED };

/**
 * Inquiry object modeling a customer inquiry from a client.
 * Type T is the product type.
 */
template<typename T>
class Inquiry
{

public:

  // ctor for an inquiry
  Inquiry(string _inquiryId, const T &_product, Side _side, long _quantity, double _price, InquiryState _state);
    Inquiry();
  // Get the inquiry ID
  const string& GetInquiryId() const;

  // Get the product
  const T& GetProduct() const;

  // Get the side on the inquiry
  Side GetSide() const;

  // Get the quantity that the client is inquiring for
  long GetQuantity() const;

  // Get the price that we have responded back with
  double GetPrice() const;

  // Get the current state on the inquiry
  InquiryState GetState() const;

private:
  string inquiryId;
  T product;
  Side side;
  long quantity;
  double price;
  InquiryState state;

};

template<typename T>
Inquiry<T>::Inquiry(){
    
}


/**
 * Service for customer inquirry objects.
 * Keyed on inquiry identifier (NOTE: this is NOT a product identifier since each inquiry must be unique).
 * Type T is the product type.
 */
template<typename T>
class InquiryService : public Service<string,Inquiry <T> >
{

public:

  // Send a quote back to the client
  //virtual void SendQuote(const string &inquiryId, double price) = 0;

  // Reject an inquiry from the client
  //virtual void RejectInquiry(const string &inquiryId) = 0;

};

class BondInquiryService : public InquiryService<Bond>
{
public:
    BondInquiryService();
    ~BondInquiryService();
    
    // Send a quote back to the client
    //virtual void SendQuote(const string &inquiryId, double price) override;
    
    // Reject an inquiry from the client
    //void RejectInquiry(const string &inquiryId) override;
    
    void OnMessage(Inquiry <Bond> & data) override;
    
    Inquiry <Bond>& GetData(string key) override;
    
    void AddListener(ServiceListener<Inquiry <Bond>>& listener) override;
    
    const vector<ServiceListener<Inquiry <Bond>>*>& GetListeners() const override;
    
    void AddConnector(Connector<Inquiry<Bond>> & connector);

private:
    map<string, Inquiry<Bond>> inquiryBook;
    
    vector<ServiceListener<Inquiry<Bond>>*> listeners;
    
    vector<Connector<Inquiry<Bond>>* > connectorList;
};



BondInquiryService::BondInquiryService()
{
    
}
BondInquiryService::~BondInquiryService()
{
    
}



class BondInquiryConnector: public Connector<Inquiry<Bond>>
{
public:
    BondInquiryConnector(string K, BondInquiryService & source);
    
    void Publish(Inquiry<Bond> & data) override;
    
private:
    BondInquiryService & bondInquiryService;
};



BondInquiryConnector::BondInquiryConnector(string address,BondInquiryService & source):bondInquiryService(source)
{
    ifstream f1(address);
    
    string value[7];
    int itr = 60;
    while (itr-- )
    {
        for(int i = 0; i < 6; i++)
        {
            getline(f1,value[i],',');
        }
        getline(f1,value[6]);
        
        int size = value[3].size();
        if(value[3][size-1] == '+')
            value[3][size-1] = '4';
        double price = (double)(value[3][size-1]-'0')/256.0+(double)(value[3][size-2]-'0')/32.0 + 10.0 * (double)(value[3][size-3]-'0')/32.0+(double)(value[3][size-5]-'0')+10.0 * double(value[3][size-6]-'0') ;
        if(size == 7)
            price += 100.0;
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
        else
        {
            maturity = new date(2045,Dec,1);
            value[0] = "912810RU4";
        }
        Bond Mybond(value[0],CUSIP,"T",0.2,*maturity);
        Side side = SELL;
        if(value[3]=="BUY"){
            side = BUY;
        }
        
        long d = rand() % 10000+ itr*5000;
        Inquiry<Bond> MyInquiry(value[1] ,Mybond,side,d,price, RECEIVED);
        //cout << MyInquiry.GetState() <<endl;
        
        bondInquiryService.OnMessage(MyInquiry);
    }
}
void BondInquiryConnector::Publish(Inquiry<Bond>& data)
{
    //cout<<data.GetInquiryId()<<","<<data.GetProduct()<<","<<data.GetSide()<<","<<data.GetQuantity()<<","<<data.GetPrice()<<endl;
}


class BondInquiryConnector2: public Connector<Inquiry<Bond>>
{
public:
    BondInquiryConnector2();
    
    void Publish(Inquiry<Bond> & data);
};

BondInquiryConnector2::BondInquiryConnector2()
{
    
}

void BondInquiryConnector2::Publish(Inquiry<Bond>& data)
{
    cout<<data.GetInquiryId()<<","<<data.GetProduct()<<","<<data.GetSide()<<","<<data.GetQuantity()<<","<<data.GetPrice()<<endl;
}










void BondInquiryService::OnMessage(Inquiry <Bond> & data)
{
    string key = data.GetProduct().GetProductId();
    
    Inquiry<Bond> MyInquiry(data.GetInquiryId(),data.GetProduct(),data.GetSide(),data.GetQuantity(),data.GetPrice(),QUOTED);
    //cout << "tst " << MyInquiry.GetState() << endl;
    inquiryBook[key] = MyInquiry;
    for(int i = 0; i < listeners.size(); i++){
        listeners[i]->ProcessAdd(MyInquiry);
    }
}

Inquiry<Bond>& BondInquiryService::GetData(string key)
{
    return inquiryBook[key];
}

void BondInquiryService::AddListener(ServiceListener<Inquiry <Bond>>& listener)
{
    listeners.push_back(&listener);
}

const vector<ServiceListener<Inquiry <Bond>>*>& BondInquiryService::GetListeners() const
{
    return listeners;
}


void BondInquiryService::AddConnector(Connector<Inquiry <Bond>>& connector)
{
    connectorList.push_back(& connector);
}


template<typename T>
Inquiry<T>::Inquiry(string _inquiryId, const T &_product, Side _side, long _quantity, double _price, InquiryState _state) :
  product(_product)
{
  inquiryId = _inquiryId;
  side = _side;
  quantity = _quantity;
  price = _price;
  state = _state;
}

template<typename T>
const string& Inquiry<T>::GetInquiryId() const
{
  return inquiryId;
}

template<typename T>
const T& Inquiry<T>::GetProduct() const
{
  return product;
}

template<typename T>
Side Inquiry<T>::GetSide() const
{
  return side;
}

template<typename T>
long Inquiry<T>::GetQuantity() const
{
  return quantity;
}

template<typename T>
double Inquiry<T>::GetPrice() const
{
  return price;
}

template<typename T>
InquiryState Inquiry<T>::GetState() const
{
  return state;
}

#endif
