/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Generic rectangle-shaped object using pangocairo
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_PACA_OBJECT_H_INCLUDED__
#define __GLESLY_SRC_PACA_OBJECT_H_INCLUDED__

#include <glesly/paca-ref.h>
#include <pacalib/pacalib.h>

namespace Glesly
{
    class _PacaObject_Base
    {
     protected:
        inline _PacaObject_Base(int width, int height):
            myPaca(width, height)
        {
        }

        PaCaLib::PacaTarget myPaca;

    }; // class _PacaObject_Base

    class PaCaObject: public _PacaObject_Base, public Glesly::PaCaRef
    {
     protected:
        inline PaCaObject(Glesly::Render & render, int width, int height):
            _PacaObject_Base(width, height),
            PaCaRef(render, myPaca)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

        inline PaCaLib::PacaTarget & PaCa(void)
        {
            return myPaca;
        };

     public:
        virtual ~PaCaObject()
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::PaCaObject");

    }; // class PaCaObject

} // namespace Glesly

#endif /* __GLESLY_SRC_PACA_OBJECT_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
