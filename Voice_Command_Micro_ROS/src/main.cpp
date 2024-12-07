#include <WiFi.h>

// Replace with your WiFi credentials
const char* ssid = "1";       // Your WiFi SSID
const char* password = "";    // Your WiFi password

WiFiServer server(80); // Create a web server on port 80

// Define GPIO pins
#define GPIO_RIGHT 2
#define GPIO_LEFT 4
#define GPIO_FRONT 5
#define GPIO_BACK 18

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nConnected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
  Serial.println("Server started.");

  // Initialize GPIO pins
  pinMode(GPIO_RIGHT, OUTPUT);
  pinMode(GPIO_LEFT, OUTPUT);
  pinMode(GPIO_FRONT, OUTPUT);
  pinMode(GPIO_BACK, OUTPUT);
  
  // Ensure all pins are LOW initially
  digitalWrite(GPIO_RIGHT, LOW);
  digitalWrite(GPIO_LEFT, LOW);
  digitalWrite(GPIO_FRONT, LOW);
  digitalWrite(GPIO_BACK, LOW);
}

void controlGPIO(int option) {
  // Reset all pins to LOW
  digitalWrite(GPIO_RIGHT, LOW);
  digitalWrite(GPIO_LEFT, LOW);
  digitalWrite(GPIO_FRONT, LOW);
  digitalWrite(GPIO_BACK, LOW);

  // Set pins based on the option
  if (option == 1) {
    digitalWrite(GPIO_RIGHT, HIGH); // Move right
    Serial.println("RIGHT: GPIO_RIGHT HIGH");
  } else if (option == 2) {
    digitalWrite(GPIO_LEFT, HIGH); // Move left
    Serial.println("LEFT: GPIO_LEFT HIGH");
  } else if (option == 3) {
    digitalWrite(GPIO_FRONT, HIGH); // Move forward
    Serial.println("FRONT: GPIO_FRONT HIGH");
  } else if (option == 4) {
    digitalWrite(GPIO_BACK, HIGH); // Move backward
    Serial.println("BACK: GPIO_BACK HIGH");
  } else {
    Serial.println("All GPIOs LOW");
  }
}

void loop() {
  // Check if a client is connected
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client connected.");
    String request = "";

    // Read the client request
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c;

        // Break out of loop once the request ends
        if (request.endsWith("\r\n\r\n")) break;
      }
    }

    // Debug: Print the request to the Serial Monitor
    Serial.println(request);

    // Parse the request for GPIO control
    if (request.indexOf("/GPIO=1") != -1) {
      controlGPIO(1); // Activate GPIO_RIGHT
    } else if (request.indexOf("/GPIO=2") != -1) {
      controlGPIO(2); // Activate GPIO_LEFT
    } else if (request.indexOf("/GPIO=3") != -1) {
      controlGPIO(3); // Activate GPIO_FRONT
    } else if (request.indexOf("/GPIO=4") != -1) {
      controlGPIO(4); // Activate GPIO_BACK
    } else if (request.indexOf("/GPIO=0") != -1) {
      controlGPIO(0); // Set all GPIOs to LOW
    }

    // Send an HTTP response to the client
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<h1>ESP32 Buggy Control</h1>");
    client.println("<p><a href=\"/GPIO=1\">Move RIGHT</a></p>");
    client.println("<p><a href=\"/GPIO=2\">Move LEFT</a></p>");
    client.println("<p><a href=\"/GPIO=3\">Move FORWARD</a></p>");
    client.println("<p><a href=\"/GPIO=4\">Move BACKWARD</a></p>");
    client.println("<p><a href=\"/GPIO=0\">STOP</a></p>");
    client.println("</html>");

    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
  }
}
