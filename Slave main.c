#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <TimerOne.h>

int8_t roll, pitch, yaw;
int angle_roll_buffer, angle_yaw_buffer;
const int trigL = 0;
const int echoL = 1;
const int trigR = 2;
const int echoR = 4;
const int vacuum = 7;
const int pump = 8;
const int m5f = 3;
const int m5r = 5;
const int m2pwm = 6;
const int m4pwm = 9;
const int m1pwm = 11;
const int m3pwm = 10;
const int m2dir = A0;
const int m1dir = A1;
const int m3dir = 13;
const int m4dir = 12;
long durationL, durationR;
int distance, distance, lcd_loop_counter;
LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
Serial.begin(57600); //we start the serial port at 57600 kbps and use it only for debugging
Wire.begin(8); // join i2c bus with address #8
Wire.onReceive(receiveEvent); // register event
Serial.begin(57600); // start serial for output
pinMode(7, OUTPUT); //Set Vacuum as Output
pinMode(8, OUTPUT); //Set Pump as Output
pinMode(3, OUTPUT); //Set transverse motion motor as Output
pinMode(5, OUTPUT); //Set transverse motion motor as Output
pinMode(6, OUTPUT); //Set pwm for 4 motors as Output
pinMode(9, OUTPUT);
pinMode(11, OUTPUT);
pinMode(10, OUTPUT);
pinMode(A0, OUTPUT); //Set dir for 4 motors as Output
pinMode(A1, OUTPUT);
pinMode(A3, OUTPUT);
pinMode(13, OUTPUT);
pinMode(12, OUTPUT);
pinMode(trigL, OUTPUT); //Trigger for Ultrasonic 1
pinMode(echoL, INPUT); //Echo for Ultrasonic 1
pinMode(trigR, OUTPUT); //Trigger for Ultrasonic 2
pinMode(echoR, INPUT); //Echo for Ultrasonic 2
lcd.backlight(); //Activate backlight
lcd.clear(); //Clear the LCD
lcd.setCursor(0, 0); //Set the LCD cursor to position to position 0,0
lcd.print(&quot; Welcome &quot;); //Print text to screen
lcd.setCursor(0, 1); //Set the LCD cursor to position to position 0,1
lcd.print(&quot; V1.0&quot;); //Print text to screen
delay(1500); //Delay 1.5 second to display the text
lcd.clear();
Timer1.initialize(2000000); //Set 0.5Hz refresh rate (2000000 uS period) for the output channels by Timer 1
Timer1.attachInterrupt(callibration);
}

