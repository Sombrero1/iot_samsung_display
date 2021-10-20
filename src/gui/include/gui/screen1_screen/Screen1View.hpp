#ifndef SCREEN1VIEW_HPP
#define SCREEN1VIEW_HPP

#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>
#include <mbed.h>

class Screen1View : public Screen1ViewBase
{
public:
    Screen1View();
    virtual ~Screen1View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    void setCalendarEventValue(int value_walk, int value_driving, int value_public, string desc);
    
protected:
    //  static const uint16_t TEXTAREA1_SIZE = 10;
    // touchgfx::Unicode::UnicodeChar textArea1Buffer[TEXTAREA1_SIZE];
};

#endif // SCREEN1VIEW_HPP
