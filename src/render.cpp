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

Render::Render():
    myObjects(new ObjectList),
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

 ObjectListPtr p = GetObjectList(); // The pointer is copied here to solve thread safety

 for (ObjectList::iterator i = p->begin(); i != p->end(); ++i) {
    (*i)->NextFrame();
 }

 UnuseProgram();
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
