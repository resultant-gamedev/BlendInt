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

#include <iostream>

#include <BlendInt/Label.hpp>

#include <BlendInt/FontCache.hpp>

namespace BlendInt {

	Label::Label (const String& text)
		: Form(), m_length(0), m_background(0x00000000)
	{
		FontCache::create(m_font);

		resize(80, 24);	// the same default size in Buttons
		set_text(text);
	}

	Label::Label (const String& text, AbstractForm *parent)
		: Form(parent), m_length(0), m_background(0x00000000)
	{
		FontCache::create(m_font);

		resize(80, 24);	// the same default size in Buttons
		set_text(text);
	}

	Label::~Label ()
	{

	}

	void Label::set_text (const String& label)
	{
		FontCache* fc = FontCache::create(m_font);

		m_text = label;

		m_text_outline = fc->get_text_outline(m_text);

		if(size().width() < m_text_outline.width()) {
			resize_priv(this, m_text_outline.width(), size().height());
		}

		if(size().height() < m_text_outline.height()) {
			resize_priv(this, size().width(), m_text_outline.height());
		}

		m_origin.set_x((size().width() - m_text_outline.width()) / 2);
		m_origin.set_y((size().height() - fc->get_height()) / 2 + std::abs(fc->get_descender()));

		m_length = m_text.length();

		set_preferred_size(m_text_outline.width(), m_text_outline.height());
	}

	void Label::set_font (const Font& font)
	{
		m_font = font;
		FontCache* fc =	FontCache::create(m_font);

		m_text_outline = fc->get_text_outline(m_text);

		m_origin.set_x((size().width() - m_text_outline.width()) / 2);
		m_origin.set_y((size().height() - fc->get_height()) / 2 + std::abs(fc->get_descender()));

		m_length = get_valid_text_size();

		set_preferred_size(m_text_outline.width(), m_text_outline.height());
	}

	void Label::update (int property_type)
	{
		switch(property_type) {

			case FormPropertySize: {
				if(size().height() < m_text_outline.height()) {
					m_length = 0;
				} else {
					FontCache* fc = FontCache::create(m_font);
					m_origin.set_y((size().height() - fc->get_height()) / 2 + std::abs(fc->get_descender()));
					m_length = get_valid_text_size();
				}

				if(size().width() < m_text_outline.width()) {
					m_origin.set_x(0);
				} else {
					m_origin.set_x((size().width() - m_text_outline.width()) / 2);
				}

				break;
			}

			default:
				break;
		}
	}

	void Label::render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position().x(),
					 position().y(),
					 z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glColor4ub(m_background.r(), m_background.g(),
				   m_background.b(), m_background.a());
		glRectf(0.0, 0.0, size().width(), size().height());

		FontCache::create(m_font)->print(m_origin.x(), m_origin.y(), m_text, m_length);

#ifdef DEBUG
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
#endif

		glDisable(GL_BLEND);

		glPopMatrix();

	}

	size_t Label::get_valid_text_size()
	{
		size_t width = 0;

		size_t str_len = m_text.length();

		while(str_len > 0) {

			width = FontCache::create(m_font)->get_text_width(m_text, str_len);

			if(width < size().width()) break;

			str_len--;
		}

		return str_len;
	}

} /* namespace BlendInt */
