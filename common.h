#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#ifdef __linux__
  #include <curses.h>
#else
  #include <conio.h>
  #include <windows.h>
#endif


#define MAXTIMEZONES 10   // Maximum number of timezone to show in main interface
#define TIMEZONEFILE "timezones.txt"

enum calendar_system { AD_DATE, BS_DATE };

// Structure to store data necessary to show calendar of a month
struct monthData {   
  int numDays;   // Number of days in the month
  int firstDay;  // First Day of the month (0 for Sun, 1 for Mon, ...)
};

// Sturcture to store current time of a timezone along with
// its time and it's difference  from UTC
struct time{
  int hr;
  int min;
  int sec;
  int utcdiff;
  char name[20];
};

// Structure to hold info about a date
struct date{
  int year;
  int month;    // Month (1 for Jan, 2 for Feb, ... )
  int day;
  enum calendar_system type;
};


//// Function Declarations

// ADandBS.c
int isLeapYear(int y);
int validDate(struct date* d);
int daysSinceADEpoch(struct date *d);
int daysSinceBSEpoch(struct date *d);
struct date nthDayOfBSEpoch(int n);
struct date nthDayOfADEpoch(int n);
struct date convertADToBS(struct date *ad);
struct date convertBSToAD(struct date *bs);
struct monthData calculateMonthDataBS(struct date *d);
struct monthData calculateMonthDataAD(struct date *d);
struct monthData calculateMonthData(struct date *d);

// time.c
struct date getCurrentDate();
struct time getCurrentTime();
void saveTimezones(struct time tz[], int count);
int readTimezones(struct time tz[]);
void updateTime(struct time tz[], int count);

// main.c
void clrscr();
void printTabs(int n);
void printTime(struct time* time);
void displayCalendarAndTime(struct date *d);
char displayMainMenu(struct date *d);
int displayTimezoneMenu();
