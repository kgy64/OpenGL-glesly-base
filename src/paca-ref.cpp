/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Rectangle-shaped object using pangocairo with independent Cairo and GLES objects
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "paca-ref.h"

using namespace Glesly;

void PaCaRef::Frame(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 UseBlend _b;

 DrawElements(GL_TRIANGLES, 6);
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
