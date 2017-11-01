
#include <LWiFi.h>
#include <dht.h>
#define dht_dpinD2
dht DHT;
char ssid[] = "Llama net";      //  your network SSID (name)
char pass[] = "lilian1234";   // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup() {
	Serial.begin(9600);      // initialize serial communication
	pinMode(LED_BUILTIN, OUTPUT);      // set the LED pin mode

	// attempt to connect to Wifi network:
	while (status != WL_CONNECTED) {
		Serial.print("Attempting to connect to Network named: ");
		Serial.println(ssid);                   // print the network name (SSID);

		// Connect to WPA/WPA2 network. Change this line if using open or WEP network:
		status = WiFi.begin(ssid, pass);
	}
	server.begin();                           // start the web server on port 80
	printWifiStatus();                        // you're connected now, so print out the status
  Serial.begin(9600);
  delay(300);
  Serial.println("Humidity and temperature\n\n");
  delay(700);
}


void loop() {
	WiFiClient client = server.available();   // listen for incoming clients

	if (client) {                             // if you get a client,
		Serial.println("new client");           // print a message out the serial port
		String currentLine = "";                // make a String to hold incoming data from the client
		while (client.connected()) {            // loop while the client's connected
			if (client.available()) {             // if there's bytes to read from the client,
				char c = client.read();             // read a byte, then
				Serial.write(c);
				if(c == '\n' && currentLineIsBlank){
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connect: close");
            client.println("Refresh: 5");
            client.println();
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.print("<p> Temperture is ");
            client.print(t);
            client.print("</p>");
            client.print("<p> Humidity is ");
            client.print(h);
            client.println("</p>");
            client.println("</html>");
            break;                    
				if (c == '\n') {                    // if the byte is a newline character
          currentLineIsBlank = true;
					// if the current line is blank, you got two newline characters in a row.
					// that's the end of the client HTTP request, so send a response:
					if (currentLine.length() == 0) {
						// HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
						// and a content-type so the client knows what's coming, then a blank line:
						client.println("HTTP/1.1 200 OK");
						client.println("Content-type:text/html");
						client.println();

						// the content of the HTTP response follows the header:
						client.print("Click <a href=\"/H\">here</a> turn the LED on pin 7 on<br>");
						client.print("Click <a href=\"/L\">here</a> turn the LED on pin 7 off<br>");

						// The HTTP response ends with another blank line:
						client.println();
						// break out of the while loop:
						break;
					} else {    // if you got a newline, then clear currentLine:
						currentLine = "";
					}
				} else if (c != '\r') {  // if you got anything else but a carriage return character,
					currentLine += c;
					currentLineIsBlank = false;      // add it to the end of the currentLine
				}

				// Check to see if the client request was "GET /H" or "GET /L":
				if (currentLine.startsWith("GET /H")) {
					digitalWrite(LED_BUILTIN, HIGH);               // GET /H turns the LED on
				}
				if (currentLine.startsWith("GET /L")) {
					digitalWrite(LED_BUILTIN, LOW);                // GET /L turns the LED off
				}
			}
		}
		// close the connection:
		client.stop();
		Serial.println("client disonnected");
	}
  DHT.read11(dht_dpin);
  Serial.print("Humidity = ");
  Serial.print(DHT.humidity);
  Serial.print("%");
  Serial.print("temperature = ");
  Serial.print(DHT.temperature);
  Serial.println("C");
  delay(1000);
}

void printWifiStatus() {
	// print the SSID of the network you're attached to:
	Serial.print("SSID: ");
	Serial.println(WiFi.SSID());

	// print your WiFi shield's IP address:
	IPAddress ip = WiFi.localIP();
	Serial.print("IP Address: ");
	Serial.println(ip);

	// print the received signal strength:
	long rssi = WiFi.RSSI();
	Serial.print("signal strength (RSSI):");
	Serial.print(rssi);
	Serial.println(" dBm");
	// print where to go in a browser:
	Serial.print("To see this page in action, open a browser to http://");
	Serial.println(ip);
}
