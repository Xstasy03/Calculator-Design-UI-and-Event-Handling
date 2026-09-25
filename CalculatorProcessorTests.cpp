#include "CppUnitTest.h"
#include <stdexcept>

// Relative path back into the main project. Adjust the folder name here if
// your main project's folder is named differently than mine.
#include "../Calculator Design UI and Event Handling/CalculatorProcessor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CalculatorTests
{
    TEST_CLASS(CalculatorProcessorTests)
    {
    public:
        // --- Singleton behavior ---

        TEST_METHOD(GetInstance_ReturnsSameInstanceEachCall)
        {
            CalculatorProcessor* first = CalculatorProcessor::GetInstance();
            CalculatorProcessor* second = CalculatorProcessor::GetInstance();
            Assert::IsTrue(first == second);
        }

        // --- Individual operations ---

        TEST_METHOD(Calculate_Addition_ReturnsSum)
        {
            double result = CalculatorProcessor::GetInstance()->Calculate("2+3");
            Assert::AreEqual(5.0, result, 0.0001);
        }

        TEST_METHOD(Calculate_Subtraction_ReturnsDifference)
        {
            double result = CalculatorProcessor::GetInstance()->Calculate("10-4");
            Assert::AreEqual(6.0, result, 0.0001);
        }

        TEST_METHOD(Calculate_Multiplication_ReturnsProduct)
        {
            double result = CalculatorProcessor::GetInstance()->Calculate("6*7");
            Assert::AreEqual(42.0, result, 0.0001);
        }

        TEST_METHOD(Calculate_Division_ReturnsQuotient)
        {
            double result = CalculatorProcessor::GetInstance()->Calculate("20/4");
            Assert::AreEqual(5.0, result, 0.0001);
        }

        TEST_METHOD(Calculate_Modulo_ReturnsRemainder)
        {
            double result = CalculatorProcessor::GetInstance()->Calculate("10%3");
            Assert::AreEqual(1.0, result, 0.0001);
        }

        // --- Order of operations ---

        TEST_METHOD(Calculate_OrderOfOperations_MultiplicationBeforeAddition)
        {
            // Must equal 14 (3*4 evaluated first), NOT 20 (which is what
            // naive left-to-right evaluation would incorrectly produce).
            double result = CalculatorProcessor::GetInstance()->Calculate("2+3*4");
            Assert::AreEqual(14.0, result, 0.0001);
        }

        TEST_METHOD(Calculate_OrderOfOperations_ComplexExpression)
        {
            // Must equal 26 (both multiplications evaluated before the
            // addition between them), NOT 100 from naive left-to-right.
            double result = CalculatorProcessor::GetInstance()->Calculate("2*3+4*5");
            Assert::AreEqual(26.0, result, 0.0001);
        }

        // --- Unary negation ---

        TEST_METHOD(Calculate_UnaryNegation_HandledCorrectly)
        {
            // "1 - -4" is 1 minus negative 4, i.e. 5 - not a malformed
            // double-operator expression.
            double result = CalculatorProcessor::GetInstance()->Calculate("1 - -4");
            Assert::AreEqual(5.0, result, 0.0001);
        }

        // --- Combined trig + arithmetic (the lab's own worked example) ---

        TEST_METHOD(Calculate_SinWithDivision_MatchesAssignmentExample)
        {
            // The exact example given in the Processor Singleton lab
            // instructions: 2 + 2.5 / Sin(5 radians) = -0.60708803192...
            double result = CalculatorProcessor::GetInstance()->Calculate("2+2.5/Sin5");
            Assert::AreEqual(-0.60708803192, result, 0.0001);
        }
    };
}