#include <ros.h>
#include <std_msgs/Float64.h>
#include <SoftwareSerial.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_I2CDevice.h>
#include "MAX30100_PulseOximeter.h"
#define pinvr A0
SoftwareSerial espSerial(5,6); // 5 = Tx, 6 = Rx
ros::NodeHandle nh;
std_msgs::Float64 Temp; 
std_msgs::Float64 sp02;
ros::Publisher chatter("sensor1",&Temp);
ros::Publisher chatter2("sensor2",&sp02);

String str1;
String str2;

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
PulseOximeter pox;
void onBeatDetected()
{
    Serial.println("B:1");
}
void setup() {
  nh.initNode();
  nh.advertise(chatter);
  nh.advertise(chatter2);
  Serial.begin(115200);
  espSerial.begin(115200);
  mlx.begin();
  pox.setOnBeatDetectedCallback(onBeatDetected);  
  delay(200);
  
}

void loop() {
   pox.update();
   float TempC =  mlx.readObjectTempC();
   float Oxy = pox.getSpO2();  
   Temp.data = TempC;
   sp02.data = Oxy;
   chatter.publish(&Temp);
   chatter2.publish(&sp02);
   str1 = TempC;
   str2 = Oxy;
   espSerial.println(str1);
   Serial.println(Oxy);
   Serial.println(TempC);
   delay(1000);                     
   nh.spinOnce();
   
}
