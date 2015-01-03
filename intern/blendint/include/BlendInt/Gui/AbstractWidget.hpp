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
 * License along with BlendInt.	 If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BLENDINT_GUI_ABSTRACTWIDGET_HPP_
#define _BLENDINT_GUI_ABSTRACTWIDGET_HPP_

#include <glm/glm.hpp>
#include <boost/smart_ptr.hpp>

#include <BlendInt/OpenGL/GLTexture2D.hpp>
#include <BlendInt/Gui/AbstractView.hpp>

namespace BlendInt {

	/**
	 * @brief A Normal widget
	 *
	 * @ingroup widgets
	 */
	class AbstractWidget: public AbstractView
	{
		DISALLOW_COPY_AND_ASSIGN(AbstractWidget);

	public:

		AbstractWidget ();

		virtual ~AbstractWidget();

		Cpp::EventRef<AbstractWidget*> destroyed ()
		{
			return *destroyed_;
		}

	protected:

		virtual bool PreDraw (const Context* context);

		// virtual ResponseType Draw (Profile& profile);

		virtual void PostDraw (const Context* context);

		virtual void PerformFocusOn (const Context* context);

		virtual void PerformFocusOff (const Context* context);

		virtual void PerformHoverIn (const Context* context);

		virtual void PerformHoverOut (const Context* context);

		virtual ResponseType PerformKeyPress (const Context* context);

		virtual ResponseType PerformContextMenuPress (const Context* context);

		virtual ResponseType PerformContextMenuRelease (const Context* context);

		virtual ResponseType PerformMousePress (const Context* context);

		virtual ResponseType PerformMouseRelease (const Context* context);

		virtual ResponseType PerformMouseMove (const Context* context);

		Cpp::ConnectionScope* events() const {return events_.get();}

		static bool RenderSubWidgetsToTexture (
			AbstractWidget* widget,
			const Context* context,
			GLTexture2D* texture);

	private:

		boost::scoped_ptr<Cpp::ConnectionScope> events_;

		boost::scoped_ptr<Cpp::Event<AbstractWidget*> > destroyed_;

	};

}

#endif /* _BLENDINT_GUI_ABSTRACTWIDGET_HPP_ */
