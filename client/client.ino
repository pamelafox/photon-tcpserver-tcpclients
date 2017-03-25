/*
 * TODOs:
 * - Determine whether we should use SYSTEM_MODE(MANUAL) and call Particle.process ourself.
 *    Or use system threaded mode. It eventually conks out, and may be due to WiFi module buffer. 
 */

TCPClient client;

byte server[] = { 192, 168, 1, 102 }; 

int led_pin = D7;

void setup() {
  // TODO: Power LED strip instead.
  pinMode( led_pin , OUTPUT ); 

  // Make sure your Serial Terminal app is closed before powering your device
  Serial.begin(9600);
  // Now open your Serial Terminal, and hit any key to continue!
  while(!Serial.isConnected()) {
      delay(300);
  }

  Serial.println("connecting...");

  if (client.connect(server, 23)) {
    Serial.println("connected");
    client.println("can you hear me?");
  } else {
    Serial.println("connection failed");
  }
}

void loop() {
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
    if ('1' == c) {
      Serial.println("Turning it on");
      digitalWrite(led_pin, HIGH);
    } else {
      Serial.println("Turning it off");
      digitalWrite(led_pin, LOW);
    }
  }

  /* TODO: Have it try to re-connect to server in loop()
     if its no longer connected, as server may start up after client does. */
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    for(;;); // or exit the program
  }
}
