/***************************************************************
 * Name:      MissWebAidMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    leoxiaofei (fox88324@yahoo.com.cn)
 * Created:   2011-07-20
 * Copyright: leoxiaofei (http://www.leoxiaofei.com)
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "MissWebAidMain.h"
#include "MissFileWriter.h"
#include "WebLogin.h"
#include "MissCookies.h"
#include "MissUniteImage.h"
#include "MissWxBitmapWriter.h"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Infos.hpp>

#include <html/ParserDom.h>
#include <css/parser_pp.h>

#include <mBase64.hpp>
#include <mAlgorithm.hpp>

#include <sstream>
#include <fstream>

#include <wx/dirdlg.h>
#include <wx/textdlg.h>
#include <wx/msgdlg.h>
#include <wx/dir.h>
//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild;
        wxbuild <<wxT("迷失漫画下载器 v0.1\n");
        wxbuild <<wxT("http://www.leoxiaofei.com\n");
        wxbuild <<wxT("xxffsoft@163.com\n");
        wxbuild << wxVERSION_STRING;
    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}


struct PayImageInfos
{
    struct _Images
    {
        int nX;
        int nY;
        std::string strImageUrl;
    };

    struct _Points
    {
        int nX;
        int nY;
        std::string strColour;
    };
    int nWidth,nHeight;
    std::map<std::string,_Images>    mapImages;
    std::vector<_Points>             vecPoints;
};

class MissWebAidFrame::MissWebAidFrameImpl
{
public:
    MissWebAidFrameImpl():m_bStop(false){}
    MissCookies m_Cookies;
    bool        m_bStop;
    std::map<wxString, PayImageInfos> m_mapPayImageInfos;
};

MissWebAidFrame::MissWebAidFrame(wxFrame *frame)
    : GUIFrame(frame),
    m_pImpl(new MissWebAidFrameImpl)
{
#if wxUSE_STATUSBAR
    statusBar->SetStatusText(wxT("欢迎使用迷失漫画下载器!"), 0);
    statusBar->SetStatusText(wxT("请打开漫画目录网址。"), 1);
#endif
    m_listIndex->InsertColumn(0,_T("ID"),wxLIST_FORMAT_LEFT,35);
    m_listIndex->InsertColumn(1,_T("章节名"),wxLIST_FORMAT_LEFT,150);
    m_listIndex->InsertColumn(2,_T("网址"),wxLIST_FORMAT_LEFT,300);


    m_listCurrentTask->InsertColumn(0,_T("S"),wxLIST_FORMAT_LEFT,25);
    m_listCurrentTask->InsertColumn(1,_T("章节"),wxLIST_FORMAT_LEFT,100);
    m_listCurrentTask->InsertColumn(2,_T("页数"),wxLIST_FORMAT_LEFT,50);
    m_listCurrentTask->InsertColumn(3,_T("网址"),wxLIST_FORMAT_LEFT,100);
    m_listCurrentTask->InsertColumn(4,_T("收费"),wxLIST_FORMAT_LEFT,50);
    m_pImpl->m_Cookies.LoadCookies("cookie.xml");
}

MissWebAidFrame::~MissWebAidFrame()
{
}

void MissWebAidFrame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void MissWebAidFrame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void MissWebAidFrame::OnLogin(wxCommandEvent& event)
{
    WebLogin LoginDlg(this);
    if( LoginDlg.ShowModal() == wxID_OK )
    {
        ///http://passport.u17.com/member/login.php
        statusBar->SetStatusText(wxT("正在登陆你的帐号..."), 1);
        try
        {
            //http://www.tc.ymcahk.org.hk/organic_farm/pic/PHOTO/Organic%20Crops2.JPG
            curlpp::Cleanup myCleanup;
            curlpp::Easy request;

            request.setOpt(new curlpp::options::Url("http://passport.u17.com/member/login.php"));
            std::list<std::string> header;
            header.push_back("application/x-www-form-urlencoded; charset=UTF-8");
            request.setOpt(new curlpp::options::HttpHeader(header));

            std::ostringstream os;
            curlpp::options::WriteStream ws(&os);
            request.setOpt(ws);

            request.setOpt(new curlpp::options::NoProgress(false));
            curlpp::types::ProgressFunctionFunctor functor(this, &MissWebAidFrame::progress_func);
            request.setOpt(new curlpp::options::ProgressFunction(functor));

            request.setOpt(new curlpp::options::CookieFile("u17.com"));
            {
                // Forms takes ownership of pointers!

                curlpp::Forms formParts;
                formParts.push_back(new curlpp::FormParts::Content("username", LoginDlg.GetUser().ToStdString()));
                formParts.push_back(new curlpp::FormParts::Content("password", LoginDlg.GetPass().ToStdString()));
                formParts.push_back(new curlpp::FormParts::Content("remember_username", "on"));
                formParts.push_back(new curlpp::FormParts::Content("save_login", "on"));

                request.setOpt(new curlpp::options::HttpPost(formParts));
            }

            request.perform();
            ///判断登陆是否成功
            GetCookies(request,m_pImpl->m_Cookies);
            std::string strCookieName;
            if(m_pImpl->m_Cookies.GetCookiesValue("xxusername",strCookieName)
               && strCookieName == LoginDlg.GetUser().ToStdString())
            {
                statusBar->SetStatusText(wxT("登录成功。"), 1);
                m_pImpl->m_Cookies.SaveCookies("cookie.xml");
            }
            else
            {
                statusBar->SetStatusText(wxT("用户名或密码错误。"), 1);
            }
        }
        catch( curlpp::RuntimeError &e )
        {
            std::cout << e.what() << std::endl;
        }
        catch( curlpp::LogicError &e )
        {
            std::cout << e.what() << std::endl;
        }
    }
}

int MissWebAidFrame::progress_func(double rDlTotal, double rDlNow, double rUlTotal, double rUlNow)
{
    m_gaugeCurrent->SetValue(100 * (rDlNow/rDlTotal));
    wxYield();
    if(m_pImpl->m_bStop != true)
    {
        return 0;
    }
    else
    {
        m_pImpl->m_bStop = false;
        return 1;
    }
    return 0;
}

void MissWebAidFrame::GetCookies( curlpp::Easy& exEasy, MissCookies& vecCookies)
{
    std::list<std::string> cookies;
    curlpp::infos::CookieList::get(exEasy, cookies);
    std::vector<std::string> &vecDes = vecCookies.GetCookies();
    vecDes.clear();
    std::copy(cookies.begin(),cookies.end(),std::back_inserter(vecDes));
}

void MissWebAidFrame::SetCookies(curlpp::Easy& exEasy, MissCookies& vecCookies)
{
    for (std::vector<std::string>::iterator it = vecCookies.GetCookies().begin();
    it != vecCookies.GetCookies().end(); ++it)
    {
        exEasy.setOpt(curlpp::options::CookieList(*it));
    }
}

void MissWebAidFrame::OnOpenIndexUrl(wxCommandEvent& event)
{
    wxString cmd = wxGetTextFromUser(wxT("请输入有妖气漫画的目录网址:"),
                                     wxT("Input text"),
                                     wxT("http://www.u17.com/comic/3420.html"),
                                     this);
    if(cmd.IsEmpty())
    {
        return ;
    }
    m_listIndex->DeleteAllItems();
    statusBar->SetStatusText(wxT("正在加载漫画目录..."), 1);
    try
	{
	    //http://www.tc.ymcahk.org.hk/organic_farm/pic/PHOTO/Organic%20Crops2.JPG
		curlpp::Cleanup myCleanup;
        curlpp::Easy request;

        //request.setOpt(new curlpp::options::Url("http://www.3qit.com/picture/article/67/66487.jpg"));
        request.setOpt(new curlpp::options::Url(cmd.ToStdString()));
        SetCookies(request,m_pImpl->m_Cookies);

        std::ostringstream os;
        curlpp::options::WriteStream ws(&os);
        request.setOpt(ws);

        ///保存成文件
        ///MissFileWriter filer;
        ///request.setOpt(new curlpp::options::WriteFunction(curlpp::types::WriteFunctionFunctor(&filer,&MissFileWriter::WriteFileCallback)));

        request.setOpt(new curlpp::options::NoProgress(false));
        curlpp::types::ProgressFunctionFunctor functor(this, &MissWebAidFrame::progress_func);
        request.setOpt(new curlpp::options::ProgressFunction(functor));
        request.perform();
        //std::cout<< os.str() <<std::endl;

        htmlcxx::HTML::ParserDom parser;
        tree<htmlcxx::HTML::Node> dom = parser.parseTree(os.str());
        tree<htmlcxx::HTML::Node>::iterator it = dom.begin();
        tree<htmlcxx::HTML::Node>::iterator end = dom.end();

        for (int nItemIndex(0); it != end; ++it)
        {
            if (strcasecmp(it->tagName().c_str(), "a") == 0)
            {
                it->parseAttributes();
                if( it->attribute("class").second == "chapter_name")
                {
                    //std::cout<<"chapter_name" <<std::endl;
                    m_listIndex->InsertItem(nItemIndex, wxEmptyString);
                    m_listIndex->SetItem(nItemIndex,0,wxString::Format(wxT("%d"),nItemIndex+1));
                    m_listIndex->SetItem(nItemIndex,1,wxString(it->attribute("title").second.c_str(),wxConvUTF8));
                    m_listIndex->SetItem(nItemIndex,2,wxString(it->attribute("href").second));
                    nItemIndex++;
                }
            }
        }
	}
    catch( curlpp::RuntimeError &e )
	{
		std::cout << e.what() << std::endl;
	}
	catch( curlpp::LogicError &e )
	{
		std::cout << e.what() << std::endl;
	}
	statusBar->SetStatusText(wxT("加载完成，请选择所要下载的章节"), 1);
}

void MissWebAidFrame::OnAbout(wxCommandEvent &event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
    return;
    try
    {
        ///http://user.u17.com/
        curlpp::Cleanup myCleanup;
        curlpp::Easy request;
        request.setOpt(new curlpp::options::Url("http://user.u17.com/"));
        std::ostringstream oss;
        curlpp::options::WriteStream wss(&oss);
        request.setOpt(wss);
        SetCookies(request,m_pImpl->m_Cookies);
        request.perform();
        std::cout<<oss.str()<<std::endl;
    }
    catch( curlpp::RuntimeError &e )
    {
        std::cout << e.what() << std::endl;
    }
    catch( curlpp::LogicError &e )
    {
        std::cout << e.what() << std::endl;
    }
}

void MissWebAidFrame::OnListIndexItemActivated(wxListEvent& event)
{
    m_btnStartDown->Enable(false);
    wxListItem Row_Info;
    Row_Info.m_itemId = event.GetIndex();
    Row_Info.m_col = 2;
    Row_Info.m_mask = wxLIST_MASK_TEXT;
    m_listIndex->GetItem(Row_Info);
    //std::cout<<Row_Info.m_text.utf8_str()<<std::endl;
    ParsePicPath(Row_Info.m_text, wxString::Format(wxT("%s"),m_listIndex->GetItemText(event.GetIndex(),1))); ///event.GetIndex() + 1
    m_btnStartDown->Enable(true);
}

void MissWebAidFrame::ParsePicPath(const wxString& strIndexPath, const wxString& strChapter)
{
    try
	{
	    //http://www.tc.ymcahk.org.hk/organic_farm/pic/PHOTO/Organic%20Crops2.JPG
		curlpp::Cleanup myCleanup;
        curlpp::Easy request;
        std::string strPath = strIndexPath.ToStdString();
        std::string strFileType = ".html";
        bool bFree(true);
        request.setOpt(new curlpp::options::Url(strPath));
        SetCookies(request,m_pImpl->m_Cookies);


        int nStrPathBase = strPath.rfind('.');
        strPath = strPath.substr(0,nStrPathBase) + "_i";

        request.setOpt(new curlpp::options::Header(true));


        request.setOpt(new curlpp::options::NoProgress(false));
        curlpp::types::ProgressFunctionFunctor functor(this, &MissWebAidFrame::progress_func);
        request.setOpt(new curlpp::options::ProgressFunction(functor));

        std::ostringstream os;
        curlpp::options::WriteStream ws(&os);
        request.setOpt(ws);
        request.perform();

        //std::cout<<os.str()<<std::endl;
        std::string strHeader = os.str();
        strHeader = strHeader.substr(0,strHeader.find("\r\n\r\n"));
        ///这里需要分析是否是付费页面
        //std::cout<<"------start------"<<std::endl;
        //std::cout<<strHeader<<std::endl;
        //std::cout<<"------end------"<<std::endl;

        int nFind = strHeader.find("location:");
        if(nFind != -1)
        {
            strHeader = strHeader.substr(nFind+10 ,strHeader.find("\r\n",nFind + 10) - nFind - 10);
            std::cout<<strHeader<<std::endl;
            if( std::string::npos != strHeader.find("buy_chapter"))
            {
                statusBar->SetStatusText(wxT("这章漫画你没有购买，无法下载。"), 1);
                return;
            }
            os.str("");
            request.setOpt(new curlpp::options::Url(strHeader));
            request.perform();
            strFileType = ".shtml";
            bFree = false;
        }
        statusBar->SetStatusText(wxT("正在分析漫画页面..."), 1);
        std::vector<std::string> m_vecChapterUrls;
        htmlcxx::HTML::ParserDom parser;
        tree<htmlcxx::HTML::Node> dom = parser.parseTree(os.str());
        tree<htmlcxx::HTML::Node>::iterator it = dom.begin();
        tree<htmlcxx::HTML::Node>::iterator end = dom.end();
        for (; it != end; ++it)
        {
            if(it->tagName() == "select")
            {
                std::cout<<"find select"<<std::endl;
                it->parseAttributes();
                if( it->attribute("id").second == "comicShow")
                {
                    std::cout<<"find id"<<std::endl;
                    //std::cout<<it->closingText()<<std::endl;
                    dom = parser.parseTree(os.str().substr(it->offset(),it->length()));
                    it = dom.begin();
                    end = dom.end();
                    for (; it != end; ++it)
                    {
                        if(it->tagName() == "option")
                        {
                             it->parseAttributes();
                             //std::cout<< strPath + it->attribute("value").second + ".html" <<std::endl;
                             m_vecChapterUrls.push_back(strPath + it->attribute("value").second + strFileType);
                        }
                    }
                    break;
                }
            }
        }

        statusBar->SetStatusText(wxT("正在获取图片地址..."), 1);
        int nItemIndex(m_listCurrentTask->GetItemCount());
        int nPage(0);
        for(std::vector<std::string>::iterator itor = m_vecChapterUrls.begin();
            itor != m_vecChapterUrls.end(); ++itor)
        {
            Sleep(100);

            std::cout<<*itor<<std::endl;

            //request.reset();
            os.str("");
            request.setOpt(new curlpp::options::Url(*itor));
            request.perform();


            m_listCurrentTask->InsertItem(nItemIndex, wxEmptyString);
            m_listCurrentTask->SetItem(nItemIndex,0,wxT("■"));
            m_listCurrentTask->SetItem(nItemIndex,1,strChapter);
            m_listCurrentTask->SetItem(nItemIndex,2,wxString::Format(wxT("%.2d"),++nPage));
            m_listCurrentTask->SetItem(nItemIndex,3,bFree?
                                       GetImageUrlByPageStr(os.str()):
                                       GetPayImageUrlByPageStr(os.str(),
                                        wxString::Format(wxT("%.2d_%.2d"),nItemIndex,nPage)));
            m_listCurrentTask->SetItem(nItemIndex,4,bFree?wxT("×"):wxT("√"));
            ++nItemIndex;
        }
        statusBar->SetStatusText(wxT("恭喜,获取图片地址完成!"), 1);
	}
    catch( curlpp::RuntimeError &e )
	{
		std::cout << e.what() << std::endl;
	}
	catch( curlpp::LogicError &e )
	{
		std::cout << e.what() << std::endl;
	}
}

wxString MissWebAidFrame::GetImageUrlByPageStr(const std::string &strPage)
{
    //std::cout<<strPage<<std::endl;
    wxString strRet;
    htmlcxx::HTML::ParserDom parser;
    tree<htmlcxx::HTML::Node> dom = parser.parseTree(strPage);
    tree<htmlcxx::HTML::Node>::iterator it = dom.begin();
    tree<htmlcxx::HTML::Node>::iterator end = dom.end();
    for (; it != end; ++it)
    {
        if(it->tagName() == "div")
        {
            //std::cout<<it->text()<<std::endl;
            it->parseAttributes();
            if(it->attribute("class").second == "comic_read_img")
            {
                std::cout<<"find comic_read_img"<<std::endl;
                std::string strContent = strPage.substr(it->offset(), it->length() );
                int nStart = strContent.find("write(eee('");
                int nEnd   = strContent.find("'));");
                if(nStart != -1 && nEnd != -1)
                {
                    strContent = strContent.substr(nStart + 11 , nEnd - nStart - 11);
                    strContent = Miss::base64_decode(strContent);

                    dom = parser.parseTree(strContent);
                    it = dom.begin();
                    end = dom.end();
                    for (; it != end; ++it)
                    {
                        if( it->tagName() == "img")
                        {
                            it->parseAttributes();
                            strRet = it->attribute("src").second;
                        }
                    }
                    //std::cout<<strRet<<std::endl;
                }
                break;
            }
        }
    }
    return strRet;
}

wxString MissWebAidFrame::GetPayImageUrlByPageStr(const std::string& strPage, const wxString& Key)
{
    htmlcxx::HTML::ParserDom parser;
    tree<htmlcxx::HTML::Node> dom = parser.parseTree(strPage);
    tree<htmlcxx::HTML::Node>::iterator it = dom.begin();
    tree<htmlcxx::HTML::Node>::iterator end = dom.end();
    for (; it != end; ++it)
    {
        if(it->tagName() == "div")
        {
            //std::cout<<it->text()<<std::endl;
            it->parseAttributes();
            if(it->attribute("class").second == "comic_read_img")
            {
                //std::cout<<"find comic_read_img"<<std::endl;

                std::vector<std::string> vecInfo = Miss::SplitStr(it->attribute("style").second,';');
                //std::copy(vecInfo.begin(),vecInfo.end(),std::ostream_iterator<std::string>(std::cout,"\n"));
                int nFind (-1);
                PayImageInfos payImageInfos;
                PayImageInfos::_Points pointInfo;
                ///分析style，得到图片高宽
                for(unsigned int ix=0; ix != vecInfo.size(); ++ix)
                {
                    nFind = vecInfo[ix].find("width:");
                    if(nFind != -1)
                    {
                        payImageInfos.nWidth = StrToInt(vecInfo[ix].substr(6));
                    }
                    else
                    {
                        nFind = vecInfo[ix].find("height:");
                        if(nFind != -1)
                        {
                            payImageInfos.nHeight = StrToInt(vecInfo[ix].substr(7));
                        }
                    }
                }

                int offset = it->offset() + it->text().length();
                int length = it->length() - it->text().length() - it->closingText().length();

                ///分析小图片位置和路径
                std::string strHtml = strPage.substr(offset, length);
                dom = parser.parseTree(strHtml);
                it = dom.begin();
                end = dom.end();
                for (; it != end; ++it)
                {
                    if(it->tagName() == "div")
                    {
                        it->parseAttributes();
                        if(it->attribute("class").first)
                        {
                            payImageInfos.mapImages[it->attribute("class").second];
                        }
                        else
                        {
                            ///处理绘制点的问题
                            ///分析点的位置和颜色
                            vecInfo = Miss::SplitStr(it->attribute("style").second,';');
                            for(unsigned int ix=0; ix != vecInfo.size(); ++ix)
                            {
                                nFind = vecInfo[ix].find("left:");
                                if(nFind != -1)
                                {
                                    pointInfo.nX = StrToInt(vecInfo[ix].substr(5));
                                }
                                else
                                {
                                    nFind = vecInfo[ix].find("top:");
                                    if(nFind != -1)
                                    {
                                        pointInfo.nY = StrToInt(vecInfo[ix].substr(4));
                                    }
                                    else
                                    {
                                        nFind = vecInfo[ix].find("background-color:");
                                        if(nFind != -1)
                                        {
                                            pointInfo.strColour = vecInfo[ix].substr(17);
                                        }
                                    }
                                }
                            }
                            payImageInfos.vecPoints.push_back(pointInfo);
                        }
                    }
                    else if(it->tagName() == "img")
                    {
                        it->parseAttributes();
                        payImageInfos.mapImages[it->attribute("class").second].strImageUrl
                            = it->attribute("src").second;
                    }
                    else if(it->tagName() == "style")
                    {
                        htmlcxx::CSS::Parser parser;
                        htmlcxx::CSS::Parser::Selector selector;
                        std::vector<htmlcxx::CSS::Parser::Selector> vecSel;
                        vecSel.push_back(selector);

                        std::map<std::string, std::string> mAttributes;

                        offset = it->offset() + it->text().length();
                        length = it->length() - it->text().length() - it->closingText().length();

                        std::string strCSS = strHtml.substr(offset, length);
                        //std::cout<<strCSS<<std::endl;
                        parser.parse(strCSS);

                        for(std::map<std::string,PayImageInfos::_Images>::iterator itor =
                            payImageInfos.mapImages.begin();itor != payImageInfos.mapImages.end(); ++itor)
                        {
                            vecSel[0].setClass(itor->first);
                            mAttributes = parser.getAttributes(vecSel);

                            itor->second.nX = StrToInt(mAttributes["left"]);
                            itor->second.nY = StrToInt(mAttributes["top"]);
                            std::map<std::string, std::string>::iterator itorFind =
                                mAttributes.find("background-image");
                            if(itorFind != mAttributes.end())
                            {
                                itor->second.strImageUrl = itorFind->second.substr(5,itorFind->second.size()-7);
                                //std::cout<<itorFind->second.substr(5,itorFind->second.size()-7)<<std::endl;
                            }

                            //std::cout<<itor->first << "->"<<StrToInt(mAttributes["left"])<<":"<<StrToInt(mAttributes["top"])<<std::endl;
                        }
                        break;
                    }
                }

                m_pImpl->m_mapPayImageInfos[Key] = payImageInfos;
                break;
            }
        }
    }
    return Key;
}

void MissWebAidFrame::OnBtnStartDownClick(wxCommandEvent& event)
{
    wxDirDialog dlg(NULL, "Choose input directory", "",
                        wxDD_DEFAULT_STYLE );
    if(dlg.ShowModal() != wxID_OK)
    {
        return;
    }

    wxString strSaveDirPath = dlg.GetPath() + "/";

    statusBar->SetStatusText(wxT("正在下载漫画图片..."), 1);
    m_btnStop->Enable(true);
    m_btnStartDown->Enable(false);
    try
	{
	    //http://www.tc.ymcahk.org.hk/organic_farm/pic/PHOTO/Organic%20Crops2.JPG
		curlpp::Cleanup myCleanup;
        curlpp::Easy request;

        //request.setOpt(new curlpp::options::Url("http://www.3qit.com/picture/article/67/66487.jpg"));
        //request.setOpt(new curlpp::options::Url("http://www.u17.com/comic/3420.html"));

        request.setOpt(new curlpp::options::NoProgress(false));
        curlpp::types::ProgressFunctionFunctor functor(this, &MissWebAidFrame::progress_func);
        request.setOpt(new curlpp::options::ProgressFunction(functor));
        SetCookies(request,m_pImpl->m_Cookies);

        ///保存成文件
        ///MissFileWriter filer;
        ///request.setOpt(new curlpp::options::WriteFunction(curlpp::types::WriteFunctionFunctor(&filer,&MissFileWriter::WriteFileCallback)));
        std::string strImagePath,strSavePath;
        int nCount = m_listCurrentTask->GetItemCount();
        for(int ix = 0; ix < nCount; ++ix)
        {
            if(m_listCurrentTask->GetItemText(ix) != wxT("★"))
            {
                m_listCurrentTask->SetItem(ix,0,wxT("→"));
                strSavePath = strSaveDirPath + m_listCurrentTask->GetItemText(ix,1) + wxT("/");
                if(!wxDir::Exists(strSavePath))
                {
                    wxDir::Make(strSavePath);
                }

                if(m_listCurrentTask->GetItemText(ix,4) != wxT("√"))
                {
                    strImagePath = m_listCurrentTask->GetItemText(ix,3).ToStdString();
                    strSavePath = strSavePath + m_listCurrentTask->GetItemText(ix,2).ToStdString() + strImagePath.substr(strImagePath.rfind("."));

                    //std::cout<<strSavePath<<std::endl;

                    MissFileWriter filer(strSavePath);
                    request.setOpt(new curlpp::options::WriteFunction(curlpp::types::WriteFunctionFunctor(&filer,&MissFileWriter::WriteFileCallback)));
                    request.setOpt(new curlpp::options::Url(strImagePath));
                    request.perform();
                }
                else
                {
                    ///付费版的图片下载
                    ///保存图像
                    std::map<wxString, PayImageInfos>::iterator itorFind =
                        m_pImpl->m_mapPayImageInfos.find(m_listCurrentTask->GetItemText(ix,3));
                    if(itorFind != m_pImpl->m_mapPayImageInfos.end())
                    {
                        MissUniteImage file;
                        MissUniteImage::ImageInfo imageInfo;
                        MissUniteImage::PointInfo pointInfo;
                        file.SetWH(itorFind->second.nWidth,itorFind->second.nHeight);
                        for(std::map<std::string, PayImageInfos::_Images>::iterator itor =
                             itorFind->second.mapImages.begin(); itor != itorFind->second.mapImages.end();
                            ++itor)
                        {
                            imageInfo.nX = itor->second.nX;
                            imageInfo.nY = itor->second.nY;
                            imageInfo.bImage = GetImageByUrl(request,itor->second.strImageUrl);
                            file.AddImage(imageInfo);
                        }
                        for(std::vector<PayImageInfos::_Points>::iterator itor =
                            itorFind->second.vecPoints.begin(); itor != itorFind->second.vecPoints.end();
                            ++itor)
                        {
                            pointInfo.nX = itor->nX;
                            pointInfo.nY = itor->nY;
                            pointInfo.cPoint.Set(itor->strColour);
                            file.AddPoint(pointInfo);
                        }
                        strSavePath = strSavePath + m_listCurrentTask->GetItemText(ix,2).ToStdString() + ".png";
                        file.UniteImage(strSavePath);
                    }
                    else
                    {
                        assert(false);
                    }
                }
                m_listCurrentTask->SetItem(ix,0,wxT("★"));

            }
        }



	}
    catch( curlpp::RuntimeError &e )
	{
		std::cout << e.what() << std::endl;
	}
	catch( curlpp::LogicError &e )
	{
		std::cout << e.what() << std::endl;
	}
	m_btnStop->Enable(false);
	m_btnStartDown->Enable(true);
	statusBar->SetStatusText(wxT("恭喜，下载完成!"), 1);
}

void MissWebAidFrame::OnBtnStopClick(wxCommandEvent& event)
{
    m_pImpl->m_bStop = true;
    m_btnStop->Enable(false);
}

void MissWebAidFrame::OnMenuDeleteSelection(wxCommandEvent& event)
{
    long item = -1;
    std::vector<int> vecDeleteItem;
    while ( 1 )
    {
        item = m_listCurrentTask->GetNextItem(item,
                                        wxLIST_NEXT_ALL,
                                        wxLIST_STATE_SELECTED);
        if ( item == -1 )
            break;
        vecDeleteItem.push_back(item);
        if(wxT("√") == m_listCurrentTask->GetItemText(item,4))
        {
            std::map<wxString, PayImageInfos>::iterator iFind =
                m_pImpl->m_mapPayImageInfos.find(m_listCurrentTask->GetItemText(item,3));
            if(iFind != m_pImpl->m_mapPayImageInfos.end())
            {
                m_pImpl->m_mapPayImageInfos.erase(iFind);
            }
            else
            {
                assert(false);
            }
        }
    }

    for(std::vector<int>::reverse_iterator ritor = vecDeleteItem.rbegin();
        ritor != vecDeleteItem.rend(); ++ritor )
    {
        m_listCurrentTask->DeleteItem(*ritor);
    }
}

void MissWebAidFrame::OnMenuDeleteAllSelection(wxCommandEvent& event)
{
    m_listCurrentTask->DeleteAllItems();
    m_pImpl->m_mapPayImageInfos.clear();
}

void MissWebAidFrame::OnMenuSelectItemSelection(wxCommandEvent& event)
{
    m_btnStartDown->Enable(false);
    long item = -1;
    while ( 1 )
    {
        item = m_listIndex->GetNextItem(item,
                                        wxLIST_NEXT_ALL,
                                        wxLIST_STATE_SELECTED);
        if ( item == -1 )
            break;
        ParsePicPath(m_listIndex->GetItemText(item,2), wxString::Format(wxT("%s"),m_listIndex->GetItemText(item,1)));
    }
    m_btnStartDown->Enable(true);
}

void MissWebAidFrame::OnMenuSelectAllSelection(wxCommandEvent& event)
{
    m_btnStartDown->Enable(false);
    int nCount = m_listIndex->GetItemCount();
    for(int ix = 0; ix < nCount; ++ix )
    {
        ParsePicPath(m_listIndex->GetItemText(ix,2), wxString::Format(wxT("%s"),m_listIndex->GetItemText(ix,1)));
    }
    m_btnStartDown->Enable(true);
}

void MissWebAidFrame::OnMenuTestSelection(wxCommandEvent& event)
{
    std::ifstream in( "image/test.txt" );
    std::string strHtml( (std::istreambuf_iterator <char> (in)), (std::istreambuf_iterator <char> ()) );
}

wxBitmap MissWebAidFrame::GetImageByUrl(curlpp::Easy& exEasy, const std::string& strUrl)
{
    wxBitmap ret;
    MissWxBitmapWriter writer;
    exEasy.setOpt(new curlpp::options::WriteFunction(curlpp::types::WriteFunctionFunctor(&writer,&MissWxBitmapWriter::WriteCallback)));
    exEasy.setOpt(new curlpp::options::Url(strUrl));
    exEasy.perform();
    writer.GetBitmap(ret);
    return ret;
}

int MissWebAidFrame::StrToInt(const std::string& strPx)
{
    int ret = 0;
    std::istringstream strm(strPx);
    strm>>ret;
	return ret;
}


