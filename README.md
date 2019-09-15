# DS1302
A Particle library for the DS1302 Real-time Clock Module

## Description
This library is for accessing the DS1302 Real-time clock module.  The specific model used is VMA301. This should not be used for high precision time keeping.  The DS1302 is only one second resolution. This does not use the Particles software timers, but can be used to update the software timers if you choose.

## Tested Boards
Particle-Argon
  
## Usage
<code>
/*  Create an instance of DS1302  */
DS1302 RTC = DS1302(A2, A3, A4);

/*  Create a DateTime variable  */
DS1302::DateTime dateTime;

<b>Setup()</b><br />
/*
    The DS1302 has a battery backup.  The DateTime does not
    need to be set everytime.

    NOTE: The DS1302 does not handle DST automaticly.

    The weekday increments at midnight. Values that correspond
    to the day of week are user-defined but must be sequential
    (i.e., if 1 equals Sunday, then 2 equals Monday, and so on.).
    Illogical time and date entries result in undefined operation.

    The example usage below is setting the DateTime to Saturday, 9/14/2019 9:29:00
    1=Sunday, 2=Monday, 3=Tuesday, 4=Wednesday, 5=Thursday, 6=Friday, 7=Saturday  
*/

/*  Set the date, time and weekday  */
dateTime = DS1302::DateTime(19, 9, 14, 21, 29, 0, 7);
RTC.WriteDateTime(dateTime);
}

loop()
{
    /*  Get the current date, time and weekday  */
    dateTime = RTC.ReadDateTime();

    Log.info("Date: " + String(dateTime.month) + "/" + String(dateTime.day) + "/20" + String(dateTime.year));
    Log.info("Time: " + String(dateTime.hours) + ":" + String(dateTime.minutes) + ":" + String(dateTime.seconds));

    Log.print("Weekday: " + String(dateTime.weekday) + "\n");
    Log.print("Seconds: " + String(RTC.SecondsSinceMidnight()) + "\n");
    Log.print("Ram Read: " + String(RTC.ReadRam(31)) + "\n");
}
</code>

## Documentation
See the examples/usage folder for details.

## LICENSE
Copyright 2019 Frank James

Licensed under the MIT license.  See the license file for details.
