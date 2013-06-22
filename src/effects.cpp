/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Classes executing layer transitions
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "effects.h"
#include <glesly/object.h>

using namespace Glesly;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *     Class LayerChangeEffectManager:                                                   *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void LayerChangeEffectManager::EffectFrame(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 LayerEffecrPtr effect = myEffect; // Copy to be thread-safe

 if (effect.get()) {
     FadeIn();
     effect->Frame();
 }

 FadeOut();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *     Class LayerChangeEffectBase:                                                      *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void LayerChangeEffectBase::Frame(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 SYS::TimeDelay now;
 int elapsed = (now - myStart).ToMillisecond();

 for (ObjectListIterator i = myObjects->begin(); i != myObjects->end(); ++i) {
    (*i)->NextFrame();
 }
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
