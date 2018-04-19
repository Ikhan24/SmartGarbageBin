#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#define FIREBASE_HOST "aaaaaaaaaaaaa.firebaseio.com"
#define FIREBASE_AUTH "BdsadsadsadsarpiyxffqY87MbgvfRtk5"
#define WIFI_SSID "1234"
#define WIFI_PASSWORD "abcd"

int TRIG_PIN = 5; //d1
int ECHO_PIN = 4; //d2

// Anything over 400 cm (23200 us pulse) is "out of range"
const unsigned int MAX_DIST = 23200;
void setup() {
   pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

int n = 0;

void loop() {
   unsigned long t1;
  unsigned long t2;
  unsigned long pulse_width;
  int cm;
  int inches;
  //
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  // Wait for pulse on echo pin
  while ( digitalRead(ECHO_PIN) == 0 );

   // Measure how long the echo pin was held high (pulse width)
  // Note: the micros() counter will overflow after ~70 min
  t1 = micros();
  while ( digitalRead(ECHO_PIN) == 1);
  t2 = micros();
  pulse_width = t2 - t1;

  // Calculate distance in centimeters and inches. The constants
  // are found in the datasheet, and calculated from the assumed speed 
  //of sound in air at sea level (~340 m/s).
  cm = pulse_width / 58.0;
  inches = pulse_width / 148.0;
  
// Print out results
  if ( pulse_width > MAX_DIST ) {
    Serial.println("Out of range");
  } else {
    Serial.print(cm);
    Serial.print(" cm \t");
    Serial.print(inches);
    Serial.println(" in");
  }
  
  // Wait at least 60ms before next measurement
  delay(60);

Firebase.setFloat("distance", pulse_width);
   if (Firebase.failed()) {
      Serial.print("setting /distance failed:");
      Serial.println(Firebase.error());  
      return;
}
      delay(1000);

       // update value
  if ( pulse_width < 5) 
  {
   Firebase.setString("message", "Mouse Detected");
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /distance failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
 
 Serial.print("distance: ");
  Serial.println(Firebase.getInt("distance"));
  delay(1000);
  
Firebase.remove("distance");
  delay(1000);


 // set bool value
  Firebase.setBool("truth", false);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /truth failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);

  // append a new value to /logs
  String name = Firebase.pushInt("logs", n++);
  // handle error
  if (Firebase.failed()) {
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error());  
      return;
  }
  Serial.print("pushed: /logs/");
  Serial.println(name);
  delay(1000);

  }  }

