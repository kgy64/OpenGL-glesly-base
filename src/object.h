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
#include <glesly/texture.h>
#include <glesly/shader-ptr.h>
#include <glesly/object-ptr.h>
#include <matrix/matrix.h>

SYS_DECLARE_MODULE(DM_GLESLY);

namespace Glesly
{
    class Object
    {
     public:
        virtual ~Object();

     protected:
        class UniformList;
        class VBOAttribBase;

        friend class UniformList;
        friend class VBOAttribBase;

        Object(Render & renderer);

        ObjectWeak mySelf;

        inline void Register(void)
        {
            ObjectPtr me = mySelf.lock();
            myIter = GetProgram().Add(me);
        }

        void DrawArrays(GLenum mode, GLint first, GLsizei count);

        inline GLint GetUniformLocation(const char * name) const
        {
            return GetProgram().GetUniformLocation(name);
        }

        inline GLint GetUniformLocationSafe(const char * name) const
        {
            return GetProgram().GetUniformLocationSafe(name);
        }

        inline GLint GetAttribLocation(const char * name) const
        {
            return GetProgram().GetAttribLocation(name);
        }

        inline GLint GetAttribLocationSafe(const char * name) const
        {
            return GetProgram().GetAttribLocationSafe(name);
        }

        inline Render & GetProgram(void)
        {
            return myProgram;
        }

        inline const Render & GetProgram(void) const
        {
            return myProgram;
        }

        class VBOAttribBase
        {
            friend class Object;

         protected:
            ///
            /*! 
                \param  parent          The parent object.
                \param  name            The variable name in the shader.
                \param  data            Variable representation in the host memory.
                \param  vector_size     Vector size of the shader variable (1 for single variable).
                \param  element_size    Size of one element in the vector (sizeof(type)).
                \param  vertices        Number of vertices.
                \param  gl_type         Type of the shader variable.
                \param  usage           Specifies the expected usage pattern of the data store. See 'glBufferData()' function specification.
                \param  target          Specifies the target buffer object. See 'glBufferData()' and 'glBindBuffer()' function specifications.
             */
            inline VBOAttribBase(const Object & parent, const char * name, const void * data, unsigned vector_size, unsigned element_size, unsigned vertices, int gl_type, GLenum usage = GL_STATIC_DRAW, GLenum target = GL_ARRAY_BUFFER):
                myName(name),
                myAttrib(target != GL_ELEMENT_ARRAY_BUFFER ? parent.GetAttribLocationSafe(myName) : 0),
                myData(data),
                myVectorSize(vector_size),
                myByteSize(myVectorSize * vertices * element_size),
                myGLType(gl_type),
                myTarget(target),
                myUsage(usage),
                next(NULL)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
                SYS_DEBUG(DL_INFO2, "Shader var '" << myName << "'");
                glGenBuffers(1, &myVBO);
                SYS_DEBUG(DL_INFO3, " - glGenBuffers(1, " << myVBO << "); returned");
                parent.Register(*this);
            }

            VIRTUAL_IF_DEBUG inline ~VBOAttribBase()
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
                SYS_DEBUG(DL_INFO2, "Shader var '" << myName << "'");
                glDeleteBuffers(1, &myVBO);
                SYS_DEBUG(DL_INFO2, "VBO=" << myVBO << " deleted.");
            }

