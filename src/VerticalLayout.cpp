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

#include <GL/glew.h>
#include <algorithm>
#include <queue>
#include <iostream>

#include <BlendInt/VerticalLayout.hpp>

#include <BlendInt/Interface.hpp>

namespace BlendInt {

	VerticalLayout::VerticalLayout (int align)
			: AbstractLayout()
	{
		set_alignment(align);
	}

	VerticalLayout::VerticalLayout (int align, AbstractWidget* parent)
			: AbstractLayout(parent)
	{
		set_alignment(align);
	}

	VerticalLayout::~VerticalLayout ()
	{

	}

	void VerticalLayout::Update (int type, const void* data)
	{
		switch (type) {

			case FormSize: {
				const Size* size_p = static_cast<const Size*>(data);
				if(items().size())
					make_layout(size_p);
				return;
			}

			default: {
				AbstractLayout::Update (type, data);
				break;
			}
		}
	}

	void VerticalLayout::Render ()
	{

		std::vector<AbstractWidget*>::const_iterator it;
		AbstractWidget *item = 0;
		for (it = items().begin(); it != items().end(); it++) {
			item = *it;
			if (item) {
				dispatch_render(item);
			}
		}

#ifdef DEBUG
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position().x(), position().y(), z());

		glLineWidth(1);
		glEnable(GL_LINE_STIPPLE);

		glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
		glLineStipple(1, 0xAAAA);
		glBegin(GL_LINE_LOOP);
		glVertex2i(0, 0);
		glVertex2i(size().width(), 0);
		glVertex2i(size().width(), size().height());
		glVertex2i(0, size().height());
		glEnd();

		glDisable(GL_LINE_STIPPLE);

