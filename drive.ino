
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#define LED_BUILTIN 2   // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED

// Set these to your desired credentials.
const char *ssid = "";
const char *password = "";

#define led_p1 d23;
#define led_n1 d22;
#define led_p2 d14;
#define led_n2 d32;
#define motor1in1 d6;
#define motor1in2 d7;
#define motor2in3 d3;
#define motor2in4 d4;
#define enable1 d5;
#define enable2 d2;
#define servoPin d1;
int pos = 0;
int m1high=255,m2high=255;
int m1mid=180,m2mid=180;
int m1low=120,m2low=120;
Servo myservo;  // create servo object to control a servo
// 16 servo objects can be created on the ESP32
 
WiFiServer server(80);


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");

  pinMode(motor1in1, OUTPUT);
  pinMode(motor1in2, OUTPUT);
  pinMode(motor2in3, OUTPUT);
  pinMode(motor2in4, OUTPUT);
  pinMode(enable1,OUTPUT);
  pinMode(enable2,OUTPUT);
  pinMode(led_p1, OUTPUT);
  pinMode(led_n1, OUTPUT);
  pinMode(led_p2, OUTPUT);
  pinMode(led_n2, OUTPUT);
  delay(10);
  myservo.attach(servoPin, 500, 2400); // attaches the servo on pin 18 to the servo object
 
}

void loop() {

  digitalWrite(led_n1, LOW);
  digitalWrite(led_p1, HIGH);
  digitalWrite(led_n2, LOW);
  digitalWrite(led_p2, HIGH);
  WiFiClient client = server.available();   // listen for incoming clients
  if (client) {                                       
    String currentLine = "";                
    while (client.connected()) {            
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c);                    
        if (c == '\n') {                    

          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            
            client.print("Click <a href=\"/F\">forward</a>.<br>");
            client.print("Click <a href=\"/B\"></a>Backward.<br>");
            client.print("Click <a href=\"/R\"></a>left.<br>");
            client.print("Click <a href=\"/L\"></a>right.<br>");
            client.print("Click <a href=\"/S\"></a>Stop.<br>");
            
            
            

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /F")) {
          digitalWrite(motor1in1, LOW);
          digitalWrite(motor1in2, HIGH);
          //FOR Right Motor Forward
          digitalWrite(motor2in3, HIGH);
          digitalWrite(motor2in4, LOW);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /B")) {
          //FOR Left Motor Backward
          digitalWrite(motor1in1, HIGH);
          digitalWrite(motor1in2, LOW);
          //FOR Right Motor Backward
          digitalWrite(motor2in3, LOW);
          digitalWrite(motor2in4, HIGH);                // GET /L turns the LED off
        }

        if(currentLine.endsWith("GET /R")){
          //FOR Left Motor Forward
          digitalWrite(motor1in1, LOW);
          digitalWrite(motor1in2, HIGH);
    //FOR Right Motor Backward
          digitalWrite(motor2in3, LOW);
          digitalWrite(motor2in4, HIGH);
        }
        if(currentLine.endsWith("GET /L")){
          //FOR Left Motor Backward
          digitalWrite(motor1in1, HIGH);
          digitalWrite(motor1in2, LOW); 
    //FOR Right Motor Forward
          digitalWrite(motor2in3, HIGH);
          digitalWrite(motor2in4, LOW);
          
        }
        if(currentLine.endsWith("GET /S")){
          //FOR STOP
          digitalWrite(motor1in1, LOW);
          digitalWrite(motor1in2, LOW);
          //FOR STOP
          digitalWrite(motor2in3, LOW);
          digitalWrite(motor2in4, LOW);
          
        }
        
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}