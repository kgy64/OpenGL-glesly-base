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

PaCaLib::DrawPtr SphereSurface::Draw(void)
{
 return PaCaLib::DrawPtr(new SphereData::Draw(*this));
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

float SphereData::Draw::DrawTextInternal(float x, float y, PaCaLib::TextMode mode, const char * text, float size, float offset, float aspect, float rotation)
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

void SphereData::Path::Stroke(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

}

void SphereData::Path::Fill(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
