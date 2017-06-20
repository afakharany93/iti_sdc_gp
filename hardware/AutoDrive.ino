#include <Servo.h>

#define us_total_nu 3

#define us_1_trig 4
#define us_1_echo 5
#define us_2_trig 6
#define us_2_echo 7
#define us_3_trig 8
#define us_3_echo 9

//Ultrasonic range in cm
#define us_max_range  250
#define uS_min_range  10

#define MAX_VAL 250
#define MIN_VAL 125
#define START_VAL 125

#define ch1  A0
#define ch2  A1

#define motR  11
#define motL  10

#define steer_pin  3
#define emergency_distance  30

#define start_bit 115
#define end_bit   101

int steering_sens, throttle_sens;
int steering_ang, throttle_ang;
int spd_val;
int throttle_arr[10];
int old_spd = 0;
int throt_ang;
unsigned int throttle_avg;
unsigned int throttle_ang_avg;
unsigned char value = 125; // set values you need to zero

int prev_val;
int prev_br_spd=0;
double lp_val;
uint8_t us_pins[us_total_nu * 2] = {
  us_1_trig, us_1_echo, us_2_trig, us_2_echo, us_3_trig, us_3_echo};

float us_data[us_total_nu];

const unsigned long us_timer = 100000; // 100ms =0.1s

unsigned long time_now, time_last;

int Final_spd;

int steer_recv, throttle_recv;

void us_setup();  //set up pinMode for all ultrasonics
float us_one_Read(uint8_t trig, uint8_t echo);
void us_DAQ();  //Aquire data from all ultrasonic
uint8_t us_avg(); //return avg value
void BR_Setup(); //set up both brushless motors
void BR_Speed(int BR1_Speed, int BR2_Speed);

void com_setup();   //setup comunication

void com_send();

void steer_setup();
void steer_loop();


Servo steer_servo;

void setup()
{
  us_setup();
  BR_Setup();
  brushless_init();
  steer_setup();
  com_setup();
  delay(5000);
}

void loop()
{

  if(com_recv())
   {
   //apply to steering
   steer_control(steer_recv);
   //apply to throttle
   throttle_control(throttle_recv);
   }
 
  //read us
  us_DAQ();
  //apply emergency brakes if object in front of us less than emergency_distance
  if(us_avg()<=emergency_distance)
  {
    //emergency_brake
    emergency_brake();
    Serial.println("Brake");
  }

//  Serial.println(Serial.read());
  

  delay(100);
}

void BR_Speed(int spd, int old)
{
  if (spd < MIN_VAL) spd = MIN_VAL;
  if (spd > MAX_VAL) spd = MAX_VAL;

  if (old < 90 && spd >= 125)
  {
    spd = 125;
    Serial.println("Zeft");
    analogWrite(motR, spd);    
    analogWrite(motL, spd);
    //delay(3000);
  }
  analogWrite(motR, spd);    
  analogWrite(motL, spd);
}

void ReadData()
{
  steering_sens = analogRead(ch1);

  steering_ang = map(steering_sens, 200, 400, 0, 100);
  //Serial.println(throttle_sens);
  throttle_ang_avg = 0;

  int flag = 0;

  for (int i = 0; i < 10; i++)
  {
    throttle_sens = analogRead(ch2);
    throttle_ang = map(throttle_sens, 389, 211, 100, 0);
    /*
      Serial.print("Throttle ang in loop");
     Serial.println(throttle_ang);
     */
    if (throttle_ang <= 50)
    {
      throttle_ang_avg = MIN_VAL;
      flag = 1;
      break;
    }
    else
      throttle_ang_avg += throttle_ang;
  }//end for loop

  if (!flag)
  {
    throttle_ang_avg /= 10;
  }

  if (throttle_ang_avg > 50)
    spd_val = map(throttle_ang_avg, 100, 50, 255, 140);
  else if (throttle_ang_avg <= 50)
    spd_val = 0;

}

void Br_control()
{
  int spd_flag;
  Final_spd = 0;
  for (int k = 0; k < 10; k++)
  {
    ReadData();
    if (spd_val == 0)
    {
      Final_spd = 0;
      break;
    }
    Final_spd += spd_val;
  }

  if (Final_spd != 0)  Final_spd /= 10;
  analogWrite(motR, Final_spd); 
  analogWrite(motL, Final_spd);

  //BR_Speed(Final_spd, old_spd);
  old_spd = Final_spd;
}

void Br_serial()
{
  /*
    Serial.print(throttle_sens);
   Serial.print(" ");
   Serial.print(throttle_ang);
   Serial.print(" ");
   */

  // range 0 100
  int ser_steer, ser_throt;
  ser_steer = map(steering_sens, 180, 370, 0, 100);
  //ser_steer = constrain(ser_steer, 0, 100);
  //ser_steer = map(ser_steer, 38, 95, 0, 100);
  Serial.println(ser_steer);
  
  //Serial.print(" ");
  // range 0 100
  if (throt_ang <= 125)  ser_throt = 0;
  else
  {
    //ser_throt=constrain(ser_throt,
    ser_throt = map(throt_ang, 255, 125, 100, 0);
  }

  Serial.println(ser_throt);
  //Serial.print(" ");
  //Serial.println();
}

