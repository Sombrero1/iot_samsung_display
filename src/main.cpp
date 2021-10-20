#include "mbed.h"
#include "stm32746g_discovery_lcd.h"
#include <touchgfx/canvas_widget_renderer/CanvasWidgetRenderer.hpp>
#include <touchgfx/hal/BoardConfiguration.hpp>
#include <touchgfx/hal/HAL.hpp>
#include "ESP8266Interface.h"



// ESP8266Interface wifi(D1, D0);
Thread* gui_thread;

void guiThread(void)
{
    touchgfx::hw_init();
    touchgfx::touchgfx_init();
    touchgfx::HAL::getInstance()->taskEntry();
}

// bool get_msg(Websocket *ws){
//     char recv [100];
//     bool receive = ws->read(recv);
//     printf("hier:%d\n",receive);
//     if (receive) {
//             printf("rcv: %s\r\n", recv);

//             picojson::value v;
//             const char *json = recv;
//             picojson::parse(v, json, json + strlen(json));

//             int states[4];//w d t b
//             //printf("res error? %s\r\n", err.c_str());
//             states[0] = (int) v.get("walking").get<double>();
//             states[2] = (int) v.get("transit").get<double>();
//             states[1] = (int) v.get("driving").get<double>();
//             states[3] = (int) v.get("beep").get<double>();
//             //states[4] = (int) v.get("temperature").get<double>();

//             // if(states[0]==-1 && states[1] == -1 && states[2] == -1) {
//             //     lcd.cls();
//             //     lcd.printf("No routes");
//             // }
//             // else set_notification(states);
//         }
//     return receive;
// }

// // void open_websocket()
// // {   

   
// //     Websocket ws("ws://192.168.43.187:8080/websocket/30", &wifi);
// //     printf("web coonn\n");
// //      bool err = ws.connect();

// //      printf("state: %d\n",err);

// //      while(err==false) {
// //          return;
// //      }

// //     // wait_us(1*1000000);
// //     // ws.send("success");
// //     wait_us(1 * 1000000);

// //     while (get_msg(&ws)){
// //         wait_us(21*1000000);
// //     };

// //     return;//не смогли по какой-то причине считать
// // }


// // BufferedSerial  dev(D1, D0, 115200);
 
// bool get_ssid_and_password(string *ssid, string*pass){
//     // dev.set_baud(115200);

//     // wait_us(1 * 1000000);
//     // dev.write("AT+RST\r\n", sizeof("AT+RST\r\n"));
//     // wait_us(0.5* 1000000);
//     // dev.write("AT+CWMODE_CUR=2\r\n", sizeof("AT+CWMODE_CUR=2\r\n"));
//     // wait_us(0.5* 1000000);
//     // dev.write("AT+CWSAP_CUR=\"ESP8266\",\"1234567890\",5,3\r\n", sizeof("AT+CWSAP_CUR=\"ESP8266\",\"1234567890\",5,3\r\n"));
//     // wait_us(0.5* 1000000);
//     // dev.write("AT+CIPMUX=1\r\n", sizeof("AT+CIPMUX=1\r\n"));
//     // wait_us(0.5* 1000000);
//     // dev.write("AT+CIPAP_CUR=\"192.168.5.1\"\r\n", sizeof("AT+CIPAP_CUR=\"192.168.5.1\"\r\n"));
//     // wait_us(0.5* 1000000);
//     // dev.write("AT+CIPSERVER=1,1001\r\n", sizeof("AT+CIPSERVER=1,1001\r\n"));

//     // // write_msg_lcd("enter SSID and\n password");

//      string t = "{\"ssid\":\"alex\", \"password\":\"20488888\"}";
//     // while(t.find("+IPD") == -1){
//     //     t = "";
//     //     char chr;
//     //     if(dev.readable()){
//     //         do{
//     //             dev.read(*chr, 1);
//     //             t = t + chr;
//     //         }
//     //         while(chr!='\n');
//     //     }
//     // }

//     printf("read : %s\n",t.c_str());
    
//     if(t.find('{') != -1){
//         picojson::value v;
        
//         string m = t.substr(t.find('{'), t.find('}')-t.find('{')+1);
//         const char *json = m.c_str();

//         picojson::parse(v, json, json + strlen(json));
//                 //printf("res error? %s\r\n", err.c_str());
//         *ssid =  v.get("ssid").get<string>();
//         *pass =  v.get("password").get<string>();

//         printf("ssid and password : %s,%s\n",ssid->c_str(),pass->c_str());

//         return true;
//     }
//     else{
//         return false;
//     }
// }
// int a = 20;
// int get_temp(){
//     return a;
// }



int main()
{
    gui_thread = new Thread(osPriorityNormal, 8 * 1024, NULL, "GUI Thread");
    gui_thread->start(guiThread);

       


    // int ret = 0;
    // string ssid = "alex";
    // string pass = "20488888";
    
    
   
    // do{
    //     while(!get_ssid_and_password(&ssid, &pass)){
    //         // write_msg_lcd("try again");
    //     }
    //   //  dev.write("AT+CWMODE_CUR=1\r\n", sizeof("AT+CWMODE_CUR=1\r\n"));
    //      printf("to client\n");
    //     wait_us(0.5* 1000000);
    
    //     ret = wifi.connect("alex","20488888", NSAPI_SECURITY_WPA_WPA2);
    //     // write_msg_lcd("Connecting to\n"+ssid);
    //     printf("\nConnecting to %s...\n", ssid.c_str());
    //     if (ret != 0) {
    //         if (ret == -3008 || ret == -3006 ||ret==-3003){
    //             // write_msg_lcd("Wi-Fi: %s. Conn error.\n Check pass");
    //         }
    //         else{
    //             // write_msg_lcd("Wi-Fi: %s. Error. Reset device");
    //             printf("\nConnection error: %d\n", ret);
    //         }
    //         wait_us(2*1000000);
    //     }
    // }while(ret!=0);
   

    // printf("Success\n\n");

    
    // // write_msg_lcd("success conn");
    // while(true){
    //     //open_websocket();
    //     //write_msg_lcd("reconnect to \nserv");
    //     wait_us(1*1000000);
    // }
    // wifi.disconnect();

   

    return 0;
}
