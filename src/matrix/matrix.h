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

#include <iostream>
#include <math.h>

namespace Glesly
{
    template <typename T, unsigned R, unsigned S>
    class Matrix
    {
     public:
        inline Matrix(void)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

        inline Matrix(const void * data)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            memcpy(myMatrix, data, sizeof(myMatrix));
        }

        VIRTUAL_IF_DEBUG inline ~Matrix()
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

        template <unsigned U, unsigned V>
        Matrix<T,R,V> operator*(const Matrix<T,U,V> & other)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            ASSERT_FATAL(U==S, "Matrix multiplication: size problem");
            Matrix<T,R,V> result;
            memset(result.myMatrix, 0, sizeof(result.myMatrix));
            for (unsigned i = 0; i < R; ++i) {
                for (unsigned j = 0; j < V; ++j) {
                    for (unsigned k = 0; k < S; ++k) {
                        result[i][j] += (*this)[k][j] * other[i][k];
                    }
                }
            }
            return result;
        }

        inline Matrix<T,R,S> & operator*=(const Matrix<T,R,S> & other)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            *this = (*this) * other;
            return *this;
        }

        inline T * operator[](int index)
        {
            return myMatrix + R * index;
        }

        inline const T * operator[](int index) const
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
            memcpy(myMatrix, other.myMatrix, sizeof(myMatrix));
            return *this;
        }

     protected:
        T myMatrix[R*S];

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::Matrix<T,R,S>");

    }; // class Matrix

} // namespace Glesly

template <typename T, unsigned R, unsigned S>
std::ostream & operator<<(std::ostream & os, const Glesly::Matrix<T,R,S> & m)
{
 os << "{";
 for (unsigned i = 0; i < R; ++i) {
    os << "{";
    for (unsigned j = 0; ; ) {
        os << m[i][j];
        if (++j >= S) {
            break;
        }
        os << " ";
    }
    os << "}";
 }
 os << "}";
 return os;
}

#endif /* __GLESLY_SRC_MATRIX_MATRIX_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
