#include "WebLogin.h"

WebLogin::WebLogin( wxWindow* parent )
:
WebLoginBase( parent )
{

}

void WebLogin::OnBtnLogin( wxCommandEvent& event )
{
// TODO: Implement OnBtnLogin
    EndModal (wxID_OK);
}

wxString WebLogin::GetUser()
{
    return m_edtUser->GetValue();
}

wxString WebLogin::GetPass()
{
    return m_edtPass->GetValue();
}
