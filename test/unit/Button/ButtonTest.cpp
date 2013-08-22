// cpp

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <stdio.h>

#include "ButtonTest.h"

#include <BIL/Application.hpp>
#include <BIL/Window.hpp>
#include <BIL/Label.hpp>
#include <BIL/Button.hpp>

using namespace BIL;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(ButtonTest);

ButtonTest::ButtonTest ()
{

}

ButtonTest::~ButtonTest ()
{

}

void ButtonTest::setUp ()
{
}

void ButtonTest::tearDown ()
{
}

void ButtonTest::show1 ()
{
	Application app;

	Window win(640, 480, "640 x 480 Window", NULL, NULL);

	app.setMainWindow(&win);
	app.initialize();

	Button button(L"Button Test");
	button.set_parent(&win);
	button.set_round_box_type(RoundBoxAll);
	button.set_pos(Point(50, 50));

	app.run();

	CPPUNIT_ASSERT(true);
}
