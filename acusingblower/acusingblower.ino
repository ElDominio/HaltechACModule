//AlphaX AC Arduino BK2


bool AcOK = false;//saves if haltech is allowing AC
bool AcRequest;
int blowerSpeed; //saves blower speed
int lastBlowerSpeed = 0;//saves previous last blow speed
byte CurrentDriveSpeed = 0; //currentdrivespeed
byte RequestedDriveSpeed = 0; // requested drive speed
unsigned int ACEvapTemp = 0;
unsigned int prevACEvapTemp = 0;
bool ACError = false;
unsigned int acErrorCounter = 0; //counter for error condition of cooling

//Pins
int AcReqOutPin = 7; //asks haltech for compressor
int AcSolenoidDrive = 9 ; //drives compressor
int AcReqOkPin = 8; //Check if haltech is allowing compressor
int BlowerSpeedInput = A1; //pin that reads blower speed



void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial.println("Init ok!");
 
  pinMode(AcReqOutPin, OUTPUT); // AC request status to send to haltech
  pinMode(AcReqOkPin, INPUT_PULLUP); // AC status input to read haltech AC confirmation
  pinMode(AcSolenoidDrive, OUTPUT); // Variable output for AC solenoid
 
  analogWrite(AcSolenoidDrive,0);// AC solenoid to 0V


}

void loop() 
{

  lastBlowerSpeed = blowerSpeed;
  Serial.print("Last Blower Speed: "); Serial.println(lastBlowerSpeed);
  blowerSpeed = analogRead(BlowerSpeedInput);
  Serial.print("Actual Blower Speed: "); Serial.println(blowerSpeed);

  if (blowerSpeed > 100){digitalWrite(AcReqOutPin, HIGH);}
  else {digitalWrite(AcReqOutPin, LOW);}
  
  
  AcOK = digitalRead(AcReqOkPin);
  AcOK = !AcOK;
  Serial.print("AC Ok?: "); Serial.println(AcOK);
  if (!AcOK)
  { 
    CurrentDriveSpeed = 0;
    AcRequest = LOW;
  }
  else
  {
   if (blowerSpeed < 263)
   { RequestedDriveSpeed = 240;}
   else if ((blowerSpeed >= 263) && (blowerSpeed < 272) )
   {RequestedDriveSpeed = 244;}
   else if ((blowerSpeed >= 272) && (blowerSpeed < 285) )
   { RequestedDriveSpeed = 246;}
   else if ((blowerSpeed >= 285) && (blowerSpeed < 300)) 
   { RequestedDriveSpeed = 248;}
   else if ((blowerSpeed >= 300) && (blowerSpeed < 320)) 
   { RequestedDriveSpeed = 250;}
   else if ((blowerSpeed >= 320)) 
   { RequestedDriveSpeed = 255;}
  }

  CurrentDriveSpeed = RequestedDriveSpeed;
ACEvapTemp = analogRead(A6);
if (ACEvapTemp > 500) { CurrentDriveSpeed = CurrentDriveSpeed - 2;}
if (ACEvapTemp > 505) { CurrentDriveSpeed = CurrentDriveSpeed - 2;}
if (ACEvapTemp > 510) { CurrentDriveSpeed = CurrentDriveSpeed - 3;}
if (ACEvapTemp > 517) { CurrentDriveSpeed = CurrentDriveSpeed - 3;}
if (ACEvapTemp > 528) { CurrentDriveSpeed = CurrentDriveSpeed - 245;}

switch (RequestedDriveSpeed){
  case 240:
    if (ACEvapTemp < 460){CurrentDriveSpeed = CurrentDriveSpeed + 2;}
    break;
  case 244:
    if (ACEvapTemp < 465){CurrentDriveSpeed = CurrentDriveSpeed + 2;}
    break;
  case 246:
    if (ACEvapTemp < 470){CurrentDriveSpeed = CurrentDriveSpeed + 2;}
    break;
  case 248:
    if (ACEvapTemp < 475){CurrentDriveSpeed = CurrentDriveSpeed + 2;}
    break;
  case 250:
    if (ACEvapTemp < 480){CurrentDriveSpeed = CurrentDriveSpeed + 2;}
    break;
  default:
    break;
}

if ((RequestedDriveSpeed > 13) && (ACEvapTemp < 460) && (AcOK))
{
  Serial.println("\n\n\nAC ERROR!");
  for (int i = 0; i < 2000; i++)
  {
    Serial.println("Confirming AC ERror");
    if (( i == 1999) && (RequestedDriveSpeed > 13) && (ACEvapTemp < 410))
    {
      Serial.println("Resetting AC!");
      analogWrite(AcSolenoidDrive,0);
      Serial.println("Compressor OFF!");
      delay(1000);
      Serial.println("Compressor ON!");
      analogWrite(AcSolenoidDrive,255);
      delay(16000);
    }
    if ( i == 1800){
    prevACEvapTemp = ACEvapTemp;
    }
  }
  
}

 if (!AcOK)
  { 
    CurrentDriveSpeed = 0;
    AcRequest = LOW;
  }
 analogWrite(AcSolenoidDrive,CurrentDriveSpeed);
  Serial.print("Requested Drive Speed: "); Serial.println(RequestedDriveSpeed);
 Serial.print("Current Drive Speed: "); Serial.println(CurrentDriveSpeed);
 Serial.print("PreviosTemp Input: "); Serial.println(prevACEvapTemp);
 Serial.print("Temp Input: "); Serial.println(ACEvapTemp);
 //delay (150);
}

