/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Generic Vector Operations
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_MATH_VECTOR_H_INCLUDED__
#define __GLESLY_SRC_MATH_VECTOR_H_INCLUDED__

#include <Debug/Debug.h>
#include <Exceptions/Exceptions.h>

#include <iostream>
#include <math.h>

SYS_DECLARE_MODULE(DM_GLESLY);

namespace Glesly
{
    template <typename T, unsigned S>
    class Vector
    {
     public:
        inline Vector(void)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

        inline Vector(std::initializer_list<T> data)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            ASSERT_FATAL(data.size() == S, "Invalid intializer for vector");
            std::copy(data.begin(), data.end(), myVector);
        }

        virtual inline ~Vector()
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

        inline T & operator[](int index)
        {
            return myVector[index];
        }

        inline T operator[](int index) const
        {
            return myVector[index];
        }

        inline Vector<T,S> & operator=(const Vector<T,S> & other)
        {
            memcpy(myVector, other.myVector, sizeof(myVector));
            return *this;
        }

     protected:
        T myVector[S];

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::Vector<T,S>");

    }; // class Vector

    typedef Glesly::Vector<float,3> Vector3D;

} // namespace Glesly

#endif /* __GLESLY_SRC_MATH_VECTOR_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
