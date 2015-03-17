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

#pragma once

#include <gui/abstract-round-widget.hpp>

namespace BlendInt {

  /**
   * @brief Container which controls sub widget round corner.
   *
   * @ingroup blendint_gui_widgets
   */
  class Block: public AbstractRoundWidget
  {
  public:

    Block (Orientation orienation = Horizontal);

    virtual ~Block ();

    void AddWidget (AbstractRoundWidget* widget);

    void InsertWidget (int index, AbstractRoundWidget* widget);

    virtual bool IsExpandX () const;

    virtual bool IsExpandY () const;

    virtual Size GetPreferredSize () const;

  protected:

    virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

    virtual Response Draw (AbstractWindow* context);

  private:

    void FillInHBlock (const Size& out_size);

    void FillInHBlock (int x, int y, int w, int h);

    void FillInVBlock (const Size& out_size);

    void FillInVBlock (int x, int y, int w, int h);

  private:

    Orientation orientation_;

  };

}
