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

TextureCubeMap::TextureCubeMap(const Target2D * target[6], GLenum format, GLenum pixelformat, bool use_mipmap):
    myTexture(0xffffffff),
    myFormat(format),
    myPixelFormat(pixelformat),
    myUseMipmap(use_mipmap),
    myTarget(target)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
}

TextureCubeMap::~TextureCubeMap()
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 if (myTexture != 0xffffffff) {
    SYS_DEBUG(DL_INFO3, " - glDeleteTextures(1, " << myTexture << ");");
    glDeleteTextures(1, &myTexture);
    CheckEGLError("glDeleteTextures()");
 }
}

void TextureCubeMap::Update(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 static const GLuint GLTargets[] = {
    GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
 };

 for (unsigned i = 0; i < 6; ++i) {
    SYS_DEBUG(DL_INFO3, " - glTexImage2D(" << GLTargets[i] << ", 0," << myFormat << ", " << myTarget[i]->GetWidth() << myTarget[i]->GetHeight() << ", 0, " << myFormat << ", GL_UNSIGNED_SHORT_5_6_5, " << myTarget[i]->GetPixelData() << ") [image #" << i << "]");
    glTexImage2D(
        GLTargets[i], 0,                    //  target, level
        myFormat,                           //  internal format
        myTarget[i]->GetWidth(),            //  width
        myTarget[i]->GetHeight(), 0,        //  height, border
        myFormat, myPixelFormat,            //  external format, type
        myTarget[i]->GetPixelData()         //  pixels
    );
    CheckEGLError("glTexImage2D()");
 }

 if (myUseMipmap) {
    SYS_DEBUG(DL_INFO3, " - glGenerateMipmap(GL_TEXTURE_CUBE_MAP)");
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    CheckEGLError("glGenerateMipmap()");
 }
}

void TextureCubeMap::InitGL(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 glGenTextures(1, &myTexture);
 SYS_DEBUG(DL_INFO3, " - glGenTextures(1, " << myTexture << "); returned");

 Bind();

 if (myUseMipmap) {
    SYS_DEBUG(DL_INFO3, " - glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);");
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
 } else {
    SYS_DEBUG(DL_INFO3, " - glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);");
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 }
 SYS_DEBUG(DL_INFO3, " - glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);");
 glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 SYS_DEBUG(DL_INFO3, " - glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);");
 glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
 SYS_DEBUG(DL_INFO3, " - glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);");
 glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);

 Update();
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
