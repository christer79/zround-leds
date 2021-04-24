#include <Arduino.h>
#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>
#include <FastLED.h>

#define NUM_LEDS 144
#define DATA_PIN 7

//These define's must be placed at the beginning before #include "TimerInterrupt.h"
//#define TIMER_INTERRUPT_DEBUG 0
//
#define USE_TIMER_1 true
#define USE_TIMER_2 true
#define USE_TIMER_3 false
#define USE_TIMER_4 false
#define USE_TIMER_5 false
#define TIMER1_INTERVAL_MS 1000L

#include "TimerInterrupt.h"

//LiquidCrystal_I2C lcd(0x3F, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

CRGB leds[NUM_LEDS];


hd44780_I2Cexp lcd;
String serial_in;

// Two timers Timer1 and Timer1 are available for UNO boards

float best_ever_lap = 1111.0;
float drivers_best[40];
float drivers_last[40];
int number_of_racers = 0;

char lcd_row_1[40], lcd_row_2[40];
int lcd_row_1_pos, lcd_row_2_pos;

void TimerHandler1()
{
  Serial.println("Timer triggered");
}

void setup()
{
  
  FastLED.addLeds<WS2811, DATA_PIN>(leds, NUM_LEDS);
  
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("Started....\n");

  //ITimer2.init();
  //
  //// Interval in unsigned long millisecs
  //if (ITimer2.attachInterruptInterval(TIMER1_INTERVAL_MS, TimerHandler1))
  //  Serial.println("Starting  ITimer1 OK, millis() = " + String(millis()));
  //else
  //  Serial.println("Can't set ITimer1. Select another freq. or timer");

 // lcd.init(); // initialize the lcd
 // lcd.backlight();
 // lcd.setCursor(1, 0);
 // lcd.print("Hello, world!");
  
  FastLED.clear();
}

void loop()
{

  leds[90] = CRGB::Blue; 

  leds[91] = CRGB::Yellow;

  leds[92] = CRGB::Blue;
  
  leds[93] = CRGB::Green; 

  FastLED.show(); 

  // put your main code here, to run repeatedly:
  if (true)
  {
    if (Serial.available())
    {
      serial_in = Serial.readStringUntil('#');
      Serial.print("Recived: ");
      Serial.println(serial_in);

      String str = serial_in;
      str.replace(";", " ");

      char to_lcd[100];
      str.toCharArray(to_lcd, 100);

      int racer_number, position, laps;
      char nick[15], lap_time[15], best_lap[15];

      char time[15];
      int total_read = sscanf(to_lcd, "$*****%[0-9.]#", time);
      if (total_read == 1)
      {
        char result[20];
        strcpy(result, "Time:");
        strcat(result, time );
        Serial.println(result);
        return;
      }
      total_read = sscanf(to_lcd, "$LAP %d %d %d %[A-Za-z ] %[0-9:,]#", &racer_number, &position, &laps, nick, lap_time, best_lap);
      Serial.println("Total: " + String(total_read));
      if (total_read == 5)
      {
        Serial.println(String(total_read));
        Serial.println(String(racer_number));

        Serial.println(String(nick));

        Serial.println(String(lap_time));
        String lap_time_string = String(lap_time);

        double lap = 88888.0000;
        Serial.println(lap_time_string.substring(2, 3));
        if (lap_time_string.substring(2, 3) == ":")
        {
          String minutes_string = lap_time_string.substring(0, 2);
          Serial.println("minutes" + minutes_string);
          double minutes = minutes_string.toFloat();
          String sec_string = lap_time_string.substring(3);
          sec_string.replace(",", ".");
          Serial.println("seconds" + sec_string);

          lap = sec_string.toFloat() + minutes * 60.0;
        }
        else
        {
          lap_time_string.replace(",", ".");
          lap = String(lap_time_string).toFloat();
        }
        //Serial.println(String(best_lap));

        //        double lap = String(lap_time).toFloat();
        Serial.print("Lap: ");
        Serial.println(lap);
        if (lap < best_ever_lap)
        {
          Serial.println("Best lap ever so far");
          best_ever_lap = lap;
          //lcd.setCursor(0, 1);
          //lcd.print("Best: " + String(best_ever_lap) + " -  " + String(nick));
        }

        if ((drivers_best[racer_number] == 0) | (drivers_best[racer_number] > lap))
        {
          Serial.println("Racers best");
          drivers_best[racer_number] = lap;
        }

        if (drivers_last[racer_number] != lap)
        {
          drivers_last[racer_number] = lap;
          String str = String(nick) + ": " + String(lap) + "(" + String(drivers_best[racer_number]) + ")";
          //lcd.setCursor(0, 0);
          //lcd.print(str.substring(0, 16));
        }

        if (racer_number > number_of_racers)
        {
          number_of_racers = racer_number;
        }

        str = String(number_of_racers) + " " + String(drivers_best[1]) + " " + String(drivers_best[2]) + " " + String(drivers_best[3]);
      }
      else
      {
        //char lcd_row_2[20];
        //lcd.setCursor(0, 0);
        //lcd.print(serial_in.substring(0, 16));
        //lcd.setCursor(0, 1);
        //lcd.print(serial_in.substring(16, 32) + "                         ");
      }
    }
  }
}