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

#ifndef _BLENDINT_GUI_BRIGHTNESSSLIDER_HPP_
#define _BLENDINT_GUI_BRIGHTNESSSLIDER_HPP_

#include <BlendInt/Gui/AbstractSlider.hpp>
#include <BlendInt/Gui/CircularPicker.hpp>

namespace BlendInt {

	class BrightnessSlider: public AbstractSlider<int>
	{
		DISALLOW_COPY_AND_ASSIGN(BrightnessSlider);

	public:

		BrightnessSlider (Orientation orientation = Horizontal);

		virtual ~BrightnessSlider ();

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

		virtual Size GetPreferredSize () const;

	protected:

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual void PerformRoundTypeUpdate (int round_type);

		virtual void PerformRoundRadiusUpdate (float radius);

		virtual ResponseType Draw (Profile& profile);

	private:

		void InitializeBrightnessSlider ();

		GLuint vao_[2];

		RefPtr<GLArrayBuffer> inner_;
		RefPtr<GLArrayBuffer> outer_;

		RefPtr<CircularPicker> picker_;

	};

}

#endif /* _BLENDINT_GUI_BRIGHTNESSSLIDER_HPP_ */