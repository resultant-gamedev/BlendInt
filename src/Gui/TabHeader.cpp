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
#endif	// __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/TabHeader.hpp>
#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>

namespace BlendInt {

	using Stock::Shaders;

	TabHeader::TabHeader()
	: AbstractContainer(),
	  vao_(0)
	{
		set_size(400, 24);
		set_margin(2, 2, 0, 0);

		glGenVertexArrays(1, &vao_);

		glBindVertexArray(vao_);

		VertexTool tool;
		tool.GenerateVertices(size(), 0, RoundNone, 0);

		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();

		inner_->set_data(tool.inner_size(), tool.inner_data());
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::reset();

		events()->connect(m_group.button_index_clicked(), &m_button_index_clicked, &Cpp::Event<int>::fire);
		//events()->connect(m_group.button_index_clicked(), this, &TabHeader::OnButtonIndexClicked);
		events()->connect(m_group.button_index_toggled(), this, &TabHeader::OnButtonIndexToggled);

		// FIXME: cannot use the following line
		//events()->connect(m_group.button_index_toggled(), &m_button_index_toggled, &Cpp::Event<int, bool>::fire);
	}

	TabHeader::~TabHeader()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	void TabHeader::PushBack (TabButton* button)
	{
		int x = GetLastPosition ();
		int y = position().y() + margin().bottom();
		int h = size().height() - margin().vsum();

		if (PushBackSubWidget(button)) {

			SetSubWidgetPosition(button, x, y);
			if (button->IsExpandY()) {
				ResizeSubWidget(button, button->size().width(), h);
			} else {

				if (button->size().height() > h) {
					ResizeSubWidget(button, button->size().width(), h);
				}

			}

			m_group.PushBack(button);

			if(m_group.button_count() == 1) {
				button->SetChecked(true);
			}
		}
	}

	bool TabHeader::IsExpandX () const
	{
		return true;
	}

	Size TabHeader::GetPreferredSize () const
	{
		Size prefer(400, 24);

		if(first() == 0) {
			Font font;
			int max_font_height = font.GetHeight();
			prefer.set_height(max_font_height + margin().vsum());
		} else {
			Size tmp_size;

			for(AbstractWidget* p = first(); p; p = p->next())
			{
				if(p->visiable()) {
					tmp_size = p->GetPreferredSize();

					prefer.add_width(tmp_size.width());
					prefer.set_height(std::max(prefer.height(), tmp_size.height()));
				}
			}

			prefer.add_width(margin().hsum());
			prefer.add_height(margin().vsum());
		}

		return prefer;
	}

	void TabHeader::PerformMarginUpdate(const Margin& request)
	{
		// TODO: change sub widgets
	}

	void TabHeader::PerformPositionUpdate (const PositionUpdateRequest& request)
	{
		if(request.target() == this) {
			int x = request.position()->x() - position().x();
			int y = request.position()->y() - position().y();
			set_position(*request.position());
			MoveSubWidgets(x, y);
		}

		if(request.source() != container()) {
			ReportPositionUpdate(request);
		}
	}

	void TabHeader::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
			tool.GenerateVertices(*request.size(), 0, RoundNone, 0);
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			GLArrayBuffer::reset();
			set_size(*request.size());
		}

		if(request.source() != container()) {
			ReportSizeUpdate(request);
		}
	}

	ResponseType TabHeader::Draw (Profile& profile)
	{
		Shaders::instance->triangle_program()->Use();

		glUniform3f(Shaders::instance->triangle_uniform_position(), (float) position().x(), (float) position().y(), 0.f);
		glUniform1i(Shaders::instance->triangle_uniform_gamma(), 0);
		glUniform1i(Shaders::instance->triangle_uniform_antialias(), 0);

		glVertexAttrib4f(Shaders::instance->triangle_attrib_color(), 0.208f, 0.208f, 0.208f, 1.0f);

		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
						GetOutlineVertices(round_type()) + 2);
		glBindVertexArray(0);

		Shaders::instance->triangle_program()->reset();

		return Ignore;
	}

	ResponseType TabHeader::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType TabHeader::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType TabHeader::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType TabHeader::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType TabHeader::MousePressEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType TabHeader::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType TabHeader::MouseMoveEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	void TabHeader::OnButtonIndexToggled(int index, bool toggled)
	{
		m_button_index_toggled.fire(index, toggled);
	}

	int TabHeader::GetLastPosition() const
	{
		int x = position().x() + margin().left();

		if(first()) {
			x = last()->position().x();
			x += last()->size().width();
		}

		return x;
	}

}
