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
        ObjectGroup(Glesly::ObjectListBase & base):
            ObjectBase(base),
            isInited(false)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

        virtual ~ObjectGroup()
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

        virtual Render & GetRenderer(void) override
        {
            return ObjectBase::GetRenderer();
        }

     protected:
        virtual void DrawFrame(const SYS::TimeDelay & frame_start_time) override;
        virtual bool MouseClick(float x, float y, int index, int count) override;
        virtual void KeyboardClick(UTF8::WChar key) override;
        virtual void initGL(void) override;

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::ObjectGroup");

        /// This function must not be called on such a class
        void ReinitGL(void);

        bool isInited;

    }; // class ObjectGroup

} // namespace Glesly

#endif /* __GLESLY_SRC_OBJECT_GROUP_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
