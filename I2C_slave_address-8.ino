
//Tanvesh Bhattad, Shivam Kurzekar
//14 September 2018
#include<I2C_Anything.h>
#include<Wire.h>
const byte MY_address=8;

const int encoder1=2;
volatile long  count = 0;
long realCount ;

unsigned long lastRead = 0;
int a;
int pa;
const int in1=12;
const int in2=8;
float pwmpin=10;
float error=0;
float previouserror=0;
float ierror=0;
float derror=0;
float kp=0.00545 , KP=0;
float ki=0.0099 ,KI=0;
float kd=0.01, KD=0;
long targetcount;
float output;
int pwm=0;



void setup(){
 // put your setup code here, to run once:
 
  Wire.begin(MY_address);
//  TWBR=12;
  Serial.begin(9600);
  Serial.println("start...");
  
  pinMode(encoder1,INPUT_PULLUP);
  attachInterrupt(0, isrCount, CHANGE); 
  a= digitalRead(encoder1);
  Wire.onReceive(event);
 
}   

void loop(){
  // put your main code here, to run repeatedly:
  analogWrite(pwmpin,pwm);
  
  

 if (millis() - lastRead >= 100)
  {
    lastRead  = millis();
 
    //noInterrupts();
    realCount = count;
     
    count = 0;
    //interrupts();
    PID();
 

  } 
  Serial.print(targetcount);
  Serial.print("        ");
  Serial.print(realCount);
  Serial.print("                 ");
  Serial.println(pwm); 

    
}

void isrCount()
{

  count++;
 }

void event()
{ 
  I2C_readAnything(pa);
  
 // Serial.println(pa);
    targetcount=abs(pa);

  if(pa>0)
  {
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
  }

  if(pa<0)
  {
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
  }
  

       
}
void PID()
{
  error=targetcount-realCount;
  ierror=ierror+error;
  derror=(error-previouserror);
//
//  if(targetcount>=4000)
//   {
// if (count==0)
//  {
//  ierror=0;
// }
//   }

 if(targetcount==0 || targetcount==1 )
 {
 
  KP=0;
  KD=0;
  KI=0;
  ierror=0;
  analogWrite(pwmpin,0);
 }
 else
 {
  KP=kp;
  KD=kd;
  KI=ki;
 }

  output=error*KP+ ierror*KI +derror*KD;

  if(output>255)
  {
   pwm=255;
  }

  else if(output<=0)
  {
    pwm=0;
  }

  else
  {
    pwm=output;
  }
  


  previouserror=error;

  
  analogWrite(pwmpin,pwm);


}

