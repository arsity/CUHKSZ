/*
 * File: calendar.cpp
 * ------------------
 * This file implements the calendar.h interface
 * You need "include" the proper header file and write the function
 * definition.
 */

// TODO

#include <iostream>
#include <string>
using namespace std;

// month enumeration
enum Month {
  JANUARY = 1,
  FEBRUARY,
  MARCH,
  APRIL,
  MAY,
  JUNE,
  JULY,
  AUGUST,
  SEPTEMBER,
  OCTOBER,
  NOVEMBER,
  DECEMBER
};

// match Month with String
string monthToString(Month month) {
  switch (month) {
    case JANUARY:
      return "JANUARY";
    case FEBRUARY:
      return "FEBRUARY";
    case MARCH:
      return "MARCH";
    case APRIL:
      return "APRIL";
    case MAY:
      return "MAY";
    case JUNE:
      return "JUNE";
    case JULY:
      return "JULY";
    case AUGUST:
      return "AUGUST";
    case SEPTEMBER:
      return "SEPTEMBER";
    case OCTOBER:
      return "OCTOBER";
    case NOVEMBER:
      return "NOVEMBER";
    case DECEMBER:
      return "DECEMBER";
    default:
      return "???";
  }
}

// consider two possible circumstances of leap years
bool isLeapYear(int year) {
  if (year % 400 == 0 || (year % 100 != 0 && year % 4 == 0)) {
    return true;
  } else {
    return false;
  }
}

// 2 possible value for February, others are sure
int daysInMonth(Month month, int year) {
  switch (month) {
    case JANUARY:
    case MARCH:
    case MAY:
    case JULY:
    case AUGUST:
    case OCTOBER:
    case DECEMBER:
      return 31;
    case APRIL:
    case JUNE:
    case SEPTEMBER:
    case NOVEMBER:
      return 30;
    case FEBRUARY:
      if (isLeapYear(year)) {
        return 29;
      } else {
        return 28;
      }
    default:
      return -1;
  }
}

// convert into integer for ++, special case for December
Month operator++(Month &month, int) {
  if (month == DECEMBER) {
    month = JANUARY;
  } else {
    int k = month;
    month = Month(++k);
  }
  return month;
}

class Date {
 private:
  /* Instance variables */

  int day;       /* Day of the month              */
  Month month;   /* Enumerated type for the month */
  int year;      /* Four-digit numeric year       */
  int dayInYear; /* Index of the day in the year  */

  /* Private method prototypes */

  // set default value and accept artificial value
  void initDate(int dd = 1, Month mm = JANUARY, int yyyy = 1900) {
    day = dd;
    month = mm;
    year = yyyy;
  }

  // take the first character to go upper
  string capitalize(string str) {
    str[0] = toupper(str[0]);
    return str;
  }

  /* Friend declarations */

  friend Date operator+(Date, int);
  friend int operator-(Date d1, Date d2);

 public:
  /*
   * Constructor: Date
   * Usage: Date()
   *        Date(month, day, year);
   *        Date(day, month, year);
   * ------------------------------
   * Creates a Date object from its components.  The month must be
   * specified using one of the enumerated constants, which allows the
   * compiler to accept the day and month parameters in either order. The
   * constructor generates an error if the date does not exist.
   */

  // default setting
  Date() { initDate(); }

  // artificial setting 1
  Date(int day, Month month, int year) {

    if (0 < day && day <= daysInMonth(month, year)) {
      initDate(day, month, year);
    }
      // raise error
    else {
      cerr << "Invalid Date!" << endl;
    }
  }

  // artificial setting 2
  Date(Month month, int day, int year) {

    if (0 < day && day <= daysInMonth(month, year)) {
      initDate(day, month, year);
    }
      // raise error
    else {
      cerr << "Invalid Date!" << endl;
    }
  }

  /*
   * Method: getDay
   * Usage: int day = date.getDay();
   * -------------------------------
   * Returns the day of the month.
   */

  int getDay() { return day; }

  /*
   * Method: getMonth
   * Usage: Month month = date.getMonth();
   * -------------------------------------
   * Returns the month.
   */

  Month getMonth() { return month; }

  /*
   * Method: getYear
   * Usage: int year = date.getYear();
   * ---------------------------------
   * Returns the year.
   */

  int getYear() { return year; }

  /*
   * Method: toString
   * Usage: string str = date.toString();
   * ------------------------------------
   * Returns the string representation of the date in the form
   * dd-mmm-yyyy where dd is the numeric date, mmm is the three-letter
   * abbreviation of the month, and yyyy is the four-digit numeric year.
   * The date of the Apollo 11 moon landing would therefore be
   * represented as "20-Jul-1969".
   */

