// ACHTUNG:
// wird ein Attiny85 Digispark (China Teil) eingesetzt, dann

// die türkise Leitung statt auf Masse, auf VCC umlegen

//    if(digitalRead(changecountdown)==0) {  // Countdown einstellen - Knopf gedrückt
//    if(digitalRead(startcountdown)==0) { // Countdown starten - Knopf gedrückt
// auf
//   if(digitalRead(changecountdown)==HIGH) {  // Countdown einstellen - Knopf gedrückt
//    if(digitalRead(startcountdown)==HIGH) { // Countdown starten - Knopf gedrückt
// ändern

#include <TinyWireM.h>
#include <Tiny4kOLED.h>

#include<EEPROM.h>

uint8_t width = 64;
uint8_t height = 32;


int btn_rt          = 1; // START / STOPP / Plus / Buzzer Output
int btn_bl          = 4; // Timer / Minus
int btn_set         = 3; // SET

int akt_timer       = 0;
int hrs             = 0;
int Min             = 0;
int sek             = 0;
int piep1           = 0;
int piep2           = 0;

int piep1cnt = 0;
int piep2cnt = 0;

unsigned int check_val = 50;

int add_chk         = 0;    // EEPROM Speicherstelle
int add_akt_timer   = 1;
int add_hrs         = 2;
int add_min         = 3;
int add_sek         = 4;
int add_piep1       = 5;
int add_piep2       = 6;

bool RUN              = true;

bool set_timer_flag   = true;
bool timer_flag       = true;
bool sek_flag         = true;
bool min_flag         = true;
bool hrs_flag         = true;
bool piep1_flag       = true;
bool piep2_flag       = true;

int val;
int bnt_set_val = 0;


void INIT()
{
  //Serial.println("INIT()");
  akt_timer = EEPROM.read(add_akt_timer);
  hrs = EEPROM.read(akt_timer * 5);
  Min = EEPROM.read(akt_timer * 5 + 1);
  sek = EEPROM.read(akt_timer * 5 + 2);
  piep1 = EEPROM.read(akt_timer * 5 + 3);
  piep2 = EEPROM.read(akt_timer * 5 + 4);

  header();
  oled.setCursor(40, 10);
  if (hrs <= 9)
  {
    oled.print('0');
  }
  oled.print(hrs);
  oled.print(':');
  if (Min <= 9)
  {
    oled.print('0');
  }
  oled.print(Min);
  oled.print(':');
  if (sek <= 9)
  {
    oled.print('0');
  }
  oled.print(sek);

  oled.setCursor(0, 30);
  oled.print("Piep: ");
  oled.print(piep1);
  oled.print("/");
  oled.print(piep2);

  piep1cnt = 0;
  piep2cnt = 0;

  set_timer_flag = false;
  timer_flag = true;
  sek_flag = true;
  min_flag = true;
  hrs_flag = true;
  piep1_flag = true;
  piep2_flag = true;
}



void setup() {
  oled.begin(128, 64, sizeof(tiny4koled_init_128x64), tiny4koled_init_128x64); //oled initialization
  oled.setRotation(1);     // The default orientation is not the most commonly used.
  oled.clear();
  oled.on();
  oled.setFont(FONT8X16P);


  pinMode(btn_rt, INPUT_PULLUP);
  pinMode(btn_bl, INPUT_PULLUP);
  pinMode(btn_set, INPUT_PULLUP);
  //pinMode(buzz, OUTPUT);
  //digitalWrite(buzz, LOW);

  if (EEPROM.read(add_chk) != check_val)
  {
    EEPROM.write(add_chk, check_val);
    EEPROM.write(add_akt_timer, 1);
    EEPROM.write(5, 0); // hrs
    EEPROM.write(6, 3); // Min
    EEPROM.write(7, 30); // sek
    EEPROM.write(8, 60);// piep1 max bis 254 da Integer
    EEPROM.write(9, 20);// piep2 max bis 254 da Integer

    EEPROM.write(10, 1); // hrs
    EEPROM.write(11, 0); // Min
    EEPROM.write(12, 0); // sek
    EEPROM.write(13, 60);// piep1 max bis 254 da Integer
    EEPROM.write(14, 30);// piep2 max bis 254 da Integer

    EEPROM.write(15, 1); // hrs
    EEPROM.write(16, 0); // Min
    EEPROM.write(17, 0); // sek
    EEPROM.write(18, 60);// piep1 max bis 254 da Integer
    EEPROM.write(19, 0);// piep2 max bis 254 da Integer

  }


  else
  {
    akt_timer = EEPROM.read   (add_akt_timer);
    hrs       = EEPROM.read   (add_akt_timer * 5);
    Min       = EEPROM.read   (add_akt_timer * 5 + 1);
    sek       = EEPROM.read   (add_akt_timer * 5 + 2);
    piep1     = EEPROM.read   (add_akt_timer * 5 + 3);
    piep2     = EEPROM.read   (add_akt_timer * 5 + 4);
  }
  //delay(1500);
  INIT();

}

