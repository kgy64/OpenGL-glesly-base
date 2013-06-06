/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Generic OpenGL object classes
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <GLES/egl.h>
#include <GLES/gl.h>

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

void Object::NextFrame(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 UseProgram();

 glMatrixMode(GL_MODELVIEW);
 glPushMatrix();

 ActivateVariables();
 BufferVariables();
 Frame();
 UnbufferVariables();

 glMatrixMode(GL_MODELVIEW);
 glPopMatrix();

 SYS_DEBUG(DL_INFO3, " - glUseProgram(0);");
 glUseProgram(0);
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
