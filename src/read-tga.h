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
        ReadTGA(const char * filename, bool swap_rgb_bgr = false);
        ReadTGA(ReadTGA & other):
            FILES::FileMap(*this)
        {
            myData = other.myData;
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
            uint8_t a;
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

            int8_t  id_length;
            int8_t  color_map_type;
            int8_t  data_type_code;
            uint8_t color_map_origin[2];
            uint8_t color_map_length[2];
            int8_t  color_map_depth;
            uint8_t x_origin[2];
            uint8_t y_origin[2];
            uint8_t width[2];
            uint8_t height[2];
            int8_t  bits_per_pixel;
            int8_t  image_descriptor;
            int8_t  image_data[0];
        };

        virtual const void * GetPixelData(void) const override;
        virtual Glesly::PixelFormat GetPixelFormat(void) const override;
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

        MEM::shared_ptr<const pixel_data> myData;

        int8_t myPixelSize;

        const pixel_data * myRawData;

        void Swap_RGB_BGR(void);

    }; // class ReadTGA

    inline ReadTGA::pixel_data & ReadTGA::pixel_data::operator=(const ReadTGA::pixel_data_rgb565 & other)
    {
        r = other.r << (8-5);
        g = other.g << (8-6);
        b = other.b << (8-5);
        a = 255;
        return *this;
    }

} // namespace Glesly

#endif /* __GLESLY_SRC_READ_TGA_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
