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

#include <glesly/read-tga-file.h>

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

 Glesly::ReadTGAFile tga(name);

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
 *  drawing functions are angles (longitude and latitude).<br>
 *  The value range of x and y is -1.0 ... +1.0 due to compatibility reasons. */
PaCaLib::DrawPtr SphereSurface::Draw(void)
{
 return PaCaLib::DrawPtr(new SphereData::Draw(*this));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *     class Glesly::SphereData::Convert3D::Operations:                                  *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

SphereData::Convert3D::Operations::Operations(const_iterator start, const_iterator _end):
    opCount(0)
{
 SYS_DEBUG_MEMBER(DM_GL_SPHERE);

 for ( ; start <= _end; ++start) {
    ASSERT(opCount < MAX_OPERS, "Too many operations");
    opcodes[opCount++] = *start;
 }

 SYS_DEBUG(DL_INFO1, "Created " << *this);
}

SphereData::Convert3D::Operations::Operations(const SphereData::Convert3D::Operations & original, int mode)
{
 SYS_DEBUG_MEMBER(DM_GL_SPHERE);

 static constexpr float Z_LIMIT = 0.06f;

 opCount = original.opCount;

 SYS_DEBUG(DL_INFO2, "Converting " << original << " ...");
 SYS_DEBUG(DL_INFO1, "count=" << opCount);

 OneOp * destination = opcodes;
 const OneOp * source = original.opcodes;
 int i;

 switch (mode) {
    case 0:     // E
        SYS_DEBUG(DL_INFO1, "East");
        for (i = 0; i < opCount; ++i, ++destination, ++source) {
            if (source->op == OneOp::OP_CLOSE) {
                *destination = *source;
                destination->flag = OneOp::ST_USED;
            } else {
                float z = source->x;
                if (z < Z_LIMIT) {
                    destination->Invalidate();
                } else {
                    destination->x = -source->z / z;
                    destination->y = -source->y / z;
                    destination->z =  z;
                    destination->Prepare(mode, *source);
                }
            }
        }
    break;
    case 1:     // W
        SYS_DEBUG(DL_INFO1, "West");
        for (i = 0; i < opCount; ++i, ++destination, ++source) {
            if (source->op == OneOp::OP_CLOSE) {
                *destination = *source;
                destination->flag = OneOp::ST_USED;
            } else {
                float z = -source->x;
                if (z < Z_LIMIT) {
                    destination->Invalidate();
                } else {
                    destination->x =  source->z / z;
                    destination->y = -source->y / z;
                    destination->z =  z;
                    destination->Prepare(mode, *source);
                }
            }
        }
    break;
    case 2:     // S
        SYS_DEBUG(DL_INFO1, "South");
        for (i = 0; i < opCount; ++i, ++destination, ++source) {
            if (source->op == OneOp::OP_CLOSE) {
                *destination = *source;
                destination->flag = OneOp::ST_USED;
            } else {
                float z = -source->y;
                if (z < Z_LIMIT) {
                    destination->Invalidate();
                } else {
                    destination->x =  source->x / z;
                    destination->y = -source->z / z;
                    destination->z =  z;
                    destination->Prepare(mode, *source);
                }
            }
        }
    break;
    case 3:     // N
        SYS_DEBUG(DL_INFO1, "North");
        for (i = 0; i < opCount; ++i, ++destination, ++source) {
            if (source->op == OneOp::OP_CLOSE) {
                *destination = *source;
                destination->flag = OneOp::ST_USED;
            } else {
                float z = source->y;
                if (z < Z_LIMIT) {
                    destination->Invalidate();
                } else {
                    destination->x =  source->x / z;
                    destination->y =  source->z / z;
                    destination->z =  z;
                    destination->Prepare(mode, *source);
                }
            }
        }
    break;
    case 4:     // Front (Africa)
        SYS_DEBUG(DL_INFO1, "Front");
        for (i = 0; i < opCount; ++i, ++destination, ++source) {
            if (source->op == OneOp::OP_CLOSE) {
                *destination = *source;
                destination->flag = OneOp::ST_USED;
            } else {
                float z = source->z;
                if (z < Z_LIMIT) {
                    destination->Invalidate();
                } else {
                    destination->x =  source->x / z;
                    destination->y = -source->y / z;
                    destination->z =  z;
                    destination->Prepare(mode, *source);
                }
            }
        }
    break;
    case 5:     // Back
        SYS_DEBUG(DL_INFO1, "Back");
        for (i = 0; i < opCount; ++i, ++destination, ++source) {
            if (source->op == OneOp::OP_CLOSE) {
                *destination = *source;
                destination->flag = OneOp::ST_USED;
            } else {
                float z = -source->z;
                if (z < Z_LIMIT) {
                    destination->Invalidate();
                } else {
                    destination->x = -source->x / z;
                    destination->y = -source->y / z;
                    destination->z =  z;
                    destination->Prepare(mode, *source);
                }
            }
        }
    break;
    default:
        ASSERT(false, "Invalid mode: " << mode);
    break;
 }

 SYS_DEBUG(DL_INFO2, "The result is " << *this);
}

SphereData::Convert3D::Operations::~Operations()
{
 SYS_DEBUG_MEMBER(DM_GL_SPHERE);
}

void SphereData::Convert3D::Operations::append(const_iterator start, const_iterator _end)
{
 SYS_DEBUG_MEMBER(DM_GL_SPHERE);

 if (size() > 0) {
    if (opcodes[opCount-1].op == OneOp::OP_CLOSE) {
        --opCount;
    }
 }

 if (start <= _end) {
    push(start);
    if (opcodes[opCount-1].op == OneOp::OP_MOVE) {
        opcodes[opCount-1].op = OneOp::OP_LINE;
    }
    for (++start; start <= _end; ++start) {
        push(start);
    }
 }
}

void SphereData::Convert3D::Operations::append(const Operations & other)
{
 SYS_DEBUG_MEMBER(DM_GL_SPHERE);

 if (other.size() == 0) {
    return;
 }

 if (size() > 0) {
    if (opcodes[opCount-1].op == OneOp::OP_CLOSE) {
        --opCount;
    }
 }

 for (const_iterator i = other.cbegin(); i < other.cend(); ++i) {
    push(i);
 }
}

const SphereData::Convert3D::OperPtr * SphereData::Convert3D::Operations::getSubOperations(void) const
{
 SYS_DEBUG_MEMBER(DM_GL_SPHERE);

 if (!sub_opers[0]) {
    enum _State
    {
        SEEK    = 0,
        ACTIVE,
    };

    struct entry
    {
        const_iterator begin;
        const_iterator end;
    };

    int state = SEEK;
    int size = -1;
    bool last_is_close = false;
    entry entries[MAX_LOOPS];
    entry * current = entries + size;
    for (const_iterator i = cbegin(), prev = nullptr; i < cend(); ++i) {
        last_is_close = i->op == OneOp::OP_CLOSE;
        if (!prev) {
            prev = i;
        }
        SYS_DEBUG(DL_INFO2, "Checking entry: " << *i);
        switch (state) {
            case SEEK:
                if (i->isUsed()) {
                    if (i->op == OneOp::OP_CLOSE && !prev->isUsed()) {
                        prev = i;
                    } else {
                        ++size;
                        ++current;
                        ASSERT(size <= MAX_LOOPS, "Too many loops");
                        current->begin = prev;
                        current->end = prev;
                        state = ACTIVE;
                        prev = i->op == OneOp::OP_CLOSE ? nullptr : i;
                        SYS_DEBUG(DL_INFO2, " - Got first entry of #" << size);
                    }
                } else {
                    prev = i;
                }
            break;
            case ACTIVE:
                current->end = i;
                if (!i->isUsed()) {
                    state = SEEK;
                    prev = i;
                    SYS_DEBUG(DL_INFO2, " - Got last entry of #" << size);
                } else if (i->op == OneOp::OP_CLOSE) {
                    state = SEEK;
                    prev = nullptr; // Don't use it in the next loop
                    SYS_DEBUG(DL_INFO2, " - Got closing entry of #" << size);
                }
            break;
        }
    }
    ++size;

    SYS_DEBUG(DL_INFO1, "There are " << size << " loops " << (last_is_close ? "with" : "without") << " close");

    if (size <= 0) {
        SYS_DEBUG(DL_INFO1, " - Creating only an empty object...");
        sub_opers[0] = Operations::Create();    // Create just one, empty list
    } else {
        for (int i = 0; i < size; ++i) {
            if (last_is_close && i == 0 && size > 1) {
                SYS_DEBUG(DL_INFO1, " - Skip the object #0...");
            } else {
                SYS_DEBUG(DL_INFO1, " - Creating object #" << i << "...");
                sub_opers[i] = Operations::Create(entries[i].begin, entries[i].end);
            }
        }
        if (last_is_close && size > 1) {
            SYS_DEBUG(DL_INFO1, " - Appending the first and last lists...");
            --size; // One less elements are needed
            sub_opers[0] = sub_opers[size];
            sub_opers[size].reset();
            sub_opers[0]->append(entries[0].begin, entries[0].end);
            SYS_DEBUG(DL_INFO1, " - Result for the first entry: " << *sub_opers[0]);
        }
    }
 }

 return sub_opers;
}

void SphereData::Convert3D::Operations::toStream(std::ostream & os) const
{
 os << "Ops={" << std::endl;
 const_iterator i = cbegin();
 const_iterator e = cend();
 while (i < e) {
    os << " - " << *i++ << std::endl;
 }
 os << "}";
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
 SYS_DEBUG_MEMBER(DM_GL_SPHERE);
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

void SphereData::Draw::DrawPath(PaCaLib::Path::DrawMode mode, const Operations & ops)
{
 SYS_DEBUG_MEMBER(DM_GL_SPHERE);

 for (int i = 0; i < 6; ++i) {
    Operations op(ops, i);

    for (const SphereData::Convert3D::OperPtr * subops = op.getSubOperations(); *subops; ++subops) {
        DrawPath(mode, i, **subops);
    }
 }
}

void SphereData::Draw::DrawPath(PaCaLib::Path::DrawMode mode, int index, const Operations & ops)
{
 SYS_DEBUG_MEMBER(DM_GL_SPHERE);

 SYS_DEBUG(DL_INFO1, "index=" << index << ", count=" << ops.size());

 switch (mode) {
    case PaCaLib::Path::DRAW_STROKE_AND_FILL:
    case PaCaLib::Path::DRAW_FILL:
        // Draw only the fill here:
        // Note: it must be executed before stroke because it uses different points and different algorythm.
        DrawFillOnly(mode, index, ops);
    break;
    default:
        // Nothing to do here.
    break;
 }

 switch (mode) {
    case PaCaLib::Path::DRAW_STROKE_AND_FILL:
    case PaCaLib::Path::DRAW_STROKE:
    {
        PaCaLib::PathPtr path;
        bool continuous = true;

        for (Operations::const_iterator i = ops.cbegin(); i < ops.cend(); ++i) {
            bool ok = i->isValid();
            SYS_DEBUG(DL_INFO1, "oper=" << *i << ", OK=" << ok);
            if (ok) {
                if (!path) {
                    path = draws[index]->NewPath();
                }
                i->Draw(path, continuous);
            }
            continuous = ok;
        }

        if (path) {
            // The fill has already been drawn (if any), stroke only:
            path->Draw(PaCaLib::Path::DRAW_STROKE);
        }
    }
    break;
    default:
        // Nothing to do here.
    break;
 }
}

bool SphereData::Draw::DrawFillOnly(PaCaLib::Path::DrawMode mode, int index, const Operations & ops)
{
 SYS_DEBUG_MEMBER(DM_GL_SPHERE);

 PaCaLib::PathPtr path;
 for (Operations::const_iterator i = ops.cbegin(); i < ops.cend(); ++i) {
     bool ok = i->isUsed();
     SYS_DEBUG(DL_INFO1, "oper=" << *i << ", OK=" << ok);
     if (ok) {
         if (!path) {
             path = draws[index]->NewPath();
         }
         // TODO!
     }
 }

 return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *     class Glesly::SphereData::Path:                                                   *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

SphereData::Path::Path(SphereData::Draw & parent):
    parent(parent)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
}

void SphereData::Path::Move(float x, float y)
{
 opers.Move(Position(x*M_PI, y*(M_PI/2.0f)));
}

void SphereData::Path::Line(float x, float y)
{
 opers.Line(Position(x*M_PI, y*(M_PI/2.0f)));
}

void SphereData::Path::Arc(float x, float y, float r, float a1, float a2)
{
 opers.Arc(Position(x*M_PI, y*(M_PI/2.0f)), r, a1, a2);
}

void SphereData::Path::Bezier(float x, float y, float dx, float dy)
{
 opers.Bezier(Position(x*M_PI, y*(M_PI/2.0f)), dx, dy);
}

void SphereData::Path::Close(void)
{
 opers.Close();
}

void SphereData::Path::Clear(void)
{
 opers.Clear();
}

void SphereData::Path::Draw(DrawMode mode)
{
 SYS_DEBUG_MEMBER(DM_GL_SPHERE);

 parent.DrawPath(mode, opers);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *     class Glesly::SphereData::Convert3D::OneOp:                                        *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void SphereData::Convert3D::Operations::OneOp::Prepare(int mode, const SphereData::Convert3D::Operations::OneOp & src)
{
 SYS_DEBUG_MEMBER(DM_GL_SPHERE);

 static constexpr float LIMIT = 1.04f;

 op = src.op;

 switch (op) {
    case OP_ARC:
    {
        float limit = u.arc.radius + LIMIT;
        if (x < -limit || x > limit || y < -limit || y > limit) {
            flag = ST_OUT_OF_RANGE;
            return;
        }
        // Fix the arcus radius and angles:
        // TODO!
    }
    break;
    case OP_BEZIER:
        if (x < -LIMIT || x > LIMIT || y < -LIMIT || y > LIMIT) {
            flag = ST_OUT_OF_RANGE;
            return;
        }
        // Fix the bezier directions:
        // TODO!
    break;
    default:
        if (x < -LIMIT || x > LIMIT || y < -LIMIT || y > LIMIT) {
            flag = ST_OUT_OF_RANGE;
            return;
        }
        u = src.u;
    break;
 }

 flag = ST_USED;
}

void SphereData::Convert3D::Operations::OneOp::Draw(PaCaLib::PathPtr & path, bool continuous) const
{
 SYS_DEBUG_MEMBER(DM_GL_SPHERE);

 switch (op) {
    case NO_OP:
        // Do nothing here
    break;
    case OP_MOVE:
        path->Move(x, y);
    break;
    case OP_LINE:
        if (continuous) {
            path->Line(x, y);
        } else {
            path->Move(x, y);
        }
    break;
    case OP_ARC:
        path->Arc(x, y, u.arc.radius, u.arc.start, u.arc.end);
    break;
    case OP_BEZIER:
        path->Bezier(x, y, u.bezier.dx, u.bezier.dy);
    break;
    case OP_CLOSE:
        path->Close();
    break;
    default:
        ASSERT(false, "Invalid opcode: " << op);
    break;
 }
}

void SphereData::Convert3D::Operations::OneOp::toStream(std::ostream & os) const
{
 const char * op_str = "unknown";
 switch (op) {
    case NO_OP:
        op_str = "NO_OP";
    break;
    case OP_MOVE:
        op_str = "OP_MOVE";
    break;
    case OP_LINE:
        op_str = "OP_LINE";
    break;
    case OP_ARC:
        op_str = "OP_ARC";
    break;
    case OP_BEZIER:
        op_str = "OP_BEZIER";
    break;
    case OP_CLOSE:
        op_str = "OP_CLOSE";
    break;
 }
 os << "{op=" << op_str << ", f=" << flag << ", x=" << x << ", y=" << y << ", z=" << z << ", data=" << u << "}";
}

void SphereData::Convert3D::Operations::OneOp::_u::toStream(std::ostream & os) const
{
 os << "{arc={r=" << arc.radius << ", s=" << arc.start << ", e=" << arc.end << "}, bezier={dx=" << bezier.dx << ", dy=" << bezier.dy << "}}";
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
