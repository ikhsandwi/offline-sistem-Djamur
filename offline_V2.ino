#include "DHT.h"

#define DHTPIN 4     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
#define fan 10    //out fan
#define pump 16   //out pompa
#define lamp 14   //out lampu
#define infan 7  //input sinyal kipas dari NodeMCU
#define inpump 6  //input sinyal pompa dari NodeMCU
#define inlamp 5  //input sinyal lampu dari NodeMCU
#define nodefan 15 //kirim sinyal kipas ke NodeMCU
#define nodepump 8 //kirim sinyal pompa ke NodeMCU
#define nodelamp 9 //kirim sinyal lampu ke NodeMCU

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,20,4);
int maxHum = 90;
int minHum = 70;
int maxTemp = 27;
int minTemp = 24;
int tombol;
int flagpompa = 0;
int flaglampu = 0;
int flagkipas = 0;
DHT dht(DHTPIN, DHTTYPE);
byte termometru[8] = //icon for termometer
{
  B00100,
  B01010,
  B01010,
  B01110,
  B01110,
  B11111,
  B11111,
  B01110
};
byte picatura[8] = //icon for water droplet
{
  B00100,
  B00100,
  B01010,
  B01010,
  B10001,
  B10001,
  B10001,
  B01110,
};
byte water[] = {
  B00100,
  B00100,
  B01110,
  B01110,
  B11111,
  B11111,
  B11111,
  B01110
};
byte lampu[]= {
  B01110,
  B10001,
  B10001,
  B10001,
  B01110,
  B01110,
  B00100,
  B00000
};
byte kipas[] = {
  B00111,
  B01011,
  B10000,
  B00000,
  B11111,
  B11001,
  B10000,
  B01000
};
byte kipas2[] = {
  B00010,
  B00001,
  B10111,
  B11111,
  B00000,
  B00001,
  B11010,
  B11100
};
void setup() {
  pinMode(fan, OUTPUT);
  pinMode(pump, OUTPUT);
  pinMode(lamp, OUTPUT);
  pinMode(infan,INPUT);
  pinMode(inlamp,INPUT);
  pinMode(inpump,INPUT);
  Serial.begin(9600);

  pinMode(nodefan,OUTPUT);
  pinMode(nodepump,OUTPUT);
  pinMode(nodelamp,OUTPUT);

  digitalWrite(fan,HIGH);
  digitalWrite(pump,HIGH);
  digitalWrite(lamp,HIGH);

  digitalWrite(nodelamp,LOW);
  digitalWrite(nodepump,LOW);
  digitalWrite(nodefan,LOW);
  
  flagpompa = 0;
  flaglampu = 0;
  flagkipas = 0;
  Serial.begin(9600); 
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  lcd.clear();
  lcd.createChar(1,termometru);
  lcd.createChar(2,picatura);
  lcd.createChar(3,water);
  lcd.createChar(4,lampu);
  lcd.createChar(5,kipas);
  lcd.createChar(6,kipas2);
  dht.begin();
  lcd.setCursor(1,0);
  lcd.print("OFF");
  lcd.setCursor(7,0);
  lcd.print("OFF");
  lcd.setCursor(12,0);
  lcd.print("OFF");
}
void button()
{
  tombol=analogRead(A2);
  Serial.print("  ADC:");
  Serial.println(tombol);
  delay(1);
}
int fansignal;
int lampsignal;
int pumpsignal;
void inputnodemcu()
{
  int fansignal=digitalRead(infan);
  int lampsignal=digitalRead(inlamp);
  int pumpsignal=digitalRead(inpump);
  Serial.print("  fan :");
  Serial.print(fansignal);
  Serial.print("  lamp :");
  Serial.print(lampsignal);
  Serial.print("  pump :");
  Serial.print(pumpsignal);
  delay(100);
}
void pompaon()
{
  lcd.setCursor(1,0);
  lcd.print("ON  ");
  digitalWrite(pump,LOW);
  flagpompa=1;
  digitalWrite(nodepump,HIGH);
 
}
void pompaoff()
{
  lcd.setCursor(1,0);
  lcd.print("OFF ");
  digitalWrite(pump,HIGH);
  flagpompa=0;
  digitalWrite(nodepump,LOW);
  
}
void kipason()
{
  lcd.setCursor(7,0);
  lcd.print("ON  ");
  digitalWrite(fan,LOW);
  digitalWrite(nodefan,HIGH);
  flagkipas=1;
}
void kipasoff()
{
  lcd.setCursor(7,0);
  lcd.print("OFF ");
  digitalWrite(fan,HIGH);
  digitalWrite(nodefan,LOW);
  flagkipas=0;
}
void lampuon()
{
  lcd.setCursor(12,0);
  lcd.print("ON  ");
  digitalWrite(lamp,LOW);
  digitalWrite(nodelamp,HIGH);
  flaglampu=1;
}
void lampuoff()
{
  lcd.setCursor(12,0);
  lcd.print("OFF ");
  digitalWrite(lamp,HIGH);
  digitalWrite(nodelamp,LOW);
  flaglampu=0;
}
void loop() { 
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Failed read");
    lcd.setCursor(0,1);
    lcd.print("DHT sensor!");
    delay(2000);
    lcd.clear();
    return;
  }
  if(flaglampu==1)
  {
    lcd.setCursor(12,0);
    lcd.print("ON  ");
  }
  if(flaglampu==0)
  {
    lcd.setCursor(12,0);
    lcd.print("OFF ");
  }
  if(flagpompa==1)
  {
    lcd.setCursor(1,0);
    lcd.print("ON  ");
  }
  if(flagpompa==0)
  {
    lcd.setCursor(1,0);
    lcd.print("OFF ");
  }
  if(flagkipas==1)
  {
    lcd.setCursor(7,0);
    lcd.print("ON  ");
  }
  if(flagkipas==0)
  {
    lcd.setCursor(7,0);
    lcd.print("OFF ");
  }
  inputnodemcu();
  lcd.setCursor(0,1);
  lcd.write(1);
  lcd.print(t,1);
  lcd.print((char)223);lcd.print("C");
  lcd.setCursor(9,1);
  lcd.write(2);
  lcd.print(h,1);
  lcd.print("%");
  //delay(200);
  
  button();
  lcd.setCursor(0,0);
  lcd.write(3);
    
  if(tombol>=105&&tombol<=113||pumpsignal==1)
  {
   if(flagpompa==0)
   {
     pompaon();    
   }
   else if(flagpompa==1)
   {
     pompaoff();
   }
  }
  
  if(h>90&&flagpompa==1)
    {
      pompaoff();
      flagpompa==0;
    }
  
  lcd.setCursor(5,0);
  lcd.write(5);
  lcd.write(6);
  
  if(tombol>=131&&tombol<=135||fansignal==1)
  {
    if(flagkipas==0)
    {
      kipason();
    }
    else if(flagkipas==1)
    {
      kipasoff();
    }
  }
  
  if(t<28&&h<90)
  {
    kipasoff();
    flagkipas==0;
  }
  
  lcd.setCursor(11,0);
  lcd.write(4);
  
  if(tombol>=203&&tombol<=206||lampsignal==1)
  {
    if(flaglampu==0)
    {
       lampuon();  
    }
    
    else if(flaglampu==1)
    {
      lampuoff();
    }
  }
  if(t>24&&h<90&&flaglampu==1)
  {
    lampuoff();
    flaglampu==0;
  }
  delay(200);
}
