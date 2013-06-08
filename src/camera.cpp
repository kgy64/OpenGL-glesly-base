/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Camera movement
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "camera.h"

using namespace Glesly;

CameraMatrix::CameraMatrix(void):
    Glesly::Matrix<float,4,4>(1.0f)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
}

CameraMatrix::~CameraMatrix()
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
}

void CameraMatrix::Update(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 float r_xy_factor = fminf(myWidth, myHeight) / fov;
 float r_x = r_xy_factor/myWidth;
 float r_y = r_xy_factor/myHeight;
 float r_zw_factor = far - near;
 float r_z = (near + far) / r_zw_factor;
 float r_w = -2.0f*near*far / r_zw_factor;

 static_cast<Matrix<float,4,4>&>(*this) = {
    r_x,    0.0f,   0.0f,   0.0f,
    0.0f,   r_y,    0.0f,   0.0f,
    0.0f,   0.0f,   r_z,    1.0f,
    0.0f,   0.0f,   r_w,    0.0f
 };
}

void Transformation::RotateX(float angle)
{
 float s = sinf(angle);
 float c = cosf(angle);

 static_cast<Matrix<float,4,4>&>(*this) = {
    1.0f,   0.0f,   0.0f,   0.0f,
    0.0f,   c,      s,      0.0f,
    0.0f,  -s,      c,      0.0f,
    0.0f,   0.0f,   0.0f,   1.0f
 };
}

void Transformation::RotateY(float angle)
{
 float s = sinf(angle);
 float c = cosf(angle);

 static_cast<Matrix<float,4,4>&>(*this) = {
    c,      0.0f,    s,      0.0f,
    0.0f,   1.0f,    0.0f,   0.0f,
   -s,      0.0f,    c,      0.0f,
    0.0f,   0.0f,    0.0f,   1.0f
 };
}

void Transformation::RotateZ(float angle)
{
 float s = sinf(angle);
 float c = cosf(angle);

 static_cast<Matrix<float,4,4>&>(*this) = {
    c,      s,      0.0f,   0.0f,
   -s,      c,      0.0f,   0.0f,
    0.0f,   0.0f,   1.0f,   0.0f,
    0.0f,   0.0f,   0.0f,   1.0f
 };
}

void Transformation::Move(float x, float y, float z)
{
 static_cast<Matrix<float,4,4>&>(*this) = {
    1.0f,   0.0f,   0.0f,   0.0f,
    0.0f,   1.0f,   0.0f,   0.0f,
    0.0f,   0.0f,   1.0f,   0.0f,
    x,      y,      z,      1.0f
 };
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
