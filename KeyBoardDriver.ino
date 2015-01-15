

#include <Wire.h>


const int PWM_Red = 22;
const int PWM_Green = 23;
const int PWM_Blue =  21;

const int Board_Led = 13;
const int Led_1 = 15;
const int Led_2 = 14;
const int Led_3 = 12;
const int Led_4 = 11;
const int Led_5 = 10;
const int Led_6 = 9;
const int Led_7 = 8;
const int Led_8 = 7;

int Button_Colors [8][3] = {
  {
    255,255,255
  }
  ,{
    255,255,255
  }
  ,{
    255,255,255
  }
  ,{
    255,255,255
  }
  ,{
    255,255,255
  }
  ,{
    255,255,255
  }
  ,{
    255,255,255
  }
  ,{
    255,255,255
  }
};

int Current_Lit_Led = 0;
int Board_Flash_Counter = 0;
boolean Board_Led_State = false;

void setup() {
  Wire.begin(9);                // Start I2C Bus as a Slave (Device Number 9)
  Wire.onReceive(receiveEvent); // register event

    pinMode(PWM_Red, OUTPUT);
  pinMode(PWM_Green, OUTPUT);
  pinMode(PWM_Blue, OUTPUT);
  analogWrite(PWM_Red, 0);
  analogWriteFrequency(PWM_Red, 187500);
  analogWrite(PWM_Green, 0);
  analogWriteFrequency(PWM_Green, 187500);
  analogWrite(PWM_Blue, 0);
  analogWriteFrequency(PWM_Blue, 187500);
  
  analogWriteResolution(8);

  pinMode(Board_Led, OUTPUT);
  pinMode(Led_1, OUTPUT);
  pinMode(Led_2, OUTPUT);
  pinMode(Led_3, OUTPUT);
  pinMode(Led_4, OUTPUT);
  pinMode(Led_5, OUTPUT);
  pinMode(Led_6, OUTPUT);
  pinMode(Led_7, OUTPUT);
  pinMode(Led_8, OUTPUT);

  digitalWrite(Board_Led, LOW);
  digitalWrite(Led_1, LOW);
  digitalWrite(Led_2, LOW);
  digitalWrite(Led_3, LOW);
  digitalWrite(Led_4, LOW);
  digitalWrite(Led_5, LOW);
  digitalWrite(Led_6, LOW);
  digitalWrite(Led_7, LOW);
  digitalWrite(Led_8, LOW);

  Serial.begin(9600); // USB is always 12 Mbit/sec

}

void loop() {

  if(Board_Flash_Counter < 100000){
    Board_Flash_Counter++;
  }
  else{
    if(Board_Led_State){
      digitalWrite(Board_Led, LOW);
      Board_Led_State = false;
    }
    else{
      digitalWrite(Board_Led, HIGH);
      Board_Led_State = true;
    }
    Board_Flash_Counter = 0;
  }

  int ledPin = getLedPin(Current_Lit_Led);  
  digitalWrite(ledPin, LOW);

  if(Current_Lit_Led + 1 <= 8){
    Current_Lit_Led++;
    switchLed(Current_Lit_Led);
  }
  else{
    Current_Lit_Led = 1;
    switchLed(Current_Lit_Led);
  }
  delayMicroseconds(50);
}

void receiveEvent(int howMany) {
  int code = Wire.receive();

  if(code < 9){
    int red = Wire.receive();  // receive byte as an integer
    int green = Wire.receive();
    int blue = Wire.receive();
    int ledAssignment = ledAssignmentMap(code);
    

    Button_Colors[ledAssignment][0] = red;
    Button_Colors[ledAssignment][1] = green;
    Button_Colors[ledAssignment][2] = blue;
  }
  else{
    while(Wire.available()){
      int led = Wire.receive();

      int red = Wire.receive();  // receive byte as an integer
      int blue = Wire.receive();
      int green = Wire.receive();
      
      int ledAssignment = ledAssignmentMap(led);
      Button_Colors[ledAssignment][0] = red;
      Button_Colors[ledAssignment][1] = green;
      Button_Colors[ledAssignment][2] = blue;
    }
  }
}

void switchLed(int led){
  int ledPin = getLedPin(Current_Lit_Led);
  int red = Button_Colors [Current_Lit_Led - 1][0];
  int green = Button_Colors [Current_Lit_Led - 1][1];
  int blue = Button_Colors [Current_Lit_Led - 1][2];
  analogWrite(PWM_Red, red);
  analogWrite(PWM_Green, blue * .7);
  analogWrite(PWM_Blue, green * .4);
  digitalWrite(ledPin, HIGH);
}

int getLedPin(int led){
  switch ( led )
  {
  case 1:
    return Led_1;
    break;
  case 2:
    return Led_2;
    break;
  case 3:
    return Led_3;
    break;
  case 4:
    return Led_4;
    break;
  case 5:
    return Led_5;
    break;
  case 6:
    return Led_6;
    break;
  case 7:
    return Led_7;
    break;
  case 8:
    return Led_8;
    break;
  default:
    return Led_1;
  }
}

int ledAssignmentMap(int led){
  switch ( led )
  {
  case 1:
    return 7;
    break;
  case 2:
    return 0;
    break;
  case 3:
    return 6;
    break;
  case 4:
    return 5;
    break;
  case 5:
    return 1;
    break;
  case 6:
    return 2;
    break;
  case 7:
    return 4;
    break;
  case 8:
    return 3;
    break;
  default:
    return 0;
  }
}







