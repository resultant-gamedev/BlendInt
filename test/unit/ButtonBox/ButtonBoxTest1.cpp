#include "ButtonBoxTest1.hpp"
#include <BlendInt/ButtonBox.hpp>
#include <BlendInt/Button.hpp>

using namespace BlendInt;

ButtonBoxTest1::ButtonBoxTest1()
: testing::Test()
{
	// TODO: add constructor code
}

ButtonBoxTest1::~ButtonBoxTest1()
{
	// TODO: add destructor code
}

/**
 * test Add() method
 *
 * Expected result: 
 */
TEST_F(ButtonBoxTest1, Add1)
{
	Init ();
	GLFWwindow* window = CreateWindow("ButtonBox - Add1");

	// TODO: add test code here

	ButtonBox* box = new ButtonBox;
	box->SetMargin(1, 1, 1, 1);

	Button* button1 = new Button;
	Button* button2 = new Button;

	box->Add(button1);
	box->Add(button2);

	Interface::Instance()->Bind(box);

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}

/**
 * test Add() method
 *
 * Expected result:
 */
TEST_F(ButtonBoxTest1, Resize1)
{
	Init ();
	GLFWwindow* window = CreateWindow("ButtonBox - Add1");

	// TODO: add test code here

	ButtonBox* box = new ButtonBox;
	box->SetMargin(1, 1, 1, 1);

	Button* button1 = new Button;
	Button* button2 = new Button;

	box->Add(button1);
	box->Add(button2);

	box->Resize(200, 40);

	Interface::Instance()->Bind(box);

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}
