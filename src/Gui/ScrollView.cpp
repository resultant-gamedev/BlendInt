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

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <iostream>

#include <BlendInt/Gui/ScrollView.hpp>

namespace BlendInt {

	ScrollView::ScrollView()
	: Frame(), m_orientation(Horizontal | Vertical), m_move_status(false)
	{
		set_size(200, 160);
		set_preferred_size(200, 160);
	}

	void ScrollView::reset_viewport_position()
	{
		if(!sub_widget_size()) return;

		AbstractWidget* p = sub_widgets().front();

		if(p) {
			int w = size().width();
			int h = size().height();

			int x = position().x() + (w - static_cast<int>(p->size().width())) / 2;
			int y = position().y() + (h - static_cast<int>(p->size().height())) / 2;

			p->SetPosition(x, y);
		}
	}

	bool ScrollView::Update (const UpdateRequest& request)
	{
		if(request.source() == Predefined) {

			switch (request.type()) {

				case FormPosition: {
					reset_viewport_position();
					return true;
				}

				default:
					return true;
			}

		} else {
			return false;
		}
	}

	ResponseType ScrollView::Draw (const RedrawEvent& event)
	{
		glEnable (GL_SCISSOR_TEST);
		glScissor (position().x(),
				position().y(),
				size().width(),
				size().height());

		glDisable(GL_SCISSOR_TEST);
	}

	void ScrollView::MousePressEvent (MouseEvent* event)
	{
		if (!sub_widget_size()) {
			event->ignore(this);
			return;
		}

		AbstractWidget* p = sub_widgets().front();

		if (event->button() == MouseButtonMiddle) {

			m_move_status = true;
			m_move_start_pos.set_x(event->position().x());
			m_move_start_pos.set_y(event->position().y());
			m_origin_pos = p->position();
		} else {
			//dispatch_mouse_press_event(m_viewport, event);
		}
	}

	void ScrollView::MouseReleaseEvent(MouseEvent* event)
	{
		if(m_move_status) m_move_status = false;

		if(!sub_widget_size()) {
			event->ignore(this);
			return;
		}

		//AbstractWidget* p = sub_widgets().front();

		//if(!m_viewport) return;
		//dispatch_mouse_release_event(m_viewport, event);
	}

	void ScrollView::MouseMoveEvent(MouseEvent* event)
	{
		/*
		if(!m_viewport) return;

		if(m_move_status) {

			int w = size().width();
			int h = size().height();

			if (m_orientation & Horizontal) {

				if (w < static_cast<int>(m_viewport->size().width())) {
					int x_min = position().x() - (m_viewport->size().width() - w);
					int x_max = position().x();
					if (x_min > x_max)
						x_min = x_max;

					m_viewport->SetPosition(
					        m_origin_pos.x() + event->position().x()
					                - m_move_start_pos.x(),
					        m_viewport->position().y());

					if (m_viewport->position().x() < x_min) {
						m_viewport->SetPosition(x_min,
						        m_viewport->position().y());
					}

					if (m_viewport->position().x() > x_max) {
						m_viewport->SetPosition(x_max,
						        m_viewport->position().y());
					}

				}
			}

			if (m_orientation & Vertical) {

				if (h < static_cast<int>(m_viewport->size().height())) {
					int y_min = position().y() - (m_viewport->size().height() - h);
					int y_max = position().y();

					if (y_min > y_max)
						y_min = y_max;

					m_viewport->SetPosition(m_viewport->position().x(),
					        m_origin_pos.y() + event->position().y()
					                - m_move_start_pos.y());


					if (m_viewport->position().y() < y_min) {
						m_viewport->SetPosition(m_viewport->position().x(),
						        y_min);
					}

					if (m_viewport->position().y() > y_max) {
						m_viewport->SetPosition(m_viewport->position().x(),
						        y_max);
					}
				}
			}

			return;
		}

		if(contain(event->position())) {
			dispatch_mouse_move_event(m_viewport, event);
		}

		*/
	}

}
