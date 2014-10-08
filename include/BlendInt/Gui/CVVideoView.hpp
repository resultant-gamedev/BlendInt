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

#ifndef _BLENDINT_GUI_CVVIDEOVIEW_HPP_
#define _BLENDINT_GUI_CVVIDEOVIEW_HPP_

// generate makefile with cmake -DENABLE_OPENCV to activate
#ifdef __USE_OPENCV__

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <BlendInt/Gui/Widget.hpp>
#include <BlendInt/OpenGL/GLBuffer.hpp>
#include <BlendInt/OpenGL/GLTexture2D.hpp>

namespace BlendInt {

	class CVVideoView: public Widget
	{
	public:

		CVVideoView();

		virtual ~CVVideoView ();

		bool OpenCamera (int n);

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

		virtual Size GetPreferredSize () const;

	protected:

		virtual ResponseType Draw (Profile& profile);

	private:

		void InitializeCVVideoView ();

		GLuint vao_;

		GLBuffer<> frame_plane_;

		GLTexture2D texture_;

		cv::VideoCapture video_stream_;

		cv::Mat frame_;
	};

}

#endif	// __USE_OPENCV__

#endif /* _BLENDINT_GUI_CVVIDEOVIEW_HPP_ */
