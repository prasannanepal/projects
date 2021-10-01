#include <ESP8266WiFi.h>
//change your ssid and password here
const char* ssid = "PrasannaNepal";
const char* password = "Nepal123";

const int trig = D4;
const int echo = D3;
const int in1 = D5;
const int in2 = D6;
const int in3 = D7;
const int in4 = D8;
const int ENA= D0;
const int ENB= D1;
const int buzz=D2;

WiFiServer server(80);
void setup() 
{
  Serial.begin(115200);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode (in1, OUTPUT);
  pinMode (in2, OUTPUT);
  pinMode (in3, OUTPUT);
  pinMode (in4, OUTPUT);
  pinMode (ENA, OUTPUT);
  pinMode (ENB, OUTPUT);
  pinMode (buzz, OUTPUT);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address on serial monitor
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");    //URL IP to be typed in mobile/desktop browser
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

long dura, dis;

void loop()
{ 
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  
  
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);  
  
  dura = pulseIn(echo, HIGH);
  dis = dura*0.034/2;
  Serial.print("Distance:");
  Serial.println(dis);
               // Match the request
           if (request.indexOf("/forward") != -1)  {  
                                if(dis<25)
                        {
                          digitalWrite(buzz, HIGH); 
                          analogWrite(ENA,900);
                          analogWrite(ENB, 900);
                          digitalWrite(in1, HIGH); 
                          digitalWrite(in2, LOW); 
                          digitalWrite(in3, LOW); 
                          digitalWrite(in4, HIGH);
                          delay(1000);
                          //stop
                          
                          digitalWrite(in1, LOW); 
                          digitalWrite(in3, LOW); 
                          digitalWrite(in2, LOW); 
                          digitalWrite(in4, LOW);
                          
                              }                

            analogWrite(ENA,900);
            analogWrite(ENB, 900);          
            digitalWrite(in1, LOW); //Start first motor
            digitalWrite(in2, HIGH);
            
            digitalWrite(in3, HIGH);
            digitalWrite(in4, LOW); //Start second motor
            delay(600);
            digitalWrite(in1, LOW); 
            digitalWrite(in3, LOW); 
            digitalWrite(in2, LOW); 
            digitalWrite(in4, LOW);
            digitalWrite(buzz, LOW); 

            
          }
          
          if (request.indexOf("/stop") != -1)  {  
            analogWrite(ENA,0);
            analogWrite(ENB, 0);
            digitalWrite(in1, LOW); //Stop first motor
            digitalWrite(in2, LOW);
          
            digitalWrite(in3, LOW); //Stop second motor
            digitalWrite(in4, LOW);
          }
          
          if (request.indexOf("/backward") != -1)  {

          analogWrite(ENA,900);
          analogWrite(ENB, 900);
          digitalWrite(in1, HIGH); 
          digitalWrite(in2, LOW); 
          digitalWrite(in3, LOW); 
          digitalWrite(in4, HIGH);

          delay(1000);
          //stop
          digitalWrite(in1, LOW); 
          digitalWrite(in3, LOW); 
          digitalWrite(in2, LOW); 
          digitalWrite(in4, LOW);
    
          }

          if (request.indexOf("/right") != -1)  {
          analogWrite(ENA,900);
          analogWrite(ENB, 900);
          digitalWrite(in1, HIGH); 
          digitalWrite(in2, LOW); 
          digitalWrite(in3, HIGH); 
          digitalWrite(in4, LOW);
          delay(1000);
          //stop
          digitalWrite(in1, LOW); 
          digitalWrite(in3, LOW); 
          digitalWrite(in2, LOW); 
          digitalWrite(in4, LOW);
          
        
          }

          if (request.indexOf("/left") != -1)  {

          analogWrite(ENA,900);
          analogWrite(ENB, 900);
            
          digitalWrite(in1, LOW); 
          digitalWrite(in2, HIGH); 
          digitalWrite(in3, LOW); 
          digitalWrite(in4, HIGH);
          delay(1000);
          //stop
          digitalWrite(in1, LOW); 
          digitalWrite(in3, LOW); 
          digitalWrite(in2, LOW); 
          digitalWrite(in4, LOW);
          
       
          }
           
   
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<h1 align=center>Prasanna Nepal Robot</h1>");
  client.println("<h2 align=center>WiFi based robot control for and obstacle detection Robot in forward direction only </h2>");
  client.println("<br><br>");
  client.println("<p align=center><a href=\"/forward\"\"><button><h1>  Forward  </h1></button></a></p><br/>");
  client.println("<p align=center><a href=\"/stop\" ><button><h1>   Stop    </h1></button></a></p><br/>");
  client.println("<p align=center><a href=\"/backward\"\" ><button> <h1>  Reverse  </h1></button></a></p><br/>");
  client.println("<p align=center><a href=\"/right\"\" ><button><h1>   Right    </h1></button></a></p><br/>");
  client.println("<p align=center><a href=\"/left\"\" ><button> <h1>  Left  </h1></button></a></p><br/>");
  client.println("</html>");
  delay(1);
  Serial.print("Client disonnected");
  Serial.print(""); 
  delay(0);
}
