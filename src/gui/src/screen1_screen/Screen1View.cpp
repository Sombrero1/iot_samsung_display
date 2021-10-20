
#include <gui/screen1_screen/Screen1View.hpp>
#include <sstream>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>



Screen1View::Screen1View()
{
   
}


void Screen1View::setupScreen()
{
   
    Unicode::snprintf(textArea3Buffer, TEXTAREA3_SIZE, "%d", 3);
    textArea3.resizeToCurrentTextWithAlignment();
   
  

    //textArea1.resizeToCurrentText();
    
    Screen1ViewBase::setupScreen();

 

    Unicode::snprintf(textArea1Buffer, TEXTAREA1_SIZE, "%d", 27);
    //    Unicode::snprintf(textArea1Buffer, TEXTAREA1_SIZE, "%d", "1");
    textArea1.resizeToCurrentTextWithAlignment();
    textArea2.resizeToCurrentTextWithAlignment();


    
        // std::string s("Last note\0");
        // const unsigned char* m = reinterpret_cast<const unsigned char*>(s.c_str());
       
        
        // Unicode::UnicodeChar* t = new Unicode::UnicodeChar[s.size()];
        // Unicode::fromUTF8(m, t, 10);

        // printf("%d %d\n", int(t[0]), int(s[0]));
 
      
        // Unicode::snprintf(textArea4Buffer, TEXTAREA4_SIZE, "%s", t);

        
  
}

void Screen1View::setCalendarEventValue(int value_walk, int value_driving, int value_public, string desc)
{
        // printf("%d\n%d\n%d\n", value_walk, value_driving,  value_public);
        
        Unicode::snprintf(textArea1Buffer, TEXTAREA1_SIZE, "%d", value_walk);
        textArea1.resizeToCurrentTextWithAlignment();
        textArea1.invalidate();

        Unicode::snprintf(textArea2Buffer, TEXTAREA2_SIZE, "%d", value_driving);
        textArea2.resizeToCurrentTextWithAlignment();
        textArea2.invalidate();

        Unicode::snprintf(textArea3Buffer, TEXTAREA3_SIZE, "%d", value_public);
        textArea3.resizeToCurrentTextWithAlignment();
        textArea3.invalidate();

     
        Unicode::strncpy(textArea4Buffer, desc.c_str(), desc.size());
        textArea4.resizeToCurrentTextWithAlignment();
        textArea4.invalidate();

        // std::string s("abcd\0");
        // const unsigned char* m = reinterpret_cast<const unsigned char*>(s.c_str());
       
        
        // Unicode::UnicodeChar* t = new Unicode::UnicodeChar[s.size()];
        // Unicode::fromUTF8(m, t, 10);

        // printf("%d %d\n", int(t[0]), int(s[0]));
 
      
        // Unicode::snprintf(textArea4Buffer, TEXTAREA4_SIZE, "%s", t);
        // textArea4.resizeToCurrentTextWithAlignment();
        // textArea4.invalidate();
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

