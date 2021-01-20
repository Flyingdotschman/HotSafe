/*
    Programmierung HotSafe von Dominik Pöschel F.G. Meier GmbH
    Stand 62.12.2019 15:17
    es fehlt noch der Piezo Buzzer
*/

#include <Adafruit_NeoPixel.h>
#include <MillisTimer.h>
#include <TimerThree.h>

//Definition Noten:
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST 0



// Definition der PINS
const byte LED_KATZE1 = 5;  // Pin LED-Streifen Katze Nr. 1
const byte LED_KATZE2 = 6;  // Pin LED-Streifen Katze Nr. 2
const byte LED_TRESOR1 = 9; // Pin LED-Streifen Tresor Nr. 1
const byte LED_TRESOR2 = 10; // Pin LED-Streifen Tresor Nr. 2
const byte KATZE_PIN = 0; // Kontakt Pin für Katze
const byte ENDE_PIN = 2;   // Kontakt Pin für Ziel / Ende
const byte START_PIN = 1; // Kontakt Pin für Start
const byte PIEZO = 8; // Kontakt für PiezoBuzzer Soundmodul
const byte TRESOR = 3; // Kontakt für Tresor offen

// Anzal LEDs pro Streifen
const int anzkatze1 = 125;  // Anzahl LEDs  Katze_LED-Streifen Nr. 1 rechts
const int anzkatze2 = 118;  // Anzahl LEDs  Katze_LED-Streifen Nr. 2 links
const int anztresor1 = 150; // Anzahl  LEDs Tresor_LED-Streifen 1
const uint32_t anztresor2 = 150; // Anzahl  LEDs Tresor_LED-Streifen 2



/*const uint32_t ROT    = strip.Color(255,0,0);
  const uint32_t GRUEN  = strip.Color(0,255,0);
  const uint32_t BLAU   = strip.Color(0,0,255);
  const uint32_t WEISS  = strip.Color(255,255,255);
*/

boolean armed_played = false;
boolean finish_played = false;
boolean death_played = false;

boolean tresor_open_played = false;
boolean tresor_closed_played = false;

// Gamestatuts, armed = Spielläuft, dead = Spieler hat Katze berührt, finished = Ziel wurde erreicht
enum GAMESTATUS {
  armed,
  dead,
  finished
};

enum TRESORSTATUS {
  geoeffnet,
  geschlossen,
  idle
};

TRESORSTATUS tresorstatus;
TRESORSTATUS temptresor;
TRESORSTATUS nowtresor;
GAMESTATUS gamestatus;


//Timer um LEDS blinken zu lassen.
MillisTimer timer1 = MillisTimer(1000);
MillisTimer timer2 = MillisTimer(500);
MillisTimer timer3 = MillisTimer(5000);
MillisTimer timer4 = MillisTimer(1000);
MillisTimer timer5 = MillisTimer(1000);
MillisTimer timerOde = MillisTimer(5000);


int melody[] = { // Ode an die Freude


  NOTE_E4, 4,  NOTE_E4, 4,  NOTE_F4, 4,  NOTE_G4, 4, //1
  NOTE_G4, 4,  NOTE_F4, 4,  NOTE_E4, 4,  NOTE_D4, 4,
  NOTE_C4, 4,  NOTE_C4, 4,  NOTE_D4, 4,  NOTE_E4, 4,
  NOTE_E4, -4, NOTE_D4, 8,  NOTE_D4, 2,

  NOTE_E4, 4,  NOTE_E4, 4,  NOTE_F4, 4,  NOTE_G4, 4, //4
  NOTE_G4, 4,  NOTE_F4, 4,  NOTE_E4, 4,  NOTE_D4, 4,
  NOTE_C4, 4,  NOTE_C4, 4,  NOTE_D4, 4,  NOTE_E4, 4,
  NOTE_D4, -4,  NOTE_C4, 8,  NOTE_C4, 2,
  /*
    NOTE_D4,4,  NOTE_D4,4,  NOTE_E4,4,  NOTE_C4,4,//8
    NOTE_D4,4,  NOTE_E4,8,  NOTE_F4,8,  NOTE_E4,4, NOTE_C4,4,
    NOTE_D4,4,  NOTE_E4,8,  NOTE_F4,8,  NOTE_E4,4, NOTE_D4,4,
    NOTE_C4,4,  NOTE_D4,4,  NOTE_G3,2,

    NOTE_E4,4,  NOTE_E4,4,  NOTE_F4,4,  NOTE_G4,4,//12
    NOTE_G4,4,  NOTE_F4,4,  NOTE_E4,4,  NOTE_D4,4,
    NOTE_C4,4,  NOTE_C4,4,  NOTE_D4,4,  NOTE_E4,4,
    NOTE_D4,-4,  NOTE_C4,8,  NOTE_C4,2
  */
};

