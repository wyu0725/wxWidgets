/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/dialog.cpp
// Author:      Peter Most, Javier Torres, Mariano Reingart, Sean D'Epagnier
// Copyright:   (c) 2010 wxWidgets dev team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/dialog.h"
#include "wx/qt/private/utils.h"
#include "wx/qt/private/winevent.h"

#include <QtWidgets/QDialog>

class wxQtDialog : public wxQtEventSignalHandler< QDialog, wxDialog >
{

    public:
        wxQtDialog( wxWindow *parent, wxDialog *handler );
};


wxQtDialog::wxQtDialog( wxWindow *parent, wxDialog *handler )
    : wxQtEventSignalHandler< QDialog, wxDialog >( parent, handler )
{
}

wxDialog::wxDialog()
{
}

wxDialog::wxDialog( wxWindow *parent, wxWindowID id,
        const wxString &title,
        const wxPoint &pos,
        const wxSize &size,
        long style,
        const wxString &name)
{
    Create( parent, id, title, pos, size, style, name );
}

wxDialog::~wxDialog()
{
}


bool wxDialog::Create( wxWindow *parent, wxWindowID id,
        const wxString &title,
        const wxPoint &pos,
        const wxSize &size,
        long style,
        const wxString &name)
{
    SetExtraStyle(GetExtraStyle() | wxTOPLEVEL_EX_DIALOG);

    // all dialogs should have tab traversal enabled
    style |= wxTAB_TRAVERSAL;

    m_qtWindow = new wxQtDialog( parent, this );
    
    PostCreation();

    return wxTopLevelWindow::Create( parent, id, title, pos, size, style, name );
}

int wxDialog::ShowModal()
{
    wxCHECK_MSG( GetHandle() != NULL, -1, "Invalid dialog" );

    bool ret = GetDialogHandle()->exec();
    if ( GetReturnCode() == 0 )
        return ret ? wxID_OK : wxID_CANCEL;
    return GetReturnCode();
}

void wxDialog::EndModal(int retCode)
{
    wxCHECK_RET( GetDialogHandle() != NULL, "Invalid dialog" );

    SetReturnCode(retCode);
    GetDialogHandle()->done( QDialog::Accepted );
}

bool wxDialog::IsModal() const
{
    wxCHECK_MSG( GetDialogHandle() != NULL, false, "Invalid dialog" );

    return GetDialogHandle()->isModal();
}

QDialog *wxDialog::GetDialogHandle() const
{
    return static_cast<QDialog*>(m_qtWindow);
}

