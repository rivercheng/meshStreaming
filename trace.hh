#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define SEPERATOR "_"
#define EXT ".txt"
#define USERINFO "DEFAULT"

#define USER_TRACE "behavior"
#define PAKT_TRACE "packet"
using namespace std;

class Trace
{ 
public:
	Trace();
	string genTraceFileName(string model_name,string type);
	void setUserID(string userid);
	string getUserID();

private:
	string _userid;
};


void Trace::setUserID(string userid) {
	_userid = userid;
}

string Trace::getUserID() {
	return _userid;
}


Trace::Trace() {
	setUserID(USERINFO);
}

//A model name + date time provide a unique filename
string genTraceFileName(string model_name,string type){

	time_t t=time(NULL);
	tm *now=localtime(&t);
	
	ostringstream year;
	year << now->tm_year+1900;
	
	ostringstream month;
	month << now->tm_mon+1;
	
	ostringstream day;
	day << now->tm_mday;
	
	ostringstream hour;
	hour << now->tm_hour;
	
	ostringstream min;
	min << now->tm_min;
	
	ostringstream sec;
	sec << now->tm_sec;
	
	string userinfo = USERINFO;
	
	string filename = userinfo + SEPERATOR + type + SEPERATOR + model_name + SEPERATOR + year.str() + SEPERATOR +
			month.str() + SEPERATOR + day.str() + SEPERATOR + hour.str() + SEPERATOR + min.str() + SEPERATOR + sec.str() + EXT ;

return filename;

}

/*

//Trace Tester
int main(int argc, char** argv)
{
	Trace trace;
	string filename = genTraceFileName("huge","user");
	cout<<filename<<endl;

	return 1;
}
*/