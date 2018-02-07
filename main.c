#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#ifdef __linux__
  #include <curses.h>
#else
  #include <conio.h>
#endif

#define MAXTIMEZONES 10
#define TIMEZONEFILE "timezones.txt"
#define AD_DATE 0
#define BS_DATE 1

int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// Information about number of days in every month since 2000 BS to 2090 BS
const dataBS[][13] = {
    {30,32,31,32,31,30,30,30,29,30,29,31},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {30,32,31,32,31,30,30,30,29,30,29,31},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {31,31,31,32,31,31,29,30,30,29,29,31},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {31,31,31,32,31,31,29,30,30,29,30,30},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {31,31,31,32,31,31,29,30,30,29,30,30},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,30,29,31},
    {31,31,31,32,31,31,30,29,30,29,30,30},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,30},
    {31,32,31,32,31,30,30,30,29,30,29,31},
    {31,31,31,32,31,31,30,29,30,29,30,30},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {30,32,31,32,31,30,30,30,29,30,29,31},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,31,32,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {30,32,31,32,31,30,30,30,29,30,29,31},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {30,32,31,32,31,31,29,30,30,29,29,31},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {31,31,31,32,31,31,29,30,30,29,30,30},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {31,31,31,32,31,31,29,30,30,29,30,30},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {31,31,31,32,31,31,30,29,30,29,30,30},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,30},
    {31,32,31,32,31,30,30,30,29,30,29,31},
    {31,31,31,32,31,31,30,29,30,29,30,30},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,30},
    {31,32,31,32,31,30,30,30,29,30,29,31},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,31,32,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {30,32,31,32,31,30,30,30,29,30,29,31},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {30,32,31,32,31,31,29,30,29,30,29,31},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {31,31,31,32,31,31,29,30,30,29,29,31},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {31,31,31,32,31,31,29,30,30,29,30,30},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {31,31,31,32,31,31,30,29,30,29,30,30},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,30},
    {31,32,31,32,31,30,30,30,29,30,29,31},
    {31,31,31,32,31,31,30,29,30,29,30,30},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,30},
    {31,31,32,32,31,30,30,30,29,30,30,30},
    {30,32,31,32,31,30,30,30,29,30,30,30},
    {31,31,32,31,31,30,30,30,29,30,30,30},
    {31,31,32,31,31,30,30,30,29,30,30,30},
    {31,32,31,32,30,31,30,30,29,30,30,30},
    {30,32,31,32,31,30,30,30,29,30,30,30},
    {31,31,32,31,31,31,30,30,29,30,30,30},
    {30,31,32,32,30,31,30,30,29,30,30,30},
    {30,32,31,32,31,30,30,30,29,30,30,30},
    {30,32,31,32,31,30,30,30,29,30,30,35}
};

struct monthData {   
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

struct date{
  int year;
  int month;    // Month (1 for Jan, 2 for Feb, ... )
  int day;
  int weekday; // Sun = 0, Mon = 1 , ...
  int type; // AD = 0 , BS = 1
};

void display(struct date*, struct time[], int);
struct date getCurrentDate();
void calculateTime(struct time[], int);
void saveTimezones(struct time[], int);
int readTimezones(struct time[]);
struct time getCurrentTime();
struct monthData calculateMonthData(struct date*);
int timezonemenu(struct time[], int);
int isLeapYear(int);
int daysUptoAD(struct month *m);

int isLeapYear(int y){
  // returns true if y is a leap year
  return (y % 4 == 0 && y % 100 != 0 || y % 400 == 0);
}

void clearscreen(){
  #ifdef __linux__
  clear();    // Clears Screen
  refresh();  // Refreshes (Syncs change in memory to terminal)
  #else
  clrscr();
  #endif

}

int daysSinceADEpoch(struct date *d){
  // Return number of days since 0001/01/01 AD upto
  // this day
  // (e.g.  0001/01/02 is 2nd day)
  extern int daysInMonth[];
  
  // Count number of days from 1 AD upto start of the given year
  int y = d->year - 1;
  int daysBeforeThisYear = y*365 + y/4 - y/100 + y/400;
    
  // Count number of days from start of year upto the give month
  int daysBeforeThisMonth = 0; 
  for (int i=0; i< d->month - 1; i++){
    daysBeforeThisMonth += daysInMonth[i];
  }
  
  // If leap year and the month is after february, add leap day of february
  if (isLeapYear(d->year) && d->month > 2){
      daysBeforeThisMonth++;
  }
  return daysBeforeThisYear + daysBeforeThisMonth + d->day;
}

int daysSinceBSEpoch(struct date *d){
  // Return number of days since 2000/01/01 BS upto
  // this day
  // (e.g.  2000/01/03 is 3rd day)
  int yr = d->year - 2000;
  int days = d->day, i =0 ,j =0;

  for (i=0; i<yr; i++){
    for (j = 0; j< 12; j++){
      days += dataBS[i][j];
    }
  }
  for (j = 0; j< d->month - 1; j++){
    days += dataBS[yr][j];
  }
  
  return days ;
}

struct date nthDayOfBSEpoch(int n){
  int month,year, days = 0, day = 1, daysInThisMonth;
  month = 0;
  year = 0;
  while (days<n){
    daysInThisMonth = dataBS[year][month];
    if (days+daysInThisMonth > n){
      day = n - days;
      days = n;
    } else {
      days += daysInThisMonth;
      month++;
      if (month == 12){
	year++;
	month = 0;
      }      
    }
  }

