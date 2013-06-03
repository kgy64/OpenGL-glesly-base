/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Generic Matrix Operations
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_MATRIX_MATRIX_H_INCLUDED__
#define __GLESLY_SRC_MATRIX_MATRIX_H_INCLUDED__

#include <Exceptions/Exceptions.h>

#include <math.h>

namespace Glesly
{
    template <typename T, unsigned R, unsigned S>
    class Matrix
    {
     protected:
        inline Matrix(void)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

        VIRTUAL_IF_DEBUG inline ~Matrix()
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

        T myMatrix[R*S];

        template <unsigned U, unsigned V>
        Matrix<T,R,V> operator*(const Matrix<T,U,V> & other)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            ASSERT_FATAL(U==S, "Matrix multiplication: size problem");
            Matrix<T,R,V> result;
            memset(result.myMatrix, 0, sizeof(result.myMatrix));
            // TODO
            return result;
        }

        inline Matrix<T,R,S> & operator*=(const Matrix<T,R,S> & other)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            *this = (*this) * other;
            return *this;
        }

     public:
        inline T * operator[](int index)
        {
            return myMatrix + R * index;
        }

        inline const T * operator=(const void * data)
        {
            memcpy(myMatrix, data, sizeof(myMatrix));
            return myMatrix;
        }

        inline Matrix<T,R,S> & operator=(const Matrix<T,R,S> & other)
        {
            myMatrix = other.myMatrix;
            return *this;
        }

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::Matrix<T,R,S>");

    }; // class Matrix

} // namespace Glesly

#endif /* __GLESLY_SRC_MATRIX_MATRIX_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
