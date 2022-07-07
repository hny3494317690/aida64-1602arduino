#include <Wire.h>
#include <LiquidCrystal_I2C.h>
String DA;
String TXT;
volatile int CT;
volatile int CF;
volatile int CU;
volatile int CFR;
volatile boolean PD;
LiquidCrystal_I2C mylcd(0x27,16,2);
byte lcd[]={0B11000,0B11111,0B01000,0B10000,0B10000,0B10000,0B01000,0B00111};
void serialEvent() 
{
  SerialRead();
  LCDPrint();
}

void LCDPrint() 
{
  mylcd.setCursor(0, 0);
  mylcd.print(CT);
  mylcd.setCursor(0, 1);
  mylcd.print(String(CU) + String("%   "));
  mylcd.setCursor(6-1, 1-1);
  mylcd.print(String(CF) + String("MHZ        "));
  mylcd.setCursor(6-1, 2-1);
  mylcd.print(String(CFR) + String("RPM        "));
  if (PD && String(CT).length() == 3) 
  {
    PD = false;
    mylcd.setCursor((String(CT).length() + 1)-1, 1-1);
    mylcd.write(0);

  } 
  else if (!PD && String(CT).length() == 2) 
  {
    PD = true;
    mylcd.setCursor((String(CT).length() + 1)-1, 1-1);
    mylcd.write(0);
    mylcd.setCursor((String(CT).length() + 2)-1, 1-1);
    mylcd.print(' ');
  }
}

void SerialRead() 
{
  DA = Serial.readString();
  Serial.flush();
  TXT = String(DA).substring((String(DA).indexOf(String("A")) + 2),(String(DA).indexOf(String("A")) + 5));
  TXT.trim();
  CT = String(TXT).toInt();
  TXT = String(DA).substring((String(DA).indexOf(String("B")) + 2),(String(DA).indexOf(String("B")) + 5));
  TXT.trim();
  CU = String(TXT).toInt();
  TXT = String(DA).substring((String(DA).indexOf(String("C")) + 2),(String(DA).indexOf(String("C")) + 6));
  TXT.trim();
  CF = String(TXT).toInt();
  TXT = String(DA).substring((String(DA).indexOf(String("D")) + 2),(String(DA).indexOf(String("D")) + 6));
  TXT.trim();
  CFR = String(TXT).toInt();
}

void setup()
{
  Serial.begin(9600);
  DA = ' ';
  TXT = ' ';
  CT = 0;
  CF = 0;
  CU = 0;
  CFR = 0;
  PD = true;
  mylcd.init();
  mylcd.backlight();
  pinMode(13, OUTPUT);
  mylcd.createChar(0, lcd);
  if (!Serial.available() > 0) 
  {
    mylcd.setCursor(0, 0);
    mylcd.print("Lost connection");
    mylcd.setCursor(0, 1);
    mylcd.print("Check upper PC");
    digitalWrite(13,HIGH);
    while (!(Serial.available() > 0))
    {
      delay(100);
    }
    digitalWrite(13,LOW);

  }
  mylcd.setCursor(3-1, 1-1);
  mylcd.write(0);
  mylcd.setCursor(4-1, 1-1);
  mylcd.print(' ');
}

void loop()
{
  
}
//A：CPU温度；B:CPU占用；C:CPU频率；D:CPU风扇
  //E:GPU占用；F:GPU温度 G:GPU风扇；H:显存占用
  //I:硬盘吞吐量；J:网络吞吐量；K:CPU电压；L:内存占用
