#include "common.h"

// Number of days in each month of English calendar (for non-leap year)
int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// Information about number of days in every month since 2000 BS to 2089 BS
const int dataBS[][13] = {
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
    {31,31,31,32,31,31,29,30,29,30,29,31},
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
    {31,32,31,32,31,30,30,30,29,30,29,31},
    {30,32,31,32,31,30,30,30,29,30,30,30},
    {31,31,32,31,31,30,30,30,29,30,30,30},
    {31,31,32,31,31,30,30,30,29,30,30,30},
    {31,32,31,32,30,31,30,30,29,30,30,30},
    {30,32,31,32,31,30,30,30,29,30,30,30},
    {31,31,32,31,31,31,30,30,30,30,30,30},
    {30,31,32,32,30,31,30,30,29,30,30,30},
    {30,32,31,32,31,30,30,30,29,30,30,30}
};


int isLeapYear(int y){
  // returns true if y is a leap year
  return (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0));
}

int validDate(struct date* d){
  // Returns 0 if the date is invalid
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

int daysSinceADEpoch(struct date *d){
  // Return number of days since 0001/01/01 AD upto
  // this day
  // (e.g.  0001/01/02 is 2nd day)
  extern int daysInMonth[];
  int i;
  // Count number of days from 1 AD upto start of the given year
  int y = d->year - 1;
  int daysBeforeThisYear = y*365 + y/4 - y/100 + y/400;
    
  // Count number of days from start of year upto the give month
  int daysBeforeThisMonth = 0; 
  for (i=0; i< d->month - 1; i++){
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
  int month,year, day = 1, days = 0, daysInThisMonth;
  month = 0;
  year = 0;
  while (days<n){
    daysInThisMonth = dataBS[year][month];
    if (days+daysInThisMonth >= n){
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
  // NOTE: if n <=  0 then the while loop is not entered and 2000/1/1 is returned
  // else processing is done normally and req. date is returned
  struct date d = {year + 2000, month + 1, day, BS_DATE};
  return d;
}

struct date nthDayOfADEpoch(int n){
  struct date d;
  int daysInThisMonth;
  int y = n/365.2425;
  int daysBeforeThisYear = y*365 + y/4 - y/100 + y/400;
  int remDays = n - daysBeforeThisYear;
  if (remDays == 0){
    d.year = y;
    d.month = 12;
    d.day = 31;
    return d;
  }
  
  d.year = y + 1;
  d.month = 1;  
  while (remDays > 0){
    daysInThisMonth = daysInMonth[d.month-1];
    if (d.month == 2 && isLeapYear(d.year))
      daysInThisMonth++;

    if (remDays <= daysInThisMonth){
      d.day = remDays;
      remDays =0;
    } else {
      d.month++;
      if (d.month > 12){
	d.month = 1;
	d.year++;
      }
      remDays -= daysInThisMonth;
    }
  }
  d.type = AD_DATE;
  return d;
}

struct date convertADToBS(struct date *ad) {
    // This function returns date in BS when ad, date in AD, is given
    static struct date BSEpoch = {1943, 4, 14, AD_DATE};
    int sinceADEpochToBSEpoch = daysSinceADEpoch(&BSEpoch);
    int sinceADEpochToNow = daysSinceADEpoch(ad);
    int sinceBSEpoch = sinceADEpochToNow - sinceADEpochToBSEpoch + 1;

    return nthDayOfBSEpoch(sinceBSEpoch);
}

struct date convertBSToAD(struct date *bs) {
    // This function returns date in AD when bs, date in BS, is given
    static struct date BSEpoch = {1943, 4, 14, AD_DATE};
    int sinceADEpochToBSEpoch = daysSinceADEpoch(&BSEpoch);
    int sinceBSEpochToNow = daysSinceBSEpoch(bs);
    int sinceADEpochToNow = sinceADEpochToBSEpoch + sinceBSEpochToNow - 1;

    return nthDayOfADEpoch(sinceADEpochToNow);
}

struct monthData calculateMonthDataBS(struct date d) {
  // Calculates and sets the num of days and starting day
  // of the given month of BS date
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

struct monthData calculateMonthDataAD(struct date d) {
    // Calculates and sets the num of days and starting day
    // of the given month of AD date
    struct monthData m;
    d.day = 1;
    extern int daysInMonth[];
    int daysPassed = daysSinceADEpoch(&d);
    // 0001/01/01 is ----Monday----- . So if n days have passed since,
    // then this day is (n mod 7) = (1 for Monday, 2 for Tues ...)
    m.firstDay = daysPassed  % 7;
    // Set the number of Days in given month
    m.numDays = daysInMonth[d.month-1];
    if (isLeapYear(d.year) && d.month == 2) {
        m.numDays++;
    }
    return m;
}

struct monthData calculateMonthData(struct date *d) {
    if (d->type == AD_DATE) {
        return calculateMonthDataAD(*d);
    } else {
        return calculateMonthDataBS(*d);
    }
}
