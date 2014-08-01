/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Shader attribute variable handler classes
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <glesly/object.h>

using namespace Glesly;
using namespace Shaders;

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
VBOAttribBase::VBOAttribBase(Object & parent, const char * name, const void * data, unsigned vector_size, unsigned element_size, unsigned vertices, int gl_type, GLenum usage, GLenum target):
    AttribList(parent),
    myParent(parent),
    myName(name),
    myVBO(0xffffffff),
    myAttrib(-1),
    myData(data),
    myVectorSize(vector_size),
    myElementSize(element_size),
    myVertices(vertices),
    myByteSize(myVectorSize * myVertices * myElementSize),
    myGLType(gl_type),
    myTarget(target),
    myUsage(usage),
    next(NULL)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 SYS_DEBUG(DL_INFO2, "Shader var '" << myName << "'");
}

VBOAttribBase::~VBOAttribBase()
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
 SYS_DEBUG(DL_INFO2, "Shader var '" << myName << "'");

 if (myVBO != 0xffffffff) {
    glDeleteBuffers(1, &myVBO);
    SYS_DEBUG(DL_INFO2, "glDeleteBuffers(1, " << myVBO << "): deleted.");
 }
}

void VBOAttribBase::InitGL(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 myAttrib = myTarget != GL_ELEMENT_ARRAY_BUFFER ? myParent.GetAttribLocationSafe(myName) : 0;

 glGenBuffers(1, &myVBO);
 SYS_DEBUG(DL_INFO3, " - glGenBuffers(1, " << myVBO << "); returned for name '" << myName << "'");
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