  struct date d = {year + 2000, month + 1, day, -1 , BS_DATE};
  return d;
}

struct date nthDayOfADEpoch(int n){
  struct date d;
  int daysInThisMonth;
  int y = n/365.24;
  int daysBeforeThisYear = y*365 + y/4 - y/100 + y/400;
  d.year = y + 1;
  int remDays = n - daysBeforeThisYear;
  if (remDays < 0){
    d.year --;
    remDays = remDays + 365 + (isLeapYear(d.year -1 )? 1 : 0);
  } else if (remDays > 365 +  (isLeapYear(d.year -1 )? 1 : 0)) {
    remDays -= (365 + (isLeapYear(d.year))? 1:0);
    d.year++;
  }
  /* printf("%d\n", n); */
  /* printf("%d\n", remDays); */
  
  d.month = 1;
  while (remDays > 0){
    daysInThisMonth = daysInMonth[d.month-1];
    if (d.month == 2 && isLeapYear(d.year))
      daysInThisMonth++;

    if (remDays < daysInThisMonth){
      d.day = remDays;
      remDays =0;
    } else {
      d.month++;
      remDays -= daysInThisMonth;
    }
  }
  d.type = AD_DATE;
  return d;
}


struct date convertADToBS(struct date ad){
  // This function returns date in BS when ad, date in AD, is given
  static struct date BSEpoch = {1943, 4, 14, -1, AD_DATE};
  int sinceADEpochToBSEpoch = daysSinceADEpoch(&BSEpoch);
  int sinceADEpochToNow = daysSinceADEpoch(&ad);
  int sinceBSEpoch = sinceADEpochToNow - sinceADEpochToBSEpoch + 1;

  return nthDayOfBSEpoch(sinceBSEpoch);  
}

struct date convertBSToAD(struct date bs){
  // This function returns date in AD when bs, date in BS, is given
  static struct date BSEpoch = {1943, 4, 14, -1, AD_DATE};
  int sinceADEpochToBSEpoch = daysSinceADEpoch(&BSEpoch);
  int sinceBSEpochToNow = daysSinceBSEpoch(&bs);
  int sinceADEpochToNow = sinceADEpochToBSEpoch + sinceBSEpochToNow - 1;

  return nthDayOfADEpoch(sinceADEpochToNow);  
}


struct monthData calculateMonthDataBS(struct date d){
  struct monthData m;
  d.day = 1;
  int daysPassed = daysSinceBSEpoch(&d);
  // 2000/1/1 is Wednesday. So if n days have passed since
  // then this day is (n+2) mod 7 
  m.firstDay = (daysPassed + 2) % 7;
  // Set number of days in this month
  m.numDays = dataBS[d.year - 2000][d.month -1];
  
  return m;
}

struct monthData calculateMonthDataAD(struct date d){
  // Calculates and sets the num of days and starting day
  // of the given month

