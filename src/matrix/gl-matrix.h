/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     GL(ES)-specific Matrix Operations
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_MATRIX_GL_MATRIX_H_INCLUDED__
#define __GLESLY_SRC_MATRIX_GL_MATRIX_H_INCLUDED__

#include <GLES/gl.h>

#include <matrix/matrix.h>

namespace Glesly
{
    class GLMatrix4: public Matrix<float, 4, 4>
    {
     public:
        inline void glMultiply(GLenum mode = GL_MODELVIEW)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            glMatrixMode(mode);
            glMultMatrix(myMatrix);
        }

        inline void glLoad(GLenum mode = GL_MODELVIEW)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            glMatrixMode(mode);
            glLoadMatrix(myMatrix);
        }
    }; // class GLMatrix4

} // namespace Glesly

#endif /* __GLESLY_SRC_MATRIX_GL_MATRIX_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
