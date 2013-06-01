/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     TGA file handling
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __GLESLY_SRC_READ_TGA_H_INCLUDED__
#define __GLESLY_SRC_READ_TGA_H_INCLUDED__

#include <File/FileMap.h>
#include <Debug/Debug.h>

SYS_DECLARE_MODULE(DM_GLESLY);

namespace Glesly
{
    class ReadTGA: public FILES::FileMap
    {
     public:
        ReadTGA(const char * filename);
        virtual ~ReadTGA();

        struct pixel_data
        {
            unsigned char r;
            unsigned char g;
            unsigned char b;
        };

        struct tga_header {
            char  id_length;
            char  color_map_type;
            char  data_type_code;
            unsigned char  color_map_origin[2];
            unsigned char  color_map_length[2];
            char  color_map_depth;
            unsigned char  x_origin[2];
            unsigned char  y_origin[2];
            unsigned char  width[2];
            unsigned char  height[2];
            char bits_per_pixel;
            char image_descriptor;
            char image_data[0];
        };

        const pixel_data & GetPixelData(void) const;

        inline const tga_header & GetHeader(void) const
        {
            return *reinterpret_cast<const tga_header*>(GetData());
        }

        inline int GetWidth(void) const
        {
            const tga_header & hdr(GetHeader());
            return (int)hdr.width[0] | ((int)hdr.width[1] << 8);
        }

        inline int GetHeight(void) const
        {
            const tga_header & hdr(GetHeader());
            return (int)hdr.height[0] | ((int)hdr.height[1] << 8);
        }

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::ReadTGA");

    }; // class ReadTGA

} // namespace Glesly

#endif /* __GLESLY_SRC_READ_TGA_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
