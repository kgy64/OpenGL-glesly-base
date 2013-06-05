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

#include <matrix/matrix.h>

namespace Glesly
{
    class CameraPosition
    {
     public:
        inline void SetWidth(float value)
        {
            width = value;
        }

        inline void SetHeight(float value)
        {
            height = value;
        }

        inline void SetX(float value)
        {
            x = value;
        }

        inline void SetY(float value)
        {
            y = value;
        }

        inline void SetZ(float value)
        {
            z = value;
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
        float width;
        float height;
        float x;
        float y;
        float z;
        float near;
        float far;
        float fov;

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::CameraPosition");

    }; // class CameraPosition

    class CameraMatrix: public Glesly::Matrix<float, 4, 4>, public CameraPosition
    {
     public:
        CameraMatrix(void);
        virtual ~CameraMatrix();

        void Update(void);

    }; // class CameraMatrix

} // namespace Glesly

#endif /* __GLESLY_SRC_CAMERA_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
