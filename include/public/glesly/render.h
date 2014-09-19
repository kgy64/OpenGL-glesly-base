/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Class to execute one OpenGL program with Objects
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_RENDER_H_INCLUDED__
#define __GLESLY_SRC_RENDER_H_INCLUDED__

#include <list>

#include <glesly/camera.h>
#include <glesly/program.h>
#include <glesly/object-list+effect.h>
#include <glesly/render-ptr.h>
#include <glesly/object-ptr.h>
#include <glesly/shader-uniforms.h>
#include <International/utf8.h>
#include <Threads/Mutex.h>

namespace Glesly
{
    /// An OpenGL program with Objects
    class Render: public Glesly::Program, public Glesly::ObjectsWithEffect
    {
     public:
        virtual ~Render();

        virtual void NextFrame(const SYS::TimeDelay & frame_start_time);
        virtual void Timer(void);
        virtual void MouseClickRaw(int x, int y, int index, int count);
        virtual void Initialize(void) { }
        virtual void Cleanup(void);
        virtual void KeyboardClick(UTF8::WChar key);

        virtual Glesly::Transformation * GetTransform(unsigned index)
        {
            return NULL;
        }

        virtual Render & GetRenderer(void) override
        {
            return *this;
        }

        inline void SetScreenAspect(float aspect)
        {
            myScreenAspect = aspect;
        }

        inline float GetScreenAspect(void) const
        {
            return myScreenAspect;
        }

        int GetCallbackTimeLimit(void) const;
        void InitGLObject(Glesly::ObjectWeak & object);

     protected:
        Render(Glesly::CameraMatrix & camera, float aspect = 1.0f);

        virtual bool IsInputBlocked(void) const
        {
            return false;
        }

        virtual void ConvertMouseCoordinates(float & x, float & y) { }
        virtual void Frame(const SYS::TimeDelay & frame_start_time) { }
        virtual void BeforeFrame(void) { }
        virtual void AfterFrame(void) { }

        float myScreenAspect;

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::Render");

        struct objectIniter
        {
            Glesly::Render::objectIniter * next;

            Glesly::ObjectWeak object;

        }; // struct Glesly::Render::objectIniter

        Glesly::ObjectPtr GetObject2Init(void);

        Shaders::UniformMatrix_ref<float, 4> myCameraMatrix;

        Threads::Mutex myObjInitMutex;

        objectIniter * objInitList;

        objectIniter * freeObjIniters;

        static constexpr int initerSize = 2000;

        objectIniter initers[initerSize];

    }; // class Glesly::Render

    class Render3D: public Render
    {
     public:
        struct RenderInfo
        {
            Glesly::CameraMatrix myCamera;

            Glesly::Transformation myTransform[4];

        }; // struct RenderInfo

        virtual Glesly::Transformation * GetTransform(unsigned index)
        {
            if (index >= 4) {
                return NULL;
            }
            return myRenderInfo.myTransform + index;
        }

     protected:
        Render3D(RenderInfo & renderInfo);
        virtual ~Render3D();

        RenderInfo & myRenderInfo;

        Shaders::UniformMatrix_ref<float, 4> myT1Matrix;
        Shaders::UniformMatrix_ref<float, 4> myT2Matrix;
        Shaders::UniformMatrix_ref<float, 4> myT3Matrix;
        Shaders::UniformMatrix_ref<float, 4> myT4Matrix;

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::Render3D");

    }; // class Render3D

}; // namespace Glesly

#define USE_VERTEX_SHADER(name) AddShader(Shader::Create(GL_VERTEX_SHADER, Glesly::Shaders::name))
#define USE_FRAGMENT_SHADER(name) AddShader(Shader::Create(GL_FRAGMENT_SHADER, Glesly::Shaders::name))

#endif /* __GLESLY_SRC_RENDER_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