            ///
            /*!
             *  \param  stride      Specifies the byte offset between consecutive generic vertex attributes.
             *                      If stride is 0, the generic vertex attributes are understood to be tightly
             *                      packed in the array.
             *  \param  offset      Specifies a offset of the first component of the first generic vertex
             *                      attribute in the array in the data store of the buffer currently bound
             *                      to the GL_ARRAY_BUFFER target.
             */
            void BufferData(GLsizei stride = 0, const GLvoid * offset = 0)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
                SYS_DEBUG(DL_INFO2, "Shader var '" << myName << "'");
                SYS_DEBUG(DL_INFO3, " - glBindBuffer(" << std::hex << myTarget << "," << std::dec << myVBO << ");");
                glBindBuffer(myTarget, myVBO);
                if (myUsage != GL_STATIC_DRAW) {
                    SYS_DEBUG(DL_INFO3, " - glBufferData(" << std::hex << myTarget << "," << std::dec << myByteSize << ",data," << std::hex << myUsage << ");");
                    glBufferData(myTarget, myByteSize, myData, myUsage);
                }
                if (myTarget == GL_ARRAY_BUFFER) {
                    SYS_DEBUG(DL_INFO3, " - glEnableVertexAttribArray(" << myAttrib << ");");
                    glEnableVertexAttribArray(myAttrib);
                    SYS_DEBUG(DL_INFO3, " - glVertexAttribPointer(" << myAttrib << "," << myVectorSize << "," << myGLType << ",FALSE," << (int)stride << "," << offset << ");");
                    glVertexAttribPointer(myAttrib, myVectorSize, myGLType, GL_FALSE, stride, offset);
                }
            }

            void UnbufferData(void)
            {
                if (myTarget == GL_ARRAY_BUFFER) {
                    SYS_DEBUG_MEMBER(DM_GLESLY);
                    SYS_DEBUG(DL_INFO2, "Shader var '" << myName << "'");
                    SYS_DEBUG(DL_INFO3, " - glDisableVertexAttribArray(" << myAttrib << ");");
                    glDisableVertexAttribArray(myAttrib);
                }
            }

            const char * myName;

            GLuint myVBO;

            GLint myAttrib;

            const void * myData;

            unsigned myVectorSize;

            unsigned myByteSize;

            int myGLType;

            GLenum myTarget;

            GLenum myUsage;

            VBOAttribBase * next;

         public:
            void Bind(const void * data = NULL)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
                if (data) {
                    SYS_DEBUG(DL_INFO2, "Overridong data pointer: " << data);
                    myData = data;
                }
                SYS_DEBUG(DL_INFO2, "Shader var '" << myName << "'");
                SYS_DEBUG(DL_INFO3, " - glBindBuffer(" << std::hex << myTarget << "," << std::dec << myVBO << ");");
                glBindBuffer(myTarget, myVBO);
                SYS_DEBUG(DL_INFO3, " - glBufferData(" << std::hex << myTarget << "," << std::dec << myByteSize << "," << myData << "," << std::hex << myUsage << ");");
                glBufferData(myTarget, myByteSize, myData, myUsage);
            }

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Object::VBOAttribBase");

        }; // class VBOAttribBase

        ///
        /*!
            \param  T_HOST  Type on the host machine
            \param  T_GL    Type in the shader (enum)
            \param  S       Size of one entry:
                            - 1 for single variables
                            - N for [N] sized vectors
                            - N*M for [N][M] sized matrices
            \param  V       Number of vertices
         */
        template <typename T_HOST, int T_GL, int S, int V>
        class VBOAttrib: public VBOAttribBase
        {
         protected:
            ///
            /*! 
                \param  parent      The parent object.
                \param  name        The variable name in the shader.
                \param  usage       Specifies the expected usage pattern of the data store. See 'glBufferData()' function specification.
                \param  target      Specifies the target buffer object. See 'glBufferData()' function specification.
             */
            inline VBOAttrib(const Object & parent, const char * name, GLenum usage, GLenum target):
                VBOAttribBase(parent, name, myData, S, sizeof(T_HOST), V, T_GL, usage, target)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

         public:
            VIRTUAL_IF_DEBUG inline ~VBOAttrib()
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            inline T_HOST & operator*()
            {
                return myData[0];
            }

            inline T_HOST operator*() const
            {
                return myData[0];
            }

            inline unsigned GetSize(void) const
            {
                return sizeof(myData);
            }

            inline T_HOST * GetData(int index = 0)
            {
                return myData + S * index;
            }

            inline const T_HOST * GetData(int index = 0) const
            {
                return myData + S * index;
            }

            T_HOST myData[S*V];

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Object::VBOAttrib<>");

        }; // class VBOAttrib

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

        template <typename T_HOST, unsigned T_GL, unsigned R, unsigned S, unsigned N>
        class VBOAttribMatrix: public VBOAttrib<T_HOST, T_GL, R*S, N>
        {
         protected:
            inline VBOAttribMatrix(const Object & parent, const char * name, GLenum usage, GLenum target):
                VBOAttrib<T_HOST, T_GL, R*S, N>(parent, name, usage, target)
            {
            }

         public:
            template<typename T, unsigned U>
            class _Indexer
            {
             public:
                _Indexer(T * data):
                    myData(data)
                {
                }

                inline T * operator[](int index)
                {
                    return myData + U * index;
                }

             private:
                T * myData;
            }; // class _Indexer

            inline _Indexer<T_HOST, R> operator[](int index)
            {
                return this->GetData(index);
            }

            inline T_HOST * operator=(const T_HOST * source)
            {
                memcpy(this->GetData(), source, this->GetSize());
                return this->GetData();
            }

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Object::VBOAttribMatrix<>");

        }; // class VBOAttribMatrix

        template <typename T_HOST, int T_GL, unsigned S, unsigned N>
        class VBOAttribVector: public VBOAttrib<T_HOST, T_GL, S, N>
        {
         protected:
            inline VBOAttribVector(const Object & parent, const char * name, GLenum usage, GLenum target):
                VBOAttrib<T_HOST, T_GL, S, N>(parent, name, usage, target)
            {
            }

         public:
            inline T_HOST * operator[](int index)
            {
                return this->GetData(index);
            }

            inline T_HOST * operator=(const T_HOST * source)
            {
                memcpy(this->GetData(), source, this->GetSize());
                return this->GetData();
            }

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Object::VBOAttribVector<>");

        }; // class VBOAttribVector

        template <typename T_HOST, int T_GL, unsigned N>
        class VBOAttribVariable: public VBOAttrib<T_HOST, T_GL, 1, N>
        {
         protected:
            inline VBOAttribVariable(const Object & parent, const char * name, GLenum usage, GLenum target):
                VBOAttrib<T_HOST, T_GL, 1, N>(parent, name, usage, target)
            {
            }

         public:
            inline T_HOST & operator[](int index)
            {
                return *this->GetData(index);
            }

            inline T_HOST operator=(T_HOST value)
            {
                return *this->GetData() = value;
            }

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Object::VBOAttribVariable<>");

        }; // class VBOAttribVariable

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

        template <unsigned R, unsigned S, unsigned N>
        class VBOAttribFloatMatrix: public VBOAttribMatrix<float, GL_FLOAT, R, S, N>
        {
         public:
            inline VBOAttribFloatMatrix(const Object & parent, const char * name, GLenum usage = GL_STATIC_DRAW, GLenum target = GL_ARRAY_BUFFER):
                VBOAttribMatrix<GLfloat, GL_FLOAT, R, S, N>(parent, name, usage, target)
            {
            }

            inline float * operator=(const float * src)
            {
                return VBOAttribMatrix<GLfloat, GL_FLOAT, R, S, N>::operator=(src);
            }

        }; // class VBOAttribFloatMatrix

        template <unsigned S, unsigned N>
        class VBOAttribFloatVector: public VBOAttribVector<float, GL_FLOAT, S, N>
        {
         public:
            inline VBOAttribFloatVector(const Object & parent, const char * name, GLenum usage = GL_STATIC_DRAW, GLenum target = GL_ARRAY_BUFFER):
                VBOAttribVector<GLfloat, GL_FLOAT, S, N>(parent, name, usage, target)
            {
            }

            inline float * operator=(const float * src)
            {
                return VBOAttribVector<GLfloat, GL_FLOAT, S, N>::operator=(src);
            }

        }; // class VBOAttribFloatVector

        template <unsigned N>
        class VBOAttribFloatVariable: public VBOAttribVariable<float, GL_FLOAT, N>
        {
         public:
            inline VBOAttribFloatVariable(const Object & parent, const char * name, GLenum usage = GL_STATIC_DRAW, GLenum target = GL_ARRAY_BUFFER):
                VBOAttribVariable<GLfloat, GL_FLOAT, N>(parent, name, usage, target)
            {
            }

            inline float operator=(float src)
            {
                return VBOAttribVariable<GLfloat, GL_FLOAT, N>::operator=(src);
            }

        }; // class VBOAttribFloatVariable

        template <unsigned R, unsigned S, unsigned N>
        class VBOAttribIntMatrix: public VBOAttribMatrix<int, GL_INT, R, S, N>
        {
         public:
            inline VBOAttribIntMatrix(const Object & parent, const char * name, GLenum usage = GL_STATIC_DRAW, GLenum target = GL_ARRAY_BUFFER):
                VBOAttribMatrix<GLint, GL_INT, R, S, N>(parent, name, usage, target)
            {
            }

            inline int * operator=(const int * src)
            {
                return VBOAttribMatrix<GLint, GL_INT, R, S, N>::operator=(src);
            }

        }; // class VBOAttribIntMatrix

        template <unsigned S, unsigned N>
        class VBOAttribIntVector: public VBOAttribVector<int, GL_INT, S, N>
        {
         public:
            inline VBOAttribIntVector(const Object & parent, const char * name, GLenum usage = GL_STATIC_DRAW, GLenum target = GL_ARRAY_BUFFER):
                VBOAttribVector<GLint, GL_INT, S, N>(parent, name, usage, target)
            {
            }

            inline int * operator=(const int * src)
            {
                return VBOAttribVector<GLint, GL_INT, S, N>::operator=(src);
            }

        }; // class VBOAttribIntVector

        template <unsigned N>
        class VBOAttribIntVariable: public VBOAttribVariable<int, GL_INT, N>
        {
         public:
            inline VBOAttribIntVariable(const Object & parent, const char * name, GLenum usage = GL_STATIC_DRAW, GLenum target = GL_ARRAY_BUFFER):
                VBOAttribVariable<GLint, GL_INT, N>(parent, name, usage, target)
            {
            }

            inline int operator=(int src)
            {
                return VBOAttribVariable<GLint, GL_INT, N>::operator=(src);
            }

        }; // class VBOAttribIntVariable

        template <unsigned N>
        class VBOUShortElementBuffer: public VBOAttribBase
        {
         public:
            inline VBOUShortElementBuffer(const Object & parent, const char * name = "<noname>"):
                VBOAttribBase(parent, name, NULL, 1, sizeof(GLushort), N, GL_INT /*not used here*/, GL_STATIC_DRAW, GL_ELEMENT_ARRAY_BUFFER)
            {
            }

        }; // class VBOAttribIntVariable

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

        class UniformList
        {
            friend class Object;

         protected:
            inline UniformList(const Object & obj)
            {
                obj.Register(*this);
            }

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Object::UniformList");

            virtual void Activate(void)=0;

            UniformList * next;

        }; // class UniformList

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

        /// Represents a generic uniform variable by name
        class UniformBase: protected UniformList
        {
         protected:
            inline UniformBase(const Object & obj, const char * name):
                UniformList(obj),
                myUniform(obj.GetProgram().GetUniformLocationSafe(name))
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            VIRTUAL_IF_DEBUG inline ~UniformBase()
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            GLint myUniform;

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Object::UniformBase");

        }; // class UniformBase

        class UniformTexture: public UniformBase, public Texture2DRaw
        {
         public:
            UniformTexture(const Object & obj, const char * name, void * pixels, int width, int height, int index = 0, GLenum format = GL_RGB):
                UniformBase(obj, name),
                Texture2DRaw(pixels, width, height, format),
                myIndex(index)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            UniformTexture(const Object & obj, const char * name, const Target2D & target, int index = 0, GLenum format = GL_RGB):
                UniformBase(obj, name),
                Texture2DRaw(target, format, false),
                myIndex(index)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            VIRTUAL_IF_DEBUG inline ~UniformTexture()
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            virtual void Activate(void)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
                SYS_DEBUG(DL_INFO3, " - glActiveTexture(GL_TEXTURE" << myIndex << ");");
                glActiveTexture(GL_TEXTURE0 + myIndex);
                SYS_DEBUG(DL_INFO3, " - glUniform1i(" << myUniform << "," << myIndex << ");");
                glUniform1i(myUniform, myIndex);
                Bind();
            }

         protected:
            int myIndex;

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Object::UniformTexture");

        }; // class UniformTexture

        template <typename T>
        class UniformMatrix2: public Matrix<T, 2, 2>, public UniformBase
        {
         public:
            inline UniformMatrix2(const Object & obj, const char * name):
                UniformBase(obj, name)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            VIRTUAL_IF_DEBUG inline ~UniformMatrix2()
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            virtual void Activate(void)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
                SYS_DEBUG(DL_INFO3, " - glUniformMatrix2fv(" << UniformBase::myUniform << ",1,GL_FALSE," << (Matrix<T,2,2>::myMatrix) << ");");
                glUniformMatrix2fv(UniformBase::myUniform, 1, GL_FALSE, Matrix<T,2,2>::myMatrix);
            }

            inline const T * operator=(const T * data)
            {
                return Matrix<T,2,2>::operator=(data);
            }

            inline const Matrix<T,2,2> & operator=(const Matrix<T,2,2> & data)
            {
                return Matrix<T,2,2>::operator=(data);
            }

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Object::UniformMatrix2<T>");

        }; // class UniformMatrix<T>

        template <typename T>
        class UniformMatrix3: public Matrix<T, 3, 3>, public UniformBase
        {
         public:
            inline UniformMatrix3(const Object & obj, const char * name):
                UniformBase(obj, name)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            VIRTUAL_IF_DEBUG inline ~UniformMatrix3()
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            virtual void Activate(void)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
                SYS_DEBUG(DL_INFO3, " - glUniformMatrix3fv(" << UniformBase::myUniform << ",1,GL_FALSE," << (Matrix<T,3,3>::myMatrix) << ");");
                glUniformMatrix3fv(UniformBase::myUniform, 1, GL_FALSE, Matrix<T,3,3>::myMatrix);
            }

            inline const T * operator=(const T * data)
            {
                return Matrix<T,3,3>::operator=(data);
            }

            inline const Matrix<T,3,3> & operator=(const Matrix<T,3,3> & data)
            {
                return Matrix<T,3,3>::operator=(data);
            }

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Object::UniformMatrix3<T>");

        }; // class UniformMatrix<T>

        template <typename T>
        class UniformMatrix4: public Matrix<T, 4, 4>, public UniformBase
        {
         public:
            inline UniformMatrix4(const Object & obj, const char * name):
                UniformBase(obj, name)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            VIRTUAL_IF_DEBUG inline ~UniformMatrix4()
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            virtual void Activate(void)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
                SYS_DEBUG(DL_INFO3, " - glUniformMatrix4fv(" << UniformBase::myUniform << ",1,GL_FALSE," << (Matrix<T,4,4>::myMatrix) << ");");
                glUniformMatrix4fv(UniformBase::myUniform, 1, GL_FALSE, Matrix<T,4,4>::myMatrix);
            }

            inline const T * operator=(const T * data)
            {
                return Matrix<T,4,4>::operator=(data);
            }

            inline const Matrix<T,4,4> & operator=(const Matrix<T,4,4> & data)
            {
                return Matrix<T,4,4>::operator=(data);
            }

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Object::UniformMatrix4<T>");

        }; // class UniformMatrix<T>

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

        class UseDepth
        {
         public:
            inline UseDepth(void)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
                glEnable(GL_DEPTH_TEST);
            }

            inline ~UseDepth()
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
                glDisable(GL_DEPTH_TEST);
            }

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Object::UseDepth");

        }; // class UseDepth

        class UseCullFace
        {
         public:
            inline UseCullFace(void)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
                glEnable(GL_CULL_FACE);
            }

            inline ~UseCullFace()
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
                glDisable(GL_CULL_FACE);
            }

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Object::UseCullFace");

        }; // class UseCullFace

        class UseBlend
        {
         public:
            inline UseBlend(GLenum source_factor = GL_SRC_ALPHA, GLenum dest_factor = GL_ONE_MINUS_SRC_ALPHA)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
                glEnable(GL_BLEND);
                glBlendFunc(source_factor, dest_factor);
            }

            inline ~UseBlend()
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
                glDisable(GL_BLEND);
            }

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Object::UseBlend");

        }; // class UseBlend

     public:
        void BufferVariables(void);
        void UnbufferVariables(void);

        virtual void Frame(void) { };

        inline void UseProgram(void)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            SYS_DEBUG(DL_INFO1, " - glUseProgram(" << GetProgram().GetProgramID() << ");");
            glUseProgram(GetProgram().GetProgramID());
        }

        inline void ActivateVariables(void)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            for (UniformList * var = myVars; var; var=var->next) {
                var->Activate();
            }
        }

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::Object");

        Render::Objects::iterator myIter;

        inline void Register(UniformList & var) const
        {
            var.next = myVars;
            myVars = &var;
        }

        inline void Register(VBOAttribBase & var) const
        {
            var.next = myAttribs;
            myAttribs = &var;
        }

        Render & myProgram;

        mutable UniformList * myVars;

        mutable VBOAttribBase * myAttribs;

    }; // class Object

} // namespace Glesly

#endif /* __GLESLY_SRC_OBJECT_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