  // use monthToString get the whole name and use substr to cut
  // select the first character to go lower
  //  to_string() for int
  string toString() {
    string monstr = monthToString(month);
    monstr = monstr.substr(0, 3);
    for (int i = 1; i < 3; i++) {
      monstr[i] = tolower(monstr[i]);
    }
    return to_string(day) + "-" + monstr + "-" + to_string(year);
  }
};

/*
 * Operator: <<
 * ------------
 * Overloads the << operator so that it is able to display Date values.
 */

// convert to string and <<
ostream &operator<<(ostream &os, Date date) {
  string datestr = date.toString();
  os << datestr;
  return os;
}

/*
 * Operators: ==, !=, <, <=, >, >=
 * -------------------------------
 * Overloads the relational operators to work with Date objects.
 */

// equal if all equal
bool operator==(Date d1, Date d2) {
  return (d1.getYear() == d2.getYear() &&
      d1.getMonth() == d2.getMonth() && d1.getDay() == d2.getDay())
      ? true
      : false;
}

// neglation of equal
bool operator!=(Date d1, Date d2) { return !(d1 == d2); }

// compare year, month and day step by step
bool operator<(Date d1, Date d2) {
  if (d1.getYear() > d2.getYear()) {
    return false;
  } else if (d1.getYear() < d2.getYear()) {
    return true;
  } else {
    if (d1.getMonth() > d2.getMonth()) {
      return false;
    } else if (d1.getMonth() < d2.getMonth()) {
      return true;
    } else {
      if (d1.getDay() >= d2.getDay()) {
        return false;
      } else {
        return true;
      }
    }
  }
}

// compare year, month and day step by step
bool operator>(Date d1, Date d2) {
  if (d1.getYear() < d2.getYear()) {
    return false;
  } else if (d1.getYear() > d2.getYear()) {
    return true;
  } else {
    if (d1.getMonth() < d2.getMonth()) {
      return false;
    } else if (d1.getMonth() > d2.getMonth()) {
      return true;
    } else {
      if (d1.getDay() <= d2.getDay()) {
        return false;
      } else {
        return true;
      }
    }
  }
}

// neglation of >
bool operator<=(Date d1, Date d2) { return !(d1 > d2); }

// neglation of <
bool operator>=(Date d1, Date d2) { return !(d1 < d2); }

/*
 * Operators: +, -
 * ---------------
 * Overloads these operators so that they work with Date objects.
 */

// add day by day
// out of days of month range, set day back to 1 and make month increment
// the same as year
Date operator+(Date date, int delta) {
  int year = date.getYear();
  Month month = date.getMonth();
  int day = date.getDay();
  while (delta > 0) {
    day++;
    delta--;
    if (day > daysInMonth(month, year)) {
      day = 1;
      if (month == DECEMBER) {
        year++;
      }
      month++;
    }
  }
  Date result(month, day, year);
  return result;
}

// the reverse of +
Date operator-(Date date, int delta) {
  int year = date.getYear();
  Month month = date.getMonth();
  int day = date.getDay();
  while (delta > 0) {
    day--;
    delta--;
    if (day == 0) {
      if (month == JANUARY) {
        year--;
        month = DECEMBER;
        day = 31;
      } else {
        int monthInt = month;
        month = Month(--monthInt);
        day = daysInMonth(month, year);
      }
    }
  }
  Date result(month, day, year);
  return result;
}

// count the steps for small date to large date
// variable sign to determine sign
int operator-(Date d1, Date d2) {
  Date small, large;
  int sign = 1;
  if (d1 < d2) {
    small = d1;
    large = d2;
    sign = -1;
  } else {
    small = d2;
    large = d1;
  }
  int spanDays = 0;
  while (small < large) {
    small = small + 1;
    spanDays++;
  }
  return sign * spanDays;
}

/*
 * Operators: +=, -=
 * -----------------
 * Overloads the shorthand assignment operators to work with Date objects.
 */

// implementation as complete pattern
Date &operator+=(Date &date, int delta) {
  date = date + delta;
  return date;
}
Date &operator-=(Date &date, int delta) {
  date = date - delta;
  return date;
}

/*
 * Operators: ++, --
 * -----------------
 * Overloads the increment/decrement operators to work with Date objects.
 */

// implementation as complete pattern
Date operator++(Date &date) {
  date += 1;
  return date;
}
Date operator++(Date &date, int) {
  date += 1;
  return date;
}
Date operator--(Date &date) {
  date -= 1;
  return date;
}
Date operator--(Date &date, int) {
  date -= 1;
  return date;
}
