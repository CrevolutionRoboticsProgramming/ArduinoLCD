#define Command_Up 0x5550
#define Command_Down 0x444F
#define Data_Length 6
#include <LiquidCrystal.h>

/*
 * Data Array Layout
 * 
 * Byte 1 & 2: Command Bytes
 * Contains any 2 byte command
 * to be sent to the arduino
 * ex: 0x5550 = UP Command
 * 
 * Byte 3: Talon Out
 * Contains the motor speed
 * being provided to the talon
 * in the Talon.Set method
 * 
 * Byte 4 and 5: Test Voltage
 * Voltage multiplied by 100 so 
 * no decimals when sent. Max value
 * is 12 volts or 4B0 in hex. 
 * Byte 4 contains the values in front
 * of the decimal point and byte 5 has 
 * all the values after the decimal
 * point.
 * 
 * Byte 6: Test Current
 * Gives current to the precision of
 * Amps (no milliamps). 
 * 
 */

byte data[Data_Length];
LiquidCrystal lcd(2,3,4,5,6,7);
void UpdateDisplay(int topLayer);
void DrawLayerZero();
void DrawLayerOne(int layer);
void DrawLayerTwo(int layer);
int topLayer;
int counter;


void setup() 
{
  pinMode(12, OUTPUT);
  counter = 0;
  topLayer = 0;
  lcd.begin(16, 2);
  Serial.begin(9600);
  
  lcd.setCursor(0, 0);
  lcd.print("Talon Out:");
  lcd.setCursor(0, 1);
  lcd.print("Voltage:");
  delay(1000);
  digitalWrite(12, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available())
  {
    Serial.readBytes(data, Data_Length);
    /*
    Serial.print(data[0]);
    Serial.print(data[1]);
    */
    if (((data[0] << 8) | data[1]) == 0x5550 && topLayer > 0)
    {
      topLayer--;
    } else if (((data[0] << 8) | data[1]) == 0x444F && topLayer < 1)
    {
      topLayer++;
    }
    
    UpdateDisplay(topLayer);
  }
 
}

void UpdateDisplay (int topLayer)
{
  switch(topLayer)
  {
    case 0:
    {
      lcd.clear();
      DrawLayerZero();
      DrawLayerOne(1);
      break;
    }
    case 1:
    {
      lcd.clear();
      DrawLayerOne(0);
      DrawLayerTwo(1);
      break;
    }
    default:
    {
      break;
    }
  }
}

void DrawLayerZero()
{
  lcd.setCursor(0, 0);
  lcd.print("Talon Out:");
  lcd.setCursor(11, 0);
  lcd.print(data[2]);
}

void DrawLayerOne(int layer)
{
  lcd.setCursor(0, layer);
  lcd.print("Voltage:");
  lcd.setCursor(9, layer);
  lcd.print(data[3]);
  lcd.setCursor(11, layer);
  lcd.print(".");
  lcd.setCursor(12, layer);
  lcd.print(data[4]);
}

void DrawLayerTwo(int layer)
{
  lcd.setCursor(0, layer);
  lcd.print("Current:");
  lcd.setCursor(9, layer);
  lcd.print(data[5]);
}

