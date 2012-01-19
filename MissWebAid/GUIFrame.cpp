///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 30 2011)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP

#include "GUIFrame.h"

///////////////////////////////////////////////////////////////////////////

GUIFrame::GUIFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	m_mgr.SetManagedWindow(this);
	
	mbar = new wxMenuBar( 0 );
	fileMenu = new wxMenu();
	wxMenuItem* m_menuLogin;
	m_menuLogin = new wxMenuItem( fileMenu, wxID_ANY, wxString( wxT("登录(&L)") ) , wxEmptyString, wxITEM_NORMAL );
	fileMenu->Append( m_menuLogin );
	
	wxMenuItem* m_menuOpenIndex;
	m_menuOpenIndex = new wxMenuItem( fileMenu, wxID_ANY, wxString( wxT("打开目录网址(&O)") ) , wxEmptyString, wxITEM_NORMAL );
	fileMenu->Append( m_menuOpenIndex );
	
	wxMenuItem* menuFileQuit;
	menuFileQuit = new wxMenuItem( fileMenu, idMenuQuit, wxString( wxT("退出(&Q)") ) + wxT('\t') + wxT("Alt+F4"), wxT("Quit the application"), wxITEM_NORMAL );
	fileMenu->Append( menuFileQuit );
	
	mbar->Append( fileMenu, wxT("功能(&F)") ); 
	
	helpMenu = new wxMenu();
	wxMenuItem* menuHelpAbout;
	menuHelpAbout = new wxMenuItem( helpMenu, idMenuAbout, wxString( wxT("&About") ) + wxT('\t') + wxT("F1"), wxT("Show info about this application"), wxITEM_NORMAL );
	helpMenu->Append( menuHelpAbout );
	
	mbar->Append( helpMenu, wxT("帮助(&H)") ); 
	
	this->SetMenuBar( mbar );
	
	m_panTask = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_mgr.AddPane( m_panTask, wxAuiPaneInfo() .Right() .CloseButton( false ).MaximizeButton( false ).MinimizeButton( false ).PinButton( true ).Dock().Resizable().FloatingSize( wxSize( 37,54 ) ).DockFixed( false ).Row( 1 ).BestSize( wxSize( 200,-1 ) ) );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	m_btnStartDown = new wxButton( m_panTask, wxID_ANY, wxT("开始下载"), wxDefaultPosition, wxDefaultSize, 0 );
	m_btnStartDown->Enable( false );
	
	bSizer4->Add( m_btnStartDown, 0, wxALL, 5 );
	
	m_btnStop = new wxButton( m_panTask, wxID_ANY, wxT("停止"), wxDefaultPosition, wxDefaultSize, 0 );
	m_btnStop->Enable( false );
	
	bSizer4->Add( m_btnStop, 0, wxALL, 5 );
	
	bSizer3->Add( bSizer4, 0, wxEXPAND, 5 );
	
	m_listCurrentTask = new wxListCtrl( m_panTask, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_REPORT|wxLC_VRULES );
	m_menuCurrentTask = new wxMenu();
	wxMenuItem* m_menuDelete;
	m_menuDelete = new wxMenuItem( m_menuCurrentTask, wxID_ANY, wxString( wxT("删除所选") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuCurrentTask->Append( m_menuDelete );
	
	wxMenuItem* m_menuDeleteAll;
	m_menuDeleteAll = new wxMenuItem( m_menuCurrentTask, wxID_ANY, wxString( wxT("删除全部") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuCurrentTask->Append( m_menuDeleteAll );
	
	m_listCurrentTask->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( GUIFrame::m_listCurrentTaskOnContextMenu ), NULL, this ); 
	
	bSizer3->Add( m_listCurrentTask, 1, wxALL|wxEXPAND, 5 );
	
	m_panTask->SetSizer( bSizer3 );
	m_panTask->Layout();
	bSizer3->Fit( m_panTask );
	statusBar = this->CreateStatusBar( 2, wxST_SIZEGRIP, wxID_ANY );
	m_listIndex = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_REPORT|wxLC_VRULES );
	m_mgr.AddPane( m_listIndex, wxAuiPaneInfo() .Left() .CloseButton( false ).MaximizeButton( false ).MinimizeButton( false ).PinButton( true ).Dock().Resizable().FloatingSize( wxDefaultSize ).DockFixed( false ).CentrePane() );
	
	m_menuIndex = new wxMenu();
	wxMenuItem* m_menuSelectItem;
	m_menuSelectItem = new wxMenuItem( m_menuIndex, wxID_ANY, wxString( wxT("加入选中任务") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuIndex->Append( m_menuSelectItem );
	
	wxMenuItem* m_menuSelectAll;
	m_menuSelectAll = new wxMenuItem( m_menuIndex, wxID_ANY, wxString( wxT("加入全部任务") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuIndex->Append( m_menuSelectAll );
	
	m_listIndex->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( GUIFrame::m_listIndexOnContextMenu ), NULL, this ); 
	
	m_gaugeCurrent = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL );
	m_mgr.AddPane( m_gaugeCurrent, wxAuiPaneInfo() .Bottom() .CloseButton( false ).MaximizeButton( false ).MinimizeButton( false ).PinButton( true ).Dock().Resizable().FloatingSize( wxSize( 116,62 ) ).DockFixed( false ).Row( 1 ) );
	
	
	m_mgr.Update();
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GUIFrame::OnClose ) );
	this->Connect( m_menuLogin->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnLogin ) );
	this->Connect( m_menuOpenIndex->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnOpenIndexUrl ) );
	this->Connect( menuFileQuit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnQuit ) );
	this->Connect( menuHelpAbout->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnAbout ) );
	m_btnStartDown->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::OnBtnStartDownClick ), NULL, this );
	m_btnStop->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::OnBtnStopClick ), NULL, this );
	this->Connect( m_menuDelete->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuDeleteSelection ) );
	this->Connect( m_menuDeleteAll->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuDeleteAllSelection ) );
	m_listIndex->Connect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( GUIFrame::OnListIndexItemActivated ), NULL, this );
	this->Connect( m_menuSelectItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuSelectItemSelection ) );
	this->Connect( m_menuSelectAll->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuSelectAllSelection ) );
}

