/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     C++ wrapper for GL/ES - Smart Pointer for Objects
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_OBJECT_PTR_H_INCLUDED__
#define __GLESLY_SRC_OBJECT_PTR_H_INCLUDED__

#include <list>
#include <Memory/Memory.h>

namespace Glesly
{
    class ObjectBase;

    typedef MEM::weak_ptr<Glesly::ObjectBase> ObjectWeak;

    class ObjectPtr: public MEM::shared_ptr<ObjectBase>
    {
        friend class ObjectBase;

     public:
        inline ObjectPtr(void)
        {
        }

        inline ObjectPtr(MEM::shared_ptr<ObjectBase> obj):
            MEM::shared_ptr<ObjectBase>(obj)
        {
        }

        inline ObjectPtr & operator=(MEM::shared_ptr<ObjectBase> & obj)
        {
            static_cast<MEM::shared_ptr<ObjectBase>&>(*this) = obj;
            return *this;
        }

        template <class C>
        inline C & getObj(void)
        {
            return static_cast<C &>(**this);
        }

     protected:
        ObjectPtr(ObjectBase * obj);

    }; // Glesly::ObjectPtr

    template<class OBJ>
    class ObjectWrapper: public ObjectPtr
    {
     public:
        inline ObjectWrapper(void)
        {
        }

        inline ObjectWrapper(ObjectPtr obj):
            ObjectPtr(obj)
        {
        }

        inline ObjectWrapper<OBJ> & operator=(ObjectPtr & obj)
        {
            static_cast<ObjectPtr&>(*this) = obj;
            return *this;
        }

        inline OBJ * operator->()
        {
            return static_cast<OBJ*>(get());
        }

        inline const OBJ * operator->() const
        {
            return static_cast<OBJ*>(get());
        }

    }; // class Glesly::ObjectWrapper

    typedef std::list<ObjectPtr> Objects;

    typedef Objects::iterator ObjectListIterator;

    typedef MEM::shared_ptr<Objects> ObjectListPtr;

} // namespace Glesly

#endif /* __GLESLY_SRC_OBJECT_PTR_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
