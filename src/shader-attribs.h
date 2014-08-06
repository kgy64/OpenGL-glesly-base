/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Shader attribute variable handler classes
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_SHADER_ATTRIBS_H_INCLUDED__
#define __GLESLY_SRC_SHADER_ATTRIBS_H_INCLUDED__

#include <glesly/shader-vars.h>

namespace Glesly
{
    class Object;

    namespace Shaders
    {
        class VBOAttribBase: public AttribList
        {
         protected:
            VBOAttribBase(Glesly::Object & parent, const char * name, const void * data, unsigned vector_size, unsigned element_size, unsigned vertices, int gl_type, GLenum usage = GL_STATIC_DRAW, GLenum target = GL_ARRAY_BUFFER);
            virtual ~VBOAttribBase();

            virtual void BufferData(void)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
                ASSERT_DBG(myVBO != 0xffffffff, "object '" << myName << "' is not initialized yet");
                SYS_DEBUG(DL_INFO3, " - glBindBuffer(" << std::hex << myTarget << ", " << std::dec << myVBO << "); name: '" << myName << "'");
                glBindBuffer(myTarget, myVBO);
                if (myUsage != GL_STATIC_DRAW) { // else will be called in Bind()
                    SYS_DEBUG(DL_INFO3, " - glBufferData(" << std::hex << myTarget << ", " << std::dec << myByteSize << ", " << std::hex << myData << ", " << myUsage << "); name: '" << myName << "'");
                    ASSERT(myData, "object '" << myName << "' has no associated data");
                    glBufferData(myTarget, myByteSize, myData, myUsage);
                }
                if (myTarget == GL_ARRAY_BUFFER) {
                    ASSERT_DBG(myAttrib != -1, "object is not initialized yet");
                    SYS_DEBUG(DL_INFO3, " - glEnableVertexAttribArray(" << myAttrib << "); name: '" << myName << "'");
                    glEnableVertexAttribArray(myAttrib);
                    SYS_DEBUG(DL_INFO3, " - glVertexAttribPointer(" << myAttrib << ", " << myVectorSize << ", " << myGLType << ", GL_FALSE, 0, 0);");
                    glVertexAttribPointer(myAttrib, myVectorSize, myGLType, GL_FALSE, 0, 0);
                }
            }

            virtual void UnbufferData(void)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
                if (myTarget == GL_ARRAY_BUFFER) {
                    ASSERT_DBG(myAttrib != -1, "object is not initialized yet");
                    SYS_DEBUG(DL_INFO3, " - glDisableVertexAttribArray(" << myAttrib << "); name: '" << myName << "'");
                    glDisableVertexAttribArray(myAttrib);
                }
            }

            Glesly::Object & myParent;

            const char * myName;

            GLuint myVBO;

            GLint myAttrib;

            const void * myData;

            unsigned myVectorSize;

            unsigned myElementSize;

            unsigned myVertices;

            unsigned myByteSize;

            int myGLType;

            GLenum myTarget;

            GLenum myUsage;

            VBOAttribBase * next;

         public:
            void InitGL(void);

