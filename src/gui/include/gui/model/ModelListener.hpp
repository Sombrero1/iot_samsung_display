#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <gui/model/Model.hpp>
#include <mbed.h>


class ModelListener
{
public:
    ModelListener() : model(0) {}
    
    virtual ~ModelListener() {}

    void bind(Model* m)
    {
        model = m;
    }
    
    virtual void newCalendarEventValue(int value_walk, int value_driving, int value_public, string desc){};
protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
