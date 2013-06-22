/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Classes representing my Objects
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_OBJECT_LIST_H_INCLUDED__
#define __GLESLY_SRC_OBJECT_LIST_H_INCLUDED__

#include <list>
#include <stack>
#include <glesly/object-ptr.h>
#include <Threads/Mutex.h>
#include <System/TimeDelay.h>
#include <Debug/Debug.h>

namespace Glesly
{
    typedef std::list<ObjectPtr> Objects;

    typedef boost::shared_ptr<Objects> ObjectListPtr;

    typedef Objects::iterator ObjectListIterator;

    class ObjectListBase;

    class ObjectListBase
    {
     public:
        class ObjectListInternal
        {
            friend class ObjectListBase;

         public:
            ~ObjectListInternal();

            void Add(ObjectPtr object);

            ObjectListIterator begin(void)
            {
                return myObjects->begin();
            }

            ObjectListIterator end(void)
            {
                return myObjects->end();
            }

         private:
            ObjectListInternal(ObjectListBase & parent);

            static Threads::Mutex myMutex;

            ObjectListBase & myParent;

            Threads::Lock myLock;

            ObjectListPtr myObjects;

            ObjectListPtr myModifiedObjects;

        }; // class ObjectListInternal

        inline ObjectListInternal GetObjectList(void)
        {
            return ObjectListInternal(*this);
        }

        inline ObjectListPtr & GetObjectListPtr(void)
        {
            return myLayers.top();
        }

     protected:
        typedef std::stack<ObjectListPtr> ObjectLayerStack;

        inline ObjectListBase(void)
        {
            myLayers.push(ObjectListPtr(new Objects));
        }

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::ObjectListBase");

        ObjectLayerStack myLayers;

    }; // class ObjectListBase

    inline ObjectListBase::ObjectListInternal::ObjectListInternal(ObjectListBase & parent):
        myParent(parent),
        myLock(myMutex),
        myObjects(parent.GetObjectListPtr())
    {
    }

    inline ObjectListBase::ObjectListInternal::~ObjectListInternal()
    {
        if (myModifiedObjects.get()) {
            myParent.GetObjectListPtr() = myModifiedObjects;
        }
    }

    inline void ObjectListBase::ObjectListInternal::Add(ObjectPtr object)
    {
        if (!myModifiedObjects.get()) {
            myModifiedObjects.reset(new Objects(*myObjects));
        }
        myModifiedObjects->push_front(object);
    }

    typedef ObjectListBase::ObjectListInternal ObjectList;

} // namespace Glesly

#endif /* __GLESLY_SRC_OBJECT_LIST_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
