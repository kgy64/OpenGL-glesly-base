/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Shader variable handler classes
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "shader-vars.h"

using namespace Glesly::Shaders;

void AttribManager::UninitGL(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
 Threads::Lock _l(membersMutex);
 for (AttribElement * i = myAttribs; i; i=i->next) {
    i->uninitGL();
 }
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
