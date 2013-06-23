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

#include <glesly/shader-uniforms.h>
#include <glesly/object-list.h>
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

        ObjectListPtr & GetObjects(void)
        {
            return myObjects;
        }

        void RestartTimer(void)
        {
            SYS::TimeDelay now;
            myStart = now;
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

     protected:
        inline LayerChangeEffectBase(LayerChangeEffectManager & parent, ObjectListPtr & objects):
            myParent(parent),
            myObjects(objects)
        {
        }

        EffectParameters & GetEffectParameters(void);

        LayerChangeEffectManager & myParent;

        ObjectListPtr myObjects;

        SYS::TimeDelay myStart;

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::LayerChangeEffectBase");

        void Frame(void);

        virtual bool Step(void) =0;

    }; // class LayerChangeEffectBase

    typedef boost::shared_ptr<LayerChangeEffectBase> LayerEffecrPtr;

    class FadeInEffect: public LayerChangeEffectBase
    {
        inline FadeInEffect(LayerChangeEffectManager & parent, ObjectListPtr & objects, float time):
            LayerChangeEffectBase(parent, objects),
            myTime(time)
        {
        }

     public:
        static inline LayerEffecrPtr Create(LayerChangeEffectManager & parent, ObjectListPtr & objects, float time)
        {
            return LayerEffecrPtr(new FadeInEffect(parent, objects, time));
        }

     protected:
        float myTime;

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::LayerChangeEffectBase");

        virtual bool Step(void);

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

        inline void Effect(LayerEffecrPtr effect = LayerEffecrPtr())
        {
            if (effect.get()) {
                effect->RestartTimer();
            }
            myEffect = effect;
        }

        inline bool IsEffectActive(void) const
        {
            return myEffect.get();
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

        void EffectFrame(void);

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::LayerChangeEffectManager");

        LayerEffecrPtr myEffect;

        virtual void EffectFinished(LayerEffecrPtr & effect) =0;

    }; // class LayerChangeEffectManager

    inline LayerChangeEffectBase::EffectParameters & LayerChangeEffectBase::GetEffectParameters(void)
    {
        return myParent.myEffectParams;
    }

} // namespace Glesly

#endif /* __GLESLY_SRC_EFFECTS_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