  struct monthData m;
  d.day = 1;
  extern int daysInMonth[];
  int daysPassed = daysSinceADEpoch(&d);
  // 0001/01/01 is ----Monday----- . So if n days have passed since, 
  // then this day is (n mod 7) = (1 for Monday, 2 for Tues ...)  
  m.firstDay = daysPassed  % 7;  
  // Set the number of Days in given month
  m.numDays = daysInMonth[d.month-1];
  if (isLeapYear(d.year) && d.month == 2){  
      m.numDays++;
  }
  return m;
}

struct monthData calculateMonthData(struct date *d){
  if (d->type == AD_DATE){
    return calculateMonthDataAD(*d);
  } else {
    return calculateMonthDataBS(*d);
  }
}

int validDate(struct date* d){
  if (d->month < 1 || d->month > 12)
    return 0;
  if (d->day > 32 || d->day < 0)
    return 0;
  if (d->type == AD_DATE) {
    return d->year > 0 ;
  } else {
    return d->year > 1999 && d->year < 2090;
  }
}

int main(){
  struct date d;
  struct time *now;
  struct time prev;
  struct timespec sleeptime = {0, 100000}; // 0 sec 100 ms
  struct time timezones[MAXTIMEZONES];
  now = &timezones[0]; // First timezone is always the local timezone

  int input;
  int flag = 1;
  int timeloop = 1;
  int tzcount = readTimezones(timezones);

  #ifdef __linux__
  // curses Terminal initialization
  WINDOW *w = initscr();
  /* cbreak(); */
  nodelay(w, TRUE);
  #endif
  /* nocbreak(); */
  d = getCurrentDate();
  *now = getCurrentTime();
  prev = *now;
  while(flag){
    // If date is out of range (due to user input)
    if (!validDate(&d)){
      printf("\nDate is Out of Range\n");
      fflush(stdin);
      scanf("%",&input);
      d = getCurrentDate();
    }
    
    calculateTime(timezones, tzcount);

    clearscreen();
   
    display(&d, timezones, tzcount);
    printf("\n\n\rPress \n\r p for previous month \t\t n for next month, \n\r d to jump to specific date \t t to edit timezones \n\r s to switch calendar \t\t q to quit \n\r");

    timeloop = 1;
    while (timeloop){
      *now = getCurrentTime();
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
      if (d.month== 1){
	d.month = 12;
	d.year--;
      } else {
	d.month--;
      }
      d.day = 1;
      break;
    case 'n':
      if (d.month == 12){
	d.month = 1;
	d.year++;
      } else {
	d.month++;
      }
      d.day = 1;
      break;
    case 'd':
      while (flag){
	printf("\n\r Enter date as Y/m/d : ");
	scanf(" %d/%d/%d", &d.year, &d.month, &d.day);
	if (validDate(&d))
	  flag = 0;
	else
	  printf("Date Invalid");
      }
      flag = 1;
      break;
    case 's':
      if (d.type == AD_DATE){
	d = convertADToBS(d);
      } else {
	d = convertBSToAD(d);
      }
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
  d.weekday = timeinfo->tm_wday;
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

void printTabs(int n){
  int i=0;
  for (;i<n;i++)
    printf("\t");
}

void printTime(struct time* time){
  printf("%s %d:%d:%d (%+d mins)", time->name, time->hr, time->min, time->sec, time->utcdiff);
}


void display(struct date* d, struct time timezones[], int count){
  // Displays Month Calendar on the screen
  int i,t=0;

  struct monthData m = calculateMonthData(d);

  // Header line (Month/Year) and Days
  printf("\t\t  %d/%d/%d %s\n\r", d->year, d->month, d->day, (d->type == AD_DATE)? "AD":"BS");
  
  printf("Sun\tMon\tTues\tWed\t\Thus\tFri\t\Sat \t\tTime\n\r");
  // Start of Days
  // Gap before 1st day
  printTabs(m.firstDay);
  int day = m.firstDay;
  i = 1;
  while (i<=m.numDays){
    // Print this day number
    if (i == d->day)
      printf("-%d-\t",i);    
    else
      printf("%d\t", i);
    // Next day
    day++;
    i++;
    if (day % 7 == 0){  // Sunday starts after a newline
      if (t < count){
	printf("\t");
	printTime(&timezones[t]);
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
  /* scanf(" %c",&input); */
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
    printf("\n\rDifference from UTC (in mins): ");
    scanf("%d", &tz[i].utcdiff);
  }

  return count;
}