void loop() {
// Clears the trigPin
digitalWrite(trigL, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigL, HIGH);
delayMicroseconds(10);
digitalWrite(trigL, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
durationL = pulseIn(echoL, HIGH);
// Calculating the distance
distanceL = durationL * 0.034 / 2;
// Clears the trigPin
digitalWrite(trigR, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigR, HIGH);
delayMicroseconds(10);
digitalWrite(trigR, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
durationR = pulseIn(echoR, HIGH);
// Calculating the distance
distanceR = durationR * 0.034 / 2;
forward();
if (35 >= roll <= 50) {
reverse();
delay(250);
while ( distanceL > 50 ) {
left();
}
while ( distanceR > 50 ) {
right_vacuum();
}
while ( distanceL > 50 ) {
left_clean();
}
}
write_LCD(); //Write the roll and pitch values to the LCD display
Serial.print(" Right distance = ");
Serial.print(distanceR);
Serial.print(" ");
Serial.print(" Left distance = ");
Serial.print(distanceL);
Serial.print(" ");
Serial.print(" Roll = ");
Serial.print(roll);
Serial.print(" ");
Serial.print(" Yaw = ");
Serial.print(yaw);
Serial.print(" ");
Serial.print(" pitch = ");
Serial.println(pitch);
}

void receiveEvent(int howMany) {
while(Wire.available()){
roll= Wire.read();
pitch= Wire.read();
yaw= Wire.read();
}
if (roll > 127) {
roll = 256 - roll;
roll *= -1;
}
if (pitch > 127) {
pitch = 256 - pitch;
pitch *= -1;
}
if (yaw > 127) {
yaw = 256 - yaw;
yaw *= -1;
}
}

void forward() {
digitalWrite(m2pwm, LOW);
digitalWrite(m4pwm, LOW);
digitalWrite(m1pwm, LOW);
digitalWrite(m3pwm, LOW);
delay(20);
digitalWrite(m2dir, HIGH);
digitalWrite(m1dir, LOW);
digitalWrite(m3dir, HIGH);
digitalWrite(m4dir, HIGH);
digitalWrite(m2pwm, HIGH);
digitalWrite(m4pwm, HIGH);
digitalWrite(m1pwm, HIGH);
digitalWrite(m3pwm, HIGH);
digitalWrite(3, LOW);
digitalWrite(5, LOW);
digitalWrite(7, LOW);
digitalWrite(8, LOW);
delay(50);
}

void reverse() {
digitalWrite(m2pwm, LOW);
digitalWrite(m4pwm, LOW);
digitalWrite(m1pwm, LOW);
digitalWrite(m3pwm, LOW);
delay(20);
digitalWrite(m2dir, LOW);
digitalWrite(m1dir, HIGH);
digitalWrite(m3dir, LOW);
digitalWrite(m4dir, LOW);
digitalWrite(m2pwm, HIGH);
digitalWrite(m4pwm, HIGH);
digitalWrite(m1pwm, HIGH);
digitalWrite(m3pwm, HIGH);
digitalWrite(3, LOW);
digitalWrite(5, LOW);
digitalWrite(7, LOW);
digitalWrite(8, LOW);
delay(50);
}

void left() {
digitalWrite(m2pwm, LOW);
digitalWrite(m4pwm, LOW);
digitalWrite(m1pwm, LOW);
digitalWrite(m3pwm, LOW);
digitalWrite(m2dir, LOW);
digitalWrite(m1dir, LOW);
digitalWrite(m3dir, LOW);
digitalWrite(m4dir, LOW);
digitalWrite(3, HIGH);
digitalWrite(5, LOW);
digitalWrite(7, LOW);
digitalWrite(8, LOW);
delay(50);
}

void right_vacuum() {
digitalWrite(m2pwm, LOW); //forward motion motors Power PWM
digitalWrite(m4pwm, LOW);
digitalWrite(m1pwm, LOW);
digitalWrite(m3pwm, LOW);
digitalWrite(m2dir, LOW);//forward motion motors direction
digitalWrite(m1dir, LOW);
digitalWrite(m3dir, LOW);
digitalWrite(m4dir, LOW);
digitalWrite(3, LOW);//left transverse motion motor power
digitalWrite(5, HIGH);//right transverse motion motor power
digitalWrite(7, HIGH);//turn ON vacuum
digitalWrite(8, LOW);//turn ON Pump
delay(50);
}

void left_clean() {
digitalWrite(m2pwm, LOW);
digitalWrite(m4pwm, LOW);
digitalWrite(m1pwm, LOW);
digitalWrite(m3pwm, LOW);
digitalWrite(m2dir, LOW);
digitalWrite(m1dir, LOW);
digitalWrite(m3dir, LOW);
digitalWrite(m4dir, LOW);
digitalWrite(3, HIGH);
digitalWrite(5, LOW);
digitalWrite(7, LOW);
digitalWrite(8, HIGH);
delay(50);
}

void rotate_left() {
digitalWrite(m2pwm, LOW);
digitalWrite(m4pwm, LOW);
digitalWrite(m1pwm, LOW);
digitalWrite(m3pwm, LOW);
delay(20);
digitalWrite(m2dir, LOW);
digitalWrite(m1dir, LOW);
digitalWrite(m3dir, LOW);
digitalWrite(m4dir, HIGH);
digitalWrite(m2pwm, HIGH);
digitalWrite(m4pwm, HIGH);
digitalWrite(m1pwm, HIGH);
digitalWrite(m3pwm, HIGH);
digitalWrite(3, LOW);
digitalWrite(5, LOW);
digitalWrite(7, LOW);
digitalWrite(8, LOW);
delay(50);
}

void rotate_right() {
digitalWrite(m2pwm, LOW);
digitalWrite(m4pwm, LOW);
digitalWrite(m1pwm, LOW);
digitalWrite(m3pwm, LOW);
delay(20);
digitalWrite(m2dir, HIGH);
digitalWrite(m1dir, HIGH);
digitalWrite(m3dir, HIGH);
digitalWrite(m4dir, LOW);
digitalWrite(m2pwm, HIGH);
digitalWrite(m4pwm, HIGH);
digitalWrite(m1pwm, HIGH);
digitalWrite(m3pwm, HIGH);
digitalWrite(3, LOW);
digitalWrite(5, LOW);
digitalWrite(7, LOW);
digitalWrite(8, LOW);
}

void callibration() {
while (yaw >= 10 ) {
rotate_left();
}
while (yaw < -10 ) {
rotate_right();
}
}

void write_LCD(){ //Subroutine for writing the LCD
//To get a 250Hz program loop (4us) it&#39;s only possible to write one character per loop
//Writing multiple characters is taking to much time
if(lcd_loop_counter == 14)lcd_loop_counter = 0; //Reset the counter after 14 characters
lcd_loop_counter ++; //Increase the counter
if(lcd_loop_counter == 1){
angle_roll_buffer = roll * 10; //Buffer the roll angle because it will change
lcd.setCursor(6,0); //Set the LCD cursor to position to position 0,0
}
if(lcd_loop_counter == 2){
if(angle_roll_buffer < 0)lcd.print("-"); //Print - if value is negative
else lcd.print("+"); //Print + if value is negative
}
if(lcd_loop_counter == 3)lcd.print(abs(angle_roll_buffer)/1000); //Print first number
if(lcd_loop_counter == 4)lcd.print((abs(angle_roll_buffer)/100)%10);//Print second number
if(lcd_loop_counter == 5)lcd.print((abs(angle_roll_buffer)/10)%10); //Print third number
if(lcd_loop_counter == 6)lcd.print("."); //Print decimal point
if(lcd_loop_counter == 7)lcd.print(abs(angle_roll_buffer)%10); //Print decimal number
if(lcd_loop_counter == 8){
angle_yaw_buffer = yaw * 10;
lcd.setCursor(6,1);
}
if(lcd_loop_counter == 9){
if(angle_yaw_buffer < 0)lcd.print("-"); //Print - if value is negative
else lcd.print("+"); //Print + if value is negative
}
if(lcd_loop_counter == 10)lcd.print(abs(angle_yaw_buffer)/1000); //Print first number
if(lcd_loop_counter == 11)lcd.print((abs(angle_yaw_buffer)/100)%10);//Print second number
if(lcd_loop_counter == 12)lcd.print((abs(angle_yaw_buffer)/10)%10); //Print third number
if(lcd_loop_counter == 13)lcd.print("."); //Print decimal point
if(lcd_loop_counter == 14)lcd.print(abs(angle_yaw_buffer)%10); //Print decimal number
}
