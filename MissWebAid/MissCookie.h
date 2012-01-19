#ifndef MISSCOOKIE_H
#define MISSCOOKIE_H

#include <string>
#include <vector>

class MissCookie
{
public:
    struct MCookie
    {
        std::string name;
        std::string value;
        std::string domain;
        std::string path;
        time_t expires;
        bool tail;
        bool secure;
    };
public:
    MissCookie();
    virtual ~MissCookie();
protected:
private:
public:
    static MCookie GetCookieInfo(const std::string &str_cookie);
    static std::vector<std::string> splitCookieStr(const std::string &str);
    static std::vector<std::string> &split_cookie_str(const std::string &str, std::vector<std::string> &in);
    static int StrToInt(const std::string &str);
};

#endif // MISSCOOKIE_H