GUIFrame::~GUIFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GUIFrame::OnClose ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnLogin ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnOpenIndexUrl ) );
	this->Disconnect( idMenuQuit, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnQuit ) );
	this->Disconnect( idMenuAbout, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnAbout ) );
	m_btnStartDown->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::OnBtnStartDownClick ), NULL, this );
	m_btnStop->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::OnBtnStopClick ), NULL, this );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuDeleteSelection ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuDeleteAllSelection ) );
	m_listIndex->Disconnect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( GUIFrame::OnListIndexItemActivated ), NULL, this );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuSelectItemSelection ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnMenuSelectAllSelection ) );
	
	m_mgr.UnInit();
	
	delete m_menuCurrentTask; 
	delete m_menuIndex; 
}

WebLoginBase::WebLoginBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	wxGridSizer* gSizer2;
	gSizer2 = new wxGridSizer( 0, 2, 0, 0 );
	
	m_lblUser = new wxStaticText( this, wxID_ANY, wxT("用户名:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblUser->Wrap( -1 );
	gSizer2->Add( m_lblUser, 0, wxALL, 5 );
	
	m_edtUser = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_edtUser->SetMaxLength( 30 ); 
	gSizer2->Add( m_edtUser, 0, wxALL, 5 );
	
	m_lblPass = new wxStaticText( this, wxID_ANY, wxT("密码:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblPass->Wrap( -1 );
	gSizer2->Add( m_lblPass, 0, wxALL, 5 );
	
	m_edtPass = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
	m_edtPass->SetMaxLength( 30 ); 
	gSizer2->Add( m_edtPass, 0, wxALL, 5 );
	
	bSizer2->Add( gSizer2, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_btnLogin = new wxButton( this, wxID_ANY, wxT("登录"), wxDefaultPosition, wxDefaultSize, 0 );
	m_btnLogin->SetDefault(); 
	bSizer3->Add( m_btnLogin, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	bSizer2->Add( bSizer3, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer2 );
	this->Layout();
	bSizer2->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_btnLogin->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WebLoginBase::OnBtnLogin ), NULL, this );
}

WebLoginBase::~WebLoginBase()
{
	// Disconnect Events
	m_btnLogin->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WebLoginBase::OnBtnLogin ), NULL, this );
	
}
