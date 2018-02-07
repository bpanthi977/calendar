#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#ifdef __linux__
  #include <curses.h>
#else
  #include <conio.h>
#endif


#define MAXTIMEZONES 10   // Maximum number of timezone to show in main interface
#define TIMEZONEFILE "timezones.txt"
#define AD_DATE 0
#define BS_DATE 1

// Number of days in each month of English calendar (for non-leap year)
int daysInMonth[12];

// Information about number of days in every month since 2000 BS to 2090 BS
const dataBS[91][13];

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
  int type; // AD = 0 , BS = 1
};


//// Function Declarations

// ADandBS.c
int isLeapYear(int y);
int validDate(struct date* d);
int daysSinceADEpoch(struct date *d);
int daysSinceBSEpoch(struct date *d);
struct date nthDayOfBSEpoch(int n);
struct date nthDayOfADEpoch(int n);
struct date convertADToBS(struct date ad);
struct date convertBSToAD(struct date bs);
struct monthData calculateMonthDataBS(struct date d);
struct monthData calculateMonthDataAD(struct date d);
struct monthData calculateMonthData(struct date *d);

// time.c
struct date getCurrentDate();
struct time getCurrentTime();
void saveTimezones(struct time tz[], int count);
int readTimezones(struct time tz[]);
void calculateTime(struct time tz[], int count);

// main.c
#ifdef __linux__
void clrscr();
#endif
void printTabs(int n);
void printTime(struct time* time);
void display(struct date* d, struct time timezones[], int count);
int timezoneMenu(struct time tz[], int count);
