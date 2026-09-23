#pragma once
#include <wx/wx.h>
#include <vector>

// CalculatorProcessor is a classic Singleton (private constructor, static
// instance pointer, public GetInstance()). All string parsing and math now
// lives here instead of in CalculatorFrame - the frame just hands it a
// string and displays whatever comes back.
//
// Usage:
//   double result = CalculatorProcessor::GetInstance()->Calculate("2+2.5/Sin5");
class CalculatorProcessor
{
public:
    static CalculatorProcessor* GetInstance();

    // Parses and evaluates a full expression, respecting order of
    // operations (Sin/Cos/Tan highest, then * / %, then + -) via the
    // shunting yard algorithm. Throws std::invalid_argument or
    // std::domain_error on malformed input - callers are expected to
    // catch these and show something like "Error" rather than let them
    // propagate.
    double Calculate(const wxString& expression);

private:
    CalculatorProcessor() = default;

    // Singletons are non-copyable.
    CalculatorProcessor(const CalculatorProcessor&) = delete;
    CalculatorProcessor& operator=(const CalculatorProcessor&) = delete;

    static CalculatorProcessor* s_instance;

    enum class TokenType { Number, Operator, Function };

    struct Token
    {
        TokenType type = TokenType::Number;
        double value = 0.0;  // used when type == Number
        char op = 0;         // used when type == Operator
        wxString func;       // used when type == Function ("sin"/"cos"/"tan")
    };

    // Turns a raw expression string (spaces optional - both "2+2.5/Sin5"
    // and "2 + 2.5 / Sin 5" work) into a flat token list.
    std::vector<Token> Tokenize(const wxString& expression) const;

    // Pre-resolves every Function token together with the Number that
    // immediately follows it (Sin/Cos/Tan have no lower-precedence
    // competition, so this can happen before the general shunting yard
    // pass runs on the remaining Number/Operator tokens).
    void ApplyFunctions(std::vector<Token>& tokens) const;

    // Runs shunting yard on a token list that contains only Number and
    // Operator tokens (i.e. after ApplyFunctions has already run).
    double EvaluateTokens(const std::vector<Token>& tokens) const;

    static int Precedence(char op);
};
