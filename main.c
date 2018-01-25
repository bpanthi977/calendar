#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <curses.h>

#define MAXTIMEZONES 10
#define TIMEZONEFILE "timezones.txt"

struct month {
  int year;
  int month;     // Month (1 for Jan, 2 for Feb, ... )
  int numDays;   // Number of days in the month
  int firstDay;  // First Day of the month (0 for Sun, 1 for Mon, ...)
};

struct time{
  int hr;
  int min;
  int sec;
  int utcdiff;
  char name[20];
};

void display(struct month*, struct time[], int);
void getCurrentMonth(struct month*);
void calculateTime(struct time[], int);
void saveTimezones(struct time[], int);
int readTimezones(struct time[]);
void getCurrentTime(struct time *);
void calculateMonthData(struct month*);
int timezonemenu(struct time[], int);
int isLeapYear(int);

void clearscreen(){
 clear();    // Clears Screen
 refresh();  // Refreshes (Syncs change in memory to terminal)

}
int main(){
  struct month m;
  struct time *now;
  struct time prev;
  struct timespec sleeptime = {0, 100000}; // 0 sec 100 ms
  struct time timezones[MAXTIMEZONES];
  now = &timezones[0]; // First timezone is always the local timezone

  int input;
  int flag = 1;
  int timeloop = 1;
  int tzcount = readTimezones(timezones);
    
  // curses Terminal initialization
  WINDOW *w = initscr();
  cbreak();
  nodelay(w, TRUE);

  getCurrentMonth(&m);
  getCurrentTime(now);
  prev = *now;

  while(flag){
    
    // Calculate required data about month and display
    calculateMonthData(&m);
    calculateTime(timezones, tzcount);

    clearscreen();
   
    display(&m, timezones, tzcount);
    printf("\n\n\rPress \n\r p for previous month \t\t n for next month, \n\r d to jump to specific date \t t to edit timezones \n\r q to quit \n\r");

    timeloop = 1;
    while (timeloop){
      getCurrentTime(now);
      // Stop when time changes
      if (now->sec != prev.sec)
	timeloop = 0;
      prev = *now;
      // or when user presses a key
      input = getch();
      if (input != ERR)
	timeloop = 0;
      nanosleep(&sleeptime, NULL);
    }

    switch(input){
    case 'p':
      if (m.month== 1){
	m.month = 12;
	m.year--;
      } else {
	m.month--;
      }
      break;
    case 'n':
      if (m.month == 12){
	m.month = 1;
	m.year++;
      } else {
	m.month++;
      }
      break;
    case 'd':
      while (flag){
	printf("\n\r Enter date as Y/m : ");
	scanf(" %d/%d", &m.year, &m.month);
	if (m.month >= 1 && m.month <= 12 && m.year > 0)
	  flag = 0;
	else
	  printf("Date Invalid");
      }
      flag = 1;
      break;
    case 't':
      {
	int c = timezonemenu(timezones, tzcount);
	if (c!= tzcount){
	  tzcount = c;
	  saveTimezones(timezones, tzcount);
	}
	break;
      }
    case 'q':
      flag = 0;
      break;

    }
  }

  // Done :)
  return 0;
}

int isLeapYear(int y){
  // returns true if y is a leap year
  return (y % 4 == 0 && y % 100 != 0 || y % 400 == 0);
}

void getCurrentMonth(struct month *m){
  // Gets current Month and year from system
  // using function of time.h
  time_t now;
  struct tm * timeinfo;
  time(&now);
  timeinfo = localtime(&now);
  m->year = timeinfo->tm_year + 1900;
  m->month = timeinfo->tm_mon + 1 ;
}

void getCurrentTime(struct time *t){
  // Gets current time from system and returns it
  time_t now;
  struct tm* timeinfo;
  time(&now);
  timeinfo = localtime(&now);
  t->sec = timeinfo->tm_sec;
  t->min = timeinfo->tm_min;
  t->hr = timeinfo->tm_hour;
  t->utcdiff = (int) timeinfo->tm_gmtoff / 60 ;
}

void calculateMonthData(struct month *m){
  // Calculates and sets the num of days and starting day
  // of the given month
  
  int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  // Set the number of Days in given month
  m->numDays = daysInMonth[m->month-1];

  // Count number of days from 1 AD upto start of the given year
  int y = m->year - 1;
  int daysBeforeThisYear = y*365 + y/4 - y/100 + y/400;
    
  // Count number of days from start of year upto the give month
  int daysBeforeThisMonth = 0; 
  for (int i=0; i< m->month - 1; i++){
    daysBeforeThisMonth += daysInMonth[i];
  }
  // If leap year, add leap day on required variables
  if (isLeapYear(m->year)){
    if (m->month == 2)
      m->numDays++;
    else if(m->month > 2)
      daysBeforeThisMonth++;
  }
  
  // First Day of the month;
  // 0001/01/01 is Monday . So if after that n days have passed
  // then this day is (n mod 7) = (1 for Monday, 2 for Tues ...)
  m->firstDay = (daysBeforeThisYear + daysBeforeThisMonth + 1) % 7;  
}

void saveTimezones(struct time tz[], int count){
  // Writes timezone information to TIMEZONEFILE
  FILE *f = fopen(TIMEZONEFILE, "w");
  int i;
  for (i=1; i<count; i++){
    fprintf(f, "%s %d", tz[i].name, tz[i].utcdiff);
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

void display(struct month* m, struct time timezones[], int count){
  // Displays Month Calendar on the screen
  int i,t=0;
  struct time* time;
  // Header line (Month/Year) and Days
  printf("\t\t %d / %d \n\r", m->month, m->year);
  printf("Sun\tMon\tTues\tWed\t\Thus\tFri\t\Sat \t\tTime\n\r");
  // Start of Days
  // Gap before 1st day
  for (i=0; i<m->firstDay; i++){
    printf("\t");
  }
  int day = m->firstDay;
  i = 1;
  while (i<=m->numDays){
    // Print this day number
    printf("%d\t", i);
    // Next day
    day++;
    i++;
    if (day % 7 == 0){  // Sunday starts after a newline
      if (t < count){
	time = &timezones[t];
	printf("\t%s %d:%d:%d (%+d mins)", time->name, time->hr, time->min, time->sec, time->utcdiff);
	time++;
	t++;
      }
      printf("\n\r"); 
    }    
  }
}

int timezonemenu(struct time tz[], int count){
  // Shows, edits, and adds timezones
  // Returns final number of tzs
  int i;
  char input;
  clearscreen();
  printf("TIMEZONES\n\r");
  printf("=========\n\r");
  printf("SN \tUTC OFFSET \tName \n\r");
  for (i=0; i<count; i++){
    printf("%d \t%+d \t%s\n\r", i, tz[i].utcdiff, tz[i].name);
  }
  i=0;
  
  printf("\nPress: \n\r");
  if (count < MAXTIMEZONES)
    printf(" a to add\n\r");
  if (count > 1)
    printf(" e to edit entry\n\r", &i);
  printf("Any other key to exit\n\r");

  input = getchar();

  switch (input){
  case 'a':
    if (count < MAXTIMEZONES){
      i = count;
      count++;
    }
    break;
  case 'e':
    if (count > 1){
    printf("Entry Number? (1-%d)", count-1);
    scanf("%d", &i);
    }
  }
  
  if (i){
    printf("Name: ");
    scanf("%s", tz[i].name);
    printf("Difference from UTC (in mins): ");
    scanf("%d", &tz[i].utcdiff);
  }

  return count;
}
