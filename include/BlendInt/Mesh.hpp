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

#ifndef _BLENDINT_MESH_HPP_
#define _BLENDINT_MESH_HPP_

#include <glm/glm.hpp>
#include <BlendInt/Object.hpp>

#include <BlendInt/GLArrayBuffer.hpp>
#include <BlendInt/GLSLProgram.hpp>
#include <BlendInt/GLElementArrayBuffer.hpp>

namespace BlendInt {

	class Mesh: public Object
	{
	public:

		Mesh ();

		void Render (const glm::mat4& MVP);

		void SetProgram (GLSLProgram* program);

		void SetVertexBuffer (GLArrayBuffer* vb);

		void SetIndexBuffer (GLElementArrayBuffer* ib);

	protected:

		virtual ~Mesh();

	private:

		GLArrayBuffer* m_vertices;

		GLElementArrayBuffer* m_index;

		GLSLProgram* m_program;

	};

}

#endif /* _BLENDINT_MESH_HPP_ */