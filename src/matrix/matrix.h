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

        Matrix<T,R,S> & operator*=(const Matrix<T,R,S> & other)
        {
            T original[R*S] = myMatrix; // Copy the original matrix
            memset(myMatrix, 0, sizeof(myMatrix)); // It will be calculated now
            //
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

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::Matrix<T,R,S>");

    }; // class Matrix

} // namespace Glesly

#endif /* __GLESLY_SRC_MATRIX_MATRIX_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
