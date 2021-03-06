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

#include <glm/gtx/matrix_transform_2d.hpp>

#include <blendint/opengl/opengl.hpp>

#include <blendint/gui/scroll-view.hpp>
#include <blendint/gui/abstract-window.hpp>

namespace BlendInt {

ScrollView::ScrollView()
    : AbstractScrollable(),
      vao_(0),
      m_orientation(Horizontal | Vertical),
      moving_(false)
{
  set_size(400, 300);

  std::vector<GLfloat> inner_verts;
  GenerateRoundedVertices(&inner_verts, 0);

  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  inner_.generate();
  inner_.bind();
  inner_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

  glEnableVertexAttribArray(AttributeCoord);
  glVertexAttribPointer(AttributeCoord, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindVertexArray(0);
  inner_.reset();
}

ScrollView::~ScrollView ()
{
  glDeleteVertexArrays(1, &vao_);
}

void ScrollView::Setup (AbstractWidget* widget)
{
  if(PushBackSubView(widget)) {
    RequestRedraw();
  }
}

void ScrollView::CentralizeViewport()
{
  if(first() == 0) return;

  AbstractView* p = first();

  int w = size().width();
  int h = size().height();

  int x = position().x();
  x += (w - static_cast<int>(p->size().width())) / 2;
  int y = position().y();
  y += (h - static_cast<int>(p->size().height())) / 2;

  MoveSubViewTo(p, x, y);
}

int BlendInt::ScrollView::GetHPercentage () const
{
  int percentage = 0;

  if(first()) {
    AbstractView* p = first();

    int w = size().width();

    if (p->size().width() <= w) {
      percentage = 100;
    } else {
      percentage = w * 100 / p->size().width();
    }

  }

  return percentage;
}

int BlendInt::ScrollView::GetVPercentage () const
{
  int percentage = 0;

  if(first()) {
    AbstractView* p = first();

    int h = size().height();

    if (p->size().height() <= h) {
      percentage = 100;
    } else {
      percentage = h * 100 / p->size().height();
    }

  }

  return percentage;
}

void ScrollView::MoveViewport(int x, int y)
{
  if(first()) {

    if(x != 0 || y != 0) {
      AbstractView* p = first();
      MoveSubViewTo(p, p->position().x() + x, p->position().y() + y);

      RequestRedraw();
    }
  }
}

void ScrollView::SetReletivePosition (int x, int y)
{
  if(first()) {
    AbstractView* p = first();

    MoveSubViewTo(p, position().x() + x, position().y() + y);

    RequestRedraw();
  }
}

bool ScrollView::IsExpandX() const
{
  if(first()) {
    return first()->IsExpandX();
  } else {
    return false;
  }
}

bool ScrollView::IsExpandY() const
{
  if(first()) {
    return first()->IsExpandY();
  } else {
    return false;
  }
}

Size ScrollView::GetPreferredSize() const
{
  Size prefer(400, 300);

  AbstractView* widget = first();

  if(widget) {
    prefer = widget->GetPreferredSize();
  }

  return prefer;
}

void ScrollView::PerformSizeUpdate (const AbstractView* source, const AbstractView* target, int width, int height)
{
  if (target == this) {

    set_size(width, height);

    std::vector<GLfloat> inner_verts;
    GenerateRoundedVertices(&inner_verts, 0);

    inner_.bind();
    inner_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
    inner_.reset();

    // align the subwidget
    if (first()) {

      int dy = height - size().height();

      first()->MoveTo(first()->position().x(),
                      first()->position().y() + dy);
    }

  }

  if(source == this) {
    report_size_update(source, target, width, height);
  }
}

bool ScrollView::PreDraw(AbstractWindow* context)
{
  glm::mat3 matrix = glm::translate(AbstractWindow::shaders()->widget_model_matrix(),
                                    glm::vec2(position().x(), position().y()));

  AbstractWindow::shaders()->PushWidgetModelMatrix();
  AbstractWindow::shaders()->SetWidgetModelMatrix(matrix);

  AbstractWindow::shaders()->widget_inner_program()->use();

  glUniform1i(AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_GAMMA), 0);
  glUniform1i(
      AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_SHADED),
      0);

  if(subview_count()) {
    glUniform4f(AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_COLOR), 0.908f, 0.208f, 0.208f, 0.25f);
  } else {
    glUniform4f(AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_COLOR), 0.947f, 0.447f, 0.447f, 0.25f);
  }

  glBindVertexArray(vao_);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

  context->BeginPushStencil();	// inner stencil
  glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
  context->EndPushStencil();

  return true;
}

Response ScrollView::Draw (AbstractWindow* context)
{
  if(subview_count()) {

    Point offset = GetOffset();

    glm::mat3 matrix = glm::translate(AbstractWindow::shaders()->widget_model_matrix(),
                                      glm::vec2(offset.x(), offset.y()));

    AbstractWindow::shaders()->PushWidgetModelMatrix();
    AbstractWindow::shaders()->SetWidgetModelMatrix(matrix);

    return Ignore;

  } else {
    return Finish;
  }
}

void ScrollView::PostDraw(AbstractWindow* context)
{
  if(subview_count())
    AbstractWindow::shaders()->PopWidgetModelMatrix();

  // draw mask
  AbstractWindow::shaders()->widget_inner_program()->use();
  glUniform1i(AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_GAMMA), 0);
  glUniform1i(
      AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_SHADED),
      0);

  if(subview_count()) {
    glUniform4f(AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_COLOR), 0.908f, 0.208f, 0.208f, 0.25f);
  } else {
    glUniform4f(AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_COLOR), 0.947f, 0.447f, 0.447f, 0.25f);
  }

  glBindVertexArray(vao_);
  context->BeginPopStencil();	// pop inner stencil
  glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
  context->EndPopStencil();

  AbstractWindow::shaders()->PopWidgetModelMatrix();
}

Response ScrollView::PerformMousePress (AbstractWindow* context)
{
  if (context->GetMouseButton() == MouseButtonMiddle) {
    moving_ = true;
    cursor_point_ = context->GetGlobalCursorPosition();
    last_offset_ = GetOffset();

    return Finish;
  }

  return subview_count() ? Ignore : Finish;
}

Response ScrollView::PerformMouseRelease(AbstractWindow* context)
{
  if(moving_) {
    moving_ = false;
    RequestRedraw();
  }

  return subview_count() ? Ignore : Finish;
}

Response ScrollView::PerformMouseMove(AbstractWindow* context)
{
  if(moving_) {

    int ox = context->GetGlobalCursorPosition().x() - cursor_point_.x();
    int oy = context->GetGlobalCursorPosition().y() - cursor_point_.y();

    set_offset(last_offset_.x() + ox, last_offset_.y() + oy);

    if((ox != 0) || (oy != 0)) {
      RequestRedraw();
    }

    return Finish;
  }

  return Ignore;
}

}
