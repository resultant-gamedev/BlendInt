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

#ifndef _BLENDINT_GUI_TAB_HPP_
#define _BLENDINT_GUI_TAB_HPP_

#include <BlendInt/Gui/AbstractVectorContainer.hpp>

namespace BlendInt {

	class Tab: public AbstractVectorContainer
	{
		DISALLOW_COPY_AND_ASSIGN(Tab);

	public:

		Tab ();

		virtual ~Tab ();

		void Add (const String& title, AbstractWidget* widget);

		virtual bool IsExpandX () const;

		virtual bool IsEXpandY () const;

		virtual Size GetPreferredSize () const;

	protected:

		virtual void UpdateContainer (const ContainerUpdateRequest& request);

		virtual void ProcessPositionUpdate (const PositionUpdateRequest& request);

		virtual void ProcessSizeUpdate (const SizeUpdateRequest& request);

		virtual ResponseType Draw (const RedrawEvent& event);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

	private:

		void OnButtonToggled (int index, bool toggled);

		void FillSubWidgetsInTab (const Size& out_size, const Margin& margin);

		void FillSubWidgetsInTab (int x, int y, int w, int h);
	};

}

#endif /* _BLENDINT_GUI_TAB_HPP_ */
