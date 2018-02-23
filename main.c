#include "common.h"

#ifdef __linux__ 
void clrscr(){
  clear();    // Clears Screen
  refresh();  // Refreshes (Syncs change in memory to terminal)
}
#endif

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
  cbreak();
  nodelay(w, TRUE);
  noecho();
  #endif

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

    clrscr();
   
    display(&d, timezones, tzcount);
    printf("\n\n\rPress \r\n p for previous month \t\t n for next month, \r\n d to jump to specific date \t t to edit timezones \r\n s to switch calendar \t\t q to quit \r\n ");

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
	int c = timezoneMenu(timezones, tzcount);
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

void printTabs(int n){
  // prints n number of tabs on the screen
  int i;
  for (i=0;i<n;i++)
    printf("\t");
}

void printTime(struct time* time){
  // prints time in a nice format
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

int timezoneMenu(struct time tz[], int count){
  // Shows, edits, and adds timezones
  // Returns final number of tzs
  int i;
  char input;
  clrscr();
  echo();
  
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
  
  fflush(stdin);
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
    printf("\n\rDifference from UTC (in mins): ");
    scanf("%d", &tz[i].utcdiff);
  }

  return count;
}
