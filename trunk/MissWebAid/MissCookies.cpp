#include "MissCookies.h"
#include <tinyxml.h>
#include <vector>
#include <string>
#include "MissCookie.h"

class MissCookies::MissCookiesImpl
{
public:
    std::vector<std::string> m_vecCookies;
};

MissCookies::MissCookies():
m_pImpl(new MissCookiesImpl)
{
    //ctor
}

MissCookies::~MissCookies()
{
    //dtor
}

 std::vector<std::string>& MissCookies::GetCookies()
{
    return m_pImpl->m_vecCookies;
}

void MissCookies::LoadCookies(const std::string &strFile)
{
    TiXmlDocument doc(strFile);
    doc.LoadFile();
    TiXmlElement* root = doc.FirstChildElement("Cookies");
    if(root)
    {
        TiXmlElement* CurElement = root->FirstChildElement();
        while(CurElement)
        {
            const char* CurStr = CurElement->GetText();
            std::cout<<CurStr<<std::endl;
            m_pImpl->m_vecCookies.push_back(CurStr);

            CurElement=CurElement->NextSiblingElement();
        }
    }

}

bool MissCookies::GetCookiesValue(const std::string& strKey, std::string& strValue)
{
    bool bRet(false);
    for(std::vector<std::string>::iterator itor = m_pImpl->m_vecCookies.begin();
        itor != m_pImpl->m_vecCookies.end(); ++itor)
    {
        MissCookie::MCookie cookie = MissCookie::GetCookieInfo(*itor);
        if(cookie.name == strKey)
        {
            bRet = true;
            strValue = cookie.value;
            break;
        }
    }
    return bRet;
}

bool MissCookies::SaveCookies(const std::string &strFile)
{
    bool ret(false);
    TiXmlDocument * xmlDocs = new TiXmlDocument();
    TiXmlDeclaration * xmlDec = new TiXmlDeclaration("1.0", "UTF-8", "yes");
    xmlDocs->LinkEndChild(xmlDec);
    TiXmlElement* root = new TiXmlElement("Cookies");
    xmlDocs->LinkEndChild(root);

    for(std::vector<std::string>::iterator itor = m_pImpl->m_vecCookies.begin();
    itor != m_pImpl->m_vecCookies.end(); ++itor)
    {
        TiXmlElement *elem = new TiXmlElement("Cookie");
        //elem->SetAttribute("显示",pSkin->GetElement(ix).m_Show?1:0);
        TiXmlText *elemText = new TiXmlText(*itor);
        elem->LinkEndChild(elemText);
        root->LinkEndChild(elem);
    }

    ret = xmlDocs->SaveFile(strFile);
    delete xmlDocs;
    return ret;
}
