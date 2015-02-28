/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     
 * Purpose:     
 * Author:      
 * License:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "sphere.h"

#include <glesly/read-tga.h>

using namespace Glesly;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *     class Glesly::SphereSurface:                                                      *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void SphereSurface::updatePointers(void)
{
 textureTargets[0] = &*pacaTargets[0];
 textureTargets[1] = &*pacaTargets[1];
 textureTargets[2] = &*pacaTargets[2];
 textureTargets[3] = &*pacaTargets[3];
 textureTargets[4] = &*pacaTargets[4];
 textureTargets[5] = &*pacaTargets[5];
}

/// Creates an empty (black or transparent) texture for the whole sphere
/*! \param      size        The width and the height of the six underlying textures, in pixels.
 *                          Note that they must be squares by design of OpenGL. Must be power
 *                          of two. Because it must be compatible with GLES, the limits are 32
 *                          and 2048.
 *  \param      format      The pixel format of the textures.
 */
void SphereSurface::reset(int size, Glesly::PixelFormat format)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 if (myFormat == Glesly::FORMAT_DEFAULT) {
    myFormat = format;
 }

 if (myFormat == Glesly::FORMAT_DEFAULT) {
    myFormat = Glesly::FORMAT_RGB_565;
 }

 for (int i = 0; i < 6; ++i) {
    if (!pacaTargets[i] || pacaTargets[i]->GetPixelFormat() != myFormat || pacaTargets[i]->GetWidth() != size || pacaTargets[i]->GetHeight() != size) {
        pacaTargets[i] = PaCaLib::Target::Create(size, size, myFormat);
    }
 }

 updatePointers();
}

/// Create texture from six bitmap files
/*! Note that they must have the same pixel format, and the same size. */
void SphereSurface::reset(const char * const * filenames)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 if (!filenames) {
    return;
 }

 int size = 0;

 for (int i = 0; i < 6; ++i) {
    reset(pacaTargets[i], filenames[i], size);
 }

 updatePointers();
}

/*! Helper function for function \ref SphereSurface::reset(const char * const *) to
 *  initialize one texture. */
void SphereSurface::reset(PaCaLib::TargetPtr & target, const char * name, int & size)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 if (!name) {
    return;
 }

 Glesly::ReadTGA tga(name);

 if (!size) {
    size = tga.GetWidth();
 }

 ASSERT(tga.GetWidth() == size && tga.GetHeight() == size, "got a texture ('" << name << "') with wrong size: " << tga.GetWidth() << "x" << tga.GetHeight() << ", instead of " << size << "x" << size);

 if (myFormat == Glesly::FORMAT_DEFAULT) {
    myFormat = tga.GetPixelFormat();
 } else {
    ASSERT(tga.GetPixelFormat() == myFormat, "got a texture ('" << name << "') with wrong format " << tga.GetPixelFormat() << ", instead of " << myFormat);
 }

 if (!target) {
    target = PaCaLib::Target::Create(size, size, myFormat);
 }

 *target = tga;
}

/// Return a \ref PaCaLib::Draw instance
/*! The surface of the sphere can be drawn using the usual drawing interface. The x and y parameters of the
 *  drawing functions are angles (longitude and latitude) in radian. */
