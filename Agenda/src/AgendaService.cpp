#include <iostream>
#include "AgendaService.hpp"

AgendaService::AgendaService() {
  startAgenda();
}

AgendaService::~AgendaService() {
  quitAgenda();
}

bool AgendaService::userLogIn(const std::string userName,
  const std::string password) {
  auto filter = [](const User& user) {
    return true;
  };
  std::list<User> userList = m_storage->queryUser(filter);
  for (auto iter = userList.begin(); iter != userList.end(); iter++) {
    if (iter->getName() == userName && iter->getPassword() == password)
      return true;
  }
  return false;
}

bool AgendaService::userRegister(const std::string userName,
  const std::string password, const std::string email,
  const std::string phone) {
  for (int i = 0; i < userName.size(); i++) {
    if (userName[i] == ',' || userName[i] == '\"' || userName[i] == '&') return false;
  }
  for (int i = 0; i < password.size(); i++) {
    if (password[i] == ',' || password[i] == '\"' || password[i] == '&') return false;
  }
  for (int i = 0; i < email.size(); i++) {
    if (email[i] == ',' || email[i] == '\"' || email[i] == '&') return false;
  }
  for (int i = 0; i < phone.size(); i++) {
    if (phone[i] == ',' || phone[i] == '\"' || phone[i] == '&') return false;
  }
  auto filter = [userName, password, email, phone](const User& user) {
    return userName == user.getName();
  };
  if (!m_storage->queryUser(filter).size()) {
    m_storage->createUser(User(userName, password, email, phone));
    return true;
  }
  return false;
}

bool AgendaService::deleteUser(const std::string userName,
  const std::string password) {
  auto filter = [userName, password](const User& user) {
    return (userName == user.getName() && password == user.getPassword());
  };
  if (m_storage->deleteUser(filter)) {
    auto filter = [userName](const Meeting& meeting) {
      return (userName == meeting.getSponsor()
        || meeting.isParticipator(userName));
    };
    m_storage->deleteMeeting(filter);
    return true;
  }
  return false;
}

std::list<User> AgendaService::listAllUsers(void) const {
  auto filter = [](const User& user) {
    return true;
  };
  return m_storage->queryUser(filter);
}

bool AgendaService::createMeeting(const std::string userName,
  const std::string title, const std::string startDate,
  const std::string endDate, const std::vector<std::string> participator) {
  // failed if time is not avaliable
  Date sDate = Date::stringToDate(startDate);
  Date eDate = Date::stringToDate(endDate);
  if (!Date::isValid(sDate) || !Date::isValid(eDate) || sDate >= eDate)
    return false;
  // failed if title is not avaliable
  for (int i = 0; i < title.size(); i++) {
    if (title[i] == ',' || title[i] == '\"' || title[i] == '&') return false;
  }
  auto filter1 = [title](const Meeting& meeting) {
    return meeting.getTitle() == title;
  };
  if (m_storage->queryMeeting(filter1).size()) return false;
  // failed if user is not avaliable
  auto filter3 = [userName](const User& user) {
    return userName == user.getName();
  };
  if (m_storage->queryUser(filter3).size()) {
    std::string t_sponsor = m_storage->queryUser(filter3).begin()->getName();
    auto filter = [userName, sDate, eDate](const Meeting& meeting) {
      return ((userName == meeting.getSponsor()
        || meeting.isParticipator(userName))
        && ((meeting.getStartDate() >= sDate && meeting.getStartDate() < eDate)
        ||(meeting.getEndDate() > sDate && meeting.getEndDate() <= eDate)
        ||(meeting.getStartDate() <= sDate && meeting.getEndDate() >= eDate)));
    };
    if ((m_storage->queryMeeting(filter)).size())
      return false;
  } else {
    return false;
  }
  // failed if participator is not avaliable
  if (participator.size() == 0) return false;
  for (auto iter = participator.begin(); iter != participator.end(); iter++) {
    std::string t_parti = *iter;
    auto j = iter;
    j++;
    for (; j != participator.end(); j++) {
      if (t_parti == (*j)) return false;
    }
    if (t_parti == userName) return false;
    auto filter2 = [t_parti](const User& user) {
      return t_parti == user.getName();
    };
    if (m_storage->queryUser(filter2).size()) {
      std::string t_participator
        = m_storage->queryUser(filter2).begin()->getName();
      auto filter = [userName, sDate, eDate](const Meeting& meeting) {
        return ((userName == meeting.getSponsor()
          || meeting.isParticipator(userName))
          && ((meeting.getStartDate() >= sDate && meeting.getStartDate() < eDate)
          ||(meeting.getEndDate() > sDate && meeting.getEndDate() <= eDate)
          ||(meeting.getStartDate() <= sDate && meeting.getEndDate() >= eDate)));
      };
      if ((m_storage->queryMeeting(filter)).size())
        return false;
    } else {
      return false;
    }
  }
  Meeting t_meeting(userName, participator, sDate, eDate, title);
  m_storage->createMeeting(t_meeting);
  return true;
}

