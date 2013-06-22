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
#include <glesly/math/matrix.h>

namespace Glesly
{
    class LayerChangeEffectBase
    {
     public:
        void Frame(void);

     protected:
        ObjectListPtr myObjects;

        SYS::TimeDelay myStart;

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::LayerChangeEffectBase");

    }; // class LayerChangeEffectBase

    typedef boost::shared_ptr<LayerChangeEffectBase> LayerEffecrPtr;

    class LayerChangeEffectManager
    {
        class EffectUniforms: public Glesly::Shaders::UniformManagerCopy
        {
            friend class LayerChangeEffectManager;

            EffectUniforms(const Glesly::Shaders::UniformManager & manager):
                Glesly::Shaders::UniformManagerCopy(manager),
                myEffectFade(1.0),
                myEffectMatrix(1.0),
                myFade_var(*this, "effect_fade", myEffectFade),
                myEffectMatrix_var(*this, "effect_matrix", myEffectMatrix)
            {
            }

            GLfloat myEffectFade;

            Glesly::Matrix<float, 4, 4> myEffectMatrix;

            Glesly::Shaders::UniformFloat_ref myFade_var;

            Glesly::Shaders::UniformMatrix_ref<float, 4> myEffectMatrix_var;

            SYS_DEFINE_CLASS_NAME("Glesly::LayerChangeEffectManager::EffectUniforms");

        }; // class EffectUniforms

        EffectUniforms myEffectFadeIn;

        EffectUniforms myEffectFadeOut;

     public:
        inline LayerChangeEffectManager(const Glesly::Shaders::UniformManager & manager):
            myEffectFadeIn(manager),
            myEffectFadeOut(manager)
        {
        }

        VIRTUAL_IF_DEBUG inline ~LayerChangeEffectManager()
        {
        }

        inline void Effect(LayerEffecrPtr effect = LayerEffecrPtr())
        {
            myEffect = effect;
        }

        inline void FadeIn(void)
        {
            myEffectFadeIn.ActivateVariables();
        }

        inline void FadeOut(void)
        {
            myEffectFadeOut.ActivateVariables();
        }

        void EffectFrame(void);

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::LayerChangeEffectManager");

        LayerEffecrPtr myEffect;

    }; // class LayerChangeEffectManager

} // namespace Glesly

#endif /* __GLESLY_SRC_EFFECTS_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
