/*  Example usage for DS1302 RTC Module library by Frank James  */

#include <DS1302.h>

SerialLogHandler logHandler(LOG_LEVEL_INFO);

/*
  ==================================
    DS1302 RTC Configuration
  ==================================
*/
int clockPin = A2;
int dataPin = A3;
int enablePin = A4;

DS1302 rtc = DS1302(clockPin, dataPin, enablePin);
DS1302::DateTime dateTime;

void setup()
{
  int year = 19;
  int month = 9;
  int day = 14;
  int hour = 17; // Must be in 24 hour format
  int minutes = 15;
  int seconds = 0;
  int weekday = 7;

  /*  Set the RTC  */
  dateTime = DS1302::DateTime(year, month, day, hour, minutes, seconds, weekday);
  rtc.WriteDateTime(dateTime);

  /*
      Write a byte to Ram
      The DS1302 has 32 bytes of battery backed up
      Ram for use as temporary storage.  See ReadRam
      in the Loop method below on how to read from Ram.
  */
  int byte = 0;       // 0-31
  int byteValue = 20; // 0-255
  rtc.WriteRam(byte, byteValue);
}

void loop()
{
  /*  Read the date and time  */
  dateTime = rtc.ReadDateTime();

  /*  Display the RTC date and time  */
  Log.info("Date: " + String(dateTime.month) + "/" + String(dateTime.day) + "/20" + String(dateTime.year));
  Log.info("Time: " + String(dateTime.hours) + ":" + String(dateTime.minutes) + ":" + String(dateTime.seconds));

  /*  Display the number of seconds since midnight  */
  Log.print("Seconds: " + String(rtc.SecondsSinceMidnight()) + "\n");

  /*
      Read a byte to Ram
      The DS1302 has 32 bytes of battery backed up
      Ram for use as temporary storage.  See WriteRam
      in the Setup method above on how to write to Ram. 
  */
  int byte = 0; // 0-31
  Log.print("Ram Read: " + String(rtc.ReadRam(byte)) + "\n");
  delay(1000);
}
