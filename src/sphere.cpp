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

SYS_DEFINE_MODULE(DM_GL_SPHERE);

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
 SYS_DEBUG_MEMBER(DM_GL_SPHERE);

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
 SYS_DEBUG_MEMBER(DM_GL_SPHERE);

 if (!filenames) {
    return;
 }

 for (int i = 0; i < 6; ++i) {
    int size = 0;
    reset(pacaTargets[i], filenames[i], size);
 }

 updatePointers();
}

/*! Helper function for function \ref SphereSurface::reset(const char * const *) to
 *  initialize one texture. */
void SphereSurface::reset(PaCaLib::TargetPtr & target, const char * name, int & size)
{
 SYS_DEBUG_MEMBER(DM_GL_SPHERE);

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

void SphereData::Convert3D::Convert(const SphereData::Convert3D::Oper * source, SphereData::Convert3D::Oper * destination, int count, int mode)
{
 SYS_DEBUG_MEMBER(DM_GL_SPHERE);

 ASSERT(count < MAX_OPERS, "Too much count: " << count);
 SYS_DEBUG(DL_INFO1, "count=" << count);

 static constexpr float Z_LIMIT = 0.4f;

 switch (mode) {
    case 0:     // E
        SYS_DEBUG(DL_INFO1, "East");
        for (int i = 0; i < count; ++i) {
            float z = source[i].data[0];
            if (z < Z_LIMIT) {
                destination[i].Invalidate();
            } else {
                destination[i].op = source[i].op;
                destination[i].flag = ST_USED;
                destination[i].data[0] = -source[i].data[2];
                destination[i].data[1] = -source[i].data[1];
                destination[i].data[2] =  z;
                destination[i].data[3] =  source[i].data[3];
                destination[i].data[4] =  source[i].data[4];
                destination[i].data[5] =  source[i].data[5];
                destination[i].Prepare();
            }
        }
    break;
    case 1:     // W
        SYS_DEBUG(DL_INFO1, "West");
        for (int i = 0; i < count; ++i) {
            float z = -source[i].data[0];
            if (z < Z_LIMIT) {
                destination[i].Invalidate();
            } else {
                destination[i].op = source[i].op;
                destination[i].flag = ST_USED;
                destination[i].data[0] =  source[i].data[2];
                destination[i].data[1] = -source[i].data[1];
                destination[i].data[2] =  z;
                destination[i].data[3] =  source[i].data[3];
                destination[i].data[4] =  source[i].data[4];
                destination[i].data[5] =  source[i].data[5];
                destination[i].Prepare();
            }
        }
    break;
    case 2:     // S
        SYS_DEBUG(DL_INFO1, "South");
        for (int i = 0; i < count; ++i) {
            float z = -source[i].data[1];
            if (z < Z_LIMIT) {
                destination[i].Invalidate();
            } else {
                destination[i].op = source[i].op;
                destination[i].flag = ST_USED;
                destination[i].data[0] =  source[i].data[0];
                destination[i].data[1] = -source[i].data[2];
                destination[i].data[2] =  z;
                destination[i].data[3] =  source[i].data[3];
                destination[i].data[4] =  source[i].data[4];
                destination[i].data[5] =  source[i].data[5];
                destination[i].Prepare();
            }
        }
    break;
    case 3:     // N
        SYS_DEBUG(DL_INFO1, "North");
        for (int i = 0; i < count; ++i) {
            float z = source[i].data[1];
            if (z < Z_LIMIT) {
                destination[i].Invalidate();
            } else {
                destination[i].op = source[i].op;
                destination[i].flag = ST_USED;
                destination[i].data[0] =  source[i].data[0];
                destination[i].data[1] =  source[i].data[2];
                destination[i].data[2] =  z;
                destination[i].data[3] =  source[i].data[3];
                destination[i].data[4] =  source[i].data[4];
                destination[i].data[5] =  source[i].data[5];
                destination[i].Prepare();
            }
        }
    break;
    case 4:     // Front (Africa)
        SYS_DEBUG(DL_INFO1, "Front");
        for (int i = 0; i < count; ++i) {
            float z = source[i].data[2];
            if (z < Z_LIMIT) {
                destination[i].Invalidate();
            } else {
                destination[i].op = source[i].op;
                destination[i].flag = ST_USED;
                destination[i].data[0] =  source[i].data[0];
                destination[i].data[1] = -source[i].data[1];
                destination[i].data[2] =  z;
                destination[i].data[3] =  source[i].data[3];
                destination[i].data[4] =  source[i].data[4];
                destination[i].data[5] =  source[i].data[5];
                destination[i].Prepare();
            }
        }
    break;
    case 5:     // Back
        SYS_DEBUG(DL_INFO1, "Back");
        for (int i = 0; i < count; ++i) {
            float z = -source[i].data[2];
            if (z < Z_LIMIT) {
                destination[i].Invalidate();
            } else {
                destination[i].op = source[i].op;
                destination[i].flag = ST_USED;
                destination[i].data[0] = -source[i].data[0];
                destination[i].data[1] = -source[i].data[1];
                destination[i].data[2] =  z;
                destination[i].data[3] =  source[i].data[3];
                destination[i].data[4] =  source[i].data[4];
                destination[i].data[5] =  source[i].data[5];
                destination[i].Prepare();
            }
        }
    break;
    default:
        ASSERT(false, "Invalid mode: " << mode);
    break;
 }

 SphereData::Convert3D::Oper * prev = nullptr;
 bool next = false;
 for (int i = 0; i < count; ++i, ++destination) {
    bool current = destination->flag != ST_UNUSED;
    if (next) {
        destination->flag |= ST_ADDED;
    }
    if (current && prev) {
        prev->flag |= ST_ADDED;
    }
    next = current;
    prev = destination;
 }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *     class Glesly::SphereData::Convert3D::Position:                                    *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

SphereData::Convert3D::Position::Position(float lon, float lat):
    cos_lat(cosf(lat)),
    cos_lon(cosf(lon)),
    x(sinf(lon)*cos_lat),
    y(sinf(lat)),
    z(cos_lon*cos_lat)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *     class Glesly::SphereData::Draw:                                                   *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

SphereData::Draw::Draw(SphereSurface & parent):
    parent(parent)
{
 SYS_DEBUG_MEMBER(DM_GL_SPHERE);

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

void SphereData::Draw::SetOutlineColour(float r, float g, float b, float a)
{
 for (int i = 0; i < 6; ++i) {
    draws[i]->SetOutlineColour(r, g, b, a);
 }
}

void SphereData::Draw::SetOutlineWidth(float outline)
{
 for (int i = 0; i < 6; ++i) {
    draws[i]->SetOutlineWidth(outline);
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
 SYS_DEBUG_MEMBER(DM_GL_SPHERE);

 return PaCaLib::PathPtr(new SphereData::Path(*this));
}

float SphereData::Draw::DrawTextInternal(const PaCaLib::Draw::TextParams & params, const PaCaLib::Draw::Distortion *)
{
 SYS_DEBUG_MEMBER(DM_GL_SPHERE);

 float lon = params.x * (float)M_PI;
 float lat = params.y * (float)(M_PI/2.0f);

 Position pos(lon, lat);

 float cl = cosf(lon);
 float cl2 = cl * cl;
 float sl = sinf(lon);
 float sl2 = sl * sl;

 float texts = 2.0f;

 PaCaLib::Draw::Distortion distortion;

 float result = 0.0f;

 {
    PaCaLib::Draw::TextParams p = params;

    if (sl2 > 0.2f) {
        p.aspect = params.aspect / sl2;
        p.size = params.size * sl;
        result += DrawTextInternal(p, distortion, -pos.z, -pos.y, +pos.x, sl, 0); // -> E
        p.size = -params.size * sl;
        result += DrawTextInternal(p, distortion, +pos.z, -pos.y, -pos.x, sl, 1); // -> W
        texts += 2.0f;
    }

    if (cl2 > 0.2f) {
        p.aspect = params.aspect / cl2;
        p.size = params.size * cl;
        result += DrawTextInternal(p, distortion, +pos.x, -pos.y, +pos.z, cl, 4); // -> Front (Africa)
        p.size = -params.size * cl;
        result += DrawTextInternal(p, distortion, -pos.x, -pos.y, -pos.z, -cl, 5); // -> Back
        texts += 2.0f;
    }
 }

 distortion.shear_y = 0.0f;
 distortion.rotation = lon;
 result += DrawTextInternal(params, distortion, +pos.x, -pos.z, -pos.y, 1.0f, 2); // -> S
 distortion.rotation = -lon;
 result += DrawTextInternal(params, distortion, +pos.x, +pos.z, +pos.y, 1.0f, 3); // -> N

 return result / texts;
}

float SphereData::Draw::DrawTextInternal(const PaCaLib::Draw::TextParams & params, PaCaLib::Draw::Distortion & distortion, float x, float y, float z, float corr, int index)
{
 SYS_DEBUG_MEMBER(DM_GL_SPHERE);

 if (z < 0.4f) {
    return params.size;
 }

 PaCaLib::Draw::TextParams par = params;

 par.x = x / z;
 par.y = y / z;

 // Note: This is a temporary code to correct the shear of text on the corresponding surfaces.
 //       It is a heuristic algorythm, not perfect, but good enough to see text on map.
 // TODO: Make it mathematically correct.

 distortion.obj_size = 1.0f / z;

 switch (index) {
    case 2:
    case 3:
        distortion.shear_y = 0.0f;
        distortion.obj_height = 1.0f;
        distortion.scene_height = distortion.obj_size;
    break;
    default:
    {
        static constexpr float correction = -0.90f;
        static constexpr float nonlinearity = -0.45f;
        distortion.shear_y = correction * par.y * (par.x + nonlinearity * par.x*par.x*par.x);
        distortion.obj_height = distortion.obj_size;
        distortion.scene_height = 1.0f;
    }
    break;
 }

 return draws[index]->DrawTextInternal(par, &distortion);
}

void SphereData::Draw::DrawPath(PaCaLib::Path::DrawMode mode, const Oper * ops, int count)
{
 SYS_DEBUG_MEMBER(DM_GL_SPHERE);

 ASSERT(count <= MAX_OPERS, "Count size too big (" << count << ">" << (int)MAX_OPERS << ")");

 for (int i = 0; i < 6; ++i) {
    Oper my_opcodes[MAX_OPERS];
    Convert(ops, my_opcodes, count, i);
    DrawPath(mode, i, my_opcodes, count);
 }
}

void SphereData::Draw::DrawPath(PaCaLib::Path::DrawMode mode, int index, const Oper * op, int count)
{
 SYS_DEBUG_MEMBER(DM_GL_SPHERE);

 SYS_DEBUG(DL_INFO1, "index=" << index << ", count=" << count);

 PaCaLib::PathPtr path;
 bool continuous = true;
 for (int i = 0; i < count; ++i, ++op) {
    bool ok = op->isUsed();
    SYS_DEBUG(DL_INFO1, "oper=" << op << ", OK=" << ok << ", data: " << *op);
    if (ok) {
        if (!path) {
            path = draws[index]->NewPath();
        }
        op->Draw(path, continuous);
    }
    continuous = ok;
 }
 if (path) {
    path->Draw(mode);
 }
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
 SYS_DEBUG_MEMBER(DM_GL_SPHERE);
}

void SphereData::Path::Move(float x, float y)
{
 push(OP_MOVE, Position(x*M_PI, y*(M_PI/2.0f)));
}

void SphereData::Path::Line(float x, float y)
{
 push(OP_LINE, Position(x*M_PI, y*(M_PI/2.0f)));
}

void SphereData::Path::Arc(float x, float y, float r, float a1, float a2)
{
 push(OP_ARC, Position(x*M_PI, y*(M_PI/2.0f)), r, a1, a2);
}

void SphereData::Path::Bezier(float x, float y, float dx, float dy)
{
 push(OP_BEZIER, Position(x*M_PI, y*(M_PI/2.0f)), dx, dy);
}

void SphereData::Path::Close(void)
{
 push(OP_CLOSE);
}

void SphereData::Path::Clear(void)
{
 opCount = 0;
}

void SphereData::Path::Draw(DrawMode mode)
{
 SYS_DEBUG_MEMBER(DM_GL_SPHERE);

 parent.DrawPath(mode, opcodes, opCount);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *     class Glesly::SphereData::Convert3D::Oper:                                        *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void SphereData::Convert3D::Oper::Prepare(void)
{
 SYS_DEBUG_MEMBER(DM_GL_SPHERE);

 if (isUsed()) {
    data[0] /= data[2];     // X
    data[1] /= data[2];     // Y
    if (data[0] < -1.1 || data[0] > 1.1 || data[1] < -1.1 || data[1] > 1.1) {
        flag = ST_UNUSED;
    }
 }
}

void SphereData::Convert3D::Oper::Draw(PaCaLib::PathPtr & path, bool continuous) const
{
 SYS_DEBUG_MEMBER(DM_GL_SPHERE);

 switch (op) {
    case NO_OP:
        // Do nothing here
    break;
    case OP_MOVE:
        path->Move(data[0], data[1]);
    break;
    case OP_LINE:
        if (continuous) {
            path->Line(data[0], data[1]);
        } else {
            path->Move(data[0], data[1]);
        }
    break;
    case OP_ARC:
        path->Arc(data[0], data[1], data[3], data[4], data[5]);
    break;
    case OP_BEZIER:
    break;
    case OP_CLOSE:
        path->Close();
    break;
    default:
        ASSERT(false, "Invalid opcode in Glesly::SphereData::Convert3D::Oper: " << op);
    break;
 }
}

void SphereData::Convert3D::Oper::toStream(std::ostream & os) const
{
 os << "{op=" << op << ", f=" << flag << ", data={";
 for (int i = 0; i < 6; ++i) {
    if (i) {
        os << ",";
    }
    os << " " << data[i];
 }
 os << " }}";
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
