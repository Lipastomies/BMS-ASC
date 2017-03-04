#include "HID-Project.h"
void doEncoderA();
void doEncoderB();

// button layout
// Button     -- Pin # --  Keyboard -- led
// Select       A0          Space        2
// Start        A1          Enter        3
// BT1          A2          a            4
// BT2          A3          s            5
// BT3          A4          d            6
// BT4          A5          f            7
// BT5          11          g            8
// BT6          12          h            9
// BT7          13          j            10
//scratch left  --          u            --
//scratch right --          i            --

// ENCODERS   DATA 1  DATA 2
// Encoder     0      1
//

#define DELAY 2  // Time interval between loops in milliseconds.

 enum PinAssignments
 {
  encoderPinA = 0,
  encoderPinB = 1
 };
//This is up to encoder pin wiring

//timing variables
unsigned long previousTime = 0, currentTime = 0;

//pins for keys
int keypins[] = {A0,A1,A2,A3,A4,A5,11,12,13};
//pins for key leds
int ledpins[] = {2,3,4,5,6,7,8,9,10};
//keys that pressing buttons will send to computer
char keyvalues[] = {' ','\n','a','s','d','f','g','h','j'};
//keys that scratching will send to computer
char scratchvalues[] = {'u','i'};

volatile int encoderPos = 0;
static boolean rotating = false;

bool bms_mode = true;

boolean A_set = false;              
boolean B_set = false;
 
//scratch parameters, change these to change scratch functionality
const int scratch_sense = 8;//how many steps the encoder has to move for scratch to register
const int  scratchdelay = 10;//how long the scratch is on after activation. Important for continuous rotation of scratch.
const int stopdelay = 5;//how long the encoder has to change its position by scratch sense, otherwise scratch will not register.
//scratch variables
int tempposition = 0;
int sc_left,sc_right=0;
int temp_direction=0;
int delaycounter=0;
int stopcounter=0;

void setup() {
    //find out encoder pin interrupt numbers
    int encoder0 = digitalPinToInterrupt(encoderPinA);
    int encoder1 = digitalPinToInterrupt(encoderPinB);

    for (int i= 0;i<9;i++)
    pinMode(keypins[i], INPUT_PULLUP);

    NKROKeyboard.begin();
    for (int i=0;i<9;i++)
    pinMode(ledpins[i], OUTPUT);
    pinMode(encoderPinA, INPUT_PULLUP);
    pinMode(encoderPinB, INPUT_PULLUP);

    attachInterrupt(encoder0, doEncoderA, CHANGE);
    attachInterrupt(encoder1, doEncoderB, CHANGE);
    //serial for debugging e.g. the scratch
    //Serial.begin(9600);
    
    //get times for first time
    previousTime = millis();
    currentTime = previousTime;
    Mouse.begin();
}