// change this to make the song slower or faster
int tempo = 200;

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// this calculates the duration of a whole note in ms (60s/tempo)*4 beats
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;


// Laufvariablen
int j = 0;
int q = 0;
int l = 0;
int m = 0;
// SETUP ***********************************************************************************************
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // Für Ausgabe an PC/Laptop




  // Setzen der PINs auf IN oder OUTPUT
  pinMode(LED_KATZE1, OUTPUT);
  pinMode(LED_KATZE2, OUTPUT);
  pinMode(PIEZO, OUTPUT);
  pinMode(KATZE_PIN, INPUT);
  pinMode(ENDE_PIN, INPUT);
  pinMode(START_PIN, INPUT);
  pinMode(TRESOR, INPUT);

  // Interrupts zur Erkennung der Berührung des Heissendrahts
  attachInterrupt(digitalPinToInterrupt(KATZE_PIN), set_death, FALLING);
  attachInterrupt(digitalPinToInterrupt(START_PIN), set_armTool, FALLING);
  attachInterrupt(digitalPinToInterrupt(ENDE_PIN), set_finishReached, FALLING);
  attachInterrupt(digitalPinToInterrupt(TRESOR), tresorChange, CHANGE);   // Muss noch ggf. angepasst werden,falls richtung nicht stimmt
  //attachInterrupt(digitalPinToInterrupt(TRESOR), tresorGeschlossen, RISING);
  gamestatus = dead;
  tresorstatus = geoeffnet;
  temptresor = geschlossen;
  nowtresor = geschlossen;


  if (digitalRead(TRESOR) == 0) {
    //Serial.println("Tresor offen interrupt");
    tresorstatus = geoeffnet;
  }
  if (digitalRead(TRESOR) == 1) {
    //Serial.println("Tresor geschlossen interrupt");
    tresorstatus = geschlossen;
  }

}


// GAME LOOP ****************************************************************************************************
void loop() {
  // Gamestatus über USB setzen zum testen, 0 , 1 oder 2 senden.
  //Serial.println(digitalRead(TRESOR));


  while (Serial.available() > 0) {
    int xxx = Serial.read();
    if (xxx == 48) {
      gamestatus = armed;
      Serial.println("Tool Armded USB");
    }
    if (xxx == 49) {
      gamestatus = dead;
      Serial.println("Tool Dead USB");

    }
    if (xxx == 50) {
      gamestatus = finished;
      Serial.println("Tool Finished USB");
    }
    if (xxx == 51) {
      getStatus();
    }
    if (xxx == 52) {
      tresorstatus = geoeffnet;
      Serial.println("Tool Finished USB");
    }
    if (xxx == 53) {
      tresorstatus = geschlossen;
      Serial.println("Tool Finished USB");
    }
  }
  timer1.run();
  timer2.run();
  timer3.run();
  timer4.run();
  timer5.run();
  timerOde.run();
  //Serial.println(gamestatus);
  // put your main code here, to run repeatedly:
  switch (gamestatus) {
    case dead:
      if (death_played == false) {
        getStatus();
        Serial.println("Tool Dead");
        death();
        armed_played = false;
        death_played = true;
        finish_played = true;
      }
      break;

    case armed:
      if (armed_played == false) {
        getStatus();
        Serial.println("Tool Armed");
        l = 0;
        armTool();
        armed_played = true;
        death_played = false;
        finish_played = false;
      }
      break;

    case finished:
      if (finish_played == false) {
        getStatus();
        Serial.println("Tool Finished");
        m = 0;
        finishReached();
        armed_played = false;
        death_played = true;
        finish_played = true;
      }
      break;
  }

  switch (tresorstatus) {
    case geschlossen:
      getStatus();
      tresorGeschlossenAnimation();
      tresorstatus = idle;
      break;
    case geoeffnet:
      getStatus();
      tresorOffenAnimation();
      tresorstatus = idle;
      break;

  }
}


