#ifndef MISSCOOKIES_H
#define MISSCOOKIES_H
#include <tr1/memory>
#include <vector>

class MissCookies
{
    class MissCookiesImpl;
    public:
        MissCookies();
        virtual ~MissCookies();
        void LoadCookies(const std::string &strFile);
        bool SaveCookies(const std::string &strFile);

        std::vector<std::string>& GetCookies();

        bool GetCookiesValue(const std::string &strKey,std::string &strValue);
    protected:
    private:
        std::tr1::shared_ptr<MissCookiesImpl> m_pImpl;
};

#endif // MISSCOOKIES_H
