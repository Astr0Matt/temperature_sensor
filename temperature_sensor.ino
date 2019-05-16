// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"
#include "LiquidCrystal.h" //ajout de la librairie
 
#define DHTPIN 8     // what digital pin we're connected to
/* Coefficient diviseur du pont de résistance */
const float COEFF_PONT_DIVISEUR_VIN = 1.25;
// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(12,11,5,4,3,2); //liaison 4 bits de données

/* 
 *  fil rouge               +5v 
 *  fil noir                gnd
 *  fil jaune pont diviseur pin A0
 *  fil vert capteur        pin 2
 *  fil vert écran lcd      pin 3
 *  fil gris lcd  (D6)      pin 4 
 *  fil violet lcd (D5)     pin 5
 *  fil gris trait (D4)     pin 6
 *  fil blanc trait (E)     pin 10
 *  fil violet (rs)         pin 11   
 *  
 *  
 */






 
/** Mesure la référence interne à 1.1 volts */
unsigned int analogReadReference(void) {
  
  /* Elimine toutes charges résiduelles */
#if defined(__AVR_ATmega328P__)
  ADMUX = 0x4F;
#elif defined(__AVR_ATmega2560__)
  ADCSRB &= ~(1 << MUX5);
  ADMUX = 0x5F;
#elif defined(__AVR_ATmega32U4__)
  ADCSRB &= ~(1 << MUX5);
  ADMUX = 0x5F;
#endif
  delayMicroseconds(5);
  
  /* Sélectionne la référence interne à 1.1 volts comme point de mesure, avec comme limite haute VCC */
#if defined(__AVR_ATmega328P__)
  ADMUX = 0x4E;
#elif defined(__AVR_ATmega2560__)
  ADCSRB &= ~(1 << MUX5);
  ADMUX = 0x5E;
#elif defined(__AVR_ATmega32U4__)
  ADCSRB &= ~(1 << MUX5);
  ADMUX = 0x5E;
#endif
  delayMicroseconds(200);

  /* Active le convertisseur analogique -> numérique */
  ADCSRA |= (1 << ADEN);
  
  /* Lance une conversion analogique -> numérique */
  ADCSRA |= (1 << ADSC);
  
  /* Attend la fin de la conversion */
  while(ADCSRA & (1 << ADSC));
  
  /* Récupère le résultat de la conversion */
  return ADCL | (ADCH << 8);
}
                
void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
  Serial.println("DHTxx test!");
   lcd.begin(16,2); //utilisation d'un écran 16 colonnes et 2 lignes
   //lcd.write("Salut les Zer0s !"); //petit test pour vérifier que tout marche
   lcd.print("Initialisation");
   delay(2000);
  lcd.clear();
  dht.begin();
}
float tensionAlim= 5;
float batteryLevel(float vout){

  return vout*100/tensionAlim;

}
int i=5;
float h,t;
void loop() {
  //lcd.begin(1,2); //utilisation d'un écran 16 colonnes et 2 lignes
  // Wait a few seconds between measurements.
  

  h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    //Serial.println("Failed to read from DHT sensor!");
    lcd.write("Failed to read from DHT sensor!");
    return;
  }
  
  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  
  //mise en forme et affichage des informations sur l'écran LCD
  lcd.setCursor(0, 0); //Positionnement du curseur
  lcd.print("Degres : ");
  lcd.setCursor(9, 0);
  lcd.print(t-1); //Affichage de la température
  lcd.setCursor(13, 0);
  lcd.print((char)223); //Affiche le caractère ° (degrés)
  lcd.setCursor(14, 0);
  lcd.print("C"); //En degrés Celsuis
  lcd.setCursor(0, 1);
  lcd.print("Humidite : ");
  lcd.setCursor(11, 1);
  lcd.print(h); //Affichage de l'humidité
  lcd.setCursor(15, 1);
  lcd.print("%");
  
  delay(2000);
  /*
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
    */
}
