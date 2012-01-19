/***************************************************************
 * Name:      MissWebAidMain.h
 * Purpose:   Defines Application Frame
 * Author:    leoxiaofei (fox88324@yahoo.com.cn)
 * Created:   2011-07-20
 * Copyright: leoxiaofei (http://www.leoxiaofei.com)
 * License:
 **************************************************************/

#ifndef MISSWEBAIDMAIN_H
#define MISSWEBAIDMAIN_H



#include "MissWebAidApp.h"
#include <tr1/memory>


#include "GUIFrame.h"

class MissCookies;

namespace curlpp
{
    class Easy;
}

class MissWebAidFrame: public GUIFrame
{
    class MissWebAidFrameImpl;
    public:
        MissWebAidFrame(wxFrame *frame);
        ~MissWebAidFrame();
    private:
        virtual void OnClose(wxCloseEvent& event);
        virtual void OnQuit(wxCommandEvent& event);
        virtual void OnAbout(wxCommandEvent& event);
        virtual void OnLogin( wxCommandEvent& event );
        virtual void OnListIndexItemActivated( wxListEvent& event );
		virtual void OnBtnStartDownClick( wxCommandEvent& event );
		virtual void OnBtnStopClick( wxCommandEvent& event );
        virtual void OnOpenIndexUrl( wxCommandEvent& event );
		virtual void OnMenuDeleteSelection( wxCommandEvent& event );
		virtual void OnMenuDeleteAllSelection( wxCommandEvent& event );
		virtual void OnMenuSelectItemSelection( wxCommandEvent& event );
		virtual void OnMenuSelectAllSelection( wxCommandEvent& event );
        virtual void OnMenuTestSelection( wxCommandEvent& event );

        std::tr1::shared_ptr<MissWebAidFrameImpl> m_pImpl;
    public:
        int progress_func(double rDlTotal, double rDlNow, double rUlTotal, double rUlNow);

        static void GetCookies( curlpp::Easy &exEasy, MissCookies &vecCookies);
        static void SetCookies(curlpp::Easy &exEasy, MissCookies &vecCookies);

        wxString GetImageUrlByPageStr(const std::string &strPage);
        wxString GetPayImageUrlByPageStr(const std::string &strPage,const wxString &Key);

        void ParsePicPath(const wxString& strIndexPath, const wxString& strChapter);
        void ParsePayImagesInfo();
    protected:
        wxBitmap GetImageByUrl(curlpp::Easy& exEasy, const std::string &strUrl);
        int      StrToInt(const std::string& strPx);
};

#endif // MISSWEBAIDMAIN_H
