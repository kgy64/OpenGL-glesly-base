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

#include <Debug/Debug.h>
#include <Exceptions/Exceptions.h>

#include <iostream>
#include <math.h>

SYS_DECLARE_MODULE(DM_GLESLY);

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

        inline Matrix(std::initializer_list<T> data)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            ASSERT_FATAL(data.size() == R*S, "Invalid intializer for matrix");
            std::copy(data.begin(), data.end(), myMatrix);
        }

        inline Matrix(T data)
        {
            *this = data;
        }

        virtual inline ~Matrix()
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

        /// Matrix multiplication
        /*! 
            \note   It is allowed to give smaller input matrix (U<S): in this case the
                    remaining part is assumed to be zero. */
        template <unsigned U, unsigned V>
        Matrix<T,R,V> operator*(const Matrix<T,U,V> & other) const
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            ASSERT_FATAL(U<=S, "Matrix multiplication: size problem");
            Matrix<T,R,V> result(0.0);
            for (unsigned r = 0; r < R; ++r) {
                for (unsigned u = 0; u < U; ++u) {
                    for (unsigned v = 0; v < V; ++v) {
                        result[r][v] += (*this)[r][u] * other[u][v];
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
            return myMatrix + S * index;
        }

        inline const T * operator[](int index) const
        {
            return myMatrix + S * index;
        }

        inline const T * operator=(std::initializer_list<T> data)
        {
            ASSERT_FATAL(data.size() == R*S, "Invalid intializer for matrix");
            std::copy(data.begin(), data.end(), myMatrix);
            return myMatrix;
        }

        T operator=(T data)
        {
            for (unsigned i = 0; i < R; ++i) {
                for (unsigned j = 0; j < S; ++j) {
                    (*this)[i][j] = (i==j) ? data : 0;
                }
            }
            return data;
        }

        inline Matrix<T,R,S> & operator=(const Matrix<T,R,S> & other)
        {
            memcpy(myMatrix, other.myMatrix, sizeof(myMatrix));
            return *this;
        }

        const float * get(void) const
        {
            return myMatrix;
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
        os << "|";
    }
    os << "}";
 }
 os << "}";
 return os;
}

#endif /* __GLESLY_SRC_MATRIX_MATRIX_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
