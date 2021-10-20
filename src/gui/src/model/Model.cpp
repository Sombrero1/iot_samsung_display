#include <cstdio>
#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include "ESP8266Interface.h"
#include "mbed.h"
#include "picojson.h"

ESP8266Interface wifi(D1, D0);

Model::Model() : modelListener(0)
{
    wifi.connect("alex", "20488888", NSAPI_SECURITY_WPA2);
}

int * get_msg(const char *recv){
    printf("rcv: %s\r\n", recv);
    picojson::value v;
    const char *json = recv;
    string err =picojson::parse(v, json, json + strlen(json));
    static int states[4];//w d t b
    if (! err.empty()) {
        return states;
    }
    states[0] = (int) v.get("walking").get<double>();
    states[2] = (int) v.get("transit").get<double>();
    states[1] = (int) v.get("driving").get<double>();

   return states;
}

string get_desc_event(const char *recv){
    picojson::value v;
    const char *json = recv;
    string err =picojson::parse(v, json, json + strlen(json));

    string desc = "Безымянное событие";
    if (!err.empty()) {
        return desc;
    }
    desc =  v.get("desc").get<string>();


   return desc;
}

char* host = "192.168.238.122";

void Model::action(){
        printf("http_request\n");
        SocketAddress a;
        wifi.get_ip_address(&a);
 
        TCPSocket socket;
        socket.open(&wifi);

        wifi.gethostbyname(host, &a);
        a.set_port(8080);
        socket.connect(a);


        char sbuffer[100];
        snprintf(sbuffer, sizeof(sbuffer), "GET /home HTTP/1.1\r\nHost: %s\r\nContent-type: application/json\r\n\r\n", host);

        printf("%s\n", sbuffer);

        //int scount = socket.send(sbuffer, sizeof sbuffer);

         char rbuffer[512];
        int rcount = socket.recv(rbuffer, sizeof rbuffer);

}


int a = 0;
int j = 0;
void Model::tick()
{
    a += 1;
    if(a % 100000 == 0){
        
        a = 0;
        j += 1;
        
        
      
        int ret = 0;
        SocketAddress a;
        wifi.get_ip_address(&a);
        printf("IP address: %s\n", a.get_ip_address() ? a.get_ip_address() : "None");

        TCPSocket socket;
        socket.open(&wifi);

        wifi.gethostbyname(host, &a);
        a.set_port(8080);
        socket.connect(a);


        char sbuffer[100];
        snprintf(sbuffer, sizeof(sbuffer), "GET /routes HTTP/1.1\r\nHost: %s\r\nContent-type: application/json\r\n\r\n", host);

        int scount = socket.send(sbuffer, sizeof sbuffer);
        // printf("sent %d [%.*s]\n", scount, strstr(sbuffer, "\r\n") - sbuffer, sbuffer);

        char rbuffer[512];
        int rcount = socket.recv(rbuffer, sizeof rbuffer);
        // printf("recv %d %d \n data %s\n", rcount, strstr(rbuffer, "\r\n") - rbuffer, rbuffer );
        const char * t = strstr(rbuffer, "\r\n\r\n");
        printf("%s\n", t);

        int* states = get_msg(t);  
      
        
        modelListener->newCalendarEventValue(*(states), *(states + 1),  *(states +  2), get_desc_event(t));     
     
        socket.close();
 

    }
}