// DEFINITION DER METHODEN ***************************************************************************************

// Set Gamestatus per Interrupt
void set_death() // Gamestatus auf dead
{
  if (gamestatus != finished) {
    gamestatus = dead;
    Serial.println("DEAD Interupt");
  }
}

void set_armTool()  // Gamestatus auf armed
{
  gamestatus = armed;
  Serial.println("START Interrupt");
}

void set_finishReached()  // Gamestatus auf finished
{
  gamestatus = finished;
  Serial.println("FINISH Interrupt");
}


// Set Tresorstatus + recheck Treseorstatus nach 1 Sek, Buttonebounce
void tresorChange() {
  byte a = digitalRead(TRESOR);

  if (a == 0) {
    //Serial.println("Tresor offen interrupt");
    temptresor = geoeffnet;
  }
  if (a == 1) {
    //Serial.println("Tresor geschlossen interrupt");
    temptresor = geschlossen;
  }

  timer5.setInterval(1000);
  timer5.expiredHandler(recheckTresor);
  timer5.setRepeats(1);
  timer5.start();
}

void recheckTresor(MillisTimer &mt) {
  byte a = digitalRead(TRESOR);

  if (a == 0) {
    //Serial.println("Tresor offen interrupt");
    nowtresor = geoeffnet;
  }
  if (a == 1) {
    //Serial.println("Tresor geschlossen interrupt");
    nowtresor = geschlossen;
  }

  if (nowtresor == temptresor) {
    tresorstatus = nowtresor;
  }

}

void getStatus() {
  Serial.println("************************************************************************");
  Serial.println("Gamestatus 0 = armed, 1 = Dead, 2 = Finished");
  Serial.println(gamestatus);
  Serial.println("Tersorstatus 0 = geoffnet, 1 = gschjlossen");
  Serial.println(tresorstatus);
}

// Animationen Sound und Licht
void death() // Passiert wenn Spieler Draht berührt
{
  // Spiel Sound und leuchte Rot dreimal
  timer1.setInterval(750);
  timer1.expiredHandler(StartBlink);
  timer1.setRepeats(3);
  timer1.start();
  //beep(150,1000);
  tone(PIEZO, 150, 750);
}

// TODO Zeiten runtersetzten, Animation beschleunugten

void armTool()  // Passiert wenn Spieler Startbereich berührt
{
  // Spiel Sound und leuchte Gruen dreimal
  timer1.setInterval(750);
  timer1.expiredHandler(StartBlink);
  timer1.setRepeats(3);
  timer1.start();
  timer3.setInterval(3500);
  timer3.expiredHandler(allWhite);
  timer3.setRepeats(3);
  timer3.start();
}

void finishReached()  // Passiert wenn Spieler Ziel erreicht
{
  //Serial.println("Play Finish");
  q = 0;
  timer4.setInterval(100);
  timer4.expiredHandler(partyShow);
  timer4.setRepeats(1000);
  timer4.start();
  timerOde.setInterval(100);
  timerOde.expiredHandler(ode_v2);
  timerOde.setRepeats(1);
  timerOde.start();
  //ode();
}

