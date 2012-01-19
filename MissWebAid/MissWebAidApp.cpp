/***************************************************************
 * Name:      MissWebAidApp.cpp
 * Purpose:   Code for Application Class
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

#include "MissWebAidApp.h"
#include "MissWebAidMain.h"

IMPLEMENT_APP(MissWebAidApp);

bool MissWebAidApp::OnInit()
{
    wxImage::AddHandler( new wxPNGHandler );
    wxImage::AddHandler( new wxJPEGHandler );

    MissWebAidFrame* frame = new MissWebAidFrame(0L);
    frame->SetIcon(wxICON(aaaa)); // To Set App Icon
    frame->Show();

    return true;
}