void loop() {
    currentTime = millis();
    if ((long)(currentTime-(unsigned long)(previousTime+DELAY))>=0){
        previousTime = currentTime;
        //check for TT type change
        //bms
        if (digitalRead(keypins[0])==LOW && digitalRead(keypins[1])==LOW&&digitalRead(keypins[3])==LOW && bms_mode==false){
            digitalWrite(ledpins[0],HIGH);
            digitalWrite(ledpins[1],HIGH);
            digitalWrite(ledpins[3],HIGH);
            for (int i = 0;i<4;i++){
                if (digitalRead(keypins[0])==LOW&&digitalRead(keypins[1])==LOW&&digitalRead(keypins[3])==LOW){
                    digitalWrite(ledpins[i*2+2],HIGH);
                    if(i==3)
                        bms_mode=true;
                }
                else break;
                delay(250);   
            }
            for (int i=0;i<9;i++)
              digitalWrite(ledpins[i],LOW);
        }
        if (digitalRead(keypins[0])==LOW && digitalRead(keypins[1])== LOW && digitalRead(keypins[7])==LOW && bms_mode==true){
            digitalWrite(ledpins[0],HIGH);
            digitalWrite(ledpins[1],HIGH);
            digitalWrite(ledpins[7],HIGH);
            for (int i = 0;i<4;i++){
                if (digitalRead(keypins[0])==LOW&&digitalRead(keypins[1])==LOW&&digitalRead(keypins[7])==LOW){
                    digitalWrite(ledpins[i*2+2],HIGH);
                    if(i==3)
                        bms_mode=false;
                }
                else break;
                delay(250);   
            }
          for (int i=0;i<9;i++)
              digitalWrite(ledpins[i],LOW);
        }
        //read the keys and communicate accordingly, as well as set the leds
        for (int i=0;i<9;i++){
            if(digitalRead(keypins[i])==LOW){
                NKROKeyboard.press(keyvalues[i]);
                digitalWrite(ledpins[i],HIGH);
            }
            if(digitalRead(keypins[i])==HIGH){
                NKROKeyboard.release(keyvalues[i]);
                digitalWrite(ledpins[i],LOW);
            }
        }
        //Encoder Reset
        rotating = true;
        if (encoderPos != 0)
        {
            //if bms-mode
            if(bms_mode)
              tempposition += encoderPos;
            //else if 'analog' mode
            if (!bms_mode)
                Mouse.move(encoderPos,0,0);
            encoderPos = 0;
        }
        //if bms-mode on
        if (bms_mode){
            //scratch registration
            delaycounter++;
            if (tempposition > scratch_sense){
                temp_direction=1;
                tempposition=0;  
                delaycounter=0;
            }
            else if (tempposition < -scratch_sense){
                temp_direction=-1;
                tempposition=0;
                delaycounter=0;
            }
            else temp_direction=0;
    
            //scratch debouncing
            if (stopcounter > stopdelay ){
                tempposition=0;
                temp_direction=0;
                stopcounter=0;
            }
            if (temp_direction == 0)
                stopcounter++;
    
            //scratch communication to the computer
            //scratching from zero to right
            if (sc_right == 0 && temp_direction == 1){
                NKROKeyboard.release(scratchvalues[0]);
                NKROKeyboard.press(scratchvalues[1]);
                sc_right =1;
            }
            //scratching from zero to left
            else if (sc_left==0 && temp_direction == -1){
                NKROKeyboard.release(scratchvalues[1]);
                NKROKeyboard.press(scratchvalues[0]);
                sc_left =1;
            }
            //scratching from left to right
            else if (sc_left==1 && temp_direction == 1){
                NKROKeyboard.release(scratchvalues[0]);
                NKROKeyboard.press(scratchvalues[1]);
                sc_left=0;
                sc_right=1;
            }
            //scratching from right to left
            else if (sc_right==1 && temp_direction == -1){
                NKROKeyboard.release(scratchvalues[1]);
                NKROKeyboard.press(scratchvalues[0]);
                sc_left=1;
                sc_right=0;
            }
            //scratch deactivation
    
            //scratch weas moving to left, but has stopped/is stopping
            else if (sc_left==1 && temp_direction==0 && delaycounter > scratchdelay){
                NKROKeyboard.release(scratchvalues[0]);
                sc_left=0;
                delaycounter= 0;
            }
            //the same, but for right
            else if (sc_right==1 && temp_direction==0 && delaycounter > scratchdelay){
                NKROKeyboard.release(scratchvalues[1]);
                sc_right=0;
                delaycounter=0;
            }
        }
        //report of scratch for debugging. Good for adjusting scratch parameters.
        /* 
        Serial.print(sc_left);
        Serial.print(' ');
        Serial.print(sc_right);
        Serial.print(' ');
        Serial.print(temp_direction);
        Serial.print(' ');
        Serial.print(tempposition);
        Serial.print('\n');  
        */
        
    }
}

void doEncoderA()
{
  if( digitalRead(encoderPinA) != A_set )
  {  
    A_set = !A_set;
    if ( A_set && !B_set )
      encoderPos += 1;
    rotating = false;  
  }
}
 
void doEncoderB()
{
 
  if( digitalRead(encoderPinB) != B_set ) {
    B_set = !B_set;
    if( B_set && !A_set )
      encoderPos -= 1;
    rotating = false;
  }
}
