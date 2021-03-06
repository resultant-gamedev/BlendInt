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

/*
 * !!! Deprecated, use GLBuffer<> instead !!!
 */

#ifndef _BLENDINT_GLELEMENTARRAYBUFFER_HPP_
#define _BLENDINT_GLELEMENTARRAYBUFFER_HPP_

#include <blendint/core/object.hpp>

namespace BlendInt {

	/**
	 * @brief A simple element array buffer with 1 property for all buffers
	 *
	 * @ingroup opengl
	 */
	class GLElementArrayBuffer: public Object
	{
	public:

		GLElementArrayBuffer ();

		virtual ~GLElementArrayBuffer ();

		/**
		 * @brief Generate buffer
		 *
		 * Generate new GL buffers, if there's buffer generated before, this will delete these.
		 */
		inline void generate ()
		{
			if(id_ != 0) clear();
			glGenBuffers(1, &id_);
		}

		/**
		 * @brief Delete buffer created
		 */
		inline void clear ()
		{
			glDeleteBuffers(1, &id_);
			id_ = 0;
		}

		/**
		 * @brief The buffer id
		 * @return
		 */
		inline GLuint id () const {return id_;}

		inline bool is_buffer () const
		{
			return glIsBuffer(id_);
		}

		inline void bind () const
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
		}

		inline void set_data (GLsizeiptr size, const GLvoid* data, GLenum usage = GL_STATIC_DRAW)
		{
			glBufferData (GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
		}

		static inline void reset ()
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		inline GLenum target ()
		{
			return GL_ELEMENT_ARRAY_BUFFER;
		}

		inline GLenum get_usage () const
		{
			GLint usage = 0;
			glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_USAGE, &usage);
			return usage;
		}

		inline GLint get_buffer_size () const
		{
			GLint buffer_size = 0;
			glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &buffer_size);
			return buffer_size;
		}

	private:

		GLuint id_;

	};

}

#endif /* _BLENDINT_GLELEMENTARRAYBUFFER_HPP_ */
