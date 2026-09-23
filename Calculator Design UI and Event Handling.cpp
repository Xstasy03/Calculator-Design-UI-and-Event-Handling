#include <wx/wx.h>
#include <wx/gbsizer.h>
#include <stdexcept>

#include "ButtonIDs.h"
#include "ButtonFactory.h"
#include "CalculatorProcessor.h"

// Frame
class CalculatorFrame : public wxFrame
{
public:
    CalculatorFrame();

private:
    wxTextCtrl* m_display = nullptr;

    void PlaceButton(wxGridBagSizer* sizer, wxButton* btn,
        int col, int row,
        int colSpan = 1, int rowSpan = 1);

    void OnButtonClicked(wxCommandEvent& event);
    void EvaluateExpression();

    wxDECLARE_EVENT_TABLE();
};

CalculatorFrame::CalculatorFrame()
    : wxFrame(nullptr, wxID_ANY, "Calculator",
        wxDefaultPosition, wxSize(340, 460))
{
    wxPanel* panel = new wxPanel(this);

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    m_display = new wxTextCtrl(panel, ID_TXT_DISPLAY, "",
        wxDefaultPosition, wxSize(-1, 50),
        wxTE_RIGHT | wxTE_READONLY);
    wxFont displayFont = m_display->GetFont();
    displayFont.SetPointSize(displayFont.GetPointSize() + 6);
    m_display->SetFont(displayFont);

    mainSizer->Add(m_display, 0, wxEXPAND | wxALL, 10);


    wxGridBagSizer* grid = new wxGridBagSizer(6, 6);

    PlaceButton(grid, ButtonFactory::CreateSinButton(panel), 0, 0);
    PlaceButton(grid, ButtonFactory::CreateCosButton(panel), 1, 0);
    PlaceButton(grid, ButtonFactory::CreateTanButton(panel), 2, 0);
    PlaceButton(grid, ButtonFactory::CreateClearButton(panel), 3, 0);
    PlaceButton(grid, ButtonFactory::CreateBackspaceButton(panel), 4, 0);

    // 7 8 9 / %
    PlaceButton(grid, ButtonFactory::CreateDigitButton(panel, 7), 0, 1);
    PlaceButton(grid, ButtonFactory::CreateDigitButton(panel, 8), 1, 1);
    PlaceButton(grid, ButtonFactory::CreateDigitButton(panel, 9), 2, 1);
    PlaceButton(grid, ButtonFactory::CreateDivButton(panel), 3, 1);
    PlaceButton(grid, ButtonFactory::CreateModButton(panel), 4, 1);

    // 4 5 6 *
    PlaceButton(grid, ButtonFactory::CreateDigitButton(panel, 4), 0, 2);
    PlaceButton(grid, ButtonFactory::CreateDigitButton(panel, 5), 1, 2);
    PlaceButton(grid, ButtonFactory::CreateDigitButton(panel, 6), 2, 2);
    PlaceButton(grid, ButtonFactory::CreateMulButton(panel), 3, 2);

    // 1 2 3 -
    PlaceButton(grid, ButtonFactory::CreateDigitButton(panel, 1), 0, 3);
    PlaceButton(grid, ButtonFactory::CreateDigitButton(panel, 2), 1, 3);
    PlaceButton(grid, ButtonFactory::CreateDigitButton(panel, 3), 2, 3);
    PlaceButton(grid, ButtonFactory::CreateSubButton(panel), 3, 3);

    // +/- 0 . +
    PlaceButton(grid, ButtonFactory::CreateNegativeButton(panel), 0, 4);
    PlaceButton(grid, ButtonFactory::CreateDigitButton(panel, 0), 1, 4);
    PlaceButton(grid, ButtonFactory::CreateDecimalButton(panel), 2, 4);
    PlaceButton(grid, ButtonFactory::CreateAddButton(panel), 3, 4);

    // equals, filling the empty column next to *, -, +
    PlaceButton(grid, ButtonFactory::CreateEqualsButton(panel), 4, 2, 1, 3);

    for (int col = 0; col < 5; ++col)
        grid->AddGrowableCol(col);
    for (int row = 0; row < 5; ++row)
        grid->AddGrowableRow(row);

    mainSizer->Add(grid, 1, wxEXPAND | wxALL, 10);

    panel->SetSizer(mainSizer);
}

void CalculatorFrame::PlaceButton(wxGridBagSizer* sizer, wxButton* btn,
    int col, int row,
    int colSpan, int rowSpan)
{
    sizer->Add(btn, wxGBPosition(row, col), wxGBSpan(rowSpan, colSpan),
        wxEXPAND);
}

