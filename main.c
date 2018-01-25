#include <stdio.h>
#include <time.h>
#include <curses.h>

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
  int zone;
};

void display(struct month*, struct time*);
void getCurrentMonth(struct month*);
struct time getCurrentTime();
void calculateMonthData(struct month*);
int isLeapYear(int);

int main(){
  struct month m;
  struct time now, prev;
  struct timespec sleeptime = {0, 100000}; // 0 sec 100 ms

  
  int input;
  int flag = 1;
  int timeloop = 1;
  
  // curses Terminal initialization
  WINDOW *w = initscr();
  cbreak();
  nodelay(w, TRUE);

  
  getCurrentMonth(&m);
  now = getCurrentTime();
  prev = now;
  while(flag){
    // Calculate required data about month and display
    calculateMonthData(&m);

    clear();    // Clears Screen
    refresh(); // Refreshes (Syncs change in memory to terminal)

    display(&m, &now);

    printf("\n\r Press \n\r p for previous month, \n\r n for next month, \n\r d for specific date and \n\r q to quit \n\r: ");

    timeloop = 1;
    while (timeloop){
      now = getCurrentTime();
      // Stop when time changes
      if (now.sec != prev.sec)
	timeloop = 0;
      prev = now;
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

struct time getCurrentTime(){
  // Gets current time from system and returns it
  struct time t;
  time_t now;
  struct tm* timeinfo;
  time(&now);
  timeinfo = localtime(&now);
  t.sec = timeinfo->tm_sec;
  t.min = timeinfo->tm_min;
  t.hr = timeinfo->tm_hour;
  return t;
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

void display(struct month* m, struct time* time){
  // Displays Month Calendar on the screen
  
  // Header line (Month/Year) and Days
  printf("\t\t %d / %d \n\r", m->month, m->year);
  printf("Sun\tMon\tTues\tWed\t\Thus\tFri\t\Sat\n\r");
  // Start of Days
  // Gap before 1st day
  for (int i=0; i<m->firstDay; i++){
    printf("\t");
  }
  int day = m->firstDay;
  int i = 1;
  while (i<=m->numDays){
    // Print this day number
    printf("%d\t", i);
    // Next day
    day++;
    i++;
    if (day % 7 == 0){  // Sunday starts after a newline
      printf("\n\r"); 
    }    
  }
  
  /* printf("\r%d : %d : %d", time->hr, time->min, time->sec); */
}

