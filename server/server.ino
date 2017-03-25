// TODO: create our own IP address

// telnet defaults to port 23
TCPServer server = TCPServer(23);
// linked list
TCPClient clients[10];

int state; // 0 or 1

// Variable for the button's current state.
// button_mode = 1 when the button is up, and 0 when the button is pressed.
// This variable is 'static' and persists because it is declared outside of a function.
int button_mode = 1;

bool sleeping_state = false;

// We will be using D1 to control our LED
int led_pin = D7;

// Our button wired to D0
int button_pin = D0;

SYSTEM_MODE(MANUAL);

void setup()
{

  // For input, we define the
  // pushbutton as an input-pullup
  // this uses an internal pullup resistor
  // to manage consistent reads from the device

  pinMode( button_pin , INPUT_PULLUP); // sets pin as input

  // We also want to use the LED
  pinMode( led_pin , OUTPUT ); // sets pin as output

  // start listening for clients
  //server.begin();

  IPAddress myAddress(192, 168, 1, 102);
  IPAddress netmask(255, 255, 255, 0);
  IPAddress gateway(192, 168, 1, 1);
  IPAddress dns(192, 168, 1, 1);
  WiFi.setStaticIP(myAddress, netmask, gateway, dns);

  // now let's use the configured IP
  WiFi.useStaticIP();

  // You need to disconnect and reconnect so it uses the static IP
  // Just calling connect() isn't enough
  WiFi.on();
  WiFi.connect();

  while(!WiFi.ready())
  {
    delay(100);
  }
  Particle.process();

  // Make sure your Serial Terminal app is closed before powering your device
  Serial.begin(9600);
  // Now open your Serial Terminal, and hit any key to continue!
  while(!Serial.isConnected()) {
      delay(300);
  }
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.subnetMask());
  Serial.println(WiFi.gatewayIP());
  Serial.println(WiFi.SSID());
}

void loop()
{
  //int new_state;
  bool state_changed = false;

  // check state of pin
  if ((digitalRead(button_pin) == LOW) && (button_mode == 1)) {
    // Button was up before, but is pressed now. Set the button to pressed
    // and report that to the serial monitor.
    button_mode = 0; // Button is pressed.
    Serial.println("Button has been pressed.");
    delay(100); // small delay to account for button bounce.
  } else if ((digitalRead(button_pin) == HIGH) && (button_mode == 0)) {
    // Button was down before, but is released now. Set the button to
    // released and report that to the serial monitor.
    button_mode = 1; // Button isn't pressed
    Serial.println("Button has been released");
    state_changed = true;
    sleeping_state = !sleeping_state;
    delay(100); // small delay to account for button bounce.
  }

  if (state_changed) {
    if (sleeping_state == true) {
      digitalWrite(led_pin, HIGH);
    } else {
      digitalWrite(led_pin, LOW);
    }
  }

  // if there's a new client available, add to array and notify
  TCPClient client = server.available();
  // notify clients
  if (client.connected()) {
    Serial.println("Found new client");
    clients[0] = client;  
  }

  // if we changed state
  if (state_changed) { 
    // notify the clients!
    for (int i = 0; i < 1; i++) {
      client = clients[i];
      if (!client.connected()) {
        Serial.println("Removing disconnected client");
        //client[0] = null;
      } else  {
        // send mode ON or OFF
        //byte msg = sleeping_state ? (byte) 1 : (byte) 0;

        char buffer[40]; 
        size_t length = sprintf(buffer,  "%d", sleeping_state);   

        Serial.println("Sending client a message");
        Serial.println(buffer);
        client.write((uint8_t *)buffer, length);
      }
    }
  }

}
