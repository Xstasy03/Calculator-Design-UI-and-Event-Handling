#pragma once
#pragma once
#include <wx/wx.h>

class ButtonFactory
{
public:

    static wxButton* CreateButton(wxWindow* parent, wxWindowID id,
        const wxString& label);

    static wxButton* CreateDigitButton(wxWindow* parent, int digit);

    // Binary operators
    static wxButton* CreateAddButton(wxWindow* parent);
    static wxButton* CreateSubButton(wxWindow* parent);
    static wxButton* CreateMulButton(wxWindow* parent);
    static wxButton* CreateDivButton(wxWindow* parent);
    static wxButton* CreateModButton(wxWindow* parent);

    // Unary operators
    static wxButton* CreateSinButton(wxWindow* parent);
    static wxButton* CreateCosButton(wxWindow* parent);
    static wxButton* CreateTanButton(wxWindow* parent);

    // Everything else
    static wxButton* CreateEqualsButton(wxWindow* parent);
    static wxButton* CreateClearButton(wxWindow* parent);
    static wxButton* CreateBackspaceButton(wxWindow* parent);
    static wxButton* CreateDecimalButton(wxWindow* parent);
    static wxButton* CreateNegativeButton(wxWindow* parent);
};