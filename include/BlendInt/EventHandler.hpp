/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BLENDINT_EVENTHANDLER_HPP_
#define _BLENDINT_EVENTHANDLER_HPP_

#include <BlendInt/MouseEvent.hpp>
#include <BlendInt/KeyEvent.hpp>
#include <BlendInt/ContextMenuEvent.hpp>

namespace BlendInt {

	class Interface;

	class EventHandler
	{
	public:

		friend class Interface;

		EventHandler ()
		: m_ticktack(0)
		{}
		virtual ~EventHandler () {}

	protected:

		virtual void press_key (KeyEvent* event) = 0;

		virtual void press_context_menu (ContextMenuEvent* event) = 0;

		virtual void release_context_menu (ContextMenuEvent* event) = 0;

		virtual void press_mouse (MouseEvent* event) = 0;

		virtual void release_mouse (MouseEvent* event) = 0;

		virtual void move_mouse (MouseEvent* event) = 0;

		virtual void render () = 0;

	private:

		/**
		 * A ticktack for one render loop
		 */
		unsigned char m_ticktack;
	};

} /* namespace BlendInt */
#endif /* _BIL_EVENTHANDLER_H_ */