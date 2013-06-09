/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     TGA file handling
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "read-tga.h"

using namespace Glesly;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *       class ReadTGA:                                                                  *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ReadTGA::ReadTGA(const char * filename):
    FILES::FileMap(filename)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 Normalize();
}

ReadTGA::~ReadTGA()
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
}

int ReadTGA::GetWidth(void) const
{
 const tga_header & hdr(GetHeader());
 return (int)hdr.width[0] | ((int)hdr.width[1] << 8);
}

int ReadTGA::GetHeight(void) const
{
 const tga_header & hdr(GetHeader());
 return (int)hdr.height[0] | ((int)hdr.height[1] << 8);
}

const void * ReadTGA::GetPixelData(void) const
{
 return myData.get();
}

void ReadTGA::Normalize(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 const tga_header & hdr(GetHeader());
 ASSERT(GetSize() >= sizeof(hdr), "tga file header truncated");
 ASSERT(hdr.data_type_code == 2, "Not an uncompressed RGB tga file");
 ASSERT(hdr.bits_per_pixel == 24, "Not a 24-bit TGA file");
 unsigned int header_length = hdr.id_length + ((int)hdr.color_map_length[0] | ((int)hdr.color_map_length[1] << 8));
 ASSERT(GetSize() >= sizeof(hdr)+header_length, "tga file truncated");
 const pixel_data * source = reinterpret_cast<const pixel_data *>(hdr.image_data + header_length);

 unsigned length = GetWidth() * GetHeight();
 pixel_data * target = reinterpret_cast<pixel_data *>(malloc(length * sizeof(pixel_data)));
 myData.reset(target);

 for (unsigned i = 0; i < length; ++i, ++target, ++source) {
    target->r = source->b;
    target->g = source->g;
    target->b = source->r;
 }
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
