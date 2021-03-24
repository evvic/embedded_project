/*

ESP Steering wheel example 23.2.2021 KN
Lidar reading
  
*/

// Import required libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <Wire.h>


//////OSCAR ADDED//////
#include<SoftwareSerial.h> //Included SoftwareSerial Library



//Started SoftwareSerial at RX and TX pin of ESP8266/NodeMCU
SoftwareSerial s(14,12);
//////this creates another Serial for the default Serial ports D5  and D6

///// NOTES TO SELF OSCAR /////
//if html is not showing up, click on "tools" and click on "ESP8266 sketch data upload"



            //            network credentials


//const char* ssid = "SOURCE";
//const char* password = "Pelle!23";

//apartment wifi
const char* ssid = "NETGEAR77";
const char* password = "H1037812";

//phone hotspot
//const char* ssid = "Oscar's Hotspot";
//const char* password = "gofuckyourself";

bool    spiffsActive = false;


          // Create AsyncWebServer object on port 80
AsyncWebServer server(80);


String Lidar="";   // lidar distance value in String format

  int is;
  char buf[40];


 
void setup()
{
  // Serial port for debugging purposes
  Serial.begin(9600);
  s.begin(9600); //starts the other Serial
  pinMode(14,INPUT); //this is pin D5 on the ESP
  pinMode(12,OUTPUT);//this is pin D6 on the ESP

  
  Serial.println("");

  Serial.println("----------------");

  Serial.println("----------------");

  // Start filing subsystem
  if (SPIFFS.begin()) {
      Serial.println("SPIFFS Active");
      Serial.println();
      spiffsActive = true;
  } else {
      Serial.println("Unable to activate SPIFFS");
  }


  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
  

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
                request->send(SPIFFS, "/index.html", String(), false, processor);

 //             Serial.println(" HTML GET ");

                int paramsNr = request->params();

 //             Serial.println(" ------ ");

 //below prints some parameter quantity
 //NOTE OSCAR I commented out the next 2 lines to work better with the drive code
  //            Serial.print("Param qty: ");
  //            Serial.println(paramsNr);
      
                for (byte i=0;i<paramsNr;i++)
                    {

                        AsyncWebParameter* p = request->getParam(0);

// after clicking on a button to move, or changing the direction
//will print the name of the parameter (either dist for distance or dire for direction)
                      //Serial.print("Param name: ");
                      //Serial.println(p->name());


                        //changed so that instead of the normal Serial printing its the "s" Serial
//temp commented these 2 lines out                       // if ((p->name())=="dist"){ s.print("Move:");s.println(p->value()); };
                       // if ((p->name())=="dire"){ s.print("Turn:");s.println(p->value()); };



if((p->name())=="dist"){
String DistVal = String(p->value());
String DistString = ("Move:" + DistVal);

  for(int i=0;i < DistString.length();i++){
    s.write(DistString[i]); 
    Serial.write(DistString[i]);
    }
    s.write('/n');
  
  
  };

//prints the parameter value of the parameter name shown above
                      //Serial.print("Param value: ");
                      //Serial.println(p->value());
                    };  //    end of for loop
                    
   
              });   // server on END



    // Route to load style.css file
  server.on("/jquery-1.11.3.min.js", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/jquery-1.11.3.min.js", "text/javascript");

    Serial.println(" JS jquery GET ");
      
  });

    // Route to load style.css file
  server.on("/roundslider.min.js", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/roundslider.min.js", "text/javascript");

    Serial.println(" min JS GET ");
      
  });

  
  // Route to load style.css file
  server.on("/roundslider.min.css", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/roundslider.min.css", "text/css");
    
    Serial.println(" min css GET ");
    
  });


   //      distance query (Lidar) from web page tag is /Lid_dist to send Lidar value
  server.on("/Lid_dist", HTTP_GET, [](AsyncWebServerRequest *request)
    {
      request->send_P(200, "text/plain", getDist().c_str()); // return value to web page JS
      
    }); 


  // Start server
  server.begin();

} //  END OF setup



 // reservation 
String processor(const String& var)
{  }



String getDist() 
  {
    return Lidar;
  }


 
void loop()
{
  if (s.available() > 0) 
    {
      serial_read();
      Lidar=buf;
       
    }  
}





void serial_read()
{
  boolean done=0; 

while (done==0) // wait end of message LF
  {
      while(s.available() > 0)
      {
        char t = s.read();
        
        if(t>13)
                {   
                 buf[is]=t;
                 // Serial.println(int(t));
                 is++;
                }
  
        if(t==10){             // check if end of message  LF   
                  buf[is]=0;   // end of string
                  is=0;
                  done=1;

                      for (int j=0; j<3; j++)
                      {
                          //Serial.println(int(buf[j]));
                      }     
                  }
    
    }  // end of while serial available  

       if(done==1)
        { done=0; break; }

  }   // End of message  LF
   
       // Serial.println(buf);
      
}   // end of func