PaCaLib::DrawPtr SphereSurface::Draw(void)
{
 return PaCaLib::DrawPtr(new SphereData::Draw(*this));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *     class Glesly::SphereData::Convert3D:                                              *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/// Convert longitude and latitude to x:y:z coordinates
void SphereData::Convert3D::Convert(float longitude, float latitude, float position[3])
{
 float temp  = cosf(latitude);          //  radius of the current latitude
 position[0] = sinf(longitude) * temp;  //  x -> East
 position[1] = sinf(latitude);          //  y -> North
 position[2] = cosf(longitude) * temp;  //  z -> 0:0 (Africa)
}

void SphereData::Convert3D::Convert(const SphereData::Convert3D::Oper * source, SphereData::Convert3D::Oper * destination, int count)
{
 for (int i = 0; i < count; ++i, ++source, ++destination) {
    destination->op = source->op;
    if (destination->data[2] < 0.2f) {
        destination->flag = 0;
        destination->data[0] = 0.0f;
        destination->data[1] = 0.0f;
        destination->data[2] = 0.0f;
    } else {
        destination->flag = 1;
        destination->data[0] = source->data[0] / destination->data[2];
        destination->data[1] = source->data[1] / destination->data[2];
        destination->data[2] = 0.0f;
    }
    destination->data[3] = source->data[3];
    destination->data[4] = source->data[4];
    destination->data[5] = source->data[5];
 }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *     class Glesly::SphereData::Draw:                                                   *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

SphereData::Draw::Draw(SphereSurface & parent):
    parent(parent)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 for (int i = 0; i < 6; ++i) {
    draws[i] = parent.GetDraw(i);
 }
}

void SphereData::Draw::Scale(float w, float h)
{
 ASSERT(false, "Scale() cannot be used on sphere");
}

void SphereData::Draw::SetColour(float r, float g, float b, float a)
{
 for (int i = 0; i < 6; ++i) {
    draws[i]->SetColour(r, g, b, a);
 }
}

void SphereData::Draw::SetColourCompose(PaCaLib::ColourCompose mode)
{
 for (int i = 0; i < 6; ++i) {
    draws[i]->SetColourCompose(mode);
 }
}

void SphereData::Draw::SetTextOutlineColour(float r, float g, float b, float a)
{
 for (int i = 0; i < 6; ++i) {
    draws[i]->SetTextOutlineColour(r, g, b, a);
 }
}

void SphereData::Draw::SetTextOutline(float outline)
{
 for (int i = 0; i < 6; ++i) {
    draws[i]->SetTextOutline(outline);
 }
}

void SphereData::Draw::SetLineWidth(float width)
{
 for (int i = 0; i < 6; ++i) {
    draws[i]->SetLineWidth(width);
 }
}

void SphereData::Draw::SetLineCap(PaCaLib::LineCap mode)
{
 for (int i = 0; i < 6; ++i) {
    draws[i]->SetLineCap(mode);
 }
}

void SphereData::Draw::Paint(void)
{
 for (int i = 0; i < 6; ++i) {
    draws[i]->Paint();
 }
}

PaCaLib::PathPtr SphereData::Draw::NewPath(void)
{
 return PaCaLib::PathPtr(new SphereData::Path(*this));
}

float SphereData::Draw::DrawTextInternal(float lon, float lat, PaCaLib::TextMode mode, const char * text, float size, float offset, float aspect, float rotation, float shear_x, float shear_y)
{
 float pos[3];
 Convert(lon, lat, pos);

 // pos[0]:     x -> East           (= sin(lon) * cos(lat))
 // pos[1]:     y -> North          (= sin(lat))
 // pos[2]:     z -> Africa         (= cos(lon) * cos(lat))

 float cl = cosf(lon);
 float cl2 = cl * cl;
 float sl = sinf(lon);
 float sl2 = sl * sl;

 float result = DrawTextInternal(+pos[0], -pos[2], -pos[1], 2, mode, text, size, offset, aspect, rotation + lon, shear_x, shear_y); // -> S
 /* ------- */  DrawTextInternal(+pos[0], +pos[2], +pos[1], 3, mode, text, size, offset, aspect, rotation - lon, shear_x, shear_y); // -> N

 if (sl2 > 0.2f) {
    DrawTextInternal(-pos[2], -pos[1], +pos[0], 0, mode, text, size*sl, offset, aspect/sl2, rotation, shear_x, shear_y); // -> E
    DrawTextInternal(+pos[2], -pos[1], -pos[0], 1, mode, text, -size*sl, offset, aspect/sl2, rotation, shear_x, shear_y); // -> W
 }

 if (cl2 > 0.2f) {
    DrawTextInternal(+pos[0], -pos[1], +pos[2], 4, mode, text, size*cl, offset, aspect/cl2, rotation, shear_x, shear_y); // -> 0:0 (Africa)
    DrawTextInternal(-pos[0], -pos[1], -pos[2], 5, mode, text, -size*cl, offset, aspect/cl2, rotation, shear_x, shear_y); //
 }

 return result;
}

float SphereData::Draw::DrawTextInternal(float x, float y, float z, int index, PaCaLib::TextMode mode, const char * text, float size, float offset, float aspect, float rotation, float shear_x, float shear_y)
{
 if (z < 0.5f) {
    return size;
 }

 x /= z;
 y /= z;

 float sy = 0.0f;

 switch (index) {
    case 2:
    case 3:
        // Nothing to do here (North and South bitmaps has no shear distortion)
    break;
    default:
    {
        // Note: This is a temporary code to correct the shear of text on the corresponding surfaces.
        //       It is a heuristic algorythm, not perfect, but good enough to see text on map.
        static constexpr float correction = 0.90f;
        static constexpr float nonlinearity = -0.45f;
        sy = correction * y * (x + nonlinearity * x*x*x);
    }
    break;
 }

 return draws[index]->DrawTextInternal(x, y, mode, text, size/(z*sqrtf(z)), offset, aspect*sqrtf(z), rotation, shear_x, shear_y - sy);
}

void SphereData::Draw::Stroke(const Oper * ops, int count)
{
 for (int i = 0; i < 6; ++i) {
    Oper my_opcodes[MAX_OPERS];
    Convert(ops, my_opcodes, count);
    Stroke(i, my_opcodes, count);
 }
}

void SphereData::Draw::Stroke(int index, const Oper * ops, int count)
{
}

void SphereData::Draw::Fill(const Oper * ops, int count)
{
 for (int i = 0; i < 6; ++i) {
    Oper my_opcodes[MAX_OPERS];
    Convert(ops, my_opcodes, count);
    Fill(i, my_opcodes, count);
 }
}

void SphereData::Draw::Fill(int index, const Oper * ops, int count)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *     class Glesly::SphereData::Path:                                                   *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

SphereData::Path::Path(SphereData::Draw & parent):
    parent(parent),
    opCount(0)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
}

void SphereData::Path::Move(float x, float y)
{
 float pos[3];
 Convert(x, y, pos);
 push(OP_MOVE, pos);
}

void SphereData::Path::Line(float x, float y)
{
 float pos[3];
 Convert(x, y, pos);
 push(OP_LINE, pos);
}

void SphereData::Path::Arc(float xc, float yc, float r, float a1, float a2)
{
 float pos[3];
 Convert(xc, yc, pos);
 push(OP_ARC, pos, r, a1, a2);
}

void SphereData::Path::Bezier(float x, float y, float dx, float dy)
{
 float pos[3];
 Convert(x, y, pos);
 push(OP_BEZIER, pos, dx, dy);
}

void SphereData::Path::Close(void)
{
 push(OP_CLOSE);
}

void SphereData::Path::Clear(void)
{
 opCount = 0;
}

void SphereData::Path::Stroke(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 parent.Stroke(opcodes, opCount);
}

void SphereData::Path::Fill(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 parent.Fill(opcodes, opCount);
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
