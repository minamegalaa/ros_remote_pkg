#include <ros.h>
#include <std_msgs/Int32.h>
#include "CytronMotorDriver.h"

int VacuumVelocity=0;
// Configure the motor driver.
CytronMD vacuummotor(PWM_DIR, 3, 4);  // PWM = Pin 3, DIR = Pin 4.
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

  //Setup ROS Stuff
 nh.getHardware()->setBaud(9600);
 nh.initNode();
 //Subscribers

 
 nh.subscribe(VacuumMotorROSSubsciber);
}


// The loop routine runs over and over again forever.
void loop() {
  nh.spinOnce();
   vacuummotor.setSpeed(VacuumVelocity);
   delay(5);
}
