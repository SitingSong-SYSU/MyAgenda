#include <iostream>
#include "AgendaService.hpp"
using namespace std;

void testStorage() {
	std::shared_ptr<Storage> S = Storage::getInstance();
	User A("Lily", "qwert", "12345@qq.com", "12567890987");
	vector<std::string> participator;
	participator.push_back("A");
	participator.push_back("B");
	participator.push_back("C");
	Date B(2016,8,18,12,56);
	Date C("2016-08-19/11:23");
	Meeting M1("sp", participator, B, C, "tit");
	//S->createMeeting(M1);
	S->sync();
}

void testAgendaService() {
	AgendaService AS;
	list<Meeting> t_metting = AS.listAllMeetings("Lucy");
	for(auto it = t_metting.begin(); it != t_metting.end(); it++) {
		cout << it->getSponsor() << " " << Date::dateToString(it->getStartDate())
			<< " " <<  Date::dateToString(it->getEndDate()) << " "
			<< it->getTitle() << " ";
		bool flag = false;
		vector<std::string> t_par = it->getParticipator();
	    for (auto it1 = t_par.begin(); it1 != t_par.end(); it1++) {
	      if (flag) cout << " ";
	      cout << (*it1);
	      flag = true;
	    }
	}
	cout << endl;
}

int main() {
	testAgendaService();
	return 0;
}