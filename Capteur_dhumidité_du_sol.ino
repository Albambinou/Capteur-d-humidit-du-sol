#include <Wire.h>
#include <rgb_lcd.h>

rgb_lcd lcd;

#define solPin A1 // port de connexion du capteur
#define ledPin 13 // port de connexion du led

int minsol = 900; // minimal d'humidité du sol
int HR;
int HRp;
int decompte = 1;
void setup()
{
   Serial.begin(9600);

   pinMode(solPin, INPUT);
   pinMode(ledPin, OUTPUT);
   
   lcd.begin(16, 2);
   lcd.setCursor(0, 0);
   lcd.print("Humidite du sol :");
   delay(1000);
}

void loop()
{
   lcd.clear();
   lcd.print("Humidite du sol :");
   Serial.print ("Mesure n° ");
   Serial.println (decompte);
   decompte++;
  
   HR = analogRead(solPin);
   Serial.print(" Humidité du sol = ");
   Serial.println(HR); // afficher variable

   lcd.setCursor(7, 1);
   HRp = 1024 - HR;
   HRp = HRp / 10;
   lcd.print(HRp);
   lcd.setCursor(9, 1);
   lcd.print(" %");
   delay(500);
}
