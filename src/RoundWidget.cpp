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
#include <BlendInt/RoundWidget.hpp>

namespace BlendInt {

	RoundWidget::RoundWidget()
	: Widget(),
	  RoundBoxBase()
	{

	}

	RoundWidget::RoundWidget(AbstractWidget* parent)
	: Widget(parent),
	  RoundBoxBase()
	{

	}

	RoundWidget::~RoundWidget()
	{

	}

	void RoundWidget::set_round_type(int type)
	{
		if(round_type() == type) return;

		update(FormRoundType, &type);

		RoundBoxBase::set_round_type(type);
	}

	void RoundWidget::set_radius(float rad)
	{
		if(radius() == rad) return;

		update(FormRoundRadius, &rad);

		RoundBoxBase::set_radius(rad);
	}

	void RoundWidget::render()
	{
		float outer_v[WIDGET_SIZE_MAX][2];	// vertices for drawing outline
		float inner_v[WIDGET_SIZE_MAX][2];	// vertices for drawing inner

		VerticesSum vert_sum;

		vert_sum = generate_vertices(&(size()), border_width(), inner_v, outer_v);

		float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2]; /* + 2 because the last pair is wrapped */

		verts_to_quad_strip (inner_v, outer_v, vert_sum.total, quad_strip);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position().x(),
					 position().y(),
					 z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// draw inner, simple fill
		glColor4ub(themes()->regular.inner.r(),
		        themes()->regular.inner.g(),
		        themes()->regular.inner.b(),
		        themes()->regular.inner.a());

		draw_inner(inner_v, vert_sum.total);

//		draw_gl_buffer(m_inner_buffer.get());

		// draw outline
		unsigned char tcol[4] = { themes()->regular.outline.r(),
		        themes()->regular.outline.g(),
		        themes()->regular.outline.b(),
		        themes()->regular.outline.a()};
		tcol[3] = tcol[3] / WIDGET_AA_JITTER;
		glColor4ubv(tcol);

//		draw_gl_buffer_anti_alias(m_outer_buffer.get());

		draw_outline(quad_strip, vert_sum.total * 2 + 2);

		glDisable(GL_BLEND);
		glPopMatrix();
	}

}