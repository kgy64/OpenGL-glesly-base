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

namespace Glesly
{
    enum PixelFormat
    {
        FORMAT_UNKNOWN      = 0,
        FORMAT_RGB_565,
        FORMAT_RGB_888,
        FORMAT_RGBA_8888

    }; // enum Glesly::PixelFormat

    inline GLenum Format2DataFormat(PixelFormat format)
    {
        GLenum result = 0;
        switch (format) {
            case FORMAT_RGB_565:
                result = GL_RGB;
            break;
            case FORMAT_RGB_888:
                result = GL_RGB;
            break;
            case FORMAT_RGBA_8888:
                result = GL_RGBA;
            break;
            default:
            break;
        }
        ASSERT(result, "unknown pixel format " << (int)format);
        return result;
    }

    inline GLenum Format2PixelFormat(PixelFormat format)
    {
        GLenum result = 0;
        switch (format) {
            case FORMAT_RGB_565:
                result = GL_UNSIGNED_SHORT_5_6_5;
            break;
            case FORMAT_RGB_888:
                result = GL_UNSIGNED_BYTE;
            break;
            case FORMAT_RGBA_8888:
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
