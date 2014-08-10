/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Classes representing an object list with effects possibility
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_OBJECT_LIST_EFFECT_H_INCLUDED__
#define __GLESLY_SRC_OBJECT_LIST_EFFECT_H_INCLUDED__

#include <glesly/effects.h>
#include <glesly/object-list-base.h>
#include <Threads/Mutex.h>
#include <System/TimeDelay.h>
#include <Debug/Debug.h>

namespace Glesly
{
    class ObjectsWithEffect: public Glesly::ObjectListBase
    {
     public:
        inline void CheckNextLayer(Glesly::Render & render)
        {
            LayerCreatorPtr creator = myNextLayer;
            if (creator.get()) {
                myNextLayer.reset();
                // Put it on the stack:
                LayerEffectPtr effect = creator->GetEffect(render);
                effect->SetPreviousObjects(GetObjectListPtr());
                myLayers.push(effect);
                effect->Start();
            }
        }

        void PushLayer(LayerCreatorPtr creator)
        {
            myNextLayer = creator; // Don't worry if overwrites the previous one
        }

        bool PopLayer(void)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            SYS_DEBUG(DL_INFO1, "size: " << myLayers.size());
            // Make sure that the root effect is not removed:
            if (myLayers.size() <= 1) {
                return false;
            }
            SYS_DEBUG(DL_INFO1, "size: " << myLayers.size() << " accepted!");
            GetActualEffect()->Drop(myLayers);
            return true;
        }

     protected:
        inline ObjectsWithEffect(void)
        {
            LayerEffectPtr root_effect = JumpEffect::Create();
            myLayers.push(root_effect);
        }

        inline LayerEffectPtr GetActualEffect(void)
        {
            return myLayers.top();
        }

        inline const LayerEffectPtr & GetActualEffect(void) const
        {
            return myLayers.top();
        }

        virtual ObjectListPtr & GetObjectListPtr(void)
        {
            return GetActualEffect()->GetObjects();
        }

        inline bool IsEffectActive(void) const
        {
            return GetActualEffect()->IsActive();
        }

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::ObjectsWithEffect");

        ObjectLayerStack myLayers;

        LayerCreatorPtr myNextLayer;

    }; // class ObjectsWithEffect

} // namespace Glesly

#endif /* __GLESLY_SRC_OBJECT_LIST_EFFECT_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
