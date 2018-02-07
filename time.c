#include "common.h"

struct date getCurrentDate(){
  // Gets current Month and year from system
  // using function of time.h
  time_t now;
  struct tm * timeinfo;
  struct date d;
  time(&now);
  timeinfo = localtime(&now);
  d.year = timeinfo->tm_year + 1900;
  d.month = timeinfo->tm_mon + 1 ;
  d.day = timeinfo->tm_mday;
  d.type = AD_DATE;
  return d;
}

struct time getCurrentTime(){
  // Gets current time from system and returns it
  time_t now;
  struct tm* timeinfo;
  struct time t;
  time(&now);
  timeinfo = localtime(&now);
  t.sec = timeinfo->tm_sec;
  t.min = timeinfo->tm_min;
  t.hr = timeinfo->tm_hour;
  t.utcdiff = (int) timeinfo->tm_gmtoff / 60 ;
  strcpy(t.name, "LOCALTIME");
  return t;
}

void saveTimezones(struct time tz[], int count){
  // Writes timezone information to TIMEZONEFILE
  FILE *f = fopen(TIMEZONEFILE, "w");
  int i;
  for (i=1; i<count; i++){
    fprintf(f, "%s %d\n", tz[i].name, tz[i].utcdiff);
  }
  fflush(f);
  fclose(f);
}

int readTimezones(struct time tz[]){
  // Reads data about timezone from TIMEZONEFILE , updates tz and
  // Returns new number of timezones
  // (local time also counted so)
  FILE *f = fopen(TIMEZONEFILE, "r");
  int i, diff;
  char name[20];

  strcpy(tz[0].name, "LOCALTIME");
  if (f == NULL){
    return 1;
  }
  for (i=1; i<MAXTIMEZONES; i++){
    if (fscanf(f, "%s %d", &name, &diff) == EOF)
      break;
    
    strcpy(tz[i].name,name);
    tz[i].utcdiff = diff;     
  }  
  return i; // New number of timezones
}

void calculateTime(struct time tz[], int count){
  // Calculates time in each time zone according to current time and
  // the timezone time difference
  int t1 = tz[0].utcdiff;
  for (int i=1; i<count; i++){
    tz[i].sec = tz[0].sec;
    tz[i].min = tz[0].min + (tz[i].utcdiff - t1);
    tz[i].hr = tz[0].hr;
    if (tz[i].min > 59 || tz[i].min < 0){
      tz[i].hr += tz[i].min / 60;
      tz[i].min = abs(tz[i].min) % 60;
    }
    if (tz[i].hr > 23 || tz[i].hr < 0){
      tz[i].hr = abs(tz[i].hr) % 24;
    }
  }
}
