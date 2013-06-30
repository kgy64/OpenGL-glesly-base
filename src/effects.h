/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Classes executing layer transitions
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_EFFECTS_H_INCLUDED__
#define __GLESLY_SRC_EFFECTS_H_INCLUDED__

#include <GLES2/gl2.h>

#include <System/TimeElapsed.h>
#include <glesly/shader-uniforms.h>
#include <glesly/object-ptr.h>
#include <glesly/effect-ptr.h>
#include <glesly/camera.h>

namespace Glesly
{
    class LayerChangeEffectManager;
    class MenuRender;

    class LayerChangeEffectBase
    {
        friend class LayerChangeEffectManager;

     public:
        virtual ~LayerChangeEffectBase()
        {
        }

        inline void SetPreviousObjects(ObjectListPtr & obj)
        {
            previousObjects = obj;
        }

        ObjectListPtr & GetObjects(void)
        {
            return myObjects;
        }

        struct EffectParameters
        {
            struct Params {
                inline Params(float initial_fade):
                    fade(initial_fade)
                {
                }

                inline void Reset(void)
                {
                    fade = 1.0;
                    projection = 1.0;
                    object = 1.0;
                }

                GLfloat fade;
                Glesly::Transformation projection;
                Glesly::Transformation object;
            };

            inline EffectParameters(void):
                fade_in(1.0f),
                fade_out(1.0f)
            {
            }

            Params fade_in;

            Params fade_out;

            inline void Reset(void)
            {
                fade_in.Reset();
                fade_out.Reset();
            }
        };

        bool IsActive(void) const
        {
            return active;
        }

        bool Step(Glesly::LayerChangeEffectBase::EffectParameters & params);

        inline void RestartTimer(void)
        {
            myStart.SetNow();
        }

        inline void Start(void)
        {
            RestartTimer();
            layerContainer = NULL;
            active = true;
        }

        inline void Drop(Glesly::ObjectLayerStack & layers)
        {
            RestartTimer();
            layerContainer = &layers;
            active = true;
        }

     protected:
        inline LayerChangeEffectBase(ObjectListPtr & objects, float time = 1.0):
            myObjects(objects),
            active(false),
            myTime(time),
            layerContainer(NULL)
        {
        }

        ObjectListPtr myObjects;

        ObjectListPtr previousObjects;

        SYS::TimeDelay myStart;

        bool active;

        float myTime;

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::LayerChangeEffectBase");

        ObjectLayerStack * layerContainer;

        void Frame(void);

        virtual void SetState(Glesly::LayerChangeEffectBase::EffectParameters & params, float state) { }

    }; // class LayerChangeEffectBase

    /// The most simple effect: Jump to the new layer
    class JumpEffect: public LayerChangeEffectBase
    {
     protected:
        JumpEffect(ObjectListPtr & objects):
            LayerChangeEffectBase(objects)
        {
        }

     public:
        static inline LayerEffectPtr Create(ObjectListPtr objects = ObjectListPtr())
        {
            return LayerEffectPtr(new JumpEffect(objects));
        }

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::JumpEffect");

        virtual bool IsActive(void) const
        {
            return false;
        }

        virtual void SetState(Glesly::LayerChangeEffectBase::EffectParameters & params, float)
        {
            active = false;
            params.Reset();
        }

    }; // class JumpEffect

    class LayerChangeEffectManager
    {
        friend class Glesly::LayerChangeEffectBase;

        class EffectUniforms: public Glesly::Shaders::UniformManagerCopy
        {
            friend class LayerChangeEffectManager;

            EffectUniforms(const Glesly::Shaders::UniformManager & manager, Glesly::LayerChangeEffectBase::EffectParameters::Params & params);

            Glesly::Shaders::UniformFloat_ref myFade_var;

            Glesly::Shaders::UniformMatrix_ref<float, 4> myProjectionMatrix_var;

            Glesly::Shaders::UniformMatrix_ref<float, 4> myObjectMatrix_var;

            SYS_DEFINE_CLASS_NAME("Glesly::LayerChangeEffectManager::EffectUniforms");

        }; // class EffectUniforms

        EffectUniforms myEffectFadeIn;

        EffectUniforms myEffectFadeOut;

        Glesly::LayerChangeEffectBase::EffectParameters myEffectParams;

     public:
        inline LayerChangeEffectManager(const Glesly::Shaders::UniformManager & manager):
            myEffectFadeIn(manager, myEffectParams.fade_in),
            myEffectFadeOut(manager, myEffectParams.fade_out)
        {
        }

        VIRTUAL_IF_DEBUG inline ~LayerChangeEffectManager()
        {
        }

        inline void FadeIn(void)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            myEffectFadeIn.ActivateVariables();
        }

        inline void FadeOut(void)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            myEffectFadeOut.ActivateVariables();
        }

        void EffectFrame(LayerEffectPtr & effect);

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::LayerChangeEffectManager");

    }; // class LayerChangeEffectManager

    class LayerEffectCreatorBase
    {
     public:
        virtual ~LayerEffectCreatorBase()
        {
        }

        virtual LayerEffectPtr GetEffect(Glesly::MenuRender &) =0;

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::LayerEffectCreatorBase");

    }; // class LayerEffectCreatorBase

} // namespace Glesly

#endif /* __GLESLY_SRC_EFFECTS_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
