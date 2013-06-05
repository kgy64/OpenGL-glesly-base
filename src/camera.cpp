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

 float r_xy_factor = fminf(width, height) / fov;
 float r_x = r_xy_factor/width;
 float r_y = r_xy_factor/height;
 float r_zw_factor = far - near;
 float r_z = (near + far) / r_zw_factor;
 float r_w = -2.0f*near*far / r_zw_factor;

 const float p_init[] = {
    r_x,    0.0,    0.0,    0.0,
    0.0,    r_y,    0.0,    0.0,
    0.0,    0.0,    r_z,    1.0,
    0.0,    0.0,    r_w,    0.0
 };
 static_cast<Matrix&>(*this) = p_init;

 // TODO: angles!
 //
 static const float alpha = 60.0f * (M_PI/180.0);

 const float mv_init[] = {
    1.0,    0.0,            0.0,            0.0,
    0.0,   +cosf(alpha),   +sinf(alpha),    0.0,
    0.0,   -sinf(alpha),   +cosf(alpha),    0.0,
    0.0,   -0.00,           1.0,            1.0
 };

 Glesly::Matrix<float,4,4> m(mv_init);
 *this *= m;
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