//Setup Functions
void us_setup()  //set up pinMode for all ultrasonics
{
  uint8_t i;
  for (i = 0; i < us_total_nu * 2; i += 2)
    pinMode(us_pins[i], OUTPUT);  //trig

  for (i = 1; i < us_total_nu * 2; i += 2)
    pinMode(us_pins[i], INPUT); //echo
}//End of us_setup

void BR_Setup() //set up both brushless motors
{
  pinMode(motR, OUTPUT);
  pinMode(motR, OUTPUT);
}

void com_setup()   //setup comunication
{
  Serial.begin(9600);
  Serial.println("Program wait for beep sound");
}


//Loop Functions
void us_DAQ()  //Aquire data from all ultrasonic
{
  uint8_t i, us_count = 0;
  for (i = 0; i < us_total_nu * 2; i += 2)
  {
    us_data[us_count] = us_one_Read(us_pins[i], us_pins[i + 1]);
    us_count++;
  }//End for loop
}//End of us_DAQ

uint8_t us_avg() //Get average reading of us
{
  int i,sum=0,avg=0;
  for(i=0;i<us_total_nu;i++)
  {
    sum+=us_data[i];
    Serial.println(us_data[i]);
  }
  Serial.println(sum);
  avg=sum/3;
  return avg;
}


float us_one_Read(uint8_t trig, uint8_t echo)
{
  float dist_cm;
  digitalWrite(trig, HIGH);  
  delayMicroseconds(10);  
  digitalWrite(trig, LOW);
  dist_cm = pulseIn(echo, HIGH) / 58.0;
  dist_cm = constrain(dist_cm, 15, 250);
  return dist_cm;
}//End of us_one_Read

void com_send()
{
  Serial.println("Start");
  Br_serial();
  us_serial();
  Serial.println("END");
}

void us_serial()
{
  uint8_t i;
  for (i = 0; i < us_total_nu; i++)
  {
    Serial.println(us_data[i]);
    //Serial.println();
  }
}

int com_recv()
{
  int flag=0;
  if(Serial.available())
  {
    for(int i=0;i<4;i++)
    {
      if(Serial.read()== start_bit)
      //if(Serial.read())
      {
        flag=1;
        break;
      }
    }//end of for loop
    if(flag==1)
    {
      steer_recv=Serial.read();
      steer_recv=map(steer_recv,48,56,0,100);
      throttle_recv=Serial.read(); 
      throttle_recv=map(throttle_recv,48,56,0,100);
      //Serial.flush();
    }//End of if
  }//End of serial available

  return flag;
}


void newRead()
{
  steering_sens = analogRead(ch1);

  steering_ang = map(steering_sens, 200, 400, 0, 100);

  //Finished steering
  //throttle_ang_avg = 0;

  throt_ang = analogRead(ch2);
  //throt_ang=map(throt_ang,395,310,100,0);
  //if less than 5   brake

  //LOW pass filter
  //lp_val=batoot.apply_filter(throt_ang);

  throt_ang = analogRead(ch2);
  if (throt_ang < 310)   throt_ang = 0;
  throt_ang -= 245;
  throt_ang += 71;
  throt_ang = constrain(throt_ang, 0, 255);
  //throt_ang=map(throt_ang,395,310,100,0);

  //Serial.println(throt_ang);
}

void newControl()
{
  if (throt_ang == 0)
  {
    analogWrite(motR, 125);  
    analogWrite(motL, 125);
  }
  else if (prev_val == 0 && throt_ang != 0)
  {
    for (int i = 125; i < 200; i++)
    {
      analogWrite(motR, i);  
      analogWrite(motL, i);
      delay(1);
    }
  }
  prev_val = throt_ang;
}

void steer_setup()
{
  steer_servo.attach(steer_pin);
}

void steer_control(int steer_ang_control)
{
  int angle=map(steer_ang_control,0,100,70,140);
  angle=constrain(angle,0,180);
  steer_servo.write(angle);
}

void throttle_control(int throt_spd)
{
  if(throt_spd==0)
  {
    emergency_brake();
  }
  else
  {
    if(throt_spd>90)  throt_spd=90;
    throt_spd=map(throt_spd,0,100,125,255);
    brushless_direct_control(throt_spd);
  }
}

void emergency_brake()
{
  brushless_direct_control(125);
}

void brushless_direct_control(int spd)
{
  analogWrite(motR,spd);  
  analogWrite(motL,spd);
}

void brushless_init()
{
  brushless_direct_control(125);
}
void brushless_apply(int spd)
{
  int volt_spd;
  if(prev_br_spd<=125 && spd>10)
  {
    for(int i=0;i<spd;i++)
    {      
      volt_spd=map(spd,0,100,125,250);
      brushless_direct_control(volt_spd);  
      delay(1);
    }
  }
  else 
  {
    volt_spd=map(spd,0,100,125,255);
    brushless_direct_control(volt_spd);  
  }
    if(spd==0)  brushless_direct_control(125);
  prev_br_spd=volt_spd;
}

