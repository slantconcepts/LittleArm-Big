// LittleArm 2C arduino code
// Allows serial control of the LittleArm Big 3D printed robot arm so that it can commmunicate with the Android or Windows App
// Designed for Arduino Nano with Meped Board
// Created by Slant Concepts

#include <Servo.h>  //arduino library
#include <math.h>   //standard c library


Servo baseServo;  
Servo shoulderServo;  
Servo elbowServo; 
Servo wristRotServo;
Servo wristFlexServo;
Servo gripperServo;

struct jointAngle{
  int base;
  int shoulder;
  int elbow;
  int wRot;
  int wFlex;
};

struct jointAngle desiredAngle; //desired angles of the servos

int desiredGrip;
int gripperPos;
int command;
int desiredDelay;

int servoSpeed = 15;
int ready = 0;

//+++++++++++++++FUNCTION DECLARATIONS+++++++++++++++++++++++++++
int servoParallelControl (int thePos, Servo theServo );
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void setup()
{ 
  Serial.begin(9600);
  baseServo.attach(2);        // attaches the servo on pin 5 to the servo object 
  shoulderServo.attach(3);
  elbowServo.attach(4);
  wristRotServo.attach(5);
  wristFlexServo.attach(6);

  gripperServo.attach(7);
  
  Serial.setTimeout(50);      //ensures the the arduino does not read serial for too long
  Serial.println("started");
  //baseServo.write(90);        //intial positions of servos
  //shoulderServo.write(100);
  //elbowServo.write(110);
  ready = 0;
} 

//primary arduino loop
void loop() 
{ 
  if (Serial.available()){
    ready = 1;
    desiredAngle.base = Serial.parseInt();
    desiredAngle.shoulder = Serial.parseInt();
    desiredAngle.elbow = Serial.parseInt();
    desiredAngle.wRot = Serial.parseInt();
    desiredAngle.wFlex = Serial.parseInt();
    desiredGrip = Serial.parseInt();
    desiredDelay = Serial.parseInt();

    if(Serial.read() == '\n'){              // if the last byte is 'd' then stop reading and execute command 'd' stands for 'done'
        Serial.flush();                     //clear all other commands piled in the buffer
        Serial.print('d');                  //send completion of the command
    }
  }
  
  int status1 = 0;
  int status2 = 0;
  int status3 = 0;
  int status4 = 0;
  int status5 = 0;
  int status6 = 0;
  int done = 0 ; 
  
  while(done == 0 && ready == 1){  
    //move the servo to the desired position
    status1 = servoParallelControl(desiredAngle.base, baseServo, desiredDelay);
    status2 = servoParallelControl(desiredAngle.shoulder,  shoulderServo, desiredDelay);
    status3 = servoParallelControl(desiredAngle.elbow, elbowServo, desiredDelay);
    status4 = servoParallelControl(desiredAngle.wRot, wristRotServo, desiredDelay); 
    status5 = servoParallelControl(desiredAngle.wFlex, wristFlexServo, desiredDelay);       
    status6 = servoParallelControl(desiredGrip, gripperServo, desiredDelay);  
    
    if (status1 == 1 & status2 == 1 & status3 == 1 & status4 == 1 & status5 ==1 & status6 == 1){
      done = 1;
    }      
  }// end of while
}

//++++++++++++++++++++++++++++++FUNCTION DEFINITIONS++++++++++++++++++++++++++++++++++++++++++

int servoParallelControl (int thePos, Servo theServo, int theSpeed ){
  
    int startPos = theServo.read();        //read the current pos
    int newPos = startPos;
    //int theSpeed = speed;
    
    //define where the pos is with respect to the command
    // if the current position is less that the actual move up
    if (startPos < (thePos-3)){
       newPos = newPos + 1;
       theServo.write(newPos);
       delay(theSpeed);
       return 0;
           
    }
  
   else if (newPos > (thePos + 3)){
  
      newPos = newPos - 1;
      theServo.write(newPos);
      delay(theSpeed);
      return 0;
          
    }  
    
    else {
        return 1;
    }  
   
} //end servo parallel control
