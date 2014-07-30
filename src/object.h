/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Generic OpenGL object classes
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_OBJECT_H_INCLUDED__
#define __GLESLY_SRC_OBJECT_H_INCLUDED__

#include <glesly/render.h>
#include <glesly/texture-2d.h>
#include <glesly/shader-ptr.h>
#include <glesly/object-base.h>
#include <glesly/math/matrix.h>
#include <glesly/shader-uniforms.h>
#include <glesly/shader-attribs.h>

namespace Glesly
{
    class Object: public ObjectBase, public Glesly::Shaders::VarManager
    {
        class UniformList;
        class VBOAttribBase;

        friend class UniformList;
        friend class VBOAttribBase;

     public:
        virtual ~Object();

        virtual void NextFrame(const SYS::TimeDelay & frame_start_time);
        virtual bool MouseClick(float x, float y, int index, int count);

        virtual GLint GetUniformLocationSafe(const char * name) const override
        {
            return GetRenderer().GetUniformLocationSafe(name);
        }

        inline GLint GetUniformLocation(const char * name) const
        {
            return GetRenderer().GetUniformLocation(name);
        }

        inline GLint GetAttribLocation(const char * name) const
        {
            return GetRenderer().GetAttribLocation(name);
        }

        inline GLint GetAttribLocationSafe(const char * name) const
        {
            return GetRenderer().GetAttribLocationSafe(name);
        }

        inline Glesly::Transformation & GetProjection(void)
        {
            return myProjection;
        }

     protected:
        Object(Render & renderer);

        void DrawArrays(GLenum mode, GLint first, GLsizei count);
        void DrawElements(GLenum mode, GLsizei count);

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::Object");

        virtual void Frame(void) { }

        /// Generic OpenGL initializer function
        /*! Note that any object can be created by any thread, but OpenGL function calls can be
         *  initiated only from one thread (the OpenGL render thread here). This function is
         *  called from the OpenGL render thread, once in a life of such OpenGL objects, before
         *  any other drawing operations. All OpenGL-specific initializations must be done here,
         *  instead of the constructor. */
        virtual void initGL(void) { }

        /// The object's Projection Matrix
        Glesly::Transformation myProjection;

        Glesly::Shaders::UniformMatrix_ref<float, 4> p_matrix;

        bool gl_inited;

    }; // class Object

} // namespace Glesly

#endif /* __GLESLY_SRC_OBJECT_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
