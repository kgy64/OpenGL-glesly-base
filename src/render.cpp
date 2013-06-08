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
    myCameraMatrix(*this, "camera_matrix", myCamera),
    myT1Matrix(*this, "t0_matrix", myTransform[0]),
    myT2Matrix(*this, "t1_matrix", myTransform[1]),
    myT3Matrix(*this, "t2_matrix", myTransform[2]),
    myT4Matrix(*this, "t3_matrix", myTransform[3])
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

 for (Objects::iterator i = myObjects.begin(); i != myObjects.end(); ++i) {
    (*i)->NextFrame();
 }

 UnuseProgram();
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
