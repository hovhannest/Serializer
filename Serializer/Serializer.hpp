//
//  Serializer.hpp
//  Serializer
//
//  Created by Sergey Chilingaryan on 7/3/18.
//  Copyright © 2018 Hovhannes Tsakanyan. All rights reserved.
//

#pragma once
#ifndef Serializer_hpp
#define Serializer_hpp

#include <iostream>

using namespace std;

template <typename T, int N>
int dim(T(&)[N])
{
    return N;
}

enum ModelMemberType
{
    UNKNOWN = 0,
    INT32,
    INT64,
    FLOAT,
    DOUBLE,
    CHAR,
    CHARP,
    CHARPC, // constant size array ie char[]
};

template<class MODEL>
struct ATypePointer
{
    ModelMemberType type;
    
    union
    {
        int MODEL::* AInt;
        int64_t MODEL::* AInt64;
        float MODEL::* AFloat;
        double MODEL::* ADouble;
        char MODEL::* AChar;
        char* MODEL::* ACharP;
        char (MODEL::* ACharPC)[];
    };
    
    // TODO: cna we do this with a single template?
    constexpr ATypePointer(int MODEL::* t) : AInt(t), type(ModelMemberType::INT32) {}
    constexpr ATypePointer(int64_t MODEL::* t) : AInt64(t), type(ModelMemberType::INT64) {}
    constexpr ATypePointer(float MODEL::* t) : AFloat(t), type(ModelMemberType::FLOAT) {}
    constexpr ATypePointer(double MODEL::* t) : ADouble(t), type(ModelMemberType::DOUBLE) {}
    constexpr ATypePointer(char MODEL::* t) : AChar(t), type(ModelMemberType::CHAR) {}
    constexpr ATypePointer(char* MODEL::* t) : ACharP(t), type(ModelMemberType::CHARP) {}
    constexpr ATypePointer(char MODEL::* t[]) : ACharPC(t), type(ModelMemberType::CHARPC) {}
};

class FormaterInterface
{
public:
    virtual ~FormaterInterface() {}
    
    virtual bool initFrom(void *data, void* dest) = 0;
    virtual bool format(void* dest) = 0;
    virtual void* getBuffer() = 0;
    virtual int getBufferSize() = 0;
};

// TODO: Create interface for FormaterHelper (destructori harcn a mnum) and create instance of FORMATER in "void _initFrom(void *data)" insight ModelHelper class, call that interface to format the data. save buffer, bufflenght and formatType in ModelHelper.


template<class MODEL>
class ModelInterface
{
public:
    ModelInterface() : formater_(NULL) {}
    virtual ~ModelInterface()
    {
        if(formater_)
        {
            delete formater_;
            formater_ = NULL;
        }
    }
    
    template<template<typename M> class FORMATER>
    bool initFrom(void *data)
    {
        FormaterInterface* formater = getFormater<FORMATER<MODEL> >();
        
        if(formater)
        {
            formater->initFrom(data, ((MODEL*)this));
//            if(!formater_->start(data)) return false;
//
//            for(int i = 0; i < dim(FORMATER<MODEL>::name); i++)
//            {
//                switch (FORMATER<MODEL>::name[0].pointer.type) {
//                    case ModelMemberType::INT :
//                        if(!formater_->add(((MODEL*)this)->*(FORMATER<MODEL>::name[0].pointer.AInt))) return false;
//                        break;
//
//                    default:
//                        break;
//                }
//            }
//
//            if(!formater_->end()) return false;
            return true;
        }
        else return false;
//
//        cout << "ModelHelper " << FORMATER<MODEL>::name[0].name << " value = " << ((MODEL*)this)->*(FORMATER<MODEL>::name[0].pointer.AInt) <<  endl;
//
//        cout << "Dimention is " << dim(FORMATER<MODEL>::name) << endl;
    }
    
    template<template<typename M> class FORMATER>
    bool format()
    {
        FormaterInterface* formater = getFormater<FORMATER<MODEL> >();
        
        if(formater)
        {
            formater->format(((MODEL*)this));
            return true;
        }
        else return false;
    }
    
    // constexpr is not working here, but if formater could behive as constexpr, then we could get the json string at compile time
    inline void* getBuffer() const
    {
        return (formater_ != NULL) ? formater_->getBuffer() : NULL;
    }
    
    inline int getBufferSize() const
    {
        return (formater_ != NULL) ? formater_->getBufferSize() : NULL;
    }
    
public:
    
protected:
    template<class T>
    inline FormaterInterface* getFormater()
    {
        // TODO: avoid this new/delate if possible!
        if(formater_)
        {
            delete formater_;
            formater_ = NULL;
        }
        formater_ = T::formater();
        return formater_;
    }
    FormaterInterface* formater_;
};

#endif /* Serializer_hpp */
