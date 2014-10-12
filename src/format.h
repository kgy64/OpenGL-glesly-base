/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     
 * Purpose:     
 * Author:      
 * License:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __INCLUDE_PUBLIC_GLESLY_FORMAT_H_INCLUDED__
#define __INCLUDE_PUBLIC_GLESLY_FORMAT_H_INCLUDED__

#include <Exceptions/Exceptions.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <iostream>

#if defined(GL_BGR_EXT)
    #define MY_GL_BGR_EXT GL_BGR_EXT
#elif defined(BGR_EXT)
    #define MY_GL_BGR_EXT BGR_EXT
#endif

#if defined(GL_BGRA_EXT)
    #define MY_GL_BGRA_EXT GL_BGRA_EXT
#elif defined(BGRA_EXT)
    #define MY_GL_BGRA_EXT BGRA_EXT
#endif

namespace Glesly
{
    enum PixelFormat
    {
        FORMAT_UNKNOWN      = 0,
        FORMAT_RGB_565,
        FORMAT_RGB_888,
        FORMAT_BGR_888,
        FORMAT_RGBA_8888,
        FORMAT_BGRA_8888

    }; // enum Glesly::PixelFormat

} // namespace Glesly

static inline std::ostream & operator<<(std::ostream & os, Glesly::PixelFormat p)
{
 switch (p) {
    case Glesly::FORMAT_RGB_565:
        os << "FORMAT_RGB_565";
    break;
    case Glesly::FORMAT_RGB_888:
        os << "FORMAT_RGB_888";
    break;
    case Glesly::FORMAT_BGR_888:
        os << "FORMAT_BGR_888";
    break;
    case Glesly::FORMAT_RGBA_8888:
        os << "FORMAT_RGBA_8888";
    break;
    case Glesly::FORMAT_BGRA_8888:
        os << "FORMAT_BGRA_8888";
    break;
    default:
        os << "unknown (" << (int)p << ")";
    break;
 }
 return os;
}

namespace Glesly
{
    inline GLenum Format2DataFormat(PixelFormat format)
    {
        GLenum result = 0;
        switch (format) {
            case FORMAT_RGB_565:
                result = GL_RGB;
            break;
            case FORMAT_RGBA_8888:
                result = GL_RGBA;
            break;
            case FORMAT_RGB_888:
                result = GL_RGB;
            break;
#ifdef MY_GL_BGR_EXT
            case FORMAT_BGR_888:
                result = MY_GL_BGR_EXT;
            break;
#endif
#ifdef MY_GL_BGRA_EXT
            case FORMAT_BGRA_8888:
                result = MY_GL_BGRA_EXT;
            break;
#endif
            default:
            break;
        }
        ASSERT(result, "unknown pixel format " << format);
        return result;
    }

    inline GLenum Format2PixelFormat(PixelFormat format)
    {
        GLenum result = 0;
        switch (format) {
            case FORMAT_RGB_565:
                result = GL_UNSIGNED_SHORT_5_6_5;
            break;
            case FORMAT_RGBA_8888:
            case FORMAT_BGRA_8888:
            case FORMAT_RGB_888:
            case FORMAT_BGR_888:
                result = GL_UNSIGNED_BYTE;
            break;
            default:
            break;
        }
        ASSERT(result, "unknown pixel format " << (int)format);
        return result;
    }

} // namespace Glesly

#endif /* __INCLUDE_PUBLIC_GLESLY_FORMAT_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