void tresorOffenAnimation() {
  Adafruit_NeoPixel strip1tresor = Adafruit_NeoPixel(anztresor1, LED_TRESOR1, NEO_GRB + NEO_KHZ800); // LED-Streifen Tresor Nr. 1
  Adafruit_NeoPixel strip2tresor = Adafruit_NeoPixel(anztresor2, LED_TRESOR2, NEO_GRB + NEO_KHZ800); // LED-Streifen Tresor Nr. 2
  strip1tresor.begin();
  strip2tresor.begin();
  // Serial.println("Tresor geoffnet");
  // Schalte Tresor LED auf Gruen
  uint32_t c1 = strip1tresor.Color(0, 0, 0);
  uint32_t c2 = strip2tresor.Color(0, 0, 0);

  /*
    for (uint16_t i = 0; i < strip1tresor.numPixels(); i++) {
    strip1tresor.setPixelColor(i, c1);
    }
    for (uint16_t i = 0; i < strip2tresor.numPixels(); i++) {
    strip2tresor.setPixelColor(i, c2);
    }*/
  strip1tresor.fill(c1, 0, anztresor1);
  strip2tresor.fill(c2, 0, anztresor2),
                    Serial.println("geoeffnet Animation beginnt");

  c1 = strip1tresor.Color(0, 255, 0);
  for (uint16_t i = 0; i < strip1tresor.numPixels(); i++) {
    strip1tresor.setPixelColor(i, c1);
  }
  c2 = strip2tresor.Color(0, 255, 0);
  for (uint16_t i = 0; i < strip2tresor.numPixels(); i++) {
    strip2tresor.setPixelColor(i, c2);
  }
  strip1tresor.show();
  strip2tresor.show();
  Serial.println("geoeffnet Animation beendet");
}

void tresorGeschlossenAnimation() {
  Adafruit_NeoPixel strip1tresor = Adafruit_NeoPixel(anztresor1, LED_TRESOR1, NEO_GRB + NEO_KHZ800); // LED-Streifen Tresor Nr. 1
  Adafruit_NeoPixel strip2tresor = Adafruit_NeoPixel(anztresor2, LED_TRESOR2, NEO_GRB + NEO_KHZ800); // LED-Streifen Tresor Nr. 2
  strip1tresor.begin();
  strip2tresor.begin();
  //Schalte Tresor LED auf WEISS
  //Serial.println("Tresor geschlossen");
  uint32_t c1 = strip1tresor.Color(255, 255, 255);
  uint32_t c2 = strip2tresor.Color(255, 255, 255);
  /*
    for (uint16_t i = 0; i < strip1tresor.numPixels(); i++) {
    strip1tresor.setPixelColor(i, c1);
    }
    for (uint16_t i = 0; i < strip2tresor.numPixels(); i++) {
    strip2tresor.setPixelColor(i, c2);
    }*/
  strip1tresor.fill(c1, 0, anztresor1);
  strip2tresor.fill(c2, 0, anztresor2),
                    strip1tresor.show();
  strip2tresor.show();
  Serial.println("geschlossen Animation beendet");
}


// Schaltet LED von Tresor



// StartBlink zusammen mit UnBlink laesst KatzenStrip 3 mal Rot oder Gruen aufleuchten, 500ms rot/gruen 500 ms aus.

