// cpp

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <stdio.h>

#include "TextureAtlasTest.h"

#include <BIL/Interface.hpp>
#include <BIL/Label.hpp>
#include <BIL/FontConfig.hpp>
#include <BIL/Rect.hpp>
#include <BIL/TextureAtlas.hpp>

using namespace BIL;
using namespace std;

//CPPUNIT_TEST_SUITE_REGISTRATION(TextureAtlasTest);

TextureAtlasTest::TextureAtlasTest ()
{

}

TextureAtlasTest::~TextureAtlasTest ()
{

}

void TextureAtlasTest::setUp ()
{
}

void TextureAtlasTest::tearDown ()
{
}

void TextureAtlasTest::show1 ()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BIL", NULL, NULL);
	if (!window) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	glfwSetWindowSizeCallback(window, &cbWindowSize);
	glfwSetKeyCallback(window, &cbKey);
	glfwSetMouseButtonCallback(window, &cbMouseButton);
	glfwSetCursorPosCallback(window, &cbCursorPos);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* initialize BIL after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	Freetype fe;
	fe.open(Font("Sans"), 96);
	fe.setCharSize(12);
	TextureAtlas atlas;
	//atlas.load(fe.getFontFace(), 48);
	atlas.initialize();
	atlas.generate(&fe, 32, 1000);

	std::cout << atlas.glyph('a').bitmap_left << std::endl;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {

		/* Render here */
		app->render();
		atlas.render_text(L"greetings, let's say yes!", 100.0, 100.0, 1.0, 1.0);

		// render character from atlas here

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	fe.close();

	/* release BIL */
	Interface::release();

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void TextureAtlasTest::show2 ()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BIL", NULL, NULL);
	if (!window) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	glfwSetWindowSizeCallback(window, &cbWindowSize);
	glfwSetKeyCallback(window, &cbKey);
	glfwSetMouseButtonCallback(window, &cbMouseButton);
	glfwSetCursorPosCallback(window, &cbCursorPos);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* initialize BIL after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	Freetype fe;
	fe.open(Font("Sans"), 96);
	fe.setCharSize(9);
	TextureAtlas atlas;
	//atlas.load(fe.getFontFace(), 48);
	atlas.initialize();
	atlas.generate(&fe, L'一', 100);	// '一' is 0x4e00 in unicode, a chinese character

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {

		/* Render here */
		app->render();

		// render character from atlas here
		atlas.render_text(L"一丁丂七丄丅丆万丈三上下丌不与丏丐丑丒专且丕世", 100.0, 100.0, 1.0, 1.0);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	fe.close();

	/* release BIL */
	Interface::release();

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void TextureAtlasTest::cbError (int error, const char* description)
{
	std::cerr << "Error: " << description
			<< " (error code: " << error << ")"
			<< std::endl;
}

void TextureAtlasTest::cbWindowSize (GLFWwindow* window, int w, int h)
{
	BIL::Interface::instance()->resizeEvent(w, h);
}

void TextureAtlasTest::cbKey (GLFWwindow* window, int key, int scancode, int action,
        int mods)
{
	BIL::Interface::instance()->keyEvent(key, scancode, action, mods);
}

void TextureAtlasTest::cbMouseButton (GLFWwindow* window, int button, int action,
        int mods)
{
	BIL::Interface::instance()->mouseButtonEvent(button, action, mods);
}

void TextureAtlasTest::cbCursorPos (GLFWwindow* window, double xpos, double ypos)
{
	BIL::Interface::instance()->cursorPosEvent(xpos, ypos);
}