void header() {
  oled.setFont(FONT8X16P);
  oled.setCursor(10, 0); // x, y
  oled.print("TIMER:");
  oled.setCursor(60, 0);
  oled.print(akt_timer);
  oled.setCursor(5, 30);
}

void loop() {
  if ( (digitalRead(btn_rt) == HIGH)  )
  {
    //oled.clear();
    delay(250);
    RUN = true;

    while (RUN)
    {
      if ( (digitalRead(btn_rt) == HIGH)  )
      {
        delay(1000);
        header();
        if ( (digitalRead(btn_rt) == HIGH)  )
        {
          oled.clear();
          header();
          oled.setCursor(20, 20);
          oled.print("TIMER STOP");
          delay(2000);
          RUN = false;
          oled.clear();
          INIT();
          break;
        }
      }       // wenn nichts gedrückt dann weiter mit herunterzählen


      // COUNTDOWN ------------------------------
      if (piep2cnt == 0) {
        sek = sek - 1; // es sollen nur die trainierten Sekunden herunterzählen
      }
      delay(1000);  // hier ist die Verzögerung
      if (sek == -1)
      {
        sek = 59;
        Min = Min - 1;
      }
      if (Min == -1)
      {
        Min = 59;
        hrs = hrs - 1;
      }
      if (hrs == -1) hrs = 0;
      // ----------------------------- COUNTDOWN


      if (piep1 > 0 && piep1cnt < piep1)
      {
        piep1cnt++;
        if (piep1 == piep1cnt)
        {
          pinMode(btn_rt, OUTPUT);
          digitalWrite(btn_rt, HIGH);
          delay(400);
          digitalWrite(btn_rt, LOW);
          delay(40);
          pinMode(btn_rt, INPUT_PULLUP);
        }
      }

      if (piep2 > 0 && piep1 == piep1cnt)
      {
        piep2cnt++;
        if (piep2 == piep2cnt)
        {
          pinMode(btn_rt, OUTPUT);
          digitalWrite(btn_rt, HIGH);
          delay(200);
          digitalWrite(btn_rt, LOW);
          delay(300);
          digitalWrite(btn_rt, HIGH);
          delay(200);
          digitalWrite(btn_rt, LOW);
          delay(300);
          piep1cnt = 0;
          piep2cnt = 0;
          pinMode(btn_rt, INPUT_PULLUP);

        }
      } else {
        if (piep1 == piep1cnt) piep1cnt = 0;
      }

      // Timer amoled ausgeben -----------------------------
      //oled.clear();
      header();

      oled.setCursor(40, 10);
      if (hrs <= 9)
      {
        oled.print('0');
      }
      oled.print(hrs);
      oled.print(':');
      if (Min <= 9)
      {
        oled.print('0');
      }
      oled.print(Min);
      oled.print(':');
      if (sek <= 9)
      {
        oled.print('0');
      }
      oled.print(sek);

      oled.setCursor(0, 30);
      oled.print("Piep:           ");
      oled.setCursor(40, 30);
      oled.print(piep1 - piep1cnt);
      oled.print("/");
      oled.print(piep2 - piep2cnt);

      // ----------------------------------Timer amoled ausgeben


      if (hrs == 0 && Min == 0 && sek == 0) // Wenn alles heruntergezählt wurde, dann piepsen
      {
        //oled.setCursor(20, 30);
        RUN = false; // While Schleife beenden

        // piepsen
        for (int i = 0; i < 5; i++)
        {
          pinMode(btn_rt, OUTPUT);
          digitalWrite(btn_rt, HIGH);
          delay(250);
          digitalWrite(btn_rt, LOW);
          delay(20);
          pinMode(btn_rt, INPUT_PULLUP);
        }
      oled.clear();
        INIT(); // auf Startzustand zurückkehren
        break;
      }
      //oled.clear();
    } // ENDE While(RUN)
  } // ENDE if ( (digitalRead(btn_rt) == LOW) && (digitalRead(btn_bl) == HIGH) )

  // #################################################################################
  // #################################################################################
  // #################################################################################


  if  (digitalRead(btn_bl) == HIGH ) // blauer Knopf - anderen Timer einstellen
  {
    delay(1000);
    if  (digitalRead(btn_bl) == HIGH)
    {
      akt_timer = akt_timer + 1;
      if (akt_timer >= 4) akt_timer = 1;
      delay(100);
      EEPROM.write(add_akt_timer, akt_timer);
      delay(500);
      INIT();
    }
  }

  // *************************************** Grüner Knopf - SET **************************************

  //pinMode(btn_set, INPUT_PULLUP);
  if  (digitalRead(btn_set) == LOW) // grünen SET Knopf gedrückt
  {
    akt_timer = EEPROM.read(add_akt_timer);
    delay(500);
    oled.clear();

    while (sek_flag) // ------------------------------------------------ Sekunden setzen --------------------------------
    {
      // Timer amoled ausgeben -----------------------------
      header();
      oled.print("Sek:        ");
      oled.setCursor(40, 30);
      if (sek <= 9)
      {
        oled.print('0');
      }
      oled.print(sek);

      delay(100);
      if (digitalRead(btn_rt) == HIGH)
      {
        sek = sek + 1;
        if (sek >= 60) sek = 1;
        delay(100);
      }
      if (digitalRead(btn_bl) == HIGH)
      {
        sek = sek - 1;
        if (sek <= -1) sek = 60;
        delay(100);
      }
      if  (digitalRead(btn_set) == LOW)
      {
        sek_flag = false;
        delay(250);
      }
      //oled.clear();
    }






    while (min_flag)  // ------------------------------------------------ Minuten setzen --------------------------------
    {
      // Timer amoled ausgeben -----------------------------
      header();
      oled.print("Min:       ");
      oled.setCursor(40, 30);
      if (Min <= 9)
      {
        oled.print('0');
      }
      oled.print(Min);

      delay(100);
      if (digitalRead(btn_rt) == HIGH)
      {
        Min = Min + 1;
        if (Min >= 60) Min = 0;
        delay(100);
      }
      if (digitalRead(btn_bl) == HIGH)
      {
        Min = Min - 1;
        if (Min <= -1) Min = 60;
        delay(100);
      }
      if  (digitalRead(btn_set) == LOW)
      {
        min_flag = false;
        delay(250);
      }
      //    oled.clear();
    }





    while (hrs_flag) // ------------------------------------------------ Stunden setzen --------------------------------
    {
      // Timer amoled ausgeben -----------------------------
      header();
      oled.print("Std:       ");
      oled.setCursor(40, 30);
      if (hrs <= 9)
      {
        oled.print('0');
      }
      oled.print(hrs);

      delay(100);
      if (digitalRead(btn_rt) == HIGH)
      {
        hrs = hrs + 1;
        if (hrs > 1000) hrs = 0;//edit max jam
        delay(100);
      }
      if (digitalRead(btn_bl) == HIGH)
      {
        hrs = hrs - 1;
        if (hrs <= -1) hrs = 0;
        delay(100);
      }
      if  (digitalRead(btn_set) == LOW)
      {
        hrs_flag = false;
        delay(250);
      }
      // oled.clear();
    }




    while (piep1_flag) // ------------------------------------------------ Piiep1 setzen --------------------------------
    {
      // Timer amoled ausgeben -----------------------------
      header();
      oled.print("Piep1:       ");
      oled.setCursor(50, 30);
      oled.print(piep1);

      delay(100);
      if (digitalRead(btn_rt) == HIGH)
      {
        piep1 = piep1 + 1;
        if (piep1 >= 255) piep1 = 0;
        delay(100);
      }
      if (digitalRead(btn_bl) == HIGH)
      {
        piep1 = piep1 - 1;
        if (piep1 <= -1) piep1 = 254;
        delay(100);
      }
      if  (digitalRead(btn_set) == LOW)
      {
        piep1_flag = false;
        delay(250);
      }
      //   oled.clear();
    }



    while (piep2_flag) // ------------------------------------------------ Piep2 setzen --------------------------------
    {
      // Timer amoled ausgeben -----------------------------
      header();
      oled.print("Piep2:       ");
      oled.setCursor(50, 30);
      oled.print(piep2);

      delay(100);
      if (digitalRead(btn_rt) == HIGH)
      {
        piep2 = piep2 + 1;
        if (piep2 >= 255) piep2 = 0;
        delay(100);
      }
      if (digitalRead(btn_bl) == HIGH)
      {
        piep2 = piep2 - 1;
        if (piep2 <= -1) piep2 = 254;
        delay(100);
      }
      if  (digitalRead(btn_set) == LOW)
      {
        piep2_flag = false;
        delay(250);
      }
      //    oled.clear();
    }




    if (hrs == 0 && Min == 0 && sek == 0)
    {
      // Timer amoled ausgeben -----------------------------
      oled.clear();
      header();
      oled.setCursor(15, 20);
      oled.print("falsche");
      oled.setCursor(15, 30);
      oled.print("Eingabe");
      // oled.clear();
      delay(2000);
    }
    else
    {
      //EEPROM.write(add_akt_timer, akt_timer);
      EEPROM.write(akt_timer * 5, hrs); // hrs
      EEPROM.write(akt_timer * 5 + 1, Min); // Min
      EEPROM.write(akt_timer * 5 + 2, sek); // sek
      EEPROM.write(akt_timer * 5 + 3, piep1);// piep1 max bis 254 da Integer
      EEPROM.write(akt_timer * 5 + 4, piep2);// piep2 max bis 254 da Integer
      delay(500);
    }
    oled.clear();
    INIT();
  }
}
