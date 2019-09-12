/* 
    Name:   DS1302 RTC library
    Author: Frank James
*/

#include "DS1302.h"

/*  Constructor  */
DS1302::DS1302(int clockPin, int dataPin, int enablePin)
{
    _clockPin = clockPin;
    _dataPin = dataPin;
    _enablePin = enablePin;
};

/*  Send DS1302 RTC Command Byte  */
void DS1302::SendCommand(int command)
{
    /*  Get clock ready to receive a command  */
    Initialize();
    digitalWrite(_enablePin, HIGH);

    for (int i = 0; i < 8; i++)
    {
        digitalWrite(_dataPin, ((command >> i) & 0x01));
        digitalWrite(_clockPin, HIGH);
        digitalWrite(_clockPin, LOW);
    }
}

/*  Initialize DS1302 RTC  */
void DS1302::Initialize()
{
    pinMode(_clockPin, OUTPUT);
    pinMode(_dataPin, OUTPUT); // Data pin will switch between input and output. Start with it set to output.
    pinMode(_enablePin, OUTPUT);

    digitalWrite(_clockPin, LOW);
    digitalWrite(_dataPin, LOW);
    digitalWrite(_enablePin, LOW);
}

/*  Get DS1302 RTC Data  */
DS1302::DateTime DS1302::ReadDateTime()
{
    DateTime dateTime;

    int commandReadHours = 0x85;
    int commandReadMinutes = 0x83;
    int commandReadSeconds = 0x81;
    int commandReadYear = 0x8d;
    int commandReadMonth = 0x89;
    int commandReadDay = 0x87;

    dateTime.hours = ReadByte(commandReadHours);
    dateTime.minutes = ReadByte(commandReadMinutes);
    dateTime.seconds = ReadByte(commandReadSeconds);
    dateTime.year = ReadByte(commandReadYear);
    dateTime.month = ReadByte(commandReadMonth);
    dateTime.day = ReadByte(commandReadDay);

    /*  Disable clock access  */
    digitalWrite(_enablePin, LOW);

    return dateTime;
}

int DS1302::ReadByte(int command)
{
    /*  Send commnad byte  */
    SendCommand(command);

    /*  Toggle data pin to input mode  */
    pinMode(_dataPin, INPUT);

    int byte = 0;

    for (int i = 0; i < 8; i++)
    {
        byte = ((digitalRead(_dataPin) << i) | byte);
        digitalWrite(_clockPin, HIGH);
        digitalWrite(_clockPin, LOW);
    }

    /*  Convert from BCD to integer  */
    return (((byte >> 4) * 10) + (byte & 0x0f));
}

void DS1302::WriteByte(int command, int data)
{
    /*  Convert from int to BCD  */
    int highNibble = 0;
    int lowNibble = 0;

    lowNibble = (data % 10);
    highNibble = ((data - lowNibble) / 10);
    data = (highNibble << 4) | lowNibble;

    /*  Send commnad byte  */
    SendCommand(command);

    /*  Toggle data pin to output mode  */
    pinMode(_dataPin, OUTPUT);

    for (int i = 0; i < 8; i++)
    {
        digitalWrite(_dataPin, ((data >> i) & 0x01));
        digitalWrite(_clockPin, HIGH);
        digitalWrite(_clockPin, LOW);
    }
}

/*  Set RTC Data  */
/*  NOTE: hours must be in 24 hour format  */
void DS1302::WriteDateTime(DateTime dateTime)
{
    int commandWriteHours = 0x84;
    int commandWriteMinutes = 0x82;
    int commandWriteSeconds = 0x80;
    int commandWriteYear = 0x8c;
    int commandWriteMonth = 0x88;
    int commandWriteDay = 0x86;

    dateTime.hours = (dateTime.hours & 0x1f);     /*  Ensure the clock is in 24 hour mode  */
    dateTime.seconds = (dateTime.seconds & 0x7f); /*  Ensure clock hold is always disabled  */

    WriteByte(commandWriteHours, dateTime.hours);
    WriteByte(commandWriteMinutes, dateTime.minutes);
    WriteByte(commandWriteSeconds, dateTime.seconds);
    WriteByte(commandWriteYear, dateTime.year);
    WriteByte(commandWriteMonth, dateTime.month);
    WriteByte(commandWriteDay, dateTime.day);

    /*  Disable clock access  */
    digitalWrite(_enablePin, LOW);
}

int DS1302::SecondsSinceMidnight()
{
    DateTime dateTime = ReadDateTime();
    return ((dateTime.hours * 360) + (dateTime.minutes * 60) + (dateTime.seconds));
}

void DS1302::WriteRam(int byte, int value)
{
    int commandWriteRam = 0xc0 + (byte * 2);
    WriteByte(commandWriteRam, value);

    /*  Disable clock access  */
    digitalWrite(_enablePin, LOW);
}

int DS1302::ReadRam(int byte)
{
    int commandReadRam = 0xc1 + (byte * 2);
    int ramByte = ReadByte(commandReadRam);

    /*  Disable clock access  */
    digitalWrite(_enablePin, LOW);

    return ramByte;
}