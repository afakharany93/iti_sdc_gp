
#define MAX_VAL 250
#define MIN_VAL 125
#define BR1_PinNum 9
#define BR2_PinNum 10
void BR_Setup();
void BR_Speed(int,int);
void setup() {

  BR_Setup();
  BR_Speed(0,0);
  Serial.begin(9600);
}
void loop() 
{
  static int value;

  if(Serial.available()) 
  {
    value = Serial.parseInt();    // Parse an Integer from Serial

  }
  BR_Speed(value,value);

}
void BR_Setup()
{
  pinMode(BR1_PinNum,OUTPUT);
  pinMode(BR2_PinNum,OUTPUT);
}

void BR_Speed(int BR1_Speed,int BR2_Speed)
{
  if(BR1_Speed <MIN_VAL) BR1_Speed=MIN_VAL;
  if(BR1_Speed >MAX_VAL) BR1_Speed=MAX_VAL;
  analogWrite(BR1_PinNum,BR1_Speed);

  if(BR2_Speed <MIN_VAL) BR2_Speed=MIN_VAL;
  if(BR2_Speed >MAX_VAL) BR2_Speed=MAX_VAL;
  analogWrite(BR2_PinNum,BR2_Speed);
}



