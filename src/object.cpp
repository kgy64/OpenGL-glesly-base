/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Generic OpenGL object classes
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include <glesly/shader.h>
#include <glesly/error.h>

using namespace Glesly;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *       class Object:                                                                   *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Object::Object(Render & renderer):
    myProgram(renderer),
    p_matrix(*this, "p_matrix", myProjection),
    toBeErased(false)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
}

Object::~Object()
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
}

void Object::DrawArrays(GLenum mode, GLint first, GLsizei count)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 SYS_DEBUG(DL_INFO3, " - glDrawArrays(" << (int)mode << "," << (int)first << "," << (int)count << ");");

 glDrawArrays(mode, first, count);
 if (eglGetError() != EGL_SUCCESS) {
    throw Error("Could not glDrawArrays()");
 }
}

void Object::DrawElements(GLenum mode, GLsizei count)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 SYS_DEBUG(DL_INFO3, " - glDrawElements(" << (int)mode << "," << (int)count << ",GL_UNSIGNED_SHORT,0);");

 glDrawElements(mode, count, GL_UNSIGNED_SHORT, (void*)0);
 if (eglGetError() != EGL_SUCCESS) {
    throw Error("Could not glDrawElements()");
 }
}

void Object::NextFrame(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 ActivateVariables();
 BufferVariables();
 Frame();
 UnbufferVariables();
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
