#include <initializer_list>
#include <string>
#include <sstream>
#include <stdlib.h>
#include "Date.hpp"

Date::Date() {
  m_year = m_month = m_day = m_hour = m_minute = 0;
}

Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute) {
  m_year = t_year;
  m_month = t_month;
  m_day = t_day;
  m_hour = t_hour;
  m_minute = t_minute;
}

Date::Date(std::string dateString) {
  bool flag = true;
  if (dateString.size() != 16 || dateString[4] != '-'
    || dateString[7] != '-' || dateString[10] != '/'
    || dateString[13] !=':') flag = false;
  if (dateString[0] < '0' || dateString[0] > '9') flag = false;
  if (dateString[1] < '0' || dateString[1] > '9') flag = false;
  if (dateString[2] < '0' || dateString[2] > '9') flag = false;
  if (dateString[3] < '0' || dateString[3] > '9') flag = false;
  if (dateString[5] < '0' || dateString[5] > '9') flag = false;
  if (dateString[6] < '0' || dateString[6] > '9') flag = false;
  if (dateString[8] < '0' || dateString[8] > '9') flag = false;
  if (dateString[9] < '0' || dateString[9] > '9') flag = false;
  if (dateString[11] < '0' || dateString[11] > '9') flag = false;
  if (dateString[12] < '0' || dateString[12] > '9') flag = false;
  if (dateString[14] < '0' || dateString[14] > '9') flag = false;
  if (dateString[15] < '0' || dateString[15] > '9') flag = false;
  if (flag) {
    std::string subString;
    subString = dateString.substr(0, 4);
    m_year = atoi(subString.c_str());
    subString = dateString.substr(5, 2);
    m_month = atoi(subString.c_str());
    subString = dateString.substr(8, 2);
    m_day = atoi(subString.c_str());
    subString = dateString.substr(11, 2);
    m_hour = atoi(subString.c_str());
    subString = dateString.substr(14, 2);
    m_minute = atoi(subString.c_str());
  } else {
    m_year = m_month = m_day = m_hour = m_minute = 0;
  }
}

int Date::getYear(void) const {
  return m_year;
}

void Date::setYear(const int t_year) {
  m_year = t_year;
}

int Date::getMonth(void) const {
  return m_month;
}

void Date::setMonth(const int t_month) {
  m_month = t_month;
}

int Date::getDay(void) const {
  return m_day;
}

void Date::setDay(const int t_day) {
  m_day = t_day;
}

int Date::getHour(void) const {
  return m_hour;
}

void Date::setHour(const int t_hour) {
  m_hour = t_hour;
}

int Date::getMinute(void) const {
  return m_minute;
}

void Date::setMinute(const int t_minute) {
  m_minute = t_minute;
}

bool Date::isValid(const Date t_date) {
  if (t_date.m_year > 999 && t_date.m_year < 10000) {
    if (t_date.m_hour >= 0 && t_date.m_hour < 24
      && t_date.m_minute >= 0 && t_date.m_minute < 60) {
      if ((t_date.m_month == 1 || t_date.m_month == 3 || t_date.m_month == 5
        || t_date.m_month == 7 || t_date.m_month == 8 || t_date.m_month == 10
        || t_date.m_month == 12) && t_date.m_day > 0 && t_date.m_day < 32)
        return true;
      if ((t_date.m_month == 4 || t_date.m_month == 6 || t_date.m_month ==9
        || t_date.m_month ==11) && t_date.m_day > 0 && t_date.m_day < 31)
        return true;
      if (t_date.m_month == 2) {
        if ((t_date.m_year % 4 == 0 && t_date.m_year % 100 != 0)
          || t_date.m_year % 400 == 0) {
          if (t_date.m_day > 0 && t_date.m_day < 30)
            return true;
        } else {
          if (t_date.m_day > 0 && t_date.m_day < 29)
            return true;
        }
      }
    }
  }
  return false;
}

