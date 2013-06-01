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
    myVars(NULL),
    myAttribs(NULL)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
}

Object::~Object()
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 GetProgram().Remove(myIter);
}

void Object::BufferVariables(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 for (VBOAttribBase * i = myAttribs; i; i=i->next) {
    i->BufferData();
 }
}

void Object::UnbufferVariables(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 for (VBOAttribBase * i = myAttribs; i; i=i->next) {
    i->UnbufferData();
 }
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

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
