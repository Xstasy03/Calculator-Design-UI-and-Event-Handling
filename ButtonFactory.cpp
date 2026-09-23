#include "ButtonFactory.h"
#include "ButtonIDs.h"

wxButton* ButtonFactory::CreateButton(wxWindow* parent, wxWindowID id,
    const wxString& label)
{
    return new wxButton(parent, id, label);
}

wxButton* ButtonFactory::CreateDigitButton(wxWindow* parent, int digit)
{
    const wxWindowID id = static_cast<wxWindowID>(ID_BTN_0 + digit);
    return CreateButton(parent, id, wxString::Format("%d", digit));
}

wxButton* ButtonFactory::CreateAddButton(wxWindow* parent)
{
    return CreateButton(parent, ID_BTN_ADD, "+");
}

wxButton* ButtonFactory::CreateSubButton(wxWindow* parent)
{
    return CreateButton(parent, ID_BTN_SUB, "-");
}

wxButton* ButtonFactory::CreateMulButton(wxWindow* parent)
{
    return CreateButton(parent, ID_BTN_MUL, "*");
}

wxButton* ButtonFactory::CreateDivButton(wxWindow* parent)
{
    return CreateButton(parent, ID_BTN_DIV, "/");
}

wxButton* ButtonFactory::CreateModButton(wxWindow* parent)
{
    return CreateButton(parent, ID_BTN_MOD, "%");
}

wxButton* ButtonFactory::CreateSinButton(wxWindow* parent)
{
    return CreateButton(parent, ID_BTN_SIN, "sin");
}

wxButton* ButtonFactory::CreateCosButton(wxWindow* parent)
{
    return CreateButton(parent, ID_BTN_COS, "cos");
}

wxButton* ButtonFactory::CreateTanButton(wxWindow* parent)
{
    return CreateButton(parent, ID_BTN_TAN, "tan");
}

wxButton* ButtonFactory::CreateEqualsButton(wxWindow* parent)
{
    return CreateButton(parent, ID_BTN_EQUALS, "=");
}

wxButton* ButtonFactory::CreateClearButton(wxWindow* parent)
{
    return CreateButton(parent, ID_BTN_CLEAR, "C");
}

wxButton* ButtonFactory::CreateBackspaceButton(wxWindow* parent)
{
    return CreateButton(parent, ID_BTN_BACKSPACE, "<-");
}

wxButton* ButtonFactory::CreateDecimalButton(wxWindow* parent)
{
    return CreateButton(parent, ID_BTN_DECIMAL, ".");
}

wxButton* ButtonFactory::CreateNegativeButton(wxWindow* parent)
{
    return CreateButton(parent, ID_BTN_NEGATIVE, "+/-");
}