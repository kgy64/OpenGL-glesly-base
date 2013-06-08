/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Camera movement classes
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_CAMERA_H_INCLUDED__
#define __GLESLY_SRC_CAMERA_H_INCLUDED__

#include <glesly/math/vector.h>
#include <glesly/math/matrix.h>

namespace Glesly
{
    class CameraParameters
    {
     public:
        inline CameraParameters(void)
        {
        }

        inline void SetSize(float width, float height)
        {
            myWidth = width;
            myHeight = height;
        }

        inline void SetNearPlane(float value)
        {
            near = value;
        }

        inline void SetFarPlane(float value)
        {
            far = value;
        }

        inline void SetFieldOfView(float value)
        {
            fov = value;
        }

     protected:
        float myWidth;
        float myHeight;

        float near;
        float far;
        float fov;

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::CameraParameters");

    }; // class CameraParameters

    class CameraMatrix: public Glesly::Matrix<float, 4, 4>, public CameraParameters
    {
     public:
        CameraMatrix(void);
        virtual ~CameraMatrix();

        void Update(void);

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::CameraMatrix");

    }; // class CameraMatrix

    class Transformation: public Glesly::Matrix<float, 4, 4>, public CameraParameters
    {
     public:
        Transformation(void):
            Glesly::Matrix<float, 4, 4>(1.0)
        {
        }

        void RotateX(float angle);
        void RotateY(float angle);
        void RotateZ(float angle);
        void Move(float x, float y, float z);

    }; // Transformation

} // namespace Glesly

#endif /* __GLESLY_SRC_CAMERA_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
