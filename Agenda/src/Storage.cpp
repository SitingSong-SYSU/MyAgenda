#include <list>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "Storage.hpp"
#include "Path.hpp"

std::shared_ptr<Storage> Storage::m_instance = nullptr;

Storage::Storage() {
  readFromFile();
  m_dirty = false;
}

bool Storage::readFromFile(void) {
  std::ifstream inUsers, inMeetings;
  std::string buffer;
  inUsers.open(Path::userPath, std::ios::in);
  if (!inUsers.is_open()) return false;
  std::string subString;
  while (!inUsers.eof()) {
    std::string name, password, email, phone;
    getline(inUsers, buffer, ',');
    if (buffer.size() == 0) break;
    name = buffer.substr(1, buffer.size() - 2);
    getline(inUsers, buffer, ',');
    password = buffer.substr(1, buffer.size() - 2);
    getline(inUsers, buffer, ',');
    email = buffer.substr(1, buffer.size() - 2);
    getline(inUsers, buffer);
    phone = buffer.substr(1, buffer.size() - 2);
    createUser(User(name, password, email, phone));
  }
  inUsers.close();
  inMeetings.open(Path::meetingPath, std::ios::in);
  if (!inMeetings.is_open()) return false;
  while (!inMeetings.eof()) {
    std::string sponsor, startdate, enddate, title;
    getline(inMeetings, buffer, ',');
    if (buffer.size() == 0) break;
    sponsor = buffer.substr(1, buffer.size() - 2);
    getline(inMeetings, buffer, ',');
    std::string subString = buffer.substr(1, buffer.size() - 2);
    int l = 0, r = 0;
    std::vector<std::string> t_participator;
    for (r = 0; r < subString.size(); r++) {
      if (subString[r] == '&') {
        std::string parti = subString.substr(l, r - l);
        t_participator.push_back(parti);
        l = r + 1;
      }
    }
    std::string partici = subString.substr(l, r - l);
    t_participator.push_back(partici);
    getline(inMeetings, buffer, ',');
    startdate = buffer.substr(1, buffer.size() - 2);
    getline(inMeetings, buffer, ',');
    enddate = buffer.substr(1, buffer.size() - 2);
    getline(inMeetings, buffer);
    title = buffer.substr(1, buffer.size() - 2);
    createMeeting(Meeting(sponsor, t_participator, Date::stringToDate(startdate),
      Date::stringToDate(enddate), title));
  }
  inMeetings.close();
  return true;
}

bool Storage::writeToFile(void) {
  std::ofstream outUsers, outMeetings;
  outUsers.open(Path::userPath, std::ios::out);
  if (!outUsers.is_open()) return false;
  for (auto iter = m_userList.begin(); iter != m_userList.end(); iter++) {
    outUsers << "\"" << iter->getName() << "\",\""
      << iter->getPassword() << "\",\"" << iter->getEmail()
      << "\",\"" << iter->getPhone() << "\"" << std::endl;
  }
  outUsers.close();
  outMeetings.open(Path::meetingPath, std::ios::out);
  if (!outMeetings.is_open()) return false;
  for (auto iter = m_meetingList.begin(); iter != m_meetingList.end(); iter++) {
    outMeetings << "\"" << iter->getSponsor() << "\",\"";
    bool flag = false;
    std::vector<std::string> t_participator = iter->getParticipator();
    for (auto it = t_participator.begin(); it != t_participator.end(); it++) {
      if (flag) outMeetings << "&";
      outMeetings << (*it);
      flag = true;
    }
    outMeetings << "\",\""
                << Date::dateToString(iter->getStartDate()) << "\",\""
                << Date::dateToString(iter->getEndDate()) << "\",\""
                << iter->getTitle() << "\"" << std::endl;
  }
  outMeetings.close();
  return true;
}

std::shared_ptr<Storage> Storage::getInstance(void) {
  if (m_instance == nullptr) m_instance = std::shared_ptr<Storage>(new Storage());
  return m_instance;
}

Storage::~Storage() {
  sync();
  Storage::m_instance = nullptr;
  m_dirty = false;
}

void Storage::createUser(const User & t_user) {
  m_userList.push_back(t_user);
  m_dirty = true;
}

std::list<User> Storage::queryUser
  (std::function<bool(const User &)> filter) const {
  std::list<User> newUserList;
  for (auto iter = m_userList.begin(); iter != m_userList.end(); iter++) {
    if (filter(*iter)) newUserList.push_back(*iter);
  }
  return newUserList;
}

int Storage::updateUser(std::function<bool(const User &)> filter,
  std::function<void(User &)> switcher) {
  int cnt = 0;
  for (auto iter = m_userList.begin(); iter != m_userList.end(); iter++) {
    if (filter(*iter)) {
      switcher(*iter);
      m_dirty = true;
      cnt++;
    }
  }
  return cnt;
}

int Storage::deleteUser(std::function<bool(const User &)> filter) {
  int cnt = 0;
  for (auto iter = m_userList.begin(); iter != m_userList.end();) {
    if (filter(*iter)) {
      auto tmp = iter;
      iter++;
      m_userList.erase(tmp);
      m_dirty = true;
      cnt++;
    } else {
      iter++;
    }
  }
  return cnt;
}

void Storage::createMeeting(const Meeting & t_meeting) {
  m_meetingList.push_back(t_meeting);
  m_dirty = true;
}

std::list<Meeting> Storage::queryMeeting
  (std::function<bool(const Meeting &)> filter) const {
  std::list<Meeting> newMeetingList;
  for (auto iter = m_meetingList.begin(); iter != m_meetingList.end(); iter++) {
    if (filter(*iter)) {
      newMeetingList.push_back(*iter);
    }
  }
  return newMeetingList;
}

int Storage::updateMeeting(std::function<bool(const Meeting &)> filter,
  std::function<void(Meeting &)> switcher) {
  int cnt = 0;
  for (auto iter = m_meetingList.begin(); iter != m_meetingList.end(); iter++) {
    if (filter(*iter)) {
      switcher(*iter);
      m_dirty = true;
      cnt++;
    }
  }
  return cnt;
}

int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter) {
  int cnt = 0;
  for (auto iter = m_meetingList.begin(); iter != m_meetingList.end();) {
    if (filter(*iter)) {
      auto tmp = iter;
      iter++;
      m_meetingList.erase(tmp);
      m_dirty = true;
      cnt++;
    } else {
      iter++;
    }
  }
  return cnt;
}

bool Storage::sync(void) {
  if(m_dirty) {
    return writeToFile();
  } else {
    return true;
  }
}
