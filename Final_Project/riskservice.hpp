/**
 * riskservice.hpp
 * Defines the data types and Service for fixed income risk.
 *
 * @author Breman Thuraisingham
 */
//  Created by Jiaxi(Jessie) Wang on 2016/12/5.
#ifndef RISK_SERVICE_HPP
#define RISK_SERVICE_HPP

#include "soa.hpp"
#include "positionservice.hpp"

/**
 * PV01 risk.
 * Type T is the product type.
 */
template<typename T>
class PV01
{

public:

  // ctor for a PV01 value
  PV01(const T &_product, double _pv01, long _quantity);
    PV01();

  // Get the product on this PV01 value
  const T& GetProduct() const;

  // Get the PV01 value
  double GetPV01() const;

  // Get the quantity that this risk value is associated with
  long GetQuantity() const;
    
    void SetQuantity(int _quantity);
    void SetPV01(double _pv01);
    void SetProduct(T _product);

private:
  T product;
  double pv01;
  long quantity;

};

template<typename T>
PV01<T>::PV01(){
    
}


template<typename T>
const T& PV01<T>::GetProduct() const{
    return product;
}


template<typename T>
double PV01<T>::GetPV01() const{
    return pv01;
}
template<typename T>
void PV01<T>::SetPV01(double _pv01){
    pv01 = _pv01;
}
template<typename T>
void PV01<T>::SetProduct(T _product){
    product = _product;
}

/**
 * A bucket sector to bucket a group of securities.
 * We can then aggregate bucketed risk to this bucket.
 * Type T is the product type.
 */
template<typename T>
class BucketedSector
{

public:

  // ctor for a bucket sector
  BucketedSector(const vector<T> &_products, string _name);

  // Get the products associated with this bucket
  const vector<T>& GetProducts() const;

  // Get the name of the bucket
  const string& GetName() const;

private:
  vector<T> products;
  string name;

};

/**
 * Risk Service to vend out risk for a particular security and across a risk bucketed sector.
 * Keyed on product identifier.
 * Type T is the product type.
 */
template<typename T>
class RiskService : public Service<string,PV01 <T> >
{

public:

  // Add a position that the service will risk
  virtual void AddPosition(Position<T> &position) = 0;

  // Get the bucketed risk for the bucket sector
  //double GetBucketedRisk(const BucketedSector<T> &sector) const = 0;

};

/**
 * Bond Risk Service to vend out risk for a particular security and across a risk bucketed sector.
 * Keyed on product identifier.
 */

class BondRiskService : public RiskService<Bond>
{
    
private:
    map<string,PV01<Bond>> PV01book;
    vector<ServiceListener<PV01<Bond>>*>  listeners;
public:
    // virtual fucntions
    virtual PV01<Bond>& GetData(string key) override;
    
    virtual void OnMessage(PV01<Bond> & data) override;
    
    virtual void AddListener(ServiceListener<PV01<Bond>> & listener) override;
    
    virtual const vector<ServiceListener<PV01<Bond>>*>&  GetListeners() const override;


    // other functions
    virtual void AddPosition(Position<Bond> &position) override;
    // Get the bucketed risk for the bucket sector
    double GetBucketedRisk(const BucketedSector<Bond> &sector);
};




class BondRiskPositionListener: public ServiceListener<Position<Bond>>
{
public:
    BondRiskPositionListener(BondRiskService&);
    
    void ProcessAdd(Position<Bond> &data);
    
private:
    BondRiskService & bondriskService;
};

BondRiskPositionListener::BondRiskPositionListener(BondRiskService& source):bondriskService(source)
{
}

void BondRiskPositionListener::ProcessAdd(Position<Bond>& data)
{
    //cout << "data goes to Risk Service via listener " << endl;
    bondriskService.AddPosition(data);
}













// Implementations of BondRiskService

PV01<Bond>& BondRiskService :: GetData(string key){
    return PV01book[key];
}

void BondRiskService :: AddListener(ServiceListener<PV01<Bond>> & listener){
    listeners.push_back(&listener);
}


const vector<ServiceListener<PV01<Bond>>*>& BondRiskService::GetListeners() const{
    return listeners;
}



void BondRiskService :: AddPosition(Position<Bond> &position)
{
    string key = position.GetProduct().GetProductId();
    long sum = position.GetAggregatePosition();
    PV01book[key].SetQuantity(sum);
    double _pv01 = 0;
    if (key == "912828U40"){
        _pv01 = 0.019742933;
    }
    else if (key == "912828U32"){
        _pv01 = 0.029365628;
    }
    else if (key == "912828U65") {
        _pv01 = 0.047432991;
    }
    else if (key == "912828U57") {
        _pv01 = 0.064416310;
    }
    else if (key == "912828U24") {
        _pv01 = 0.084494428;
    }
    else if (key == "912810RU4") {
        _pv01 = 0.188279281;
    }
    PV01book[key].SetPV01(_pv01);
    PV01book[key].SetProduct(position.GetProduct());
    for(int i = 0; i < listeners.size(); i++){
        listeners[i]->ProcessAdd(PV01book[key]);
        //cout<< key << PV01book[key].GetProduct().GetProductId()<<endl;
    }
}


void BondRiskService::OnMessage(PV01<Bond> & data){
    string key = data.GetProduct().GetProductId();
    PV01book[key] = data;
    for(int i = 0; i < listeners.size(); i++){
        listeners[i]->ProcessAdd(data);
    }
}




// Implementations of PV01 and BucketedSector

template<typename T>
PV01<T>::PV01(const T &_product, double _pv01, long _quantity) :
  product(_product)
{
  pv01 = _pv01;
  quantity = _quantity;
}

template<typename T>
long PV01<T>::GetQuantity() const{
    return quantity;
}

template<typename T>
void PV01<T>::SetQuantity(int _quantity){
    quantity = _quantity;
}

template<typename T>
BucketedSector<T>::BucketedSector(const vector<T>& _products, string _name) :
  products(_products)
{
  name = _name;
}

template<typename T>
const vector<T>& BucketedSector<T>::GetProducts() const
{
  return products;
}

template<typename T>
const string& BucketedSector<T>::GetName() const
{
  return name;
}


double BondRiskService::GetBucketedRisk(const BucketedSector<Bond> &sector)
{
    double sum = 0;
    for(int i = 0; i < sector.GetProducts().size();i++)
    {
        string k = sector.GetProducts()[i].GetProductId();
        sum = sum + PV01book[k].GetQuantity() * PV01book[k].GetPV01();
    }
    return sum;
}

#endif