		glPopMatrix();
#endif
	}

	void VerticalLayout::KeyPressEvent (KeyEvent* event)
	{
		std::vector<AbstractWidget*>::iterator it;
		for (it = items().begin(); it != items().end(); it++) {
			dispatch_key_press_event(*it, event);
		}
	}

	void VerticalLayout::ContextMenuPressEvent (ContextMenuEvent* event)
	{
	}

	void VerticalLayout::ContextMenuReleaseEvent (ContextMenuEvent* event)
	{
	}

	void VerticalLayout::MousePressEvent (MouseEvent* event)
	{
		std::vector<AbstractWidget*>::iterator it;
		for (it = items().begin(); it != items().end(); it++) {
			dispatch_mouse_press_event(*it, event);
		}
	}

	void VerticalLayout::MouseReleaseEvent (MouseEvent* event)
	{
		std::vector<AbstractWidget*>::iterator it;
		for (it = items().begin(); it != items().end(); it++) {
			dispatch_mouse_release_event(*it, event);
		}
	}

	void VerticalLayout::MouseMoveEvent (MouseEvent* event)
	{
		std::vector<AbstractWidget*>::iterator it;
		for (it = items().begin(); it != items().end(); it++) {
			dispatch_mouse_move_event(*it, event);
		}
	}

	void VerticalLayout::add_item (AbstractWidget* object)
	{
		// don't fire events when adding a widget into a layout
		object->deactivate_events();
		deactivate_events();

		Size min = minimal_size();
		Size preferred = preferred_size();
		Size current_size = size();

		unsigned int w_plus = margin().left() + margin().right();

		if (items().size() == 0) {
			min.add_height(object->minimal_size().height());
			preferred.add_height(object->preferred_size().height());
		} else {
			min.add_height(object->minimal_size().height() + space());
			preferred.add_height(object->preferred_size().height() + space());
		}

		min.set_width(
		        std::max(min.width(), object->minimal_size().width() + w_plus));
		preferred.set_width(
		        std::max(preferred.width(),
		                object->preferred_size().width() + w_plus));

		if(!locked()) {
			if (current_size.width() < preferred.width()) {
				current_size.set_width(preferred.width());
			}
			if (current_size.height() < preferred.height()) {
				current_size.set_height(preferred.height());
			}
		} else {
			// TODO: if geometry is not locked, to sth.
		}

		items().push_back(object);

		SetPreferredSize(preferred);
		SetMinimalSize(min);

		if(object->expand_y()) m_expandable_items.insert(object);
		else m_fixed_items.insert(object);

		if(! (current_size == size()))
			Resize(this, current_size);	// call make_layout() through this function
		else
			make_layout(&current_size);

		activate_events();
		object->activate_events();

		bind(object);
		LockGeometry(object, true);
	}

	void VerticalLayout::remove_item(AbstractWidget * object)
	{
		deactivate_events();

		std::vector<AbstractWidget*>::iterator it;
		for(it = items().begin(); it != items().end();)
		{
			if ((*it) == object) {
				it = items().erase(it);
			} else {
				it++;
			}
		}

		if(object->expand_y())
				m_expandable_items.erase(object);
		else
			m_fixed_items.erase(object);

		Size new_preferred_size;
		Size new_minimal_size;

		get_size_hint(true, true, 0, &new_minimal_size, &new_preferred_size);

		SetMinimalSize(new_minimal_size);
		SetPreferredSize(new_preferred_size);

		make_layout();

		activate_events();

		LockGeometry(object, false);
		unbind(object);
	}

	void VerticalLayout::make_layout()
	{
		if (size().height() == preferred_size().height()) {
			distribute_with_preferred_height();			// layout along y with preferred size
		} else if (size().height() < preferred_size().height()) {
			distribute_with_small_height();			// layout along y with small size
		} else {
			distribute_with_large_height();			// layout along y with large size
		}

		align();
	}

	void VerticalLayout::make_layout(const Size* size)
	{
		if (size->height() == preferred_size().height()) {
			distribute_with_preferred_height(size);			// layout along y with preferred size
		} else if (size->height() < preferred_size().height()) {
			distribute_with_small_height(size);			// layout along y with small size
		} else {
			distribute_with_large_height(size);			// layout along y with large size
		}

		align(size);
	}

	void VerticalLayout::distribute_with_preferred_height()
	{
		int y = position().y() + size().height() - margin().top();

		std::vector<AbstractWidget*>::reverse_iterator it;
		AbstractWidget* child = 0;
		for(it = items().rbegin(); it != items().rend(); it++)
		{
			if(! (it == items().rbegin()))
				y -= space();

			child = *it;
			Resize(child, child->size().width(), child->preferred_size().height());

			y -= child->size().height();
			SetPosition(child, child->position().x(), y);
		}
	}

	void VerticalLayout::distribute_with_preferred_height(const Size* size)
	{
		int y = position().y() + size->height() - margin().top();

		std::vector<AbstractWidget*>::reverse_iterator it;
		AbstractWidget* child = 0;
		for(it = items().rbegin(); it != items().rend(); it++)
		{
			if(! (it == items().rbegin()))
				y -= space();

			child = *it;
			Resize(child, child->size().width(), child->preferred_size().height());
			y -= child->size().height();
			SetPosition(child, child->position().x(), y);
		}
	}


	void VerticalLayout::distribute_with_small_height()
	{
		unsigned int min_exp_h = minimal_expandable_height();
		unsigned int fixed_h = fixed_height();

		unsigned int current_height = size().height();

		unsigned int h_plus = margin().top() + margin().bottom();

		std::vector<AbstractWidget*>::reverse_iterator it;
		AbstractWidget* child = 0;
		int y = position().y() + size().height() - margin().top();

		if((current_height - h_plus) >=
				(min_exp_h + fixed_h + (items().size() - 1) * space())) {

			unsigned int single_height = current_height - h_plus - fixed_h - (items().size() - 1) * space();
			single_height = single_height / m_expandable_items.size();

			for(it = items().rbegin(); it != items().rend(); it++)
			{
				child = *it;

				if(! (it == items().rbegin()))
					y -= space();

				if(m_expandable_items.count(child)) {
					Resize(child, child->size().width(), single_height);
				} else {
					Resize(child, child->size().width(), child->preferred_size().height());
				}

				y -= child->size().height();
				SetPosition(child, child->position().x(), y);
			}

		} else {

			std::set<AbstractWidget*> normal_items(m_fixed_items);	// set of unminimized items

			unsigned int unminimal_height = current_height - h_plus - min_exp_h - (items().size() - 1) * space();
			unsigned int h = unminimal_height / normal_items.size();

			for(it = items().rbegin(); it != items().rend(); it++)
			{
				child = *it;

				if(! (it == items().rbegin()))
					y -= space();

				if(m_expandable_items.count(child)) {
					Resize(child, child->size().width(), child->minimal_size().height());
				} else {

					if(h < child->minimal_size().height()) {
						Resize(child, child->size().width(), child->minimal_size().height());
						normal_items.erase(child);
						unminimal_height = unminimal_height - child->minimal_size().height();
						h = unminimal_height / normal_items.size();
						reset_height_of_fixed_items(&normal_items, h);
					} else {
						Resize(child, child->size().width(), h);
					}

				}

				y -= child->size().height();
				SetPosition(child, child->position().x(), y);
			}

		}
	}

	void VerticalLayout::distribute_with_small_height(const Size* size)
	{
		unsigned int min_exp_h = minimal_expandable_height();
		unsigned int fixed_h = fixed_height();

		unsigned int current_height = size->height();

		unsigned int h_plus = margin().top() + margin().bottom();

		std::vector<AbstractWidget*>::reverse_iterator it;
		AbstractWidget* child = 0;
		int y = position().y() + size->height() - margin().top();

		if((current_height - h_plus) >=
				(min_exp_h + fixed_h + (items().size() - 1) * space())) {

			unsigned int single_height = current_height - h_plus - fixed_h - (items().size() - 1) * space();
			single_height = single_height / m_expandable_items.size();

			for(it = items().rbegin(); it != items().rend(); it++)
			{
				child = *it;

				if(! (it == items().rbegin()))
					y -= space();

				if(m_expandable_items.count(child)) {
					Resize(child, child->size().width(), single_height);
				} else {
					Resize(child, child->size().width(), child->preferred_size().height());
				}

				y -= child->size().height();
				SetPosition(child, child->position().x(), y);
			}

		} else {

			std::set<AbstractWidget*> normal_items(m_fixed_items);	// set of unminimized items

			unsigned int unminimal_height = current_height - h_plus - min_exp_h - (items().size() - 1) * space();
			unsigned int h = unminimal_height / normal_items.size();

			for(it = items().rbegin(); it != items().rend(); it++)
			{
				child = *it;

				if(! (it == items().rbegin()))
					y -= space();

				if(m_expandable_items.count(child)) {
					Resize(child, child->size().width(), child->minimal_size().height());
				} else {

					if(h < child->minimal_size().height()) {
						Resize(child, child->size().width(), child->minimal_size().height());
						normal_items.erase(child);
						unminimal_height = unminimal_height - child->minimal_size().height();
						h = unminimal_height / normal_items.size();
						reset_height_of_fixed_items(&normal_items, h);
					} else {
						Resize(child, child->size().width(), h);
					}

				}

				y -= child->size().height();
				SetPosition(child, child->position().x(), y);
			}

		}
	}

	void VerticalLayout::distribute_with_large_height()
	{
		unsigned int fixed_h = fixed_height();

		unsigned int current_height = size().height();

		unsigned int h_plus = margin().top() + margin().bottom();

		std::vector<AbstractWidget*>::reverse_iterator it;
		AbstractWidget* child = 0;
		int y = position().y() + size().height() - margin().top();

		unsigned int single_height = current_height - h_plus - fixed_h - (items().size() - 1) * space();

		if(m_expandable_items.size())
			single_height = single_height / m_expandable_items.size();

		for(it = items().rbegin(); it != items().rend(); it++)
		{
			child = *it;

			if (!(it == items().rbegin()))
				y -= space();

			if (m_expandable_items.count(child)) {
				Resize(child, child->size().width(), single_height);
			} else {
				Resize(child, child->size().width(), child->preferred_size().height());
			}

			y -= child->size().height();
			SetPosition(child, child->position().x(), y);
		}
	}

	void VerticalLayout::distribute_with_large_height(const Size* size)
	{
		unsigned int fixed_h = fixed_height();

		unsigned int current_height = size->height();

		unsigned int h_plus = margin().top() + margin().bottom();

		std::vector<AbstractWidget*>::reverse_iterator it;
		AbstractWidget* child = 0;
		int y = position().y() + size->height() - margin().top();

		unsigned int single_height = current_height - h_plus - fixed_h - (items().size() - 1) * space();

		if(m_expandable_items.size())
			single_height = single_height / m_expandable_items.size();

		for(it = items().rbegin(); it != items().rend(); it++)
		{
			child = *it;

			if (!(it == items().rbegin()))
				y -= space();

			if (m_expandable_items.count(child)) {
				Resize(child, child->size().width(), single_height);
			} else {
				Resize(child, child->size().width(), child->preferred_size().height());
			}

			y -= child->size().height();
			SetPosition(child, child->position().x(), y);
		}
	}


	void VerticalLayout::align()
	{
		int x = position().x() + margin().left();

		unsigned int w = size().width() - margin().left() - margin().right();

		std::vector<AbstractWidget*>::reverse_iterator it;
		AbstractWidget* child = 0;
		for(it = items().rbegin(); it != items().rend(); it++)
		{
			child = *it;

			if (child->expand_x() ||
					(child->size().width() > w)) {
				Resize(child, w, child->size().height());
				SetPosition(child, x, child->position().y());
			} else {

				if (alignment() & AlignLeft) {
					SetPosition(child, x, child->position().y());
				} else if (alignment() & AlignRight) {
					SetPosition(child, x + (w - child->size().width()), child->position().y());
				} else if (alignment() & AlignVerticalCenter) {
					SetPosition(child, x + (w - child->size().width()) / 2, child->position().y());
				}

			}
		}

	}

	void VerticalLayout::align(const Size* size)
	{
		int x = position().x() + margin().left();

		unsigned int w = size->width() - margin().left() - margin().right();

		std::vector<AbstractWidget*>::reverse_iterator it;
		AbstractWidget* child = 0;
		for(it = items().rbegin(); it != items().rend(); it++)
		{
			child = *it;

			if (child->expand_x() ||
					(child->size().width() > w)) {
				Resize(child, w, child->size().height());
				SetPosition(child, x, child->position().y());
			} else {

				if (alignment() & AlignLeft) {
					SetPosition(child, x, child->position().y());
				} else if (alignment() & AlignRight) {
					SetPosition(child, x + (w - child->size().width()), child->position().y());
				} else if (alignment() & AlignVerticalCenter) {
					SetPosition(child, x + (w - child->size().width()) / 2, child->position().y());
				}

			}
		}

	}

	void VerticalLayout::reset_height_of_fixed_items (std::set<AbstractWidget*>* items,
													  unsigned int height)
	{
		std::set<AbstractWidget*>::iterator it;

		for(it = items->begin(); it != items->end(); it++)
		{
			Resize(*it, (*it)->size().width(), height);
		}
	}

	unsigned int VerticalLayout::minimal_expandable_height()
	{
		unsigned int height = 0;

		std::set<AbstractWidget*>::iterator it;
		for(it = m_expandable_items.begin(); it != m_expandable_items.end(); it++)
		{
			height += (*it)->minimal_size().height();
		}

		return height;
	}

	unsigned int VerticalLayout::fixed_height()
	{
		unsigned int height = 0;

		std::set<AbstractWidget*>::iterator it;
		for(it = m_fixed_items.begin(); it != m_fixed_items.end(); it++)
		{
			height += (*it)->size().height();
		}

		return height;
	}

	void VerticalLayout::get_size_hint (bool count_margin,
										bool count_space,
										Size* size,
										Size* min,
										Size* preferred)
	{
		Size size_out;
		Size min_size_out;
		Size preferred_size_out;

		AbstractWidget* child;
		std::vector<AbstractWidget*>::reverse_iterator it;

		if(count_margin) {
			size_out.set_height(margin().top());
			min_size_out.set_height(margin().top());
			preferred_size_out.set_height(margin().top());
		}

		for(it = items().rbegin(); it != items().rend(); it++)
		{
			child = *it;

			size_out.set_width(std::max(size_out.width(), child->size().width()));
			size_out.add_height(child->size().height());

			min_size_out.set_width(std::max(min_size_out.width(), child->minimal_size().width()));
			min_size_out.add_height(child->minimal_size().height());

			preferred_size_out.set_width(std::max(preferred_size_out.width(), child->preferred_size().width()));
			preferred_size_out.add_height(child->preferred_size().height());
		}

		if(count_margin) {
			size_out.add_height(margin().bottom());
			size_out.add_width(margin().left() + margin().right());

			min_size_out.add_height(margin().bottom());
			min_size_out.add_width(margin().left() + margin().right());

			preferred_size_out.add_height(margin().bottom());
			preferred_size_out.add_width(margin().left() + margin().right());
		}

		if(count_space) {
			if(items().size()) {
				size_out.add_height((items().size() - 1) * space());
				min_size_out.add_height((items().size() - 1) * space());
				preferred_size_out.add_height((items().size() - 1) * space());
			}
		}

		if(size) *size = size_out;
		if(min) *min = min_size_out;
		if(preferred) *preferred = preferred_size_out;
	}

}
