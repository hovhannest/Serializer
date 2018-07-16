//
//  main.cpp
//  Serializer
//
//  Created by Sergey Chilingaryan on 7/3/18.
//  Copyright © 2018 Hovhannes Tsakanyan. All rights reserved.
//

#include <iostream>
#include <string>

#include "Serializer.hpp"
#include "JsonSerializer.h"

using namespace std;

// TODO: change it to struct{int type; union{};}

//template<class MODEL>
//union ATypePointers
//{
//    int MODEL::* AInt;
//
//    char MODEL::* AChar;
//
//    constexpr ATypePointers(int MODEL::* t) : AInt(t) {}
//
//    constexpr ATypePointers(char MODEL::* t) : AChar(t) {}
//
//};

class MyModel : public ModelInterface<MyModel>
{
public:
    
    template<template<typename M> class FORMATER>
    void initFrom(const char* data)
    {
        // proxy for typecast
        
        cout << "char* called" << endl;
        
        ModelInterface::initFrom<FORMATER>((void*)data);
    }
    
    int i;
};



/////////////////////////////////////////////////////////////////




//
//class B
//{
//public:
//    constexpr B(char const *i)  : name(i) {}
//
//    char const *name;
//};
//
//class A
//{
//public:
//    constexpr A(char const *i) : name(i) {}
//
//
//    char const * name;
//};


template<>
class Json<MyModel>
{
public:
    
    static constexpr const JsonMeta<MyModel> name[] =
    {
        
        JsonMeta<MyModel>("AaaAAaaAA", ATypePointer<MyModel>(&MyModel::i)),
        
        JsonMeta<MyModel>("bbBbBBBBbb", ATypePointer<MyModel>(&MyModel::i)),
        
    };
    
    
    static FormaterInterface* formater()
    {
        return new JsonCFormaterInterface<MyModel>();
    }
    
};



constexpr const JsonMeta<MyModel> Json<MyModel>::name[];

//template<>
//class Json<MyModel>
//{
//public:
//    static inline constexpr const JsonMeta<MyModel>* getName()
//    {
//        return Json<MyModel>::name;
//    };
//
//    static constexpr const JsonMeta<MyModel> name[]
//    {
//        JsonMeta<MyModel>("JsonNNN", ATypePointers<MyModel>(&MyModel::i)),
//        JsonMeta<MyModel>("Json", ATypePointers<MyModel>(&MyModel::i))
//    };
//};


template<class MODEL>
constexpr const JsonMeta<MODEL> Json<MODEL>::name[];



//template <>
//class Json<MyModel>
//{
//public:
//    static inline constexpr const char* getName()
//    {
//        return "MyModel Json";
//    }
//};


//
//constexpr MyModel foo()
//{
//    return  MyModel();
//}



//template<>
//void ModelBase::initFrom<Json>(void *data)
//{
//    cout << "Json init" << endl;
//}


int main(int argc, const char * argv[]) {
    
    // insert code here...
    
    std::cout << "Hello, World!\n";
    
    const char* jsonString = "{\"key\": \"value\"}";
    
    MyModel model;
    
    model.initFrom<Json>("{\"AaaAAaaAA\": 199}");
    
    cout << "Adter init " << model.i << endl;
    
    model.format<Json>();
    
    if(model.getBuffer())
    cout << (const char*) model.getBuffer();
    
    model.initFrom<Json>(jsonString);
    
    
    return 0;
    
}
