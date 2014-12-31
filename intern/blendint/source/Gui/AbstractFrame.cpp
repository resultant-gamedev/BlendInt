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

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/AbstractFrame.hpp>
#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	glm::mat4 AbstractFrame::default_view_matrix = glm::lookAt(
		glm::vec3(0.f, 0.f, 1.f),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.f, 1.f, 0.f));

	AbstractFrame::AbstractFrame()
	: AbstractView()
	{
		events_.reset(new Cpp::ConnectionScope);
		destroyed_.reset(new Cpp::Event<AbstractFrame*>);
	}

	AbstractFrame::~AbstractFrame()
	{
		destroyed_->fire(this);
	}

	Point AbstractFrame::GetAbsolutePosition (const AbstractWidget* widget)
	{
#ifdef DEBUG
		assert(widget);
#endif

		Point pos = widget->position();

		AbstractView* p = widget->superview();
		while(p && (p != this)) {
			pos = pos + p->position() + p->offset();
			p = p->superview();
		}

		pos = pos + position() + offset();
		return pos;
	}

	AbstractFrame* AbstractFrame::GetFrame(AbstractView* widget)
	{
		AbstractView* container = widget->superview ();
		AbstractFrame* frame = 0;

		if(container == 0) {
			return dynamic_cast<AbstractFrame*>(widget);
		} else {
			while(container) {
				frame = dynamic_cast<AbstractFrame*>(container);
				if(frame) break;
				container = container->superview ();
			}
		}

		return frame;
	}

	ResponseType AbstractFrame::ContextMenuPressEvent (const Context* context)
	{
		return subs_count() ? Ignore : Finish;
	}

	ResponseType AbstractFrame::ContextMenuReleaseEvent (const Context* context)
	{
		return subs_count() ? Ignore : Finish;
	}

	ResponseType AbstractFrame::DispatchKeyEvent(AbstractView* subview, const Context* context)
	{
		if(subview == this) {
			return Ignore;
		} else {

			ResponseType response = Ignore;

			if(subview->superview ()) {
				response = DispatchKeyEvent(subview->superview(), context);
				if(response == Finish) {
					return response;
				} else {
					return subview->KeyPressEvent(context);
				}
			} else {
				return subview->KeyPressEvent(context);
			}

		}
	}

	AbstractView* AbstractFrame::DispatchMousePressEvent(
			AbstractView* subview, const Context* context)
	{
		if(subview == this) {
			return 0;
		} else {

			ResponseType response = Ignore;
			AbstractView* ret_val = 0;

			if(subview->superview ()) {

				ret_val = DispatchMousePressEvent(subview->superview(), context);

				if(ret_val == 0) {

					response = subview->MousePressEvent(context);

					return response == Finish ? subview : 0;

				} else {
					return ret_val;
				}

			} else {
				response = subview->MousePressEvent(context);
				return response == Finish ? subview : 0;
			}

		}
	}

	ResponseType AbstractFrame::DispatchMouseMoveEvent(AbstractView* subview, const Context* context)
	{
		if(subview == this) {
			return Ignore;
		} else {

			if(subview->superview ()) {
				if(DispatchMouseMoveEvent(subview->superview (), context) == Ignore) {
					return subview->MouseMoveEvent(context);
				} else {
					return Finish;
				}

			} else {
				return subview->MouseMoveEvent(context);
			}

		}
	}

	ResponseType AbstractFrame::DispatchMouseReleaseEvent(
			AbstractView* subview, const Context* context)
	{
		if(subview == this) {
			return Ignore;
		} else {

			if(subview->superview ()) {
				if(DispatchMouseReleaseEvent(subview->superview (), context) == Ignore) {
					return subview->MouseReleaseEvent(context);
				} else {
					return Finish;
				}

			} else {
				DBG_PRINT_MSG("mouse press in %s", subview->name().c_str());
				return subview->MouseReleaseEvent(context);
			}

		}
	}

	AbstractWidget* AbstractFrame::DispatchHoverEventsInSubWidgets(AbstractWidget* orig,
			const Context* context)
	{
		AbstractWidget* hovered_widget = orig;

		SetLeafFrame(context, this);
		Point local_position;	// the relative local position of the cursor in a widget

		// find the new top hovered widget
		if (hovered_widget) {

			AbstractView* superview = hovered_widget->superview();
			Point parent_position;

			AbstractWidget* parent_widget = dynamic_cast<AbstractWidget*>(superview);
			if(parent_widget) {
				parent_position = this->GetAbsolutePosition(parent_widget);
			} else {
				assert(superview == this);
				parent_position = position();
			}

			bool not_hover_through = context->cursor_position().x() < parent_position.x() ||
					context->cursor_position().y() < parent_position.y() ||
					context->cursor_position().x() > (parent_position.x() + superview->size().width()) ||
					context->cursor_position().y() > (parent_position.y() + superview->size().height());

			local_position.reset(context->cursor_position().x() - parent_position.x() - superview->offset().x(),
					context->cursor_position().y() - parent_position.y() - superview->offset().y());

			if(!not_hover_through) {

				if(hovered_widget->Contain(local_position)) {

					AbstractWidget* orig = hovered_widget;

					hovered_widget = DispatchHoverEventDeeper(hovered_widget, context, local_position);

					if(orig != hovered_widget) {
//						orig->destroyed().disconnectOne(this,
//								&SingleFrame::OnHoverWidgetDestroyed);
//						events()->connect(hovered_widget->destroyed(), this,
//						        &SingleFrame::OnHoverWidgetDestroyed);
					}

				} else {

//					hovered_widget->destroyed ().disconnectOne (this,
//								&SingleFrame::OnHoverWidgetDestroyed);
					delegate_mouse_hover_out_event(hovered_widget, context);
//					hovered_widget->set_hover(false);
//					hovered_widget->MouseHoverOutEvent(event);


					// find which contianer contains cursor position
					while (superview) {

						if (superview == this) {	// FIXME: the widget may be mvoed to another context
							superview = 0;
							break;
						}

						local_position.reset(
								local_position.x() + superview->position().x() + superview->offset().x(),
								local_position.y() + superview->position().y() + superview->offset().y());

						if (superview->Contain(local_position)) break;

						superview = superview->superview();
					}

					hovered_widget = dynamic_cast<AbstractWidget*>(superview);

					if(hovered_widget) {
						hovered_widget = DispatchHoverEventDeeper(hovered_widget, context, local_position);
//						events()->connect(hovered_widget->destroyed(), this,
//						        &SingleFrame::OnHoverWidgetDestroyed);
					}

				}

			} else {

//				hovered_widget->destroyed().disconnectOne(this,
//					        &SingleFrame::OnHoverWidgetDestroyed);
				delegate_mouse_hover_out_event(hovered_widget, context);

//				hovered_widget->set_hover(false);
//				hovered_widget->MouseHoverOutEvent(event);


				// find which contianer contains cursor position
				superview = superview->superview();
				while (superview) {

					if (superview == this) {	// FIXME: the widget may be mvoed to another context
						superview = 0;
						break;
					}

					local_position.reset(
							local_position.x() + superview->position().x() + superview->offset().x(),
							local_position.y() + superview->position().y() + superview->offset().y());

					if(IsHoverThroughExt(superview, context->cursor_position())) break;
					superview = superview->superview();
				}

				hovered_widget = dynamic_cast<AbstractWidget*>(superview);
				if(hovered_widget) {
					hovered_widget = DispatchHoverEventDeeper(hovered_widget, context, local_position);
//					events()->connect(hovered_widget->destroyed(), this,
//					        &SingleFrame::OnHoverWidgetDestroyed);
				}

			}

		} else {

			local_position.reset(
					context->cursor_position().x() - position().x() - offset().x(),
					context->cursor_position().y() - position().y() - offset().y());

			for(AbstractView* p = last_subview(); p; p = p->previous_view())
			{
				if (p->visiable() && p->Contain(local_position)) {

					hovered_widget = dynamic_cast<AbstractWidget*>(p);
					delegate_mouse_hover_in_event(hovered_widget, context);
//					hovered_widget->set_hover(true);
//					hovered_widget->MouseHoverInEvent(event);

					break;
				}
			}

			if(hovered_widget) {
				hovered_widget = DispatchHoverEventDeeper(hovered_widget, context, local_position);
//				events()->connect(hovered_widget->destroyed(), this,
//				        &SingleFrame::OnHoverWidgetDestroyed);
			}

		}

		return hovered_widget;
	}

	AbstractFrame* AbstractFrame::CheckHoveredFrame(AbstractFrame* old, const Context* context)
	{
		AbstractFrame* frame_hovered = old;

		if(frame_hovered) {
			if(!frame_hovered->Contain(context->cursor_position())) {

				frame_hovered = 0;
				for(AbstractView* p = last_subview(); p; p = p->previous_view()) {
					if(p->Contain(context->cursor_position())) {
						frame_hovered = dynamic_cast<AbstractFrame*>(p);
						break;
					}
				}

			}
		} else {

			for(AbstractView* p = last_subview(); p; p = p->previous_view()) {
				if(p->Contain(context->cursor_position())) {
					frame_hovered = dynamic_cast<AbstractFrame*>(p);
					break;
				}
			}

		}

		return frame_hovered;
	}

	void AbstractFrame::ClearHoverWidgets(AbstractView* hovered_widget)
	{
#ifdef DEBUG
		assert(hovered_widget);
#endif

		while (hovered_widget && (hovered_widget != this)) {
			hovered_widget->set_hover(false);
			hovered_widget = hovered_widget->superview();
		}
	}

	void AbstractFrame::ClearHoverWidgets(AbstractView* hovered_widget, const Context* context)
	{
#ifdef DEBUG
		assert(hovered_widget);
#endif

		while (hovered_widget && (hovered_widget != this)) {
			hovered_widget->set_hover(false);
			hovered_widget->MouseHoverOutEvent(context);
			hovered_widget = hovered_widget->superview();
		}
	}

	void AbstractFrame::SetLeafFrame(const Context* context, AbstractFrame* frame)
	{
		const_cast<Context*>(context)->leaf_frame_ = frame;
	}

	AbstractWidget* AbstractFrame::DispatchHoverEventDeeper(AbstractWidget* widget, const Context* context,
			Point& local_position)
	{
		AbstractWidget* retval = widget;

		local_position.reset(
				local_position.x () - widget->position ().x ()
				- widget->offset ().x (),
				local_position.y () - widget->position ().y ()
				- widget->offset ().y ()
		);

		for (AbstractView* p = widget->last_subview (); p;
				p = p->previous_view ()) {

			if (p->visiable () && p->Contain (local_position)) {
				retval = dynamic_cast<AbstractWidget*>(p);
				delegate_mouse_hover_in_event (retval, context);

				retval = DispatchHoverEventDeeper(retval, context, local_position);
				break;
			}
	
		}

		return retval;
	}

}
