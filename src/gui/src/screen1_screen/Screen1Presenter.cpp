#include <gui/screen1_screen/Screen1View.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>
#include "mbed.h"


Screen1Presenter::Screen1Presenter(Screen1View& v)
    : view(v)
{

}

void Screen1Presenter::activate()
{
   

}

void Screen1Presenter::newCalendarEventValue(int value_walk, int value_driving, int value_public, string desc)
{
    view.setCalendarEventValue(value_walk, value_driving, value_public, desc);
}

void Screen1Presenter::deactivate()
{

}
