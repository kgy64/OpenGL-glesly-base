/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Class to execute one OpenGL program with Objects
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <GLES2/gl2.h>

#include <glesly/object.h>

#include "render.h"

using namespace Glesly;

Render::Render(float aspect):
    myScreenAspect(aspect),
    myCameraMatrix(*this, "camera_matrix", myCamera)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
}

Render::~Render()
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
}

void Render::NextFrame(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 UseProgram();

 ActivateVariables();

 Frame();

 ObjectListPtr p = GetObjectListPtr(); // The pointer is copied here to solve thread safety

 for (ObjectListIterator i = p->begin(); i != p->end(); ++i) {
    (*i)->NextFrame();
 }

 UnuseProgram();
}

void Render::MouseClickRaw(int x, int y, int index, int count)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 if (IsInputBlocked()) {
    return;
 }

 try {
    float horiz = x;
    float vert = y;

    ConvertMouseCoordinates(horiz, vert);

    ObjectListPtr p = GetObjectListPtr(); // The pointer is copied here to solve thread safety

    for (ObjectListIterator i = p->begin(); i != p->end(); ++i) {
       if ((*i)->MouseClick(horiz, vert, index, count)) {
           break;
       }
    }
 } catch(::EX::Assert & ex) {
    SYS_DEBUG(DL_WARNING, "Cannot calculate mouse position!");
 }
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
