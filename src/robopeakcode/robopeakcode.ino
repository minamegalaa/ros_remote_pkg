#include <ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>
#include "CytronMotorDriver.h"
#include <Servo.h>
#include <Wire.h>
//pwm channels D3,D5,D6,D9,D10
int VacuumVelocity=0;
// Configure the motor driver.
CytronMD vacuummotor(PWM_DIR, 9, 4);  // PWM = Pin 3, DIR = Pin 4.
// Main Brush Motor
int enA = 9;
int in1 = 5;
int in2 = 6;
// Side Brush Motors
int enB = 10;
int in3 = 7;
int in4 = 8; 
// Configure the motor driver.
CytronMD leftmotor(PWM_DIR, 5, 4);  
CytronMD rightmotor(PWM_DIR, 6, 7);  
//encoder interrupt pins
int encoderPinLeft=2;  
int encoderPinRight=3;  
int ENA=8;
int ENB=9;
//left encoder pulses
volatile unsigned long totalPulsesLeft = 0;
//right encoder pulses
volatile unsigned long totalPulsesRight = 0;
//left servo angle
volatile unsigned long AngleLeft = 0;
//right servo angle
volatile unsigned long AngleRight = 0;
// motor velocities - these variables are set by ROS
// left motor
int motorVelocityLeft=0;
// right motor
int motorVelocityRight=0;
// configure the servo
Servo leftservo;
Servo rightservo;
int posleft=0;
int posright=0;

ros::NodeHandle nh;

void callBackFunctionMotorVacuum(const std_msgs::Int32 &VacuumVelocityROS){
  VacuumVelocity=VacuumVelocityROS.data;
}

void callBackFunctionServoLeft(const std_msgs::Int32 &servoAngleLeftROS){
  posleft=servoAngleLeftROS.data;
}
void callBackFunctionServoRight(const std_msgs::Int32 &servoAngleRightROS){
  posright=servoAngleRightROS.data;
}

void callBackFunctionMotorLeft(const std_msgs::Int32 &motorVelocityLeftROS){
  motorVelocityLeft=motorVelocityLeftROS.data;
}
void callBackFunctionMotorRight(const std_msgs::Int32 &motorVelocityRightROS){
  motorVelocityRight=motorVelocityRightROS.data;
}


//publishers for the encoder pulses
//left encoder publisher
std_msgs::Int32 leftEncoderROS;
ros::Publisher leftEncoderROSPublisher("left_encoder_pulses", &leftEncoderROS);
//right encoder publisher
std_msgs::Int32 rightEncoderROS;
ros::Publisher rightEncoderROSPublisher("right_encoder_pulses", &rightEncoderROS);

//left angle publisher
std_msgs::String leftAngleROS;
ros::Publisher leftAngleROSPublisher("left_angle", &leftAngleROS);
//right angle publisher
std_msgs::String rightAngleROS;
ros::Publisher rightAngleROSPublisher("right_angle", &rightAngleROS);


//subscribers for vacuum motor
//vacuum motor
ros::Subscriber<std_msgs::Int32>VacuumMotorROSSubsciber("vacuum_motor_velocity",&callBackFunctionMotorVacuum);

//subscribers for left and right motor velocities
//left motor
ros::Subscriber<std_msgs::Int32>leftMotorROSSubsciber("left_motor_velocity",&callBackFunctionMotorLeft);
//right motor
ros::Subscriber<std_msgs::Int32>rightMotorROSSubsciber("right_motor_velocity",&callBackFunctionMotorRight);

//left servo angle
ros::Subscriber<std_msgs::Int32>leftServoROSSubsciber("left_servo_angle",&callBackFunctionServoLeft);
//right servo angle
ros::Subscriber<std_msgs::Int32>rightServoROSSubsciber("right_servo_angle",&callBackFunctionServoRight);


// The setup routine runs once when you press reset.
void setup() {
  vacuummotor.setSpeed(VacuumVelocity);
   // Set all the motor control pins to outputs
 
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  //set the encoder pins
  pinMode(encoderPinLeft, INPUT);
  pinMode(encoderPinRight, INPUT);
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  //servo attach pins
  leftservo.attach(10);
  rightservo.attach(11);
  //attach the interrupts for tracking the pulses
  attachInterrupt(digitalPinToInterrupt(encoderPinLeft), interruptFunctionLeft, RISING);
  attachInterrupt(digitalPinToInterrupt(encoderPinRight), interruptFunctionRight, RISING);
  //left motor
 leftmotor.setSpeed(0);     
 //right motor
 rightmotor.setSpeed(0);
 //left servo
 leftservo.write(0);
 //right servo
 rightservo.write(0);

//Setup ROS Stuff
 nh.getHardware()->setBaud(9600);
 nh.initNode();
 //Publishers
 nh.advertise(leftEncoderROSPublisher);
 nh.advertise(rightEncoderROSPublisher);
 nh.advertise(leftAngleROSPublisher);
 nh.advertise(rightAngleROSPublisher);


 //Subscribers
 nh.subscribe(leftMotorROSSubsciber);
 nh.subscribe(rightMotorROSSubsciber);
 nh.subscribe(leftServoROSSubsciber);
 nh.subscribe(rightServoROSSubsciber);
 nh.subscribe(VacuumMotorROSSubsciber);
  
}


// The loop routine runs over and over again forever.
void loop() {
nh.spinOnce();
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

  vacuummotor.setSpeed(VacuumVelocity);

//analogWrite(ENA, motorVelocityLeft);
//Set the direction and turn ON
//left motor
leftmotor.setSpeed(motorVelocityLeft);
//right motor
rightmotor.setSpeed(motorVelocityRight);
//left servo
leftservo.write(posleft);
//right servo
rightservo.write(posright);
//send back the encoder readings
leftEncoderROS.data = totalPulsesLeft;
leftEncoderROSPublisher.publish(&leftEncoderROS);

rightEncoderROS.data = totalPulsesRight;
rightEncoderROSPublisher.publish(&rightEncoderROS);

//send back the servo readings
leftAngleROS.data = AngleLeft;
leftAngleROSPublisher.publish(&leftAngleROS);

rightAngleROS.data = AngleRight;
rightAngleROSPublisher.publish(&rightAngleROS);
ServoLeft();

}

//interrupt function left encoder
void interruptFunctionLeft(){
        if(motorVelocityLeft < 0){
          totalPulsesLeft--;
        }
        if(motorVelocityLeft > 0){
          totalPulsesLeft++;
        }
}
void interruptFunctionRight(){
        if(motorVelocityRight < 0){
          totalPulsesRight--;
        }
        if(motorVelocityRight > 0){
          totalPulsesRight++;
        }
}
void ServoLeft(){
        if(motorVelocityRight == 0) {
          AngleRight = "SS";
        }
        else if(posright == 45){
          if(motorVelocityRight > 0){
            AngleRight = "BR";
          }
          if(motorVelocityRight < 0){
            AngleRight = "FL";
          }
        }
        else if(posright == 135){
          if(motorVelocityRight > 0){
            AngleRight = "FR";
          }
          if(motorVelocityRight < 0){
            AngleRight = "BL";
          }
        }
        else if(posright == 90){
          if(motorVelocityRight > 0){
            AngleRight = "RR";
          }
          if(motorVelocityRight < 0){
            AngleRight = "LL";
          }
        }
        else {
          if(motorVelocityRight > 0){
            AngleRight = "FF";
          }
          if(motorVelocityRight < 0){
            AngleRight = "BB";
          }
        }    
}
void ServoRight(){
        if(posright < 0){
          AngleRight--;
        }
        if(posright> 0){
          AngleRight++;
        }
}