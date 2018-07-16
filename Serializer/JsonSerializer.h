//
//  JsonSerializer.h
//  Serializer
//
//  Created by Hovhannes Tsakanyan on 7/16/18.
//  Copyright © 2018 Hovhannes Tsakanyan. All rights reserved.
//

#pragma once
#ifndef JsonSerializer_h
#define JsonSerializer_h

#include "Serializer.hpp"
#include <json.h>

char* trimQuotes(char* str1, const char* str2)
{
    if(str1 && str2)
    {
        int length = (int)strlen(str2);
        if (str2[0] == '\"' && str2[length - 1] == '\"')
        {
            strncpy(str1, &str2[1], length -2);
            str1[length-2] = '\0';
        }
        else
        {
            strcpy(str1 , str2);
        }
        return  str1;
    }
    return NULL;
}

template<class MODEL>
struct JsonMeta
{
    constexpr JsonMeta(char const* name_, ATypePointer<MODEL> ptr) : name(name_), pointer(ptr) { }
    
    char const*  name;
    
    ATypePointer<MODEL> pointer;
};

template<class MODEL>
class Json
{
public:
    
    static constexpr const JsonMeta<MODEL> name[] =
    {
    };
    
    static constexpr FormaterInterface* formater()
    {
        return NULL;
    }
    //static inline constexpr const JsonMeta<MODEL>* getName()
    //{
    //    return Json<MODEL>::name;
    //};
    
};

template<class MODEL>
class JsonCFormaterInterface : public FormaterInterface
{
public:
    JsonCFormaterInterface(): jsonObject_(NULL) {}
    
    virtual ~JsonCFormaterInterface()
    {
        release();
    }
    
    virtual bool initFrom(void *data_, void* dest_)
    {
        MODEL* dest = (MODEL*)dest_;
        if(dest == NULL) return false;
        
        const char* data = (const char*)data_;
        if(data == NULL) return false;
        
        int len = (int)strlen(data);
        if(len <= 0) return false;
        
        release();
        json_tokener *tok = json_tokener_new();
        
        json_object* myJson = json_tokener_parse_ex(tok, data, len);
        
        json_object_object_foreach(myJson, key, val)
        {
            for(int i = 0; i < dim(Json<MODEL>::name); i++)
            {
                if(!strcmp(key, Json<MODEL>::name[i].name))
                {
                    switch (Json<MODEL>::name[i].pointer.type) {
                        case ModelMemberType::INT32 :
                            dest->*(Json<MODEL>::name[i].pointer.AInt) = json_object_get_int(val);
                            break;
                        case ModelMemberType::INT64 :
                            dest->*(Json<MODEL>::name[i].pointer.AInt64) = json_object_get_int64(val);
                            break;
                            // TODO: add other types
                        default:
                            break;
                    }
                    break;
                }
            }
        }
        
        json_object_put(myJson);
        json_tokener_free(tok);
        return true;
//
//        for(int i = 0; i < dim(Json<MODEL>::name); i++)
//        {
//            switch (Json<MODEL>::name[0].pointer.type) {
//                case ModelMemberType::INT32 :
//                    if(!add(dest->*(Json<MODEL>::name[0].pointer.AInt))) return false;
//                    break;
//
//                default:
//                    break;
//            }
//            cout << "MyFormaterInterface " << Json<MODEL>::name[i].name << " value = " << dest->*(Json<MODEL>::name[i].pointer.AInt) <<  endl;
//
//            cout << "Dimention is " << dim(Json<MODEL>::name) << endl;
//        }
        
    }
    
    virtual bool format(void* dest_)
    {
        MODEL* dest = (MODEL*)dest_;
        if(dest == NULL) return false;
        release();
        
        jsonObject_ = json_object_new_object();
        
        for(int i = 0; i < dim(Json<MODEL>::name); i++)
        {
            switch (Json<MODEL>::name[i].pointer.type) {
                case ModelMemberType::INT32 :
                    json_object_object_add(jsonObject_, Json<MODEL>::name[i].name, json_object_new_int(dest->*(Json<MODEL>::name[i].pointer.AInt)));
                    break;
                case ModelMemberType::INT64 :
                    json_object_object_add(jsonObject_, Json<MODEL>::name[i].name, json_object_new_int64(dest->*(Json<MODEL>::name[i].pointer.AInt64)));
                    break;
                    // TODO: add other types
                default:
                    break;
            }
        }
        
        return true;
    }
    
    virtual void* getBuffer()
    {
        if(jsonObject_ != NULL)
        {
            return (void*)json_object_to_json_string(jsonObject_);
        }
        else return NULL;
    }
    
    virtual int getBufferSize()
    {
        if(jsonObject_ != NULL)
        {
            return (int)strlen(json_object_to_json_string(jsonObject_));
        }
        else return 0;
    }
    
private:
    template <class T>
    bool add(T) {return false; };
    
    inline void release()
    {
        if(jsonObject_)
        {
            json_object_put(jsonObject_);
            jsonObject_ = NULL;
        }
    }
    
private:
    json_object *jsonObject_;
};

#endif /* JsonSerializer_h */