Date Date::stringToDate(const std::string t_dateString) {
  bool flag = true;
  Date newDate;
  if (t_dateString.size() != 16 || t_dateString[4] != '-'
    || t_dateString[7] != '-' || t_dateString[10] != '/'
    || t_dateString[13] !=':') flag = false;
  if (t_dateString[0] < '0' || t_dateString[0] > '9') flag = false;
  if (t_dateString[1] < '0' || t_dateString[1] > '9') flag = false;
  if (t_dateString[2] < '0' || t_dateString[2] > '9') flag = false;
  if (t_dateString[3] < '0' || t_dateString[3] > '9') flag = false;
  if (t_dateString[5] < '0' || t_dateString[5] > '9') flag = false;
  if (t_dateString[6] < '0' || t_dateString[6] > '9') flag = false;
  if (t_dateString[8] < '0' || t_dateString[8] > '9') flag = false;
  if (t_dateString[9] < '0' || t_dateString[9] > '9') flag = false;
  if (t_dateString[11] < '0' || t_dateString[11] > '9') flag = false;
  if (t_dateString[12] < '0' || t_dateString[12] > '9') flag = false;
  if (t_dateString[14] < '0' || t_dateString[14] > '9') flag = false;
  if (t_dateString[15] < '0' || t_dateString[15] > '9') flag = false;
  if (flag) {
    std::string subString;
    subString = t_dateString.substr(0, 4);
    newDate.setYear(atoi(subString.c_str()));
    subString = t_dateString.substr(5, 2);
    newDate.setMonth(atoi(subString.c_str()));
    subString = t_dateString.substr(8, 2);
    newDate.setDay(atoi(subString.c_str()));
    subString = t_dateString.substr(11, 2);
    newDate.setHour(atoi(subString.c_str()));
    subString = t_dateString.substr(14, 2);
    newDate.setMinute(atoi(subString.c_str()));
    if (!isValid(newDate)) {
      newDate.setYear(0);
      newDate.setMonth(0);
      newDate.setDay(0);
      newDate.setHour(0);
      newDate.setMinute(0);
    }
  }
  return newDate;
}

std::string Date::dateToString(Date t_date) {
  if (!isValid(t_date)) {
    return "0000-00-00/00:00";
  } else {
    std::stringstream newString;
    if (t_date.m_year < 10) newString << "000";
    else if (t_date.m_year < 100) newString << "00";
    else if (t_date.m_year < 1000) newString << '0';
    newString << t_date.m_year << '-';
    if (t_date.m_month < 10) newString << '0';
    newString << t_date.m_month << '-';
    if (t_date.m_day < 10) newString << '0';
    newString << t_date.m_day << '/';
    if (t_date.m_hour < 10) newString << '0';
    newString << t_date.m_hour << ':';
    if (t_date.m_minute < 10) newString << '0';
    newString << t_date.m_minute;
    return newString.str();
  }
}

Date & Date::operator=(const Date &t_date) {
  m_year = t_date.getYear();
  m_month = t_date.getMonth();
  m_day = t_date.getDay();
  m_hour = t_date.getHour();
  m_minute = t_date.getMinute();
  return *this;
}

bool Date::operator==(const Date &t_date) const {
  if (m_year == t_date.getYear() && m_month == t_date.getMonth()
    && m_day == t_date.getDay() && m_hour == t_date.getHour()
    && m_minute == t_date.getMinute())
    return true;
  else
    return false;
}

bool Date::operator>(const Date &t_date) const {
  if (m_year > t_date.getYear()) return true;
  else if (m_year < t_date.getYear()) return false;
  if (m_month > t_date.getMonth()) return true;
  else if (m_month < t_date.getMonth()) return false;
  if (m_day > t_date.getDay()) return true;
  else if (m_day < t_date.getDay()) return false;
  if (m_hour > t_date.getHour()) return true;
  else if (m_hour < t_date.getHour()) return false;
  if (m_minute > t_date.getMinute())
    return true;
  else
    return false;
}

bool Date::operator<(const Date &t_date) const {
  if (!(*this > t_date) && !(*this ==t_date))
    return true;
  else
    return false;
}

bool Date::operator>=(const Date &t_date) const {
  if (!(*this < t_date))
    return true;
  else
    return false;
}

bool Date::operator<=(const Date &t_date) const {
  if (!(*this > t_date))
    return true;
  else
    return false;
}
