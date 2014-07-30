/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Texture handling: Cube Mapping
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "texture-cube.h"

#include <glesly/target2d.h>

using namespace Glesly;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *       class TextureCubeMap:                                                             *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

TextureCubeMap::TextureCubeMap(const Target2D * target[6], GLenum format):
    myFormat(format),
    myTarget(target)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
}

TextureCubeMap::~TextureCubeMap()
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 SYS_DEBUG(DL_INFO3, " - glDeleteTextures(1, " << myTexture << ");");

 glDeleteTextures(1, &myTexture);
 CheckEGLError("glDeleteTextures()");
}

void TextureCubeMap::Update(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 Bind();

 static const GLuint GLTargets[] = {
    GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
 };

 for (unsigned i = 0; i < 6; ++i) {
    SYS_DEBUG(DL_INFO3, " - glTexImage2D(...)");

    glTexImage2D(
        GLTargets[i], 0,                    //  target, level
        myFormat,                           //  internal format
        myTarget[i]->GetWidth(),            //  width
        myTarget[i]->GetHeight(), 0,        //  height, border
        myFormat, GL_UNSIGNED_SHORT_5_6_5,  //  external format, type
        myTarget[i]->GetPixelData()         //  pixels
    );
    CheckEGLError("glTexImage2D()");
 }

 glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
 CheckEGLError("glGenerateMipmap()");
}

void TextureCubeMap::InitGL(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 glGenTextures(1, &myTexture);
 SYS_DEBUG(DL_INFO3, " - glGenTextures(1, " << myTexture << "); returned");
 SYS_DEBUG(DL_INFO3, " - glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);");
 glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
 SYS_DEBUG(DL_INFO3, " - glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);");
 glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 SYS_DEBUG(DL_INFO3, " - glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);");
 glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
 SYS_DEBUG(DL_INFO3, " - glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);");
 glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
