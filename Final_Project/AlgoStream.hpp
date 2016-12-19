//
//  AlgoStream.h
//  Final_Project
//
//  Created by Jessie Wang on 2016/12/18.
//  Copyright © 2016年 Jessie Wang. All rights reserved.
//

#ifndef AlgoStream_h
#define AlgoStream_h



#include "streamingservice.hpp"



template<typename T>
class AlgoStream
{
    
private:
    PriceStream<T>  priceStream;
public:
    //getter
    PriceStream<T> GetPriceStream() const;
    
    AlgoStream(PriceStream<T> & s);
    
    AlgoStream();
    
};

template<typename T>
PriceStream<T> AlgoStream<T>::GetPriceStream() const
{
    return priceStream;
}
template<typename T>
AlgoStream<T>::AlgoStream(PriceStream<T> & s):priceStream( s)
{
    
}
template<typename T>
AlgoStream<T>::AlgoStream()
{
    
}
#endif /* AlgoStream_h */