wxBEGIN_EVENT_TABLE(CalculatorFrame, wxFrame)
EVT_BUTTON(ID_BTN_0, CalculatorFrame::OnButtonClicked)
EVT_BUTTON(ID_BTN_1, CalculatorFrame::OnButtonClicked)
EVT_BUTTON(ID_BTN_2, CalculatorFrame::OnButtonClicked)
EVT_BUTTON(ID_BTN_3, CalculatorFrame::OnButtonClicked)
EVT_BUTTON(ID_BTN_4, CalculatorFrame::OnButtonClicked)
EVT_BUTTON(ID_BTN_5, CalculatorFrame::OnButtonClicked)
EVT_BUTTON(ID_BTN_6, CalculatorFrame::OnButtonClicked)
EVT_BUTTON(ID_BTN_7, CalculatorFrame::OnButtonClicked)
EVT_BUTTON(ID_BTN_8, CalculatorFrame::OnButtonClicked)
EVT_BUTTON(ID_BTN_9, CalculatorFrame::OnButtonClicked)
EVT_BUTTON(ID_BTN_ADD, CalculatorFrame::OnButtonClicked)
EVT_BUTTON(ID_BTN_SUB, CalculatorFrame::OnButtonClicked)
EVT_BUTTON(ID_BTN_MUL, CalculatorFrame::OnButtonClicked)
EVT_BUTTON(ID_BTN_DIV, CalculatorFrame::OnButtonClicked)
EVT_BUTTON(ID_BTN_MOD, CalculatorFrame::OnButtonClicked)
EVT_BUTTON(ID_BTN_SIN, CalculatorFrame::OnButtonClicked)
EVT_BUTTON(ID_BTN_COS, CalculatorFrame::OnButtonClicked)
EVT_BUTTON(ID_BTN_TAN, CalculatorFrame::OnButtonClicked)
EVT_BUTTON(ID_BTN_EQUALS, CalculatorFrame::OnButtonClicked)
EVT_BUTTON(ID_BTN_CLEAR, CalculatorFrame::OnButtonClicked)
EVT_BUTTON(ID_BTN_BACKSPACE, CalculatorFrame::OnButtonClicked)
EVT_BUTTON(ID_BTN_DECIMAL, CalculatorFrame::OnButtonClicked)
EVT_BUTTON(ID_BTN_NEGATIVE, CalculatorFrame::OnButtonClicked)
wxEND_EVENT_TABLE()

void CalculatorFrame::OnButtonClicked(wxCommandEvent& event)
{
    const int id = event.GetId();

    switch (id)
    {
    case ID_BTN_CLEAR:
        m_display->Clear();
        return;

    case ID_BTN_BACKSPACE:
    {
        try
        {
            wxString text = m_display->GetValue();
            if (!text.IsEmpty())
                m_display->SetValue(text.RemoveLast(1));
        }
        catch (const std::exception&)
        {
        }
        return;
    }

    case ID_BTN_EQUALS:
        EvaluateExpression();
        return;

    case ID_BTN_NEGATIVE:
        m_display->AppendText("-");
        return;

    default:
        break;
    }

    wxButton* btn = wxDynamicCast(event.GetEventObject(), wxButton);
    if (!btn)
        return;

    const wxString label = btn->GetLabel();

    if (id == ID_BTN_ADD || id == ID_BTN_SUB || id == ID_BTN_MUL ||
        id == ID_BTN_DIV || id == ID_BTN_MOD)
    {
        m_display->AppendText(" " + label + " ");
    }
    else if (id == ID_BTN_SIN || id == ID_BTN_COS || id == ID_BTN_TAN)
    {
        m_display->AppendText(label + " ");
    }
    else
    {
        m_display->AppendText(label);
    }
}

void CalculatorFrame::EvaluateExpression()
{
    wxString expr = m_display->GetValue();
    expr.Trim(true).Trim(false);

    if (expr.IsEmpty())
        return;

    wxString resultText;

    try
    {
        const double result = CalculatorProcessor::GetInstance()->Calculate(expr);
        resultText = wxString::Format("%.11g", result);
    }
    catch (const std::invalid_argument&)
    {
        resultText = "Error";
    }
    catch (const std::out_of_range&)
    {
        resultText = "Error";
    }
    catch (const std::domain_error&)
    {
        resultText = "Error";
    }
    catch (const std::exception&)
    {
        resultText = "Error";
    }
    catch (...)
    {
        resultText = "Error";
    }

    m_display->Clear();
    m_display->AppendText(resultText);
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