#ifndef RTC_H
#define RTC_H

#define byte unsigned char

void setDS3231time(byte minute, byte hour);

void readDS3231time(byte *minute, byte *hour);

#endif
