#pragma once
#include <wx/wx.h>
#include <vector>

class CalculatorProcessor
{
public:
    static CalculatorProcessor* GetInstance();

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
        double value = 0.0;
    };

    std::vector<Token> Tokenize(const wxString& expression) const;

    void ApplyFunctions(std::vector<Token>& tokens) const;

    double EvaluateTokens(const std::vector<Token>& tokens) const;

    static int Precedence(char op);
};
