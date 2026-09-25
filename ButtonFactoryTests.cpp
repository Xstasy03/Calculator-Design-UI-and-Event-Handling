#include "CppUnitTest.h"

// Relative paths back into the main project. Adjust the folder name here
// if your main project's folder is named differently than mine.
#include "../Calculator Design UI and Event Handling/ButtonFactory.h"
#include "../Calculator Design UI and Event Handling/ButtonIDs.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// wxWidgets needs its library minimally initialized before ANY window
// object - even an invisible test one - can be constructed. Normally
// wxIMPLEMENT_APP does this for you as part of starting a real app with a
// message loop, but a unit test project never calls that macro. wxEntryStart
// does the minimum initialization needed without requiring a running event
// loop, which is exactly what a test runner needs. Without this, every
// ButtonFactory::Create*Button() call below would crash.
TEST_MODULE_INITIALIZE(WxModuleInitialize)
{
    wxApp::SetInstance(new wxApp());
    int argc = 0;
    wxEntryStart(argc, static_cast<char**>(nullptr));
}

TEST_MODULE_CLEANUP(WxModuleCleanup)
{
    wxEntryCleanup();
}

namespace CalculatorTests
{
    TEST_CLASS(ButtonFactoryTests)
    {
    public:
        // A hidden top-level window every test in this class uses as the
        // "parent" argument ButtonFactory's methods require. wxButton (like
        // every wxWindow) needs a real parent window to attach to - it
        // can't be constructed with a null parent. Created once for the
        // whole test class and cleaned up after the last test runs.
        static wxFrame* s_parent;

        TEST_CLASS_INITIALIZE(ClassInitialize)
        {
            s_parent = new wxFrame(nullptr, wxID_ANY, "Test Parent");
        }

        TEST_CLASS_CLEANUP(ClassCleanup)
        {
            // Using delete rather than wxWindow::Destroy() here on purpose:
            // Destroy() schedules the actual deletion for the next idle
            // event, which never fires because there's no running message
            // loop in a test host. That would leak every window we create.
            // A direct delete runs the destructor immediately, which is
            // safe here since nothing is mid-event-handling.
            delete s_parent;
        }

        // --- Generic creator ---

        TEST_METHOD(CreateButton_Generic_SetsParentIdAndLabel)
        {
            wxButton* btn = ButtonFactory::CreateButton(s_parent, ID_BTN_7, "7");

            Assert::IsTrue(btn != nullptr);
            Assert::AreEqual(static_cast<int>(ID_BTN_7), btn->GetId());
            Assert::IsTrue(btn->GetLabel() == "7");
            Assert::IsTrue(btn->GetParent() == s_parent);

            delete btn;
        }

        // --- Binary operators ---

        TEST_METHOD(CreateAddButton_HasPlusLabelAndCorrectId)
        {
            wxButton* btn = ButtonFactory::CreateAddButton(s_parent);
            Assert::IsTrue(btn->GetLabel() == "+");
            Assert::AreEqual(static_cast<int>(ID_BTN_ADD), btn->GetId());
            delete btn;
        }

        TEST_METHOD(CreateSubButton_HasMinusLabelAndCorrectId)
        {
            wxButton* btn = ButtonFactory::CreateSubButton(s_parent);
            Assert::IsTrue(btn->GetLabel() == "-");
            Assert::AreEqual(static_cast<int>(ID_BTN_SUB), btn->GetId());
            delete btn;
        }

        TEST_METHOD(CreateMulButton_HasStarLabelAndCorrectId)
        {
            wxButton* btn = ButtonFactory::CreateMulButton(s_parent);
            Assert::IsTrue(btn->GetLabel() == "*");
            Assert::AreEqual(static_cast<int>(ID_BTN_MUL), btn->GetId());
            delete btn;
        }

        // --- Digits ---

        TEST_METHOD(CreateDigitButton_Seven_HasCorrectLabelAndId)
        {
            wxButton* btn = ButtonFactory::CreateDigitButton(s_parent, 7);
            Assert::IsTrue(btn->GetLabel() == "7");
            // ID_BTN_0 through ID_BTN_9 are declared consecutively, so
            // digit 7's ID should be exactly ID_BTN_0 + 7.
            Assert::AreEqual(static_cast<int>(ID_BTN_0 + 7), btn->GetId());
            delete btn;
        }

        // --- Unary functions ---

        TEST_METHOD(CreateSinButton_HasSinLabelAndCorrectId)
        {
            wxButton* btn = ButtonFactory::CreateSinButton(s_parent);
            Assert::IsTrue(btn->GetLabel() == "sin");
            Assert::AreEqual(static_cast<int>(ID_BTN_SIN), btn->GetId());
            delete btn;
        }

        // --- Special buttons ---

        TEST_METHOD(CreateEqualsButton_HasEqualsLabelAndCorrectId)
        {
            wxButton* btn = ButtonFactory::CreateEqualsButton(s_parent);
            Assert::IsTrue(btn->GetLabel() == "=");
            Assert::AreEqual(static_cast<int>(ID_BTN_EQUALS), btn->GetId());
            delete btn;
        }

        TEST_METHOD(CreateClearButton_HasCLabelAndCorrectId)
        {
            wxButton* btn = ButtonFactory::CreateClearButton(s_parent);
            Assert::IsTrue(btn->GetLabel() == "C");
            Assert::AreEqual(static_cast<int>(ID_BTN_CLEAR), btn->GetId());
            delete btn;
        }

        TEST_METHOD(CreateBackspaceButton_HasArrowLabelAndCorrectId)
        {
            wxButton* btn = ButtonFactory::CreateBackspaceButton(s_parent);
            Assert::IsTrue(btn->GetLabel() == "<-");
            Assert::AreEqual(static_cast<int>(ID_BTN_BACKSPACE), btn->GetId());
            delete btn;
        }

        TEST_METHOD(CreateDecimalButton_HasDotLabelAndCorrectId)
        {
            wxButton* btn = ButtonFactory::CreateDecimalButton(s_parent);
            Assert::IsTrue(btn->GetLabel() == ".");
            Assert::AreEqual(static_cast<int>(ID_BTN_DECIMAL), btn->GetId());
            delete btn;
        }
    };

    wxFrame* ButtonFactoryTests::s_parent = nullptr;
}