// Schaltet LEDs aus
void StartBlink(MillisTimer &mt) {
  //Serial.println("UN-Blink");
  Adafruit_NeoPixel strip1katze = Adafruit_NeoPixel(anzkatze1, LED_KATZE1, NEO_GRB + NEO_KHZ800); // LED-Streifen Katze Nr. 1
  Adafruit_NeoPixel strip2katze = Adafruit_NeoPixel(anzkatze2, LED_KATZE2, NEO_GRB + NEO_KHZ800); // LED-Streifen Katze Nr. 2
  strip1katze.begin();
  strip2katze.begin();

  uint32_t c1 = strip1katze.Color(0, 0, 0);
  uint32_t c2 = strip2katze.Color(0, 0, 0);
  /*
    for (uint16_t i = 0; i < strip1katze.numPixels(); i++) {
    strip1katze.setPixelColor(i, c1);
    }
    for (uint16_t i = 0; i < strip2katze.numPixels(); i++) {
    }
  */
  strip1katze.clear();
  strip2katze.clear();
  strip1katze.show();
  strip2katze.show();
  timer2.setInterval(250);
  timer2.expiredHandler(Blink);
  timer2.setRepeats(1);
  timer2.start();
}

// Schaltet LEDs rot/gruen
void Blink(MillisTimer &mt) {

  if (gamestatus == dead) {
    Adafruit_NeoPixel strip1katze = Adafruit_NeoPixel(anzkatze1, LED_KATZE1, NEO_GRB + NEO_KHZ800); // LED-Streifen Katze Nr. 1
    Adafruit_NeoPixel strip2katze = Adafruit_NeoPixel(anzkatze2, LED_KATZE2, NEO_GRB + NEO_KHZ800); // LED-Streifen Katze Nr. 2
    strip1katze.begin();
    strip2katze.begin();
    uint32_t c1 = strip1katze.Color(0, 0, 0);
    uint32_t c2 = strip2katze.Color(0, 0, 0);

    Serial.println("REd Blink");
    c1 = strip1katze.Color(255, 0, 0);
    c2 = strip2katze.Color(255, 0, 0);
    /*
      for (uint16_t i = 0; i < strip1katze.numPixels(); i++) {
      strip1katze.setPixelColor(i, c1);
      }
      for (uint16_t i = 0; i < strip2katze.numPixels(); i++) {
      strip2katze.setPixelColor(i, c2);
      }
    */
    strip1katze.fill(c1, 0, anzkatze1);
    strip2katze.fill(c2, 0, anzkatze2),
                     strip1katze.show();
    strip2katze.show();

  }

  if (gamestatus == armed) {
    Adafruit_NeoPixel strip1katze = Adafruit_NeoPixel(anzkatze1, LED_KATZE1, NEO_GRB + NEO_KHZ800); // LED-Streifen Katze Nr. 1
    Adafruit_NeoPixel strip2katze = Adafruit_NeoPixel(anzkatze2, LED_KATZE2, NEO_GRB + NEO_KHZ800); // LED-Streifen Katze Nr. 2
    strip1katze.begin();
    strip2katze.begin();
    uint32_t c1 = strip1katze.Color(0, 0, 0);
    uint32_t c2 = strip2katze.Color(0, 0, 0);

    Serial.println("GREEN Blink");
    c1 = strip1katze.Color(0, 255, 0);
    c2 = strip2katze.Color(0, 255, 0);
    if (l < 1) {
      tone(PIEZO, 300, 500);
    } else if (l == 1) {
      tone(PIEZO, 300, 500);
    } else {
      tone(PIEZO, 450, 750);
    }
    l = l + 1;
    /*
      for (uint16_t i = 0; i < strip1katze.numPixels(); i++) {
      strip1katze.setPixelColor(i, c1);
      }
      for (uint16_t i = 0; i < strip2katze.numPixels(); i++) {
      strip2katze.setPixelColor(i, c2);
      }*/
    strip1katze.fill(c1, 0, anzkatze1);
    strip2katze.fill(c2, 0, anzkatze2),
                     strip1katze.show();
    strip2katze.show();

  }



}


