#include "ToggleButtonTest1.hpp"
#include <BlendInt/ToggleButton.hpp>
#include <Common/Window.hpp>

using namespace BlendInt;

ToggleButtonTest1::ToggleButtonTest1()
: testing::Test()
{
	// TODO: add constructor code
}

ToggleButtonTest1::~ToggleButtonTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(ToggleButtonTest1, Foo1)
{
	Init ();
	GLFWwindow* win = CreateWindow("ToggleButton - Foo1");

    // TODO: add test code here
    ToggleButton* btn = new ToggleButton;
    btn->SetPosition(200, 200);
    btn->Register();

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}