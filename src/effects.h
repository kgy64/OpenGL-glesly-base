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

#include <System/TimeDelay.h>
#include <glesly/shader-uniforms.h>
#include <glesly/object-ptr.h>
#include <glesly/effect-ptr.h>
#include <glesly/camera.h>

namespace Glesly
{
    class LayerChangeEffectManager;

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

        inline void RestartTimer(void)
        {
            SYS::TimeDelay now;
            myStart = now;
            Start();
        }

        struct EffectParameters
        {
            struct Params {
                inline Params(float initial_fade):
                    fade(initial_fade)
                {
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
        };

        virtual void Step(Glesly::LayerChangeEffectBase::EffectParameters &) { }

        virtual void Start(void) { }

        virtual void Drop(Glesly::ObjectLayerStack &) { }

        virtual bool IsActive(void) const =0;

     protected:
        inline LayerChangeEffectBase(ObjectListPtr & objects):
            myObjects(objects)
        {
        }

        ObjectListPtr myObjects;

        ObjectListPtr previousObjects;

        SYS::TimeDelay myStart;

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::LayerChangeEffectBase");

        void Frame(void);

    }; // class LayerChangeEffectBase

    class JumpEffect: public LayerChangeEffectBase
    {
     protected:
        JumpEffect(ObjectListPtr & objects):
            LayerChangeEffectBase(objects)
        {
        }

     public:
        static inline LayerEffecrPtr Create(ObjectListPtr objects = ObjectListPtr())
        {
            return LayerEffecrPtr(new JumpEffect(objects));
        }

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::JumpEffect");

        virtual bool IsActive(void) const
        {
            return false;
        }

    }; // class JumpEffect

    class FadeInEffect: public LayerChangeEffectBase
    {
        inline FadeInEffect(ObjectListPtr & objects, float time):
            LayerChangeEffectBase(objects),
            myTime(time),
            active(true),
            layerContainer(NULL)
        {
        }

     public:
        static inline LayerEffecrPtr Create(ObjectListPtr & objects, float time)
        {
            return LayerEffecrPtr(new FadeInEffect(objects, time));
        }

     protected:
        float myTime;

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::LayerChangeEffectBase");

        bool active;

        ObjectLayerStack * layerContainer;

        virtual void Step(Glesly::LayerChangeEffectBase::EffectParameters & params);

        virtual bool IsActive(void) const
        {
            return active;
        }

        virtual void Start(void)
        {
            layerContainer = NULL;
            active = true;
        }

        virtual void Drop(Glesly::ObjectLayerStack & layers)
        {
            layerContainer = &layers;
            active = true;
        }

        void SetState(Glesly::LayerChangeEffectBase::EffectParameters & params, float state);

    }; // class FadeInEffect

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

        void EffectFrame(LayerEffecrPtr & effect);

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::LayerChangeEffectManager");

    }; // class LayerChangeEffectManager

} // namespace Glesly

#endif /* __GLESLY_SRC_EFFECTS_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