// Laesst alle LED Weissaufleuchten nach 4 Sekunden
void allWhite(MillisTimer &mt) {
  Adafruit_NeoPixel strip1katze = Adafruit_NeoPixel(anzkatze1, LED_KATZE1, NEO_GRB + NEO_KHZ800); // LED-Streifen Katze Nr. 1
  Adafruit_NeoPixel strip2katze = Adafruit_NeoPixel(anzkatze2, LED_KATZE2, NEO_GRB + NEO_KHZ800); // LED-Streifen Katze Nr. 2
  strip1katze.begin();
  strip2katze.begin();
  if (gamestatus == armed) {
    Serial.println("all White");
    uint32_t c1 = strip1katze.Color(255, 255, 255);
    uint32_t c2 = strip2katze.Color(255, 255, 255);
    /*
      for (uint16_t i = 0; i < strip1katze.numPixels(); i++) {
      strip1katze.setPixelColor(i, c1);
      }
      for (uint16_t i = 0; i < strip2katze.numPixels(); i++) {
      strip2katze.setPixelColor(i, c2);
      }*/
    strip1katze.fill(c1, 0, anzkatze1);
    strip2katze.fill(c2, 0, anzkatze2),
                     strip1katze.show();
    strip2katze.show();
  }
}

// Partymodus
void partyShow(MillisTimer &mt) {
  Adafruit_NeoPixel strip1katze = Adafruit_NeoPixel(anzkatze1, LED_KATZE1, NEO_GRB + NEO_KHZ800); // LED-Streifen Katze Nr. 1
  Adafruit_NeoPixel strip2katze = Adafruit_NeoPixel(anzkatze2, LED_KATZE2, NEO_GRB + NEO_KHZ800); // LED-Streifen Katze Nr. 2
  strip1katze.begin();
  strip2katze.begin();
  // Serial.println("PARTYYYY");
  if (gamestatus == finished) {
    j++;
    if (j > 255) {
      j = 0;
    }

    for (int i = 0; i < strip1katze.numPixels(); i++) {
      strip1katze.setPixelColor(i, 0);      //turn every third pixel off
    }
    for (int i = 0; i < strip2katze.numPixels(); i++) {
      strip2katze.setPixelColor(i, 0);      //turn every third pixel off
    }
    for (int i = 0; i < strip1katze.numPixels(); i = i + 5) {
      strip1katze.setPixelColor(i + q, Wheel( (i + j) % 255, strip1katze)); //turn every third pixel on
    }
    for (int i = 0; i < strip2katze.numPixels(); i = i + 5) {
      strip2katze.setPixelColor(i + q, Wheel( (i + j) % 255, strip2katze)); //turn every third pixel on
    }
    strip1katze.show();
    strip2katze.show();
    q++;
    if (q > 2) {
      q = 0;
    }
    //Serial.println(q);
  }
}
uint32_t Wheel(byte WheelPos, Adafruit_NeoPixel &strip) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void beep(int note, int duration)
{
  //Play tone on buzzerPin
  tone(PIEZO, note, duration);

  //Play different LED depending on value of 'counter'

  //delay(duration);

}

void ode() {
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(PIEZO, melody[thisNote], noteDuration * 0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(PIEZO);
  }
}

void ode_v2(MillisTimer &mt) {
  noTone(PIEZO);
  // calculates the duration of each note
  divider = melody[m + 1];
  if (divider > 0) {
    // regular note, just proceed
    noteDuration = (wholenote) / divider;
  } else if (divider < 0) {
    // dotted notes are represented with negative durations!!
    noteDuration = (wholenote) / abs(divider);
    noteDuration *= 1.5; // increases the duration in half for dotted notes
  }

  // we only play the note for 90% of the duration, leaving 10% as a pause
  tone(PIEZO, melody[m], noteDuration * 0.9);
  if (m < 60) {
    timerOde.setInterval(noteDuration * 1.1);
    timerOde.expiredHandler(ode_v2);
    timerOde.setRepeats(1);
    timerOde.start();
  } else {
    noTone(PIEZO);
  }
  m = m + 2;
}
