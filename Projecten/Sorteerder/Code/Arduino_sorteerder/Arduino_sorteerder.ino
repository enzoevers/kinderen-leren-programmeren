//===================================
// Bibliotheken
//===================================
#include <Wire.h>              //
#include "Adafruit_TCS34725.h" // Communicatie met de sensor

#include <Servo.h> // Servo aansturing

//===================================
// Variabelen
//===================================

//-----------------------------------
// Variabelen voor servos
//-----------------------------------
Servo servo_glijbaan;
Servo servo_meting;

int servo_glijbaan_pin = 9;
int servo_meting_pin   = 10;

int hoek_pak_snoep    = 5;
int hoek_kleur_sensor = 65;
int hoek_glijbaan    = 110;

int hoek_bakje_rood  = 45;
int hoek_bakje_groen = 90;
int hoek_bakje_blauw = 135;

int wachttijd_na_servo_beweging_ms = 500;

//-----------------------------------
// Variabelen voor de kleurensensor
//-----------------------------------
Adafruit_TCS34725 kleurensensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS,
                                                    TCS34725_GAIN_1X);
uint16_t r;
uint16_t g;
uint16_t b;
uint16_t c;

int meettijd_ms = 615;

//===================================
// Functies
//===================================
void led_aan(bool aan)
{
  kleurensensor.setInterrupt(!aan);
}

void beweeg_servo_naar_hoek(Servo& servo, int hoek)
{
  servo.write(hoek);
  delay(wachttijd_na_servo_beweging_ms);
}

//===================================
// Programma
//===================================
void setup() 
{
  // Start de kleurensensor
  kleurensensor.begin();

  // Sluit de servo variabelen aan op de pinnen.
  servo_glijbaan.attach(servo_glijbaan_pin, 500, 2500);
  servo_meting.attach(servo_meting_pin, 500, 2500);
}

void loop() 
{
  //-------------------------
  // Pak een nieuw snoepje
  //-------------------------
  beweeg_servo_naar_hoek(servo_meting, hoek_pak_snoep);
  beweeg_servo_naar_hoek(servo_meting, hoek_kleur_sensor);

  //-------------------------
  // Meet de kleur van
  // het snoepje
  //-------------------------
  led_aan(true);
  delay(meettijd_ms);
  led_aan(false);
  kleurensensor.getRawData(&r, &g, &b, &c);

  //-------------------------
  // Bepaal naar welk bakje
  // het het snoepje gaat
  //-------------------------
  int hoek_naar_bakje;
  
  if ((r > g) && (r > b))
  {
    hoek_naar_bakje = hoek_bakje_rood;
  }
  else if ((g > r) && (g > b))
  {
    hoek_naar_bakje = hoek_bakje_groen;
  }
  else if ((b > g) && (b > r))
  {
    hoek_naar_bakje = hoek_bakje_blauw;
  }

  //-------------------------
  // Laat het snoepje naar
  // het juist bakje glijden
  //-------------------------
  beweeg_servo_naar_hoek(servo_glijbaan, hoek_naar_bakje);
  beweeg_servo_naar_hoek(servo_meting, hoek_glijbaan);
}