std::list<Meeting> AgendaService::meetingQuery(const std::string userName,
  const std::string title) const {
  auto filter = [userName, title](const Meeting& meeting) {
    return ((userName == meeting.getSponsor()
      || meeting.isParticipator(userName))
      && title == meeting.getTitle());
  };
  return m_storage->queryMeeting(filter);
}

std::list<Meeting> AgendaService::meetingQuery(const std::string userName,
  const std::string startDate, const std::string endDate) const {
  // failed if time is not avaliable
  Date sDate = Date::stringToDate(startDate);
  Date eDate = Date::stringToDate(endDate);
  if (!Date::isValid(sDate) || !Date::isValid(eDate) || sDate > eDate) {
    std::list<Meeting> nullMeeting;
    return nullMeeting;
  }
  auto filter = [userName, sDate, eDate](const Meeting& meeting) {
    return ((userName == meeting.getSponsor() || meeting.isParticipator(userName))
      && ((meeting.getStartDate() >= sDate && meeting.getStartDate() <= eDate)
      ||(meeting.getEndDate() >= sDate && meeting.getEndDate() <= eDate)
      ||(meeting.getStartDate() <= sDate && meeting.getEndDate() >= eDate)));
  };
  return m_storage->queryMeeting(filter);
}

std::list<Meeting> AgendaService::listAllMeetings
  (const std::string userName) const {
  auto filter = [userName](const Meeting& meeting) {
    return (userName == meeting.getSponsor()
      || meeting.isParticipator(userName));
  };
  return m_storage->queryMeeting(filter);
}

std::list<Meeting> AgendaService::listAllSponsorMeetings
  (const std::string userName) const {
  auto filter = [userName](const Meeting& meeting) {
  return userName == meeting.getSponsor();
  };
  return m_storage->queryMeeting(filter);
}

std::list<Meeting> AgendaService::listAllParticipateMeetings
  (const std::string userName) const {
  auto filter = [userName](const Meeting& meeting) {
    return meeting.isParticipator(userName);
  };
  return m_storage->queryMeeting(filter);
}

bool AgendaService::deleteMeeting(const std::string userName,
  const std::string title) {
  auto filter = [userName, title](const Meeting& meeting) {
    return (userName == meeting.getSponsor() && title == meeting.getTitle());
  };
  return m_storage->deleteMeeting(filter);
}

bool AgendaService::deleteAllMeetings(const std::string userName) {
  if (listAllSponsorMeetings(userName).size()) {
    auto filter = [userName](const Meeting& meeting) {
      return userName == meeting.getSponsor();
    };
    return m_storage->deleteMeeting(filter);
  }
  return true;
}

void AgendaService::startAgenda(void) {
  m_storage = Storage::getInstance();
}

void AgendaService::quitAgenda(void) {
  m_storage->sync();
}
