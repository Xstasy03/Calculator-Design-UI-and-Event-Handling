#include <wx/wx.h>
#include <wx/gbsizer.h>
#include <wx/tokenzr.h>
#include <cmath>
#include <stdexcept>
#include <string>

static const double kPi = 3.14159265358979323846;

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

    void OnButtonClicked(wxCommandEvent& event);

    void EvaluateExpression();

    static double ParseDouble(const wxString& token);

    static double Evaluate(const wxArrayString& tokens);

    wxDECLARE_EVENT_TABLE();
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

// event table - connects every button to the same function
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
        // spaces around operators
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

double CalculatorFrame::ParseDouble(const wxString& token)
{
    std::string s = token.ToStdString();

    size_t charsUsed = 0;
    double value = std::stod(s, &charsUsed);

    if (charsUsed != s.size())
        throw std::invalid_argument("bad number: " + s);

    return value;
}

double CalculatorFrame::Evaluate(const wxArrayString& tokens)
{
    if (tokens.GetCount() == 1)
    {
        return ParseDouble(tokens[0]);
    }

    if (tokens.GetCount() == 2)
    {
        wxString func = tokens[0].Lower();
        if (func != "sin" && func != "cos" && func != "tan")
            throw std::invalid_argument("not a real function");

        double value = ParseDouble(tokens[1]);
        double radians = value * kPi / 180.0;

        if (func == "sin") return std::sin(radians);
        if (func == "cos") return std::cos(radians);
        return std::tan(radians);
    }

    // number, operator, number
    if (tokens.GetCount() == 3)
    {
        double left = ParseDouble(tokens[0]);
        wxString op = tokens[1];
        double right = ParseDouble(tokens[2]);

        if (op == "+") return left + right;
        if (op == "-") return left - right;
        if (op == "*") return left * right;

        if (op == "/")
        {
            if (right == 0.0)
                throw std::domain_error("cant divide by zero");
            return left / right;
        }

        if (op == "%")
        {
            if (right == 0.0)
                throw std::domain_error("cant mod by zero");
            return std::fmod(left, right);
        }

        throw std::invalid_argument("not a real operator");
    }

    throw std::invalid_argument("expression doesn't make sense");
}

void CalculatorFrame::EvaluateExpression()
{
    wxString expr = m_display->GetValue();
    expr.Trim(true).Trim(false);

    if (expr.IsEmpty())
        return;

    wxStringTokenizer tokenizer(expr, " ");
    wxArrayString tokens;
    while (tokenizer.HasMoreTokens())
        tokens.Add(tokenizer.GetNextToken());

    wxString resultText;

    try
    {
        double result = Evaluate(tokens);
        resultText = wxString::Format("%g", result);
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