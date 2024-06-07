#include <ros.h>
#include <std_msgs/Int32.h>
#include "CytronMotorDriver.h"

int VacuumVelocity=0;
// Configure the motor driver.
CytronMD vacuummotor(PWM_DIR, 3, 4);  // PWM = Pin 3, DIR = Pin 4.

// Main Brush Motor
 
int enA = 9;
int in1 = 5;
int in2 = 6;
 
// Side Brush Motors
 
int enB = 10;
int in3 = 7;
int in4 = 8;

ros::NodeHandle nh;

void callBackFunctionMotorVacuum(const std_msgs::Int32 &VacuumVelocityROS){
  VacuumVelocity=VacuumVelocityROS.data;
}

//subscribers for vacuum motor
//vacuum motor
ros::Subscriber<std_msgs::Int32>VacuumMotorROSSubsciber("vacuum_motor_velocity",&callBackFunctionMotorVacuum);



// The setup routine runs ounce when you press reset.
void setup() {
  vacuummotor.setSpeed(VacuumVelocity);
   // Set all the motor control pins to outputs
 
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  //Setup ROS Stuff
 nh.getHardware()->setBaud(9600);
 nh.initNode();
 //Subscribers

 
 nh.subscribe(VacuumMotorROSSubsciber);
}


// The loop routine runs over and over again forever.
void loop() {
  // Turn on motor A
 
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
 
  // Set speed to 200 out of possible range 0~255
 
  analogWrite(enA, VacuumVelocity);
 
  // Turn on motor B
 
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
 
  // Set speed to 200 out of possible range 0~255
 
  analogWrite(enB, VacuumVelocity);
  nh.spinOnce();
   vacuummotor.setSpeed(VacuumVelocity);
   delay(5);
}
