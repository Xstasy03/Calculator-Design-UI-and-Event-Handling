#include "CalculatorProcessor.h"
#include <cmath>
#include <cctype>
#include <stdexcept>
#include <string>

CalculatorProcessor* CalculatorProcessor::s_instance = nullptr;

CalculatorProcessor* CalculatorProcessor::GetInstance()
{
    if (!s_instance)
        s_instance = new CalculatorProcessor();
    return s_instance;
}

int CalculatorProcessor::Precedence(char op)
{
    switch (op)
    {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
    case '%':
        return 2;
    default:
        return 0;
    }
}

std::vector<CalculatorProcessor::Token>
CalculatorProcessor::Tokenize(const wxString& expression) const
{
    std::vector<Token> tokens;
    const std::string expr = expression.ToStdString();
    size_t i = 0;
    const size_t n = expr.size();

    while (i < n)
    {
        const char c = expr[i];

        if (std::isspace(static_cast<unsigned char>(c)))
        {
            ++i;
            continue;
        }

        // Build up a function name (sin/cos/tan).
        if (std::isalpha(static_cast<unsigned char>(c)))
        {
            std::string word;
            while (i < n && std::isalpha(static_cast<unsigned char>(expr[i])))
                word += expr[i++];

            Token t;
            t.type = TokenType::Function;
            t.func = wxString(word).Lower();
            tokens.push_back(t);
            continue;
        }

        // Digits or a decimal point
        if (std::isdigit(static_cast<unsigned char>(c)) || c == '.')
        {
            std::string num;
            while (i < n && (std::isdigit(static_cast<unsigned char>(expr[i])) ||
                expr[i] == '.'))
            {
                num += expr[i++];
            }

            size_t used = 0;
            const double value = std::stod(num, &used); // may throw
            if (used != num.size())
                throw std::invalid_argument("Malformed number: " + num);

            Token t;
            t.type = TokenType::Number;
            t.value = value;
            tokens.push_back(t);
            continue;
        }

        // One of the five operator characters.
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%')
        {
            const bool couldBeUnary =
                tokens.empty() ||
                tokens.back().type == TokenType::Operator ||
                tokens.back().type == TokenType::Function;

            if (c == '-' && couldBeUnary)
            {
                std::string num;
                num += expr[i++]; // the '-' itself
                while (i < n && (std::isdigit(static_cast<unsigned char>(expr[i])) ||
                    expr[i] == '.'))
                {
                    num += expr[i++];
                }

                size_t used = 0;
                const double value = std::stod(num, &used);
                if (used != num.size())
                    throw std::invalid_argument("Malformed number: " + num);

                Token t;
                t.type = TokenType::Number;
                t.value = value;
                tokens.push_back(t);
                continue;
            }

            Token t;
            t.type = TokenType::Operator;
            t.op = c;
            tokens.push_back(t);
            ++i;
            continue;
        }

        throw std::invalid_argument(std::string("Unexpected character: ") + c);
    }

    return tokens;
}

void CalculatorProcessor::ApplyFunctions(std::vector<Token>& tokens) const
{
    std::vector<Token> resolved;
    resolved.reserve(tokens.size());

    for (size_t i = 0; i < tokens.size(); ++i)
    {
        if (tokens[i].type == TokenType::Function)
        {
            if (i + 1 >= tokens.size() || tokens[i + 1].type != TokenType::Number)
                throw std::invalid_argument(
                    "Function with no operand: " + tokens[i].func.ToStdString());

            const wxString& func = tokens[i].func;
            const double value = tokens[i + 1].value;
            double result = 0.0;

            if (func == "sin")      result = std::sin(value);
            else if (func == "cos") result = std::cos(value);
            else if (func == "tan") result = std::tan(value);
            else throw std::invalid_argument("Unknown function: " + func.ToStdString());

            Token t;
            t.type = TokenType::Number;
            t.value = result;
            resolved.push_back(t);

            ++i;
        }
        else
        {
            resolved.push_back(tokens[i]);
        }
    }

    tokens = std::move(resolved);
}

double CalculatorProcessor::EvaluateTokens(const std::vector<Token>& tokens) const
{
    std::vector<Token> output;
    std::vector<char> operators;

    for (const Token& t : tokens)
    {
        if (t.type == TokenType::Number)
        {
            output.push_back(t);
        }
        else if (t.type == TokenType::Operator)
        {
            while (!operators.empty() &&
                Precedence(operators.back()) >= Precedence(t.op))
            {
                Token popped;
                popped.type = TokenType::Operator;
                popped.op = operators.back();
                output.push_back(popped);
                operators.pop_back();
            }
            operators.push_back(t.op);
        }
        else
        {
            throw std::invalid_argument("Unexpected function token during evaluation");
        }
    }

    while (!operators.empty())
    {
        Token popped;
        popped.type = TokenType::Operator;
        popped.op = operators.back();
        output.push_back(popped);
        operators.pop_back();
    }

    // --- Evaluate queue
    std::vector<double> stack;

    for (const Token& t : output)
    {
        if (t.type == TokenType::Number)
        {
            stack.push_back(t.value);
            continue;
        }

        if (stack.size() < 2)
            throw std::invalid_argument("Malformed expression");

        const double right = stack.back(); stack.pop_back();
        const double left = stack.back(); stack.pop_back();
        double result = 0.0;

        switch (t.op)
        {
        case '+':
            result = left + right;
            break;
        case '-':
            result = left - right;
            break;
        case '*':
            result = left * right;
            break;
        case '/':
            if (right == 0.0)
                throw std::domain_error("Division by zero");
            result = left / right;
            break;
        case '%':
            if (right == 0.0)
                throw std::domain_error("Modulo by zero");
            result = std::fmod(left, right);
            break;
        default:
            throw std::invalid_argument("Unknown operator");
        }

        stack.push_back(result);
    }

    if (stack.size() != 1)
        throw std::invalid_argument("Malformed expression");

    return stack.back();
}

double CalculatorProcessor::Calculate(const wxString& expression)
{
    wxString expr = expression;
    expr.Trim(true).Trim(false);

    if (expr.IsEmpty())
        throw std::invalid_argument("Empty expression");

    std::vector<Token> tokens = Tokenize(expr);
    if (tokens.empty())
        throw std::invalid_argument("Empty expression");

    ApplyFunctions(tokens);
    return EvaluateTokens(tokens);
}