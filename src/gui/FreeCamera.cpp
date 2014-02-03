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

#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <BlendInt/FreeCamera.hpp>

namespace BlendInt {

	FreeCamera::FreeCamera ()
	{
		m_translation = glm::vec3(0);
	}

	FreeCamera::~FreeCamera ()
	{
	}

	void FreeCamera::Update ()
	{
		glm::mat4 R = glm::yawPitchRoll(yaw(), pitch(), roll());

		set_position(position() + m_translation);
		// m_translation = glm::vec3(0);

		set_look(glm::vec3(R * glm::vec4(0, 0, 1, 0)));
		glm::vec3 target = position() + look();
		set_up(glm::vec3(R * glm::vec4(0, 1, 0, 0)));
		set_right(glm::cross(look(), up()));

		set_view(glm::lookAt(position(), target, up()));
	}

}
