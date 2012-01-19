///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 30 2011)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __GUIFRAME_H__
#define __GUIFRAME_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/listctrl.h>
#include <wx/panel.h>
#include <wx/statusbr.h>
#include <wx/gauge.h>
#include <wx/frame.h>
#include <wx/aui/aui.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

#define idMenuQuit 1000
#define idMenuAbout 1001

///////////////////////////////////////////////////////////////////////////////
/// Class GUIFrame
///////////////////////////////////////////////////////////////////////////////
class GUIFrame : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* mbar;
		wxMenu* fileMenu;
		wxMenu* helpMenu;
		wxPanel* m_panTask;
		wxButton* m_btnStartDown;
		wxButton* m_btnStop;
		wxListCtrl* m_listCurrentTask;
		wxMenu* m_menuCurrentTask;
		wxStatusBar* statusBar;
		wxListCtrl* m_listIndex;
		wxMenu* m_menuIndex;
		wxGauge* m_gaugeCurrent;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnLogin( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOpenIndexUrl( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnQuit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAbout( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnStartDownClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnStopClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuDeleteSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuDeleteAllSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnListIndexItemActivated( wxListEvent& event ) { event.Skip(); }
		virtual void OnMenuSelectItemSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuSelectAllSelection( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		GUIFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("迷失漫画下载器 v0.1 内部测试版"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 715,606 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		wxAuiManager m_mgr;
		
		~GUIFrame();
		
		void m_listCurrentTaskOnContextMenu( wxMouseEvent &event )
		{
			m_listCurrentTask->PopupMenu( m_menuCurrentTask, event.GetPosition() );
		}
		
		void m_listIndexOnContextMenu( wxMouseEvent &event )
		{
			m_listIndex->PopupMenu( m_menuIndex, event.GetPosition() );
		}
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class WebLoginBase
///////////////////////////////////////////////////////////////////////////////
class WebLoginBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_lblUser;
		wxTextCtrl* m_edtUser;
		wxStaticText* m_lblPass;
		wxTextCtrl* m_edtPass;
		wxButton* m_btnLogin;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnBtnLogin( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		WebLoginBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("登录网站"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~WebLoginBase();
	
};

#endif //__GUIFRAME_H__
