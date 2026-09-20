#include <wx/wx.h>
#include <wx/gbsizer.h>
enum
{
    ID_BTN_0 = wxID_HIGHEST + 1,
    ID_BTN_1,
    ID_BTN_2,
    ID_BTN_3,
    ID_BTN_4,
    ID_BTN_5,
    ID_BTN_6,
    ID_BTN_7,
    ID_BTN_8,
    ID_BTN_9,

    ID_BTN_ADD,
    ID_BTN_SUB,
    ID_BTN_MUL,
    ID_BTN_DIV,
    ID_BTN_MOD,

    ID_BTN_SIN,
    ID_BTN_COS,
    ID_BTN_TAN,

    ID_BTN_EQUALS,
    ID_BTN_CLEAR,
    ID_BTN_BACKSPACE,
    ID_BTN_DECIMAL,
    ID_BTN_NEGATIVE,

    ID_TXT_DISPLAY
};

// Frame
class CalculatorFrame : public wxFrame
{
public:
    CalculatorFrame();

private:
    wxTextCtrl* m_display = nullptr;

    // Helper to reduce repetition when building the grid of buttons.
    wxButton* AddButton(wxWindow* parent, wxGridBagSizer* sizer,
        wxWindowID id, const wxString& label,
        int col, int row,
        int colSpan = 1, int rowSpan = 1);
};

CalculatorFrame::CalculatorFrame()
    : wxFrame(nullptr, wxID_ANY, "Calculator",
        wxDefaultPosition, wxSize(340, 460))
{
    wxPanel* panel = new wxPanel(this);

    // Outer vertical sizer: display on top, button grid below.
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Text control (display)
    m_display = new wxTextCtrl(panel, ID_TXT_DISPLAY, "",
        wxDefaultPosition, wxSize(-1, 50),
        wxTE_RIGHT | wxTE_READONLY);
    wxFont displayFont = m_display->GetFont();
    displayFont.SetPointSize(displayFont.GetPointSize() + 6);
    m_display->SetFont(displayFont);

    mainSizer->Add(m_display, 0, wxEXPAND | wxALL, 10);

    // Button grid
    wxGridBagSizer* grid = new wxGridBagSizer(6, 6);

    AddButton(panel, grid, ID_BTN_SIN, "sin", 0, 0);
    AddButton(panel, grid, ID_BTN_COS, "cos", 1, 0);
    AddButton(panel, grid, ID_BTN_TAN, "tan", 2, 0);
    AddButton(panel, grid, ID_BTN_CLEAR, "C", 3, 0);
    AddButton(panel, grid, ID_BTN_BACKSPACE, "<-", 4, 0);

    // 7 8 9 / %
    AddButton(panel, grid, ID_BTN_7, "7", 0, 1);
    AddButton(panel, grid, ID_BTN_8, "8", 1, 1);
    AddButton(panel, grid, ID_BTN_9, "9", 2, 1);
    AddButton(panel, grid, ID_BTN_DIV, "/", 3, 1);
    AddButton(panel, grid, ID_BTN_MOD, "%", 4, 1);

    // 4 5 6 *
    AddButton(panel, grid, ID_BTN_4, "4", 0, 2);
    AddButton(panel, grid, ID_BTN_5, "5", 1, 2);
    AddButton(panel, grid, ID_BTN_6, "6", 2, 2);
    AddButton(panel, grid, ID_BTN_MUL, "*", 3, 2);

    // 1 2 3 -
    AddButton(panel, grid, ID_BTN_1, "1", 0, 3);
    AddButton(panel, grid, ID_BTN_2, "2", 1, 3);
    AddButton(panel, grid, ID_BTN_3, "3", 2, 3);
    AddButton(panel, grid, ID_BTN_SUB, "-", 3, 3);

    // +/- 0 . +
    AddButton(panel, grid, ID_BTN_NEGATIVE, "+/-", 0, 4);
    AddButton(panel, grid, ID_BTN_0, "0", 1, 4);
    AddButton(panel, grid, ID_BTN_DECIMAL, ".", 2, 4);
    AddButton(panel, grid, ID_BTN_ADD, "+", 3, 4);

    // equals, spanning the width of the grid
    AddButton(panel, grid, ID_BTN_EQUALS, "=", 4, 2, 1, 3);

    // Let every column grow evenly so the grid stretches with the window.
    for (int col = 0; col < 5; ++col)
        grid->AddGrowableCol(col);
    for (int row = 0; row < 5; ++row)
        grid->AddGrowableRow(row);

    mainSizer->Add(grid, 1, wxEXPAND | wxALL, 10);

    panel->SetSizer(mainSizer);
}

wxButton* CalculatorFrame::AddButton(wxWindow* parent, wxGridBagSizer* sizer,
    wxWindowID id, const wxString& label,
    int col, int row,
    int colSpan, int rowSpan)
{
    wxButton* btn = new wxButton(parent, id, label);
    sizer->Add(btn, wxGBPosition(row, col), wxGBSpan(rowSpan, colSpan),
        wxEXPAND);
    return btn;
}

// App
class CalculatorApp : public wxApp
{
public:
    bool OnInit() override
    {
        CalculatorFrame* frame = new CalculatorFrame();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(CalculatorApp);
