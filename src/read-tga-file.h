/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Ducktor Navi - my Navigation Project
 * Purpose:     
 * Author:      Copyright (c) 2014 - Gyorgy Kovesdi <kgy@teledigit.eu>
 * License:     GPLv2
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __SRC_READTGAFILE_H_INCLUDED__
#define __SRC_READTGAFILE_H_INCLUDED__

#include <glesly/read-tga.h>
#include <glesly/read-file-path.h>

namespace Glesly
{
    class ReadTGAFile: public Glesly::ReadFilePath, public Glesly::ReadTGA
    {
     public:
        ReadTGAFile(const char * filename, bool swap_rgb_bgr = false):
            ReadTGA(ConvertFileName(filename), swap_rgb_bgr)
        {
            SYS_DEBUG_MEMBER(DM_FILE);
        }

        ReadTGAFile(Glesly::ReadTGAFile && other):
            ReadTGA(std::move(other))
        {
            SYS_DEBUG_MEMBER(DM_FILE);
            pathname = other.pathname;
        }

        virtual ~ReadTGAFile()
        {
            SYS_DEBUG_MEMBER(DM_FILE);
        }

     private:
        ReadTGAFile(Glesly::ReadTGAFile & other);

        SYS_DEFINE_CLASS_NAME("Glesly::ReadTGAFile");

    }; // class Glesly::ReadTGAFile

}; // namespace Glesly

#endif /* __SRC_READTGAFILE_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
