#include "MissCookie.h"
#include <sstream>
#include <exception>

MissCookie::MissCookie()
{
    //ctor
}

MissCookie::~MissCookie()
{
    //dtor
}

int MissCookie::StrToInt(const std::string& str)
{
    std::istringstream strm(str);
	int i = 0;
	if (!(strm >> i)) {
		throw std::bad_exception();//"Unable to convert string '" + str + "' to integer!"
	}
	return i;
}

MissCookie::MCookie MissCookie::GetCookieInfo(const std::string& str_cookie)
{
	std::vector<std::string> vC = splitCookieStr(str_cookie);
	MCookie cook;

	cook.domain = vC[0];
	cook.tail = vC[1] == "TRUE";
	cook.path = vC[2];
	cook.secure = vC[3] == "TRUE";
	cook.expires = StrToInt(vC[4]);
	cook.name = vC[5];
	cook.value = vC[6];

	return cook;
}
std::vector<std::string> MissCookie::splitCookieStr(const std::string& str)
{
    std::vector<std::string> split;
	split_cookie_str(str, split);
	return split;
}
std::vector<std::string>& MissCookie::split_cookie_str(const std::string& str, std::vector<std::string>& in)
{
    std::string part;

	std::istringstream strm(str);
	while (getline(strm, part, '\t'))
		in.push_back(part);

	return in;
}
