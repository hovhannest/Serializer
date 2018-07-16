//
//  Serializer.hpp
//  Serializer
//
//  Created by Sergey Chilingaryan on 7/3/18.
//  Copyright © 2018 Hovhannes Tsakanyan. All rights reserved.
//

#ifndef Serializer_hpp
#define Serializer_hpp

#include <stdio.h>

/*#include <vector>
#include <tuple>
#include <iostream>

using namespace std;

class A
{
public:
    int i;
    int j;
    char c;

public:
    A() : i(4), j(9), c(69)
    {
        
    }
};

typedef int A::* APT;
typedef char A::* APC;

enum ATypes
{
    CHAR = 0,
    INT
};

union ATypePointers
{
    APT AInt;
    APC AChar;
    
    ATypePointers(APT t) : AInt(t) {}
    ATypePointers(APC t) : AChar(t) {}
};

int main()
{
    A a;
    
    std::vector<std::tuple<ATypes, ATypePointers>> pointers = {
        std::make_tuple<ATypes, ATypePointers>(ATypes::INT, ATypePointers(&A::i)),
        std::make_tuple<ATypes, ATypePointers>(ATypes::CHAR, ATypePointers(&A::c)),
        std::make_tuple<ATypes, ATypePointers>(ATypes::INT, ATypePointers(&A::j))
    };
    
    for (int i = 0; i < pointers.size(); i++)
    {
        if (std::get<0>(pointers[i]) == ATypes::INT)
        {
            cout << "pa[" << i << "] = " << a.*(std::get<1>(pointers[i]).AInt) << endl;
        }
        else if (std::get<0>(pointers[i]) == ATypes::CHAR)
        {
            cout << "pa[" << i << "] = " << a.*(std::get<1>(pointers[i]).AChar) << endl;
        }
    }
    
    return 0;
}

//*/
#endif /* Serializer_hpp */
