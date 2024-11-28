// BRAYDEN PATERSON
// 2023-12-21
// LIGHT CONTROLLED CAR CODE
// THIS CODE IS FOR A CAR THAT IS CONTROLLED WITH A FLASHLIGHT AND LIGHT RESISTORS

const int BR_Resistor = A1; // setting up all the pin numbers
const int BL_Resistor = A2;
const int TR_Resistor = A4;
const int TL_Resistor = A3;
const int TriggerPin = 10;
const int EchoPin = 9;
const int MotBEnable = 6;
const int MotAEnable = 3;
const int MotBIn2 = 7;
const int MotBIn1 = 5;
const int MotAIn2 = 4;
const int MotAIn1 = 2;
const int Switch1 = 8;

int CarSpeed = 150; // this controls the speed of the car

int BR_AV_Val = 22; // these values are the values that are read on average when the car is in the classroom directly under a floresent light
int BL_AV_Val = 18; // these are used as a average value so that we can see if the resistors are above or below the average
int TR_AV_Val = 19;
int TL_AV_Val = 17;

int BR_Val; // values that will be used to record the value of the light resistors
int BL_Val;
int TR_Val;
int TL_Val;

long duration, cm; // variables for the ultrasonic sensor
bool Follow; // variable for the state of the switch

long microsecondsToCentimeters(long microseconds) { // function for converting the time from the ultrasonic sensor to distance from the surface infront of it
  return microseconds / 29 / 2;
}

float WallDistance() { // function for finding the distance between the car and the surface infront of it

  digitalWrite(TriggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TriggerPin, HIGH); // sending out a soundwave
  delayMicroseconds(5);
  digitalWrite(TriggerPin, LOW);
  duration = pulseIn(EchoPin, HIGH); // finding the time it takes for the sensor to detect the sound
  cm = microsecondsToCentimeters(duration); // using the function to convert the microsecond recorded by the sensor to cm
  return duration, cm;
}

bool ModeCheck(){ // function that is used to check which state the switch is in, and what mode the car is in

  if (digitalRead(Switch1) == HIGH){ // checking the state of the switch
    Follow = false;
  }
  else{
    Follow = true;
  }
  return Follow; // returning true or false depending on the state of the switch
}

float LightValues(){ // function used to return all the values of the light resistors
  BR_Val = map(analogRead(BR_Resistor), 20, 930, 0, 100);// mapping all the values of the light resistors from 0 - 100 to make them easier to work with
  BL_Val = map(analogRead(BL_Resistor), 9, 740, 0, 100);
  TR_Val = map(analogRead(TR_Resistor), 13, 850, 0, 100);
  TL_Val = map(analogRead(TL_Resistor), 6, 760, 0, 100);
  return TL_Val, TR_Val, BR_Val, BL_Val; 
}

void LightControl(){ // function to control the movement of the car using the values from the light resistors
  if (BL_Val < BL_AV_Val and BR_Val < BR_AV_Val) { // checking to see if the flashlight is pointed at the back resistors
    if(Follow == true){ // checking what mode the car is in
      MoveBackward();
    }
    else{
      if (cm <= 20){ // ensuring that if we are about to move forward we will not go into a wall
        DontMove();
      }
      else {
      MoveForward();
      }
    }
  }
  else if (TL_Val < TL_AV_Val and TR_Val < TR_AV_Val){ // checking to see if the light is pointed at the front resistors
    if(Follow == true){ // checking which mode the car is in
      if (cm <= 20){ // making sure the car doesnt crash into a wall
        DontMove();
      }
      else { // allowing the car to move forward if its not close to the wall and its on follow mode
      MoveForward();
      }
    }
    else{ // allowing the car to move backwards if its not close to a wall and if its not on Follow mode
      MoveBackward();
    }    
  }
  else if (TL_Val < TL_AV_Val and BL_Val < BL_AV_Val){ // checking if the light is pointed at the left resistors
    if(Follow == true){ // checking the mode
      TurnLeft();
    }
    else{
      TurnRight();
    }        
  }
  else if (TR_Val < TR_AV_Val and BR_Val < BR_AV_Val){ // checking if the light is pointed at the right resistors
    if(Follow == true){ // checking what mode the car is in
      TurnRight();
    }
    else{
      TurnLeft();
    }       
  }
  else { // if the light isnt pointed anywhere, dont move
    DontMove();
  }
}
void SerialDisplay(){ // function to control the serial display

  Serial.print("BR: ");
  Serial.print(BR_Val);
  Serial.print(" - BL:");
  Serial.print(BL_Val);
  Serial.print(" - TR:");
  Serial.print(TR_Val);
  Serial.print(" - TL:");
  Serial.print(TL_Val);
  Serial.print(" Distance (cm) - ");
  Serial.println(cm);
}

void EnableMotars() { // turning on the motars
  analogWrite(MotAEnable, CarSpeed); // using analog write to dictate the speed that the motars turn when used
  analogWrite(MotBEnable, CarSpeed);
}

void DontMove() { // function to stop the movement of the car
  digitalWrite(MotBIn1, LOW); // turning all motar inputs to low so the car stops
  digitalWrite(MotAIn1, LOW);
  digitalWrite(MotBIn2, LOW);
  digitalWrite(MotAIn2, LOW);
}

void MoveForward() { // function to move the car forward
  digitalWrite(MotBIn1, HIGH); // turning both primary inputs to high to make the wheels turn forwards
  digitalWrite(MotAIn1, HIGH);
  digitalWrite(MotBIn2, LOW);
  digitalWrite(MotAIn2, LOW);
}

void MoveBackward() { // function to move the car backwards
  digitalWrite(MotBIn1, LOW);
  digitalWrite(MotAIn1, LOW);
  digitalWrite(MotBIn2, HIGH); // turning both secondary inputs to high to make both wheels turn backwards
  digitalWrite(MotAIn2, HIGH);
}

void TurnLeft() { // function to turn the car left
  digitalWrite(MotBIn1, HIGH); // turning the primary input of motar B high to turn this wheel forwards
  digitalWrite(MotAIn1, LOW);
  digitalWrite(MotBIn2, LOW);
  digitalWrite(MotAIn2, HIGH); // turning the secondary input of motar A to high to turn this wheel backwards
}

void TurnRight() { // function to turn the car right
  digitalWrite(MotBIn1, LOW);
  digitalWrite(MotAIn1, HIGH); // turning the primary input of motar A to high to make this wheel turn forwards
  digitalWrite(MotBIn2, HIGH); // turning the secondary input of motar B to low to make this wheel turn backwards
  digitalWrite(MotAIn2, LOW);
}

void setup() { // setting up all the pinmodes and starting the serial monitor

  pinMode(BR_Resistor, INPUT); // setting all light resistors to be inputs
  pinMode(BL_Resistor, INPUT);
  pinMode(TR_Resistor, INPUT);
  pinMode(TL_Resistor, INPUT);

  pinMode(TriggerPin, OUTPUT); // setting my send and recivie pins on the ultrasonic sensor
  pinMode(EchoPin, INPUT);

  pinMode(MotAEnable, OUTPUT); // setting all the motar pins to be outputs
  pinMode(MotBEnable, OUTPUT);
  pinMode(MotBIn2, OUTPUT);
  pinMode(MotBIn1, OUTPUT);
  pinMode(MotAIn2, OUTPUT);
  pinMode(MotAIn1, OUTPUT);

  Serial.begin(9600); // starting the serial monitor
}

void loop() { // main loop

  EnableMotars(); //calling all the functions used for the car
  ModeCheck();
  LightValues();
  WallDistance();
  LightControl();
  SerialDisplay();

}