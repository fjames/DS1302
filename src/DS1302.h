#ifndef __DS1302_CLIENT_H_
#define __DS1302_CLIENT_H_

/* 
    Name:   DS1302 RTC library
    Author: Frank James
*/

#include "application.h"

/*
    This is your main class that users will import into their application
*/
class DS1302
{
public:
  class DateTime
  {
  public:
    int year = 0;
    int month = 0;
    int day = 0;
    int hours = 0;
    int minutes = 0;
    int seconds = 0;
    int weekday = 1;

    /*  Constructors  */
    DateTime(){};
    DateTime(int _year, int _month, int _day, int _hours, int _minutes, int _seconds, int _weekday)
    {
      year = _year;
      month = _month;
      day = _day;
      hours = _hours;
      minutes = _minutes;
      seconds = _seconds;
      weekday = _weekday;
    }
  };

  int _clockPin;
  int _dataPin;
  int _enablePin;

  DS1302(int clockPin, int dataPin, int enablePin);
  void Initialize();
  DateTime ReadDateTime();
  void WriteDateTime(DateTime dateTime);
  int ReadRam(int byte);
  void WriteRam(int byte, int value);
  int SecondsSinceMidnight();

private:
  void SendCommand(int command);
  int ReadByte(int command);
  void WriteByte(int command, int data);
};

#endif /* __DS1302_CLIENT_H_ */