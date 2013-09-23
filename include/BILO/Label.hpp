/*
 * This file is part of BILO (Blender Interface Library).
 *
 * BILO (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BILO_LABEL_HPP_
#define _BILO_LABEL_HPP_

#include <string>

#include <BILO/Widget.hpp>
#include <BILO/Font.hpp>
#include <BILO/Color.hpp>

#include <BILO/String.hpp>
#include <BILO/Types.hpp>

using namespace std;

namespace BILO {

	class Label: public Widget
	{
	public:

		Label (const String& label);

		Label (const String& label, Drawable * parent);

		virtual ~Label ();

		void set_text (const String& label);

		void set_font (const Font& font);

		void set_foreground (const Color& fg)
		{
			foreground_ = fg;
		}

		void set_background (const Color& color = Color(0x00000000))
		{
			background_ = color;
		}

	protected:

		virtual void render ();

	private:
		// member functions

		void updateVertexArray (float x, float y, float sx, float sy);

		virtual void cursorPosEvent (double xpos, double ypos);

	private:

		/**
		 * @brief the text of the label
		 *
		 * new line character is not allowed
		 */
		String text_;

		Font font_;

		Color foreground_;

		/** Background color, default: transparent */
		Color background_;

		Vertex2D* vertex_array_;

		int valid_text_length_;

	private:

		Label (const Label& orig);

		Label& operator = (const Label& orig);
	};

} /* namespace BILO */
#endif /* _BILOLABEL_HPP_ */