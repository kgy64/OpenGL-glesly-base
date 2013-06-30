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
    myProjectionMatrix_var(*this, "projection_effect", params.projection),
    myObjectMatrix_var(*this, "object_effect", params.object)
{
}

void LayerChangeEffectManager::EffectFrame(LayerEffectPtr & effect, const SYS::TimeDelay & frame_start_time)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 if (effect->Step(myEffectParams)) {
    FadeOut();
    effect->Frame(frame_start_time);
 }

 FadeIn();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *     Class LayerChangeEffectBase:                                                      *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void LayerChangeEffectBase::Frame(const SYS::TimeDelay & frame_start_time)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 for (ObjectListIterator i = previousObjects->begin(); i != previousObjects->end(); ++i) {
    (*i)->NextFrame(frame_start_time);
 }
}

bool LayerChangeEffectBase::Step(Glesly::LayerChangeEffectBase::EffectParameters & params)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 if (!IsActive()) {
    return false;
 }

 float state = 1e-3*(float)SYS::TimeElapsed(myStart).ToMillisecond()/myTime;

 SYS_DEBUG(DL_INFO2, "Layer Effect: State=" << state << ", " << (layerContainer ? "outgoing" : "incoming"));

 if (layerContainer) {
    if (state >= 1.0f) {
        layerContainer->pop();
        layerContainer = NULL;
        params.Reset();
        active = false;
        state = 1.0f;
    } else {
        state = 1.0f - state;
    }
 } else {
    if (state >= 1.0f) {
        active = false;
        state = 1.0f;
    }
 }

 SetState(params, state);

 return true;
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
