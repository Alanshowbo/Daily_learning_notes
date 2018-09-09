//编写day_of_year(),将某年某月某日的日期表示转换为该年中第几天的形式
//编写month_day(),将某年中的第几天转化为某月末日的形式表示
//编写month_name(int mon),返回指向月名字符串的指针
#include "stdio.h"

static int month_days[2][12] = {
    {31,28,31,30,31,30,31,31,30,31,30,31},
    {31,29,31,30,31,30,31,31,30,31,30,31}
};

int day_of_year(int year, int month, int day)
{
    int leap = year%4==0 && year%100!=0 || year%400 == 0;
    for(int i = 0; i<month-1; i++)
    {
        day += month_days[leap][i];
    }
    return day;
}

int month_day(int year, int nday, int *mon, int *day)
{
    *mon = 1;
    int leap = year%4==0 && year % 100!=0 ||year%400 == 0;
    while(nday>month_days[leap][*mon-1])
    {
        nday-=month_days[leap][*mon-1];
        (*mon)++;//注意括号
    }
    *day = nday;
    return 0;
}

char *month_name(int mon)
{
	static char *mon_name[]={
		"Illeagal month name!","January","February","March",
		"April","May","June","July","August","September",
		"October","November","December"
	};
	return (mon<1||mon>12)?mon_name[0]:mon_name[mon];
}

int main()
{
    int t = day_of_year(2018,9,9);
    printf("2018-9-9 is the %d day in 2018\n", t);
    int day = 0, mon = 0;
    month_day(2018, t, &mon, &day);
    printf("the %d day in 2018 is:%s-%d\n", t, month_name(mon), day);
    return 0;
}
