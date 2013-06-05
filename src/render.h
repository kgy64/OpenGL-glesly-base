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

#include <matrix/matrix.h>
#include <glesly/program.h>
#include <glesly/object-ptr.h>
#include <glesly/render-ptr.h>

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

        inline Glesly::Matrix<float, 4, 4> & GetProjectionMatrix(void)
        {
            return myProjectionMatrix;
        }

        void NextFrame(void);

        virtual void Frame(void) { }
        virtual void Initialize(void) { }
        virtual void Cleanup(void) { }

     protected:
        Render(void);

        std::list<ObjectPtr> myObjects;

        Glesly::Matrix<float, 4, 4> myProjectionMatrix;

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::Render");

    }; // class Render

}; // namespace Glesly

#define USE_VERTEX_SHADER(name) Attach(Shader::Create(GL_VERTEX_SHADER, Glesly::Shaders::name))
#define USE_FRAGMENT_SHADER(name) Attach(Shader::Create(GL_FRAGMENT_SHADER, Glesly::Shaders::name))

#endif /* __GLESLY_SRC_RENDER_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
