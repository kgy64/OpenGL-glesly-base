/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Texture handling
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "texture-2d.h"

#include <glesly/target2d.h>

using namespace Glesly;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *       class Texture2DRaw:                                                             *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Texture2DRaw::Texture2DRaw(const Target2D & target, bool use_mipmap):
    myWidth(target.GetWidth()),
    myHeight(target.GetHeight()),
    myFormat(Glesly::Format2DataFormat(target.GetPixelFormat())),
    myPixelFormat(Glesly::Format2PixelFormat(target.GetPixelFormat())),
    myUseMipmap(use_mipmap),
    myTarget(target),
    myTexture(0xffffffff)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 if (myPixelFormat == 0) {
    switch (myFormat) {
        case GL_RGB:
            myPixelFormat = GL_UNSIGNED_SHORT_5_6_5;
        break;
        case GL_RGBA:
            myPixelFormat = GL_UNSIGNED_BYTE;
        break;
    }
 }

 ASSERT(myPixelFormat, "pixel format is not defined for format " << myFormat);
}

Texture2DRaw::~Texture2DRaw()
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 if (myTexture != 0xfffffffff) {
    SYS_DEBUG(DL_INFO3, " - glDeleteTextures(1, " << myTexture << ");");
    glDeleteTextures(1, &myTexture);
    CheckEGLError("glDeleteTextures()");
 }
}

void Texture2DRaw::Update(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 SYS_DEBUG(DL_INFO3, " - glTexImage2D(GL_TEXTURE_2D, 0, " << myFormat << ", " << myWidth << ", " << myHeight << ", 0, " << myFormat << ", " << myPixelFormat << ", " << myTarget.GetPixelData() << ")");
 glTexImage2D(
    GL_TEXTURE_2D, 0,           // target, level
    myFormat,                   // internal format
    myWidth, myHeight, 0,       // width, height, border
    myFormat, myPixelFormat,    // external format, type
    myTarget.GetPixelData()     // pixels
 );
 CheckEGLError("glTexImage2D()");

 if (myUseMipmap) {
    SYS_DEBUG(DL_INFO3, " - glGenerateMipmap(GL_TEXTURE_2D)");
    glGenerateMipmap(GL_TEXTURE_2D);
    CheckEGLError("glGenerateMipmap()");
 }
}

void Texture2DRaw::InitGL(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 glGenTextures(1, &myTexture);
 SYS_DEBUG(DL_INFO3, " - glGenTextures(1, " << myTexture << "); returned");

 Bind();

 Initialize();

 Update();
}

void Texture2DRaw::Initialize(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 if (myUseMipmap) {
    SYS_DEBUG(DL_INFO3, " - glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
 } else {
    SYS_DEBUG(DL_INFO3, " - glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 }
 SYS_DEBUG(DL_INFO3, " - glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);");
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 SYS_DEBUG(DL_INFO3, " - glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT);");
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT);
 SYS_DEBUG(DL_INFO3, " - glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);");
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
