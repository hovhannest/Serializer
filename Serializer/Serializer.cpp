//
//  main.cpp
//  Serializer
//
//  Created by Sergey Chilingaryan on 7/3/18.
//  Copyright © 2018 Hovhannes Tsakanyan. All rights reserved.
//

#include <iostream>
#include <string>

using namespace std;

// TODO: Create JsonHelper and move getName()   there to avoid redefination in partial specialisations
// TODO: Create interface for FormaterHelper (destructori harcn a mnum) and create instance of FORMATER in "void _initFrom(void *data)" insight ModelHelper class, call that interface to format the data. save buffer, bufflenght and formatType in ModelHelper.

template <typename T, int N>
int dim(T(&)[N])
{
    return N;
}

class ModelHelper
{
public:
    
    template<class MODEL, class FORMATER>
    
    void _initFrom(void *data)
    {
        // TODO: ((MODEL*)this) replace with staticcast? or not!
        cout << "ModelHelper " << FORMATER::name[0].name << " value = " << ((MODEL*)this)->*(FORMATER::name[0].pointer.AInt) <<  endl;
        
        cout << "Dimention is " << dim(FORMATER::name) << endl;
    }
    
};

template<class MODEL>
class ModelInterface : protected ModelHelper
{
public:
    template<template<typename M> class FORMATER>
    
    void initFrom(void *data)
    {
        _initFrom<MODEL, FORMATER<MODEL> >(NULL);
    }
    
public:
    
private:
    
};


template<class MODEL, class T>
struct ATypePointer
{
    T MODEL::* ptr;
    
    constexpr ATypePointer(T MODEL::* t) : ptr(t) {}
};


// TODO: change it to struct{int type; union{};}

template<class MODEL>
union ATypePointers
{
    int MODEL::* AInt;
    
    char MODEL::* AChar;
    
    constexpr ATypePointers(int MODEL::* t) : AInt(t) {}
    
    constexpr ATypePointers(char MODEL::* t) : AChar(t) {}
    
};

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

template<class MODEL>
struct JsonMeta
{
    constexpr JsonMeta(char const* name_, ATypePointers<MODEL> ptr) : name(name_), pointer(ptr) { }
    
    char const*  name;
    
    ATypePointers<MODEL> pointer;
};

template<class MODEL>
class Json
{
public:
    
    static constexpr const JsonMeta<MODEL> name[] =
    {
        JsonMeta<MODEL>("Dedault", ATypePointers<MODEL>(&MyModel::i)),
        
    };
    
    //static inline constexpr const JsonMeta<MODEL>* getName()
    //{
    //    return Json<MODEL>::name;
    //};
    
};



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
        
        JsonMeta<MyModel>("AaaAAaaAA", ATypePointers<MyModel>(&MyModel::i)),
        
        JsonMeta<MyModel>("bbBbBBBBbb", ATypePointers<MyModel>(&MyModel::i)),
        
    };
    
    
    
    //static inline constexpr const JsonMeta<MyModel>* getName()
    //{
    //    return Json<MyModel>::name;
    //};
    
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



constexpr MyModel foo()
{
    return  MyModel();
}



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
    
    model.i = 156;
    
    model.initFrom<Json>(jsonString);
    
    return 0;
    
}
