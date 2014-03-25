/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free
 * software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is
 * distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BLENDINT_GUI_SCROLLBAR_HPP_
#define _BLENDINT_GUI_SCROLLBAR_HPP_

#include <BlendInt/OpenGL/GLArrayBuffer.hpp>
#include <BlendInt/Gui/Slider.hpp>

#include <BlendInt/Gui/AbstractButton.hpp>

namespace BlendInt {

	class ScrollBar: public AbstractSlider
	{
		public:

			ScrollBar (Orientation orientation = Horizontal);

			virtual ~ScrollBar ();

		protected:

			virtual bool Update (const UpdateRequest& request);

			virtual ResponseType Draw (const RedrawEvent& event);

			virtual void MouseMoveEvent (MouseEvent* event);

			virtual void MousePressEvent (MouseEvent* event);

			virtual void MouseReleaseEvent (MouseEvent* event);

			/**
			 * @brief calculate the space width in which slider can move
			 * @return
			 */
			int GetSpace ();

		private:

			void InitOnce ();

			/**
			 * @brief Check if cursor is on the slide icon
			 */
			bool CursorOnSlideIcon (const Point& cursor);

			/**
			 * @brief Get the new value at the cursor position
			 * @param[in] cursor The cursor position
			 * @param[out] vout The new value
			 * @return
			 * 	- true if the cursor is in range
			 * 	- false if the cursor is out of range
			 */
			bool GetNewValue (const Point& cursor, int* vout);

			/**
			 * @brief
			 */
			inline float get_position ()
			{
				return value() * GetSpace()
				                / ((float) maximum() - (float) minimum());
			}

			SlideIcon m_bar;

			GLuint m_vao;
			RefPtr<GLArrayBuffer> m_slot_inner_buffer;
			RefPtr<GLArrayBuffer> m_slot_outline_buffer;

			Point m_line_start;	// where start to draw line
			int m_line_width;

			/**
			 * @brief If the slide switch is pressed
			 */
			bool m_pressed;

	};

}

#endif /* _BLENDINT_GUI_SCROLLBAR_HPP_ */