            void Bind(const void * data, unsigned elements = 0U)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
                SYS_DEBUG(DL_INFO2, "Shader var '" << myName << ": Overriding data pointer to " << data);
                myData = data;
                if (elements) {
                    myVertices = elements;
                    myByteSize = myVectorSize * myVertices * myElementSize;
                    SYS_DEBUG(DL_INFO2, "Shader var '" << myName << ": Overriding size (vector size=" << myVectorSize << ", element size=" << myElementSize << ", vertices=" << myVertices << ", bytes=" << myByteSize << ")");
                }
            }

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Shaders::VBOAttribBase");

            inline void Bind(void) const
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
                ASSERT_DBG(myVBO != 0xffffffff, "object '" << myName << "' is not initialized yet");
                SYS_DEBUG(DL_INFO3, " - glBindBuffer(" << std::hex << myTarget << ", " << std::dec << myVBO << "); name: '" << myName << "'");
                glBindBuffer(myTarget, myVBO);
                if (myUsage == GL_STATIC_DRAW) { // else will be called in BufferData()
                    SYS_DEBUG(DL_INFO3, " - glBufferData(" << std::hex << myTarget << ", " << std::dec << myByteSize << ", " << std::hex << myData << ", " << myUsage << "); name: '" << myName << "'");
                    ASSERT(myData, "object '" << myName << "' has no associated data");
                    glBufferData(myTarget, myByteSize, myData, myUsage);
                }
            }

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
            inline VBOAttrib(Glesly::Object & parent, const char * name, GLenum usage, GLenum target):
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
            SYS_DEFINE_CLASS_NAME("Glesly::Shaders::VBOAttrib<>");

        }; // class VBOAttrib

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

        template <typename T_HOST, unsigned T_GL, unsigned N, unsigned R, unsigned S>
        class VBOAttribMatrix: public VBOAttrib<T_HOST, T_GL, R*S, N>
        {
         protected:
            inline VBOAttribMatrix(Glesly::Object & parent, const char * name, GLenum usage, GLenum target):
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
            SYS_DEFINE_CLASS_NAME("Glesly::Shaders::VBOAttribMatrix<>");

        }; // class VBOAttribMatrix

        template <typename T_HOST, int T_GL, unsigned N, unsigned S>
        class VBOAttribVector: public VBOAttrib<T_HOST, T_GL, S, N>
        {
         protected:
            inline VBOAttribVector(Glesly::Object & parent, const char * name, GLenum usage, GLenum target):
                VBOAttrib<T_HOST, T_GL, S, N>(parent, name, usage, target)
            {
            }

         public:
            inline T_HOST * operator[](int index)
            {
                return this->GetData(index);
            }

            inline const T_HOST * operator[](int index) const
            {
                return this->GetData(index);
            }

            inline T_HOST * operator=(const T_HOST * source)
            {
                memcpy(this->GetData(), source, this->GetSize());
                return this->GetData();
            }

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Shaders::VBOAttribVector<>");

        }; // class VBOAttribVector

        template <typename T_HOST, int T_GL, unsigned N>
        class VBOAttribVariable: public VBOAttrib<T_HOST, T_GL, 1, N>
        {
         protected:
            inline VBOAttribVariable(Glesly::Object & parent, const char * name, GLenum usage, GLenum target):
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
            SYS_DEFINE_CLASS_NAME("Glesly::Shaders::VBOAttribVariable<>");

        }; // class VBOAttribVariable

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

        template <unsigned N, unsigned R, unsigned S>
        class VBOAttribFloatMatrix: public VBOAttribMatrix<float, GL_FLOAT, N, R, S>
        {
         public:
            inline VBOAttribFloatMatrix(Glesly::Object & parent, const char * name, GLenum usage = GL_STATIC_DRAW, GLenum target = GL_ARRAY_BUFFER):
                VBOAttribMatrix<GLfloat, GL_FLOAT, N, R, S>(parent, name, usage, target)
            {
            }

            inline float * operator=(const float * src)
            {
                return VBOAttribMatrix<GLfloat, GL_FLOAT, N, R, S>::operator=(src);
            }

        }; // class VBOAttribFloatMatrix

        template <unsigned N, unsigned S>
        class VBOAttribFloatVector: public VBOAttribVector<float, GL_FLOAT, N, S>
        {
         public:
            inline VBOAttribFloatVector(Glesly::Object & parent, const char * name, GLenum usage = GL_STATIC_DRAW, GLenum target = GL_ARRAY_BUFFER):
                VBOAttribVector<float, GL_FLOAT, N, S>(parent, name, usage, target)
            {
            }

            inline float * operator=(const float * src)
            {
                return VBOAttribVector<GLfloat, GL_FLOAT, N, S>::operator=(src);
            }

        }; // class VBOAttribFloatVector

        template <unsigned N>
        class VBOAttribFloatVariable: public VBOAttribVariable<float, GL_FLOAT, N>
        {
         public:
            inline VBOAttribFloatVariable(Glesly::Object & parent, const char * name, GLenum usage = GL_STATIC_DRAW, GLenum target = GL_ARRAY_BUFFER):
                VBOAttribVariable<GLfloat, GL_FLOAT, N>(parent, name, usage, target)
            {
            }

            inline float operator=(float src)
            {
                return VBOAttribVariable<GLfloat, GL_FLOAT, N>::operator=(src);
            }

        }; // class VBOAttribFloatVariable

        template <unsigned N, unsigned R, unsigned S>
        class VBOAttribIntMatrix: public VBOAttribMatrix<int, GL_INT, N, R, S>
        {
         public:
            inline VBOAttribIntMatrix(Glesly::Object & parent, const char * name, GLenum usage = GL_STATIC_DRAW, GLenum target = GL_ARRAY_BUFFER):
                VBOAttribMatrix<GLint, GL_INT, N, R, S>(parent, name, usage, target)
            {
            }

            inline int * operator=(const int * src)
            {
                return VBOAttribMatrix<GLint, GL_INT, N, R, S>::operator=(src);
            }

        }; // class VBOAttribIntMatrix

        template <unsigned N, unsigned S>
        class VBOAttribIntVector: public VBOAttribVector<int, GL_INT, N, S>
        {
         public:
            inline VBOAttribIntVector(Glesly::Object & parent, const char * name, GLenum usage = GL_STATIC_DRAW, GLenum target = GL_ARRAY_BUFFER):
                VBOAttribVector<GLint, GL_INT, N, S>(parent, name, usage, target)
            {
            }

            inline int * operator=(const int * src)
            {
                return VBOAttribVector<GLint, GL_INT, N, S>::operator=(src);
            }

        }; // class VBOAttribIntVector

        template <unsigned N>
        class VBOAttribIntVariable: public VBOAttribVariable<int, GL_INT, N>
        {
         public:
            inline VBOAttribIntVariable(Glesly::Object & parent, const char * name, GLenum usage = GL_STATIC_DRAW, GLenum target = GL_ARRAY_BUFFER):
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
            inline VBOUShortElementBuffer(Glesly::Object & parent):
                VBOAttribBase(parent, "__ELEM_ARRAY_BUFFER__", NULL, 1, sizeof(GLushort), N, GL_INT /*not used here*/, GL_STATIC_DRAW, GL_ELEMENT_ARRAY_BUFFER)
            {
            }

        }; // class VBOAttribIntVariable

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

        inline void AttribManager::Register(AttribList & var) const
        {
            var.next = myAttribs;
            myAttribs = &var;
        }

        inline void AttribManager::BufferVariables(void)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            for (AttribList * i = myAttribs; i; i=i->next) {
                i->BufferData();
            }
        }

        inline void AttribManager::UnbufferVariables(void)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            for (AttribList * i = myAttribs; i; i=i->next) {
                i->UnbufferData();
            }
        }

    } // namespace Shaders

} // namespace Glesly

#endif /* __GLESLY_SRC_SHADER_ATTRIBS_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
