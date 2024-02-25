/*
  Author: Arvid Pettersson

  Date: 2024-02-25

  Description: This program works as a radar, utilizing an ultrasonic sensor and an Oled screen. 
  You can follow the sensors movements on the screen and if it detects an object it will draw that on the screen
*/

#include <Servo.h> // Includes the Servo library
#include <Adafruit_GFX.h> //Includes Adafruit libraries
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128 // defines Variable SCREEN_WIDTH to the OLED display width in pixels
#define SCREEN_HEIGHT 64 // defines Variable SCREEN_HEIGHT to the OLED display height in pixels

// Defines Trig and Echo pins of the Ultrasonic Sensor
const int trigPin = 11;
const int echoPin = 10;
// Variables for the duration and the distance
long duration;
int distance;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Servo myServo; // Creates a servo object for controlling the servo motor

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600);
  myServo.attach(12); // Defines the pin used by the servo motor
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { //if the Oled screen dosn't start, notify the user by writing a message in the serial monitor
    Serial.println("SSD1306 allocation failed");
    for (;;);
  }
}
void loop() {
  // rotates the servo motor from 15 to 165 degrees, the servo motor turns +5 degrees every time the loop is ran
  for (int i = 15; i <= 165; i += 5) {
    myServo.write(i);
    delay(50);
    distance = calculateDistance();
    updateOled(i, distance);



    Serial.println ("grader:" + String(i)); // Sends the current degree into the Serial Port
    Serial.print(","); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
    Serial.println("distans:" + String(distance)); // Sends the distance value into the Serial Port
    Serial.print("."); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
  }
  // rotates the servo motor from 165 to 15 degrees, the servo motor turns -5 degrees every time the loop is ran
  for (int i = 165; i > 15; i -= 5) {
    myServo.write(i);
    delay(50);
    distance = calculateDistance();
    updateOled(i, distance);

    //Serial.println("grader:" + String(i));
    //Serial.print(",");
    Serial.println("Distans:" + String(distance));
    Serial.print(".");
  }
  delay (100);
}
// Function for calculating the distance measured by the Ultrasonic sensor
int calculateDistance() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
  distance = (duration / 29 / 2); //((duration * 0.034) / 2);
  return distance;
}

// Function calling the different help functions. Updates the Oled screen with current servo position and distance to surrounding object
void updateOled(int v, int d) {
  display.clearDisplay();
  screenBackground();
  drawRadius(v);
  drawElement(v, d);
}


void screenBackground() { // Help function that draws all the static, non-changing patterns on the Oled screen
  display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE); //draws a rectangle as a frame
  display.drawLine(64, 64, 128, 47, WHITE);//Draws the lines indicating the 15 to 165 degree field where the servo moves
  display.drawLine(64, 64, 0, 47, WHITE);
  display.drawCircle(64, 64, 64, WHITE);//Draws a circle, of which only half is seen to indicate the maximum distance that the ultrasonic sensor can effectively and reliably detect an object
  display.display();
}

void drawRadius(int v) { //Help function that draws the radius using values taken from the servo and ultrasonic sensor
  display.drawLine(64, 64, 64 + int(cos(radians(v)) * 64), 64 - int(sin(radians(v)) * 64), WHITE);// Draws the radius
  display.display();
}

void drawElement(int v, int d) { //Help function that draws a circle at the distance of a detected object
  d = map(d, 0, 150, 0, 64); //The span of 0 to 150 cm detected by the ultrasonic sensor is 0 to 64 pixels on the Oled screen
  int x = int(cos(radians(v)) * d);
  int y = int(sin(radians(v)) * d);
  if (distance != 0) {
    display.fillCircle(64 + x, 64 - y, 4, WHITE);//Draws the small circle att the right distance along the radius
  }
  display.display();
}
