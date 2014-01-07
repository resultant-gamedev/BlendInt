#include "Window.hpp"

#include <iostream>

//using BlendInt::Interface;
#include <BlendInt/AbstractWidget.hpp>
#include <BlendInt/KeyEvent.hpp>
#include <BlendInt/MouseEvent.hpp>

namespace BlendInt {

	static KeyEvent global_key_event;
	static MouseEvent global_mouse_event;

	static void CbError (int error, const char* description)
	{
		std::cerr << "Error: " << description
				  << " (error code: " << error << ")"
				  << std::endl;
	}

	static void CbWindowSize(GLFWwindow* window, int w, int h)
	{
		Interface::Instance()->Resize(w, h);
	}

	static void CbKey(GLFWwindow* window, int key, int scancode, int action,
					  int mods)
	{
		KeyAction key_action = KeyNone;
		switch (action) {
			case GLFW_PRESS:
				key_action = KeyPress;
				break;
			case GLFW_RELEASE:
				key_action = KeyRelease;
				break;
			case GLFW_REPEAT:
				key_action = KeyRepeat;
				break;
			default:
				break;
		}

		global_key_event.set_key(key);
		global_key_event.set_scancode(scancode);
		global_key_event.set_action(key_action);
		global_key_event.set_modifiers(mods);
		global_key_event.clear_text();

		Interface::Instance()->DispatchKeyEvent(&global_key_event);
	}

	static void CbChar(GLFWwindow* window, unsigned int character)
	{
		global_key_event.set_text(character);

		Interface::Instance()->DispatchKeyEvent(&global_key_event);
	}

	static void CbMouseButton(GLFWwindow* window, int button, int action,
							  int mods)
	{
		MouseAction mouse_action = MouseNone;

		switch (action) {
			case GLFW_RELEASE:
				mouse_action = MouseRelease;
				break;
			case GLFW_PRESS:
				mouse_action = MousePress;
				break;
			case GLFW_REPEAT:
				mouse_action = MouseNone;
				break;
			default:
				break;
		}

		MouseButton mouse_button = MouseButtonNone;

		switch(button) {
			case GLFW_MOUSE_BUTTON_1:
				mouse_button = MouseButtonLeft;
				break;
			case GLFW_MOUSE_BUTTON_2:
				mouse_button = MouseButtonRight;
				break;
			case GLFW_MOUSE_BUTTON_3:
				mouse_button = MouseButtonMiddle;
				break;
			default:
				break;
		}

		global_mouse_event.set_button(mouse_button);
		global_mouse_event.set_action(mouse_action);
		global_mouse_event.set_modifiers(mods);

		Interface::Instance()->DispatchMouseEvent(&global_mouse_event);
	}

	static void CbCursorPos(GLFWwindow* window, double xpos, double ypos)
	{
		global_mouse_event.set_position(static_cast<int>(xpos), static_cast<int>(ypos));

		Interface::Instance()->DispatchMouseEvent(&global_mouse_event);
	}

	void Init ()
	{
		/* Initialize the library */
		if (!glfwInit())
			exit(-1);

		glfwSetErrorCallback(&CbError);
	}

	GLFWwindow* CreateWindow (const char* name)
	{
		GLFWwindow* window = glfwCreateWindow(1200, 800, name, NULL, NULL);
		if (!window) {
			glfwTerminate();
			exit(-1);
		}

		glfwSetWindowSizeCallback(window, &CbWindowSize);
		glfwSetKeyCallback(window, &CbKey);
		glfwSetCharCallback(window, &CbChar);
		glfwSetMouseButtonCallback(window, &CbMouseButton);
		glfwSetCursorPosCallback(window, &CbCursorPos);

		/* Make the window's context current */
		glfwMakeContextCurrent(window);
	
		/* initialize BlendInt after OpenGL content is created */
		if (!Interface::Initialize()) {
			glfwTerminate();
			exit(-1);
		}

		Interface::Instance()->Resize(1200, 800);
	
		return window;
	}

	void RunLoop (GLFWwindow* window, RenderCallback callback, void* param)
	{
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window)) {
			/* Render here */
			Interface::Instance()->Draw();

            if(callback) {
                (*callback)(param);
            }
		
			/* Swap front and back buffers */
			glfwSwapBuffers(window);
		
			/* Poll for and process events */
			glfwPollEvents();
		}
	
	}

	void Terminate ()
	{
		/* release BlendInt */
		Interface::Release();
	
		glfwTerminate();
	}

	bool CheckAllocatedObjects ()
	{
#ifdef DEBUG
		unsigned int mapsize = Object::GetMapSize();

        cout << "map size: " << mapsize << endl;

		if(mapsize > 0) {
			map<uint64_t, Object*>::const_iterator it;
			for (it = Object::GetMap().begin(); it != Object::GetMap().end(); it++)
			{
				cout << "id: " << it->first << " name: " << it->second->name() << " was not deleted!" << endl;
			}
		}

		return (mapsize == 0);
#else
		return true;
#endif
	}

}
