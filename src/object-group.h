/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Group of Objects
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_OBJECT_GROUP_H_INCLUDED__
#define __GLESLY_SRC_OBJECT_GROUP_H_INCLUDED__

#include <glesly/object-list+effect.h>
#include <glesly/object-base.h>

namespace Glesly
{
    class ObjectGroup: public ObjectBase, public ObjectListBase
    {
     public:
        ObjectGroup(Render & renderer):
            ObjectBase(renderer)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

        virtual ~ObjectGroup()
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

     protected:
        virtual void NextFrame(void);
        virtual bool MouseClick(float x, float y, int index, int count);
        virtual void KeyboardClick(UTF8::WChar key);

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::ObjectGroup");

    }; // class ObjectGroup

} // namespace Glesly

#endif /* __GLESLY_SRC_OBJECT_GROUP_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
