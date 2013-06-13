/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Texture handling
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "texture.h"

#include <glesly/target2d.h>

using namespace Glesly;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *       class Texture2DRaw:                                                             *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Texture2DRaw::Texture2DRaw(void * pixels, int width, int height, GLenum format):
    myWidth(width),
    myHeight(height),
    myFormat(format),
    myPixels(pixels)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 Initialize();

 SYS_DEBUG(DL_INFO3, " - glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB," << myWidth << "," << myHeight << ", 0, " << format << ", GL_UNSIGNED_BYTE, " << myPixels << ");");

 glTexImage2D(
    GL_TEXTURE_2D, 0,           /* target, level */
    format,                     /* internal format */
    myWidth, myHeight, 0,       /* width, height, border */
    format, GL_UNSIGNED_BYTE,   /* external format, type */
    myPixels                    /* pixels */
 );
}

Texture2DRaw::Texture2DRaw(const Target2D & target, GLenum format, bool update_now):
    myWidth(target.GetWidth()),
    myHeight(target.GetHeight()),
    myFormat(format),
    myPixels(target.GetPixelData())
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 Initialize();

 SYS_DEBUG(DL_INFO3, " - glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, " << myWidth << "," << myHeight << ", 0, " << "GL_RGB, GL_UNSIGNED_BYTE, &target);");

 if (update_now) {
    Update();
 };
}

Texture2DRaw::~Texture2DRaw()
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 SYS_DEBUG(DL_INFO3, " - glDeleteTextures(1, " << myTexture << ");");

 glDeleteTextures(1, &myTexture);
}

void Texture2DRaw::Update(void)
{
 glTexImage2D(
    GL_TEXTURE_2D, 0,           /* target, level */
    myFormat,                   /* internal format */
    myWidth, myHeight, 0,       /* width, height, border */
    myFormat, GL_UNSIGNED_BYTE, /* external format, type */
    myPixels                    /* pixels */
 );
}

void Texture2DRaw::Initialize(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 glGenTextures(1, &myTexture);
 SYS_DEBUG(DL_INFO3, " - glGenTextures(1, " << myTexture << "); returned");
 Bind();
 SYS_DEBUG(DL_INFO3, " - glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);");
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 SYS_DEBUG(DL_INFO3, " - glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);");
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 SYS_DEBUG(DL_INFO3, " - glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);");
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
 SYS_DEBUG(DL_INFO3, " - glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);");
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
