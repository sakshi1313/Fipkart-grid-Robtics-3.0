
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#define LED_BUILTIN 2   // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED

// Set these to your desired credentials.
const char *ssid = "";
const char *password = "";

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
}

void loop() {
  WiFiClient client = server.available();   
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