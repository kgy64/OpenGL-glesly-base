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

LayerChangeEffectManager::EffectUniforms::EffectUniforms(const Glesly::Shaders::UniformManager & manager, Glesly::LayerChangeEffectBase::EffectParameters::Params & params):
    Glesly::Shaders::UniformManagerCopy(manager),
    myFade_var(*this, "effect_fade", params.fade),
    myEffectMatrix_var(*this, "effect_matrix", params.projection)
{
}

void LayerChangeEffectManager::EffectFrame(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 LayerEffecrPtr effect = myEffect; // Copy to be thread-safe

 if (!effect.get()) {
    FadeIn();
    return;
 }

 bool is_over = effect->Step();

 FadeIn();

 effect->Frame();
 if (is_over) {
    EffectFinished(effect);
 } else {
    FadeOut();
 }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *     Class LayerChangeEffectBase:                                                      *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void LayerChangeEffectBase::Frame(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 for (ObjectListIterator i = myObjects->begin(); i != myObjects->end(); ++i) {
    (*i)->NextFrame();
 }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *     Class FadeInEffect:                                                               *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FadeInEffect::Step(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 SYS::TimeDelay now;
 float state = 1e-3*(float)(now - myStart).ToMillisecond()/myTime;

 SYS_DEBUG(DL_INFO2, "KGY: state=" << state);

 bool status = false;

 if (state >= 1.0f) {
    status = true;
    state = 1.0f;
 }

 EffectParameters & params(GetEffectParameters());

 float opposite = 1.0f - state;

 params.fade_in.fade = state;

 params.fade_out.fade = opposite;

 return status;
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
