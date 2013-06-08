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
#include <glesly/object-ptr.h>
#include <glesly/render-ptr.h>
#include <glesly/shader-uniforms.h>

namespace Glesly
{
    /// An OpenGL program with Objects
    class Render: public Glesly::Program
    {
     public:
        virtual ~Render();

        typedef std::list<ObjectPtr> Objects;

        inline Objects::iterator Add(ObjectPtr object)
        {
            myObjects.push_front(object);
            return myObjects.begin();
        }

        inline void Remove(Objects::iterator & object)
        {
            myObjects.erase(object);
        }

        inline Glesly::CameraMatrix & GetCamera(void)
        {
            return myCamera;
        }

        inline Glesly::Transformation & GetTransform(unsigned index)
        {
            return myTransform[index];
        }

        void NextFrame(void);

        virtual void Frame(void) { }
        virtual void Initialize(void) { }
        virtual void Cleanup(void) { }

     protected:
        Render(void);

        std::list<ObjectPtr> myObjects;

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::Render");

        Glesly::CameraMatrix myCamera;
        Glesly::Transformation myTransform[4];

        Shaders::UniformMatrix_ref<float, 4> myCameraMatrix;
        Shaders::UniformMatrix_ref<float, 4> myT1Matrix;
        Shaders::UniformMatrix_ref<float, 4> myT2Matrix;
        Shaders::UniformMatrix_ref<float, 4> myT3Matrix;
        Shaders::UniformMatrix_ref<float, 4> myT4Matrix;

    }; // class Render

}; // namespace Glesly

#define USE_VERTEX_SHADER(name) Attach(Shader::Create(GL_VERTEX_SHADER, Glesly::Shaders::name))
#define USE_FRAGMENT_SHADER(name) Attach(Shader::Create(GL_FRAGMENT_SHADER, Glesly::Shaders::name))

#endif /* __GLESLY_SRC_RENDER_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
