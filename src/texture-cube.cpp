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

TextureCubeMap::TextureCubeMap(bool use_mipmap):
    myTexture(0xffffffff),
    myUseMipmap(use_mipmap)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
}

TextureCubeMap::~TextureCubeMap()
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 if (myTexture != 0xffffffff) {
    // Note: exception is not allowed here:
    DEBUG_OUT("ERROR in TextureCubeMap::~TextureCubeMap(): texture " << myTexture << " has not been deleted");
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

 const Target2D * const * targets = getTargets();

 if (!targets[0]) {
    return; // not initialized yet
 }

 GLenum format = Glesly::Format2DataFormat(targets[0]->GetPixelFormat());
 GLenum pixelformat = Glesly::Format2PixelFormat(targets[0]->GetPixelFormat());

 for (unsigned i = 0; i < 6; ++i) {
    SYS_DEBUG(DL_INFO3, " - glTexImage2D(" << GLTargets[i] << ", 0," << format << ", " << targets[i]->GetWidth() << targets[i]->GetHeight() << ", 0, " << format << ", " << pixelformat << ", " << targets[i]->GetPixelData() << ") [image #" << i << "]");
    glTexImage2D(
        GLTargets[i],                       //  target
        0,                                  //  level
        format,                             //  internal format
        targets[i]->GetWidth(),             //  width
        targets[i]->GetHeight(),            //  height
        0, format, pixelformat,             //  border, external format, type
        targets[i]->GetPixelData()          //  pixels
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

void TextureCubeMap::UninitGL(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 if (myTexture == 0xffffffff) {
    return;
 }

 SYS_DEBUG(DL_INFO3, " - glDeleteTextures(1, " << myTexture << ");");
 glDeleteTextures(1, &myTexture);
 CheckEGLError("glDeleteTextures()");
 myTexture = 0xffffffff;
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
