/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Low-level base of all Objects
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_OBJECT_LIST_BASE_H_INCLUDED__
#define __GLESLY_SRC_OBJECT_LIST_BASE_H_INCLUDED__

#include <glesly/object-ptr.h>
#include <Threads/Mutex.h>
#include <International/utf8.h>
#include <glesly/effect-ptr.h>
#include <Debug/Debug.h>

SYS_DECLARE_MODULE(DM_GLESLY);

namespace Glesly
{
    class Render;

    class ObjectListBase
    {
     public:
        class ObjectListInternal
        {
            friend class ObjectListBase;

         public:
            ~ObjectListInternal();

            void Insert(ObjectPtr object);
            void Append(ObjectPtr object);
            void Cleanup(void);

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::ObjectListBase::ObjectListInternal");

            ObjectListInternal(ObjectListBase & parent);

            void CopyObjects(void);

            ObjectListBase & myParent;

            Threads::Lock myLock;

            ObjectListPtr myObjects;

            ObjectListPtr myModifiedObjects;

        }; // class ObjectListInternal

        inline ObjectListInternal GetObjectList(void)
        {
            return ObjectListInternal(*this);
        }

        Threads::Mutex & GetObjectMutex(void)
        {
            return myObjectMutex;
        }

        virtual Glesly::Render & GetRenderer(void) =0;

        inline const Glesly::Render & GetRenderer(void) const
        {
            return const_cast<ObjectListBase&>(*this).GetRenderer();
        }

        float GetScreenAspect(void) const;
        void KeyboardClick(UTF8::WChar ch);

     protected:
        inline ObjectListBase(void)
        {
        }

        virtual ObjectListPtr & GetObjectListPtr(void)
        {
            return myObjects;
        }

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::ObjectListBase");

        ObjectListPtr myObjects;

        Threads::Mutex myObjectMutex;

    }; // class ObjectListBase

    inline ObjectListBase::ObjectListInternal::ObjectListInternal(ObjectListBase & parent):
        myParent(parent),
        myLock(parent.GetObjectMutex()),
        myObjects(parent.GetObjectListPtr())
    {
        SYS_DEBUG_MEMBER(DM_GLESLY);
    }

    inline ObjectListBase::ObjectListInternal::~ObjectListInternal()
    {
        SYS_DEBUG_MEMBER(DM_GLESLY);
        SYS_DEBUG(DL_INFO1, "Passing " << (myModifiedObjects ? myModifiedObjects->size() : 0) << " objects");
        myParent.GetObjectListPtr() = myModifiedObjects;
    }

    inline void ObjectListBase::ObjectListInternal::CopyObjects(void)
    {
        SYS_DEBUG_MEMBER(DM_GLESLY);
        if (!myModifiedObjects) {
            if (myObjects.get()) {
                SYS_DEBUG(DL_INFO1, "Copying " << myObjects->size() << " objects");
                myModifiedObjects.reset(new Objects(*myObjects));
            } else {
                SYS_DEBUG(DL_INFO1, "Creating empty container");
                myModifiedObjects.reset(new Objects);
            }
        }
    }

    inline void ObjectListBase::ObjectListInternal::Insert(ObjectPtr object)
    {
        SYS_DEBUG_MEMBER(DM_GLESLY);
        CopyObjects();
        myModifiedObjects->push_front(object);
        SYS_DEBUG(DL_INFO1, "ObjectListBase::ObjectListInternal::Insert(): size=" << myModifiedObjects->size());
    }

    inline void ObjectListBase::ObjectListInternal::Append(ObjectPtr object)
    {
        SYS_DEBUG_MEMBER(DM_GLESLY);
        CopyObjects();
        myModifiedObjects->push_back(object);
        SYS_DEBUG(DL_INFO1, "ObjectListBase::ObjectListInternal::Append(): size=" << myModifiedObjects->size());
    }

    inline void ObjectListBase::ObjectListInternal::Cleanup(void)
    {
        SYS_DEBUG_MEMBER(DM_GLESLY);
        CopyObjects();
        myModifiedObjects->clear();
    }

    typedef ObjectListBase::ObjectListInternal ObjectList;

} // namespace Glesly

#endif /* __GLESLY_SRC_OBJECT_LIST_BASE_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
