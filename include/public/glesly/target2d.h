/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Interface for bitmap targets
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_TARGET_BASE_H_INCLUDED__
#define __GLESLY_SRC_TARGET_BASE_H_INCLUDED__

#include <Debug/Debug.h>

namespace Glesly
{
    class Target2D
    {
     protected:
        inline Target2D(void)
        {
        }

     public:
        virtual ~Target2D()
        {
        }

        virtual int GetWidth(void) const =0;
        virtual int GetHeight(void) const =0;
        virtual const void * GetPixelData(void) const =0;

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::Target2D");

    }; // class Target2D

} // namespace Glesly

#endif /* __GLESLY_SRC_TARGET_BASE_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
