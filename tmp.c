
int daystotal (int y, int m, int d)
{
        static char daytab[2][13] =
        {
                {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
                {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
        };
        int daystotal = d;
        for (int year = 1 ; year <= y ; year++)
        {
                int max_month = ( year < y ? 12 : m-1 );
                int leap = (year%4 == 0);
                if (year%100 == 0 && year%400 != 0)
                        leap = 0;
                for (int month = 1 ; month <= max_month ; month++)
                {
                        daystotal += daytab[leap][month];
                }
        }
        return daystotal;
}
