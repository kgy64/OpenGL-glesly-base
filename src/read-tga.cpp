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
}

ReadTGA::~ReadTGA()
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
}

const ReadTGA::pixel_data & ReadTGA::GetPixelData(void) const
{
 const tga_header & hdr(GetHeader());
 ASSERT(GetSize() >= sizeof(hdr), "tga file header truncated");
 ASSERT(hdr.data_type_code == 2, "Not an uncompressed RGB tga file");
 ASSERT(hdr.bits_per_pixel == 24, "Not a 24-bit TGA file");
 unsigned int header_length = hdr.id_length + ((int)hdr.color_map_length[0] | ((int)hdr.color_map_length[1] << 8));
 ASSERT(GetSize() >= sizeof(hdr)+header_length, "tga file truncated");
 union {
    const pixel_data * pd;
    const char * cd;
 } pixel_pointer;
 pixel_pointer.cd = hdr.image_data + header_length;
 return *pixel_pointer.pd;
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
