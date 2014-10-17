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
 *     class Glesly::SphereBitmaps:                                                      *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void SphereBitmaps::reset(int size, Glesly::PixelFormat format)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 if (myFormat == Glesly::FORMAT_DEFAULT) {
    myFormat = format;
 }

 if (myFormat == Glesly::FORMAT_DEFAULT) {
    myFormat = Glesly::FORMAT_RGB_565;
 }

 if (!texture_0 || texture_0->GetPixelFormat() != myFormat || texture_0->GetWidth() != size || texture_0->GetHeight() != size) {
    texture_0 = PaCaLib::Target::Create(size, size, myFormat);
 }
 if (!texture_1 || texture_1->GetPixelFormat() != myFormat || texture_1->GetWidth() != size || texture_1->GetHeight() != size) {
    texture_1 = PaCaLib::Target::Create(size, size, myFormat);
 }
 if (!texture_2 || texture_2->GetPixelFormat() != myFormat || texture_2->GetWidth() != size || texture_2->GetHeight() != size) {
    texture_2 = PaCaLib::Target::Create(size, size, myFormat);
 }
 if (!texture_3 || texture_3->GetPixelFormat() != myFormat || texture_3->GetWidth() != size || texture_3->GetHeight() != size) {
    texture_3 = PaCaLib::Target::Create(size, size, myFormat);
 }
 if (!texture_4 || texture_4->GetPixelFormat() != myFormat || texture_4->GetWidth() != size || texture_4->GetHeight() != size) {
    texture_4 = PaCaLib::Target::Create(size, size, myFormat);
 }
 if (!texture_5 || texture_5->GetPixelFormat() != myFormat || texture_5->GetWidth() != size || texture_5->GetHeight() != size) {
    texture_5 = PaCaLib::Target::Create(size, size, myFormat);
 }

 updatePointers();
}

void SphereBitmaps::reset(const char * const * filenames)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 if (!filenames) {
    return;
 }

 int size = 0;

 reset(texture_0, filenames[0], size);
 reset(texture_1, filenames[1], size);
 reset(texture_2, filenames[2], size);
 reset(texture_3, filenames[3], size);
 reset(texture_4, filenames[4], size);
 reset(texture_5, filenames[5], size);

 updatePointers();
}

void SphereBitmaps::updatePointers(void)
{
 textureTargets[0] = &*texture_0;
 textureTargets[1] = &*texture_1;
 textureTargets[2] = &*texture_2;
 textureTargets[3] = &*texture_3;
 textureTargets[4] = &*texture_4;
 textureTargets[5] = &*texture_5;
 pacaTargets[0] = &*texture_0;
 pacaTargets[1] = &*texture_1;
 pacaTargets[2] = &*texture_2;
 pacaTargets[3] = &*texture_3;
 pacaTargets[4] = &*texture_4;
 pacaTargets[5] = &*texture_5;
}

void SphereBitmaps::reset(PaCaLib::TargetPtr & target, const char * name, int & size)
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

 if (!target || target->GetPixelFormat() != myFormat) {
    target = PaCaLib::Target::Create(size, size, myFormat);
 }

 *target = tga;
}

PaCaLib::DrawPtr SphereBitmaps::Draw(void)
{
 return PaCaLib::DrawPtr(new SphereData::Draw(*this));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *     class Glesly::SphereData::Draw:                                                   *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

SphereData::Draw::Draw(SphereBitmaps & parent):
    parent(parent)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 for (int i = 0; i < 6; ++i) {
    draws[i] = parent.GetDraw(i);
 }
}

void SphereData::Draw::Scale(float w, float h)
{
 for (int i = 0; i < 6; ++i) {
    draws[i]->Scale(w, h);
 }
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

float SphereData::Draw::DrawTextInternal(float x, float y, PaCaLib::TextMode mode, const char * text, float size, float offset, float aspect)
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
 push(OP_MOVE, x, y);
}

void SphereData::Path::Line(float x, float y)
{
 push(OP_LINE, x, y);
}

void SphereData::Path::Arc(float xc, float yc, float r, float a1, float a2)
{
 push(OP_ARC, xc, yc, r, a1, a2);
}

void SphereData::Path::Bezier(float x, float y, float dx, float dy)
{
 push(OP_BEZIER, x, y, dx, dy);
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
}

void SphereData::Path::Fill(void)
{
}

void SphereData::Path::push(Opcode op, float d1, float d2, float d3, float d4, float d5)
{
 ASSERT(opCount < MAX_OPERS, "too many opearions on a Glesly::SphereData::Path");

 Oper & o = opcodes[opCount++];
 o.op = op;
 o.data[0] = d1;
 o.data[1] = d2;
 o.data[2] = d3;
 o.data[3] = d4;
 o.data[4] = d5;
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
