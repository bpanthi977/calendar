#include "common.h"

#ifdef __linux__ 
void clrscr(){
  clear();    // Clears Screen
  refresh();  // Refreshes (Syncs change in memory to terminal)
}
void Sleep(int ms){
  char command[20];
  sprintf(command, "sleep %ds", ms / 1000);
  system(command);
}
#else
void clrscr(){
  system("cls");
}
#endif

struct time timezones[MAXTIMEZONES];  // List of timezones to show on time section
int tzcount; // Number of timezones

int main(){
  // Variable declarations
  struct date d;
  char input;
  int flag = 1;
  
  // Initial Value for variables
  tzcount = readTimezones(timezones); // Read timezone from file
  d = getCurrentDate();
  
#ifdef __linux__
  // curses Terminal initialization
  WINDOW *w = initscr();
  cbreak();
  nodelay(w, TRUE);
  noecho();
#endif
  while(flag) {
    timezones[0] = getCurrentTime();
    updateTime(timezones, tzcount);
    clrscr();
    displayCalendarAndTime(&d);
    input = displayMainMenu(&d);
    // Handle cases not handled by displayMainMenu
    switch (input){
    case 't': 
      tzcount = displayTimezoneMenu();      
      break;
    case 'q':
      flag = 0;
      break;
    }
    // If date is out of range (due to user input)
    if (!validDate(&d)) {
      printf("\nDate is Out of Range\n");
      fflush(stdin);
      #ifdef __linux__
      getchar();
      #else
      getch();
      #endif
      d = getCurrentDate();
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
  printf("%s %d:%d:%d %s (%+d mins)", time->name, time->hr % 12 , time->min, time->sec, (time->hr<13)? "AM":"PM", time->utcdiff);
}

void displayCalendarAndTime(struct date *d){
  // Displays Month Calendar And Time on the screen
  int i,t=0;

  struct monthData m = calculateMonthData(d);

  // Header line (Month/Year) and Days
  printf("\t\t  %d/%d/%d %s\r\n", d->year, d->month, d->day, (d->type == AD_DATE)? "AD":"BS");
  printf("Sun \tMon \tTues \tWed \tThus \tFri \tSat \t\tTime\r\n");

  // Start of Days
  // Gap before 1st day
  printTabs(m.firstDay);
  int day = m.firstDay;
  i = 1;
  for (i=1; i<=m.numDays; i++, day++){
    // Print this day number
    if (i == d->day)
      printf("-%d-\t",i);
    else
      printf("%d\t", i);

    if (day % 7 == 6) { // After Saturday the calender portion ends and time portion starts
      if (t < tzcount) {
	printf("\t");
	printTime(&timezones[t]);
	t++;
      }
      printf("\r\n");
    }
  }
 
  // Continue showing the remaining timezones
  printTabs(8 - day % 7);
  for (;t<tzcount;t++){
      printTime(&timezones[t]);
      printf("\r\n");
      printTabs(8);
  }
}

char displayMainMenu(struct date *d){
  // Show the main menu/options at the bottom
  // also if user pressed a button changes/converts date accordingly
  // returns the key user pressed or returns 0 if time has to be updated
  char input=0;
  struct time now, prev;
  printf("\n\r\nPress \r\n p for previous month \t\t n for next month, \r\n d to jump to specific date \t t to edit timezones \r\n s to switch calendar \t\t q to quit \r\n ");

  prev = getCurrentTime();
  while (1) {
    // Sleep for 100 ms
    Sleep(100);
    now = getCurrentTime();
    // Stop when time changes
    if (now.sec != prev.sec)
      break;
    prev = now;
    // or when user presses a key
#ifdef __linux__
    input = getch();
    if (input != ERR)
      break;
#else    
    if (_kbhit()) {
      input = getch();
      break;
    }
#endif
  }

  switch(input) {
  case 'p':
    if (d->month== 1) {
      d->month = 12;
      d->year--;
    } else {
      d->month--;
    }
    d->day = 1;
    break;
  case 'n':
    if (d->month == 12) {
      d->month = 1;
      d->year++;
    } else {
      d->month++;
    }
    d->day = 1;
    break;
  case 'd':

    while (1) {
      printf("\r\n Enter date as Y/m/d : ");
      d->year = -1; // Make date invalid before entry
      scanf(" %d/%d/%d", &d->year, &d->month, &d->day);
      if (validDate(d))
	break;
      else{
	printf("\r\nDate Invalid");
	fflush(stdin);
	#ifdef __linux__
	getchar();
	#else
	getch();
	#endif
      }
      
    }
    break;
  case 's':
    if (d->type == AD_DATE) {
      *d = convertADToBS(d);
    } else {
      *d = convertBSToAD(d);
    }
    break;
  }
  return input;
}


int displayTimezoneMenu(){
  // Shows, edits, and adds timezones
  // Returns final number of tzs
  int i, entry, count = tzcount;
  char input;
  clrscr();

  printf("TIMEZONES\r\n");
  printf("=========\r\n");
  // List timezones
  printf("SN \tUTC OFFSET \tName \r\n");
  for (i=0; i<count; i++) {
    printf("%d \t%+d \t\t%s\r\n", i, timezones[i].utcdiff, timezones[i].name);
  }
  // Show menu
  printf("\nPress: \r\n");
  if (count < MAXTIMEZONES)
    printf(" a to add\r\n");
  if (count > 1){
    printf(" e to edit entry\r\n");
    printf(" r to remove entry\r\n");
  }
  printf("Any other key to exit\r\n");

  // Get Input and Process it
  fflush(stdin);
#ifdef __linux__
  input = getchar();
#else 
  input = getch();
#endif
  switch (input) {
  case 'a':
    if (count < MAXTIMEZONES) {
      entry = count;
      count++;
    }
    break;
  case 'e':
    if (count > 1) {
      printf("Entry Number? (1-%d)", count-1);
      scanf("%d", &entry);
    }
    break;
  case 'r':
    if (count > 1){
      printf("Entry Number? (1-%d)", count-1);
      scanf("%d", &entry);
      if (entry>0 && entry<count){
	for (i=entry;i<count;i++)
	  timezones[i] = timezones[i+1];      
	count = --tzcount;
	saveTimezones(timezones, tzcount);
      }
      entry = 0;
    }
    break;
  default:
    entry = 0;
  }

  // Modify/Add entry if it was selected
  if (entry && entry<=count) {
    printf("Name: ");
    scanf("%s", timezones[entry].name);
    printf("\r\nDifference from UTC (in mins): ");
    scanf("%d", &timezones[entry].utcdiff);
    saveTimezones(timezones, count);
  }

  return count;
}
