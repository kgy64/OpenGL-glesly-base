/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     
 * Purpose:     
 * Author:      
 * License:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "target2d.h"

#include <string.h>

using namespace Glesly;

/// Default implementation of target copy
/*! This implementation just copies the raw memory between targets directly. */
Target2D & Target2D::operator=(const Target2D & other)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
 ASSERT(GetWidth() == other.GetWidth() && GetHeight() == other.GetHeight(), "trying to copy different bitmap size");
 ASSERT(GetPixelFormat() == other.GetPixelFormat(), "trying to copy different bitmap format");
 int size = 0;
 switch (GetPixelFormat()) {
    case Glesly::FORMAT_RGB_565:
        size = 2;
    break;
    case Glesly::FORMAT_RGBA_8888:
        size = 4;
    break;
    default:
        DEBUG_OUT("Having unknown (unused) pixel format " << (int)GetPixelFormat() << " - not copied");
        return *this;
    break;
 }
 memcpy(const_cast<void*>(GetPixelData()), other.GetPixelData(), size * GetWidth() * GetHeight());
 return *this;
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
