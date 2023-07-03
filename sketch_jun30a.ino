
//benötigte Bibliotheken
#include <LiquidCrystal_I2C.h> //um LCD-Display über I2C zu steuern
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // Adresse des Displays

// Anschlüsse:
// GND - GND
// VCC - 5V
// SDA - ANALOG Pin 4
// SCL - ANALOG pin 5

//Variablen deklaieren
long time_start, time_end; 
int value_light_barrier_1, value_light_barrier_2;
float distance;
float velocity;
float mass_liquid, mass_sphere;
float radius_sphere;
float density_liquid;
float viscosity;
float volume_liquid;
float density_sphere;




int sensorValue;

const float g = 9.81;
const float Pi = 3.141592654;

const int buttonPin = 12;
int PotiPin = A0;
int light_barrier_1 = A1;
int light_barrier_2 = A2;



// ===========================
// =======   SETUP   =========
// ===========================

void setup()
   {
    Serial.begin(9600); // Schrittgeschwindigkeit (Baudrate)
    
    lcd.begin(16,2);        // Initialisierung des LCD-Displays
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("Kugelfall-");
    lcd.setCursor(0,1);
    lcd.print("Viskosimeter");
    
    delay(3000);
    
    lcd.setCursor(0,0);
    lcd.print("             ");
    lcd.setCursor(0,1);
    lcd.print("            ");

    volume_liquid = 250.0;  // Volumen der Flüssigkeit im Zylinder

    radius_sphere = 0.002;  // Radius der Kugel

    mass_sphere = 0.0002669;   // Masse der Kugel

    distance = 0.153;      // Distanz zwischen den zwei Lichtschranken

    density_sphere = 7850.0;  //Dichte der Kugel


    pinMode(buttonPin, INPUT); //Pin-Modus auf Eingang Input setzen

    delay(10);
    
    value_light_barrier_1 = analogRead(light_barrier_1);    // miteinbeziehen des Umgebungslichts

    delay(10);
    
    value_light_barrier_2 = analogRead(light_barrier_2);    //  miteinbeziehen des Umgebungslichts

    Serial.print("Wert Lichtschranke 1 = ");    //Anzeige am Montitor
    Serial.print(value_light_barrier_1);
    
    Serial.print("    Wert Lichtschranke 2 = ");
    Serial.println(value_light_barrier_2);

    
   }


// ===========================
// =======    LOOP   =========
// ===========================
void loop()
   {
    while(digitalRead(buttonPin))     // lest Wert von Poti
       {
       Serial.println(String(readTemperature()));

        sensorValue = analogRead(PotiPin);    // 

        //mass_liquid = 230 + sensorValue * 30.0 / 1023;   // mit Potentiometer kann man Masse von Öl einstellen

        density_liquid = mass_liquid / volume_liquid;           //kalkuliert Dichte von Öl

        lcd.setCursor(0,0);
        lcd.print("m_liquid = ");
        lcd.print(mass_liquid,1);
        lcd.print(" g     ");
        lcd.setCursor(0,1);
        lcd.print("if ok button...   ");

        delay(500);
    
       }


    lcd.setCursor(0,0);
    lcd.print("waiting for the ");
    lcd.setCursor(0,1);
    lcd.print("sphere...       ");

    while(analogRead(light_barrier_1) > 0.8 * value_light_barrier_1)    //warten bis die Kugel Lichtschranke 1 passiert
       {

       }
 
    time_start = micros();

    lcd.setCursor(0,0); //Cursor auf erste Zeile des LCD-Displays positionieren
    lcd.print("t_start = "); //t_start auf Display ausgeben
    lcd.print(time_start/1000000, 2); //umwandeln in Sekunden, 2 Nachkommastellen
    lcd.print("    ");
    lcd.setCursor(0,1); //Cursor auf erste Spalte und zweite Zeile positionieren
    lcd.print("                ");
      
    
    while(analogRead(light_barrier_2) > 0.8 * value_light_barrier_2)    //warten bis Kugel Lichtschranke 2 passiert
       {

       }
 
    time_end = micros();

    lcd.setCursor(0,1);
    lcd.print("t_end = ");
    lcd.print(time_end/1000.0, 2);
    lcd.print("    ");

    delay(4000);
    
    velocity = (distance) / ((time_end - time_start) / 100000.0);      // Geschwindigkeit berechnen


    // Calculation of the term(named viscosity)
    // ============================

    viscosity=(2*(radius_sphere*radius_sphere)*(density_sphere-density_liquid))/(9*velocity);
  

    lcd.setCursor(0,0);
    lcd.print("Wert = ");
    lcd.print(viscosity,3); 
    lcd.print("     ");
    lcd.setCursor(0,1);
    lcd.print("Knopf drücken... ");
    Serial.println(String(viscosity)); //am Computermonitor printen
      Serial.read();
    while(digitalRead(buttonPin))     // auf Knopf drücken, um nächste Messung zu starten
        delay(20); //kurze Verzögerung von 20 Millisekunden
      } 
    
      Serial.read();
    delay(1000);
  }
  
  #define MCP9808_ADDRESS 0x18 //Adresse definieren

void temperatursensor() {
  Wire.begin(); //startet I2C-Kommunikation
  Serial.begin(9600);
}

//Temperatur messen
void tempsenor() {   ///////
  float temperature = readTemperature(); //aufrufen der Funktion
  Serial.print("Temperatur: "); //Temperatur auf seriellen Monitor anzeigen lassen
  Serial.print(temperature);
  Serial.println(" °C");
  delay(1000);
}

float readTemperature() { //misst Temperatur und gibt diesen Wert aus
  Wire.beginTransmission(MCP9808_ADDRESS);
  Wire.write(0x05);
  Wire.endTransmission(); //beendet Kommunikation mit I2C

  Wire.requestFrom(MCP9808_ADDRESS, 2);
  byte msb = Wire.read(); //most significant byte
  byte lsb = Wire.read(); //least significant byte

  float temp = ((msb & 0x1F) * 256 + lsb) / 16.0; //Temperatur basierend auf den Bytes
  if (msb & 0x10) {
    temp -= 256.0;
  }
  return temp; //Temperatur ausgeben
}

