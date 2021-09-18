// ------- Tank details ------//
const int total_height = 30; // Tank height in CM
const int hold_height = 25;// Water hold height in CM

//----- Your Phone no. -------//
char cntry_code[] = "+91"; // Country code
char ph[] = "xxxxxxxxxx"; // owner's (receiver's) phone no.

const int trigger = 2;
const int echo = 3;
int var_1 = 0;
char input_string[15];

long Time;
int distanceCM;
int resultCM;
int tnk_lvl = 0;
int sensr_to_wtr = 0;
/*YF- S201 water Flow sensor code for Arduino */
const int Output_Pin = 4;

volatile int  Pulse_Count;
unsigned int  Liter_per_hour;
unsigned long Current_Time, Loop_Time;

void setup()
{
  Serial.begin(9600);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  sensr_to_wtr = total_height - hold_height;

  delay(20000);
  delay(20000);
  delay(20000);
  
  Serial.println("AT+CNMI=2,2,0,0,0");
  delay(1000);
  Serial.println("AT+CMGF=1");
  delay(500);
  Serial.print("AT+CMGS=");
  Serial.print("\"");
  Serial.print(ph);
  Serial.println("\"");
  delay(1000);
  Serial.println("System is ready to receive SMS commands.");
  delay(100);
  Serial.println((char)26);
     pinMode(Output_Pin, INPUT);
   Serial.begin(9600); 

                                     
   Current_Time = millis();
   Loop_Time = Current_Time;
    attachInterrupt(0, Detect_Rising_Edge, RISING);
}

void loop()
{
     Current_Time = millis();
   if(Current_Time >= (Loop_Time + 1000))
   {
      Loop_Time = Current_Time;
      Liter_per_hour = (Pulse_Count * 60 / 7.5);
      Pulse_Count = 0;
      Serial.print(Liter_per_hour, DEC);
      Serial.println(" Liter/hour");
   }
  
}

void serialEvent()
{
  while (Serial.available())
  {
    if (Serial.find("/"))
    {
      delay(1000);
      while (Serial.available())
      {
        char input_char = Serial.read();
        input_string[var_1++] = input_char;
        if (input_char == '/')
        {
          if (!(strncmp(input_string, "status", 6)))
          {
            measure();
            Serial.print("AT+CMGS=");
            Serial.print("\"");
            Serial.print(cntry_code);
            Serial.print(ph);
            Serial.println("\"");
            delay(1000);
            Serial.print("Tank water level is: ");
            Serial.print(tnk_lvl);
            Serial.println("%");
            delay(100);
            Serial.println((char)26);
          }
          var_1 = 0;
          return;
        }
      }
    }
  }
}

void measure()
{
  delay(100);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  Time = pulseIn(echo, HIGH);
  distanceCM = Time * 0.034;
  resultCM = distanceCM / 2;

  tnk_lvl = map(resultCM, sensr_to_wtr, total_height, 100, 0);
  if (tnk_lvl > 100) tnk_lvl = 100;
  if (tnk_lvl < 0) tnk_lvl = 0;
}
void Detect_Rising_Edge ()
{ 
   Pulse_Count++;
}
