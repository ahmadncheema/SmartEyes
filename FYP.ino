//SMART EYE - FINAL YEAR PROJECT
//BY: 
//AHMAD NAEEM CHEEMA BS(SE)-23017
//IQRA UNIVERSITY ISLAMABAD


//UNIQUE INDENTIFIERS FOR THE ANDROID SIDE
// @ = Ultrasonic Sensor 1 "Object Detection"
// # = Ultrasonic Sensor 2 "Ditch Detection"
// $ = MPU 6050 "Accident Detection"
// % = Emergency SMS Button


#include<SoftwareSerial.h>
#include<Wire.h>
#include <stdlib.h>
//#include <WProgram.h>
#include <LiquidCrystal.h>

SoftwareSerial BlueTooth(12,11);
//PINS USED ON THE ARDUINO BOARD
const int echoPin1 = 2;
const int trigPin1 = 3;
const int echoPin2 = 4;
const int trigPin2 = 5;
const int led1 = 6;    //RED LED SENSOR1
const int button = 8;   //MESSAGE BUTTON
const int speaker = 7; //SPEAKER


//VARIABLES BEING USED
long duration;
int distance=0;
int duration2;
int distance2=0;
int valuechanger;
int setdistance=0;
const int MPU_addr=0x68;
int minVal=265; int maxVal=402;
double x; double y; double z;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int gyro = 0;
int alert=0;


void setup() {
pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
pinMode(trigPin2, OUTPUT);
pinMode(echoPin2, INPUT);
pinMode(led1, OUTPUT);
pinMode(speaker, OUTPUT);
pinMode(button, INPUT_PULLUP);
digitalWrite(button, HIGH);
Wire.begin(); Wire.beginTransmission(MPU_addr); 
Wire.write(0x6B); Wire.write(0); 
Wire.endTransmission(true); 
Serial.begin(9600); // Starts the serial communication
}


//MAIN LOOP
void loop() {
//POTENTIOMETER CODE
valuechanger = analogRead(A0);  
//Serial.println(valuechanger);
delay(1);
if(valuechanger <20)
{
  setdistance = 10;
}
else if(valuechanger<200 && valuechanger>100)
{
  setdistance = 20;
}
else if(valuechanger<600 && valuechanger>300)
{
  setdistance = 30;
}
else
{
  setdistance = 40;
}

//MPU6050 "Accident Detection" CODE
Wire.beginTransmission(MPU_addr); 
Wire.write(0x3B);
Wire.endTransmission(false);
Wire.requestFrom(MPU_addr,14,true);
AcX=Wire.read()<<8|Wire.read();
AcY=Wire.read()<<8|Wire.read();
AcZ=Wire.read()<<8|Wire.read(); 
int xAng = map(AcX,minVal,maxVal,-90,90);
int yAng = map(AcY,minVal,maxVal,-90,90);
int zAng = map(AcZ,minVal,maxVal,-90,90);
x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);

//ULTRASONIC SENSOR 1 "Object Collision" CODE

// Clears the trigPin
digitalWrite(trigPin1, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin1, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin1, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin1, HIGH);
// Calculating the distance
distance = duration*0.034/2;

//ULTRASONIC SENSOR 2 "Ditch Detection" CODE

//Clears the trigPin
digitalWrite(trigPin2,LOW);
delayMicroseconds(2);
//Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin2, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin2,LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration2 = pulseIn(echoPin2, HIGH);
// Calculating the distance2
distance2 = duration2*0.034/2;

//PRINTING AND SENDING THE RESULTS TO THE SMART EYE ANDROID APP



if(distance < setdistance)
{
//  Serial.println();
//  Serial.print("@");
//  Serial.print(distance);
//  Serial.print("@");
//  Serial.println();

sendValuesToAndroid();
}
else 
{
//  Serial.println();
//  Serial.print("@");
//  Serial.print("0");
//  Serial.print("@");
//  Serial.println();
  //DO NOTHING IF DISTANCE IS GREATER
}

if(distance2 < 10)
{
//  Serial.println();
//  Serial.print("#");
//  Serial.print(distance2);
//  Serial.print("#");
//  Serial.println();
sendValuesToAndroid();
}
else 
{
//  Serial.println();
//  Serial.print("#");
//  Serial.print("0");
//  Serial.print("#");
//  Serial.println();
  //DO NOTHING IF DISTANCE IS GREATER
}

//BUTTON CODE
int sensorVal = digitalRead(button);
if(sensorVal == HIGH){
  //DO NOTHING
//  Serial.println();
//  Serial.print("%");
//  Serial.print("0");
//  Serial.print("%");
//  Serial.println();
}
  else
  {
    alert=88;
    sendValuesToAndroid();
    alert=0;
   // Serial.println("%BUTTON IS ON%");
  }


//MPU6050 "Accident Detection" CODE

if(x>=-45 || y>=-90)
{
  gyro = gyro + 1;
  if (gyro == 3)
  {
  
    sendValuesToAndroid();
    //  Serial.println("$ACCIDENT HAPPENED$");
    
  }
  else
  {
//    Serial.println();
//  Serial.print("$");
//  Serial.print("0");
//  Serial.print("$");
//  Serial.println();
  }
 
 //STICK LOCATOR CODE
  if (Serial.available()) {
    delay(100);
    while (Serial.available() > 0) {

      Serial.write(Serial.read());

		tone(speaker,300);
		delay(1000);
		noTone(speaker);
      
    }
  }
}


  delay(2000);
}

void sendValuesToAndroid()
{
  Serial.print("$");
  Serial.print(",");
  Serial.print(distance);
  Serial.print(",");
  Serial.print(distance2);
  Serial.print(",");
  Serial.print(gyro);
  Serial.print(",");
  Serial.print(alert);
  Serial.print(",");
  Serial.print(setdistance);
  Serial.print("#");
  }