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

#include <System/Generic.h>

#include <glesly/target2d.h>
#include <File/FileMap.h>
#include <Debug/Debug.h>

#include <Memory/Memory.h>

SYS_DECLARE_MODULE(DM_GLESLY);

namespace Glesly
{
    class ReadTGA: public FILES::FileMap, public Target2D
    {
     public:
        ReadTGA(const char * filename, bool convert_2_rgb = false);
        ReadTGA(ReadTGA & other):
            FILES::FileMap(*this)
        {
            myData.swap(other.myData);
            myRawData = other.myRawData;
            other.myRawData = NULL;
        }
        virtual ~ReadTGA();

        struct pixel_data_rgb565;

        struct pixel_data
        {
            pixel_data & operator=(const pixel_data_rgb565 & other);
            uint8_t b;
            uint8_t g;
            uint8_t r;
        };

        struct pixel_data_rgb565
        {
            inline pixel_data_rgb565 & operator=(const pixel_data & other)
            {
                r = other.r >> (8-5);
                g = other.g >> (8-6);
                b = other.b >> (8-5);
                return *this;
            }

            uint16_t b:5;
            uint16_t g:6;
            uint16_t r:5;
        };

        struct tga_header {
            tga_header(int w, int h, int pixel_depth)
            {
                memset(this, 0, sizeof(*this));
                data_type_code = 2;
                width[0] = w & 0x00ff;
                width[1] = (w >> 8) & 0x00ff;
                height[0] = h & 0x00ff;
                height[1] = (h >> 8) & 0x00ff;
                bits_per_pixel = pixel_depth;
            }

            pixel_data * GetPixelData_888(void)
            {
                return (pixel_data*)image_data;
            }

            pixel_data_rgb565 * GetPixelData_565(void)
            {
                return (pixel_data_rgb565*)image_data;
            }

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

        virtual const void * GetPixelData(void) const override;
        virtual int GetWidth(void) const override;
        virtual int GetHeight(void) const override;

        inline const tga_header & GetHeader(void) const
        {
            return *reinterpret_cast<const tga_header*>(GetData());
        }

        inline const pixel_data * operator[](int index) const
        {
            return reinterpret_cast<const pixel_data *>(GetPixelData()) + index * GetWidth();
        }

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::ReadTGA");

        MEM::scoped_ptr<const pixel_data> myData;

        const pixel_data * myRawData;

        void Normalize(void);

    }; // class ReadTGA

    inline ReadTGA::pixel_data & ReadTGA::pixel_data::operator=(const ReadTGA::pixel_data_rgb565 & other)
    {
        r = other.r << (8-5);
        g = other.g << (8-6);
        b = other.b << (8-5);
        return *this;
    }

} // namespace Glesly

#endif /* __GLESLY_SRC_READ_TGA_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
