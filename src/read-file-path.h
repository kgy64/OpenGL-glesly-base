/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Ducktor Navi - my Navigation Project
 * Purpose:     
 * Author:      Copyright (c) 2014 - Gyorgy Kovesdi <kgy@teledigit.eu>
 * License:     GPLv2
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __SRC_READ_FILE_PATH_H_INCLUDED__
#define __SRC_READ_FILE_PATH_H_INCLUDED__

#include <Config/MainConfig.h>
#include <Debug/Debug.h>

#include <string>

namespace Glesly
{
    class ReadFilePath
    {
     protected:
        inline std::string GetRootDir(void)
        {
            SYS_DEBUG_MEMBER(DM_FILE);
            return MainConfig::GetRootDir();
        }

        inline const char * ConvertFileName(const char * name)
        {
            SYS_DEBUG_MEMBER(DM_FILE);
            pathname = MainConfig::FullPathOf(name);
            SYS_DEBUG(DL_INFO1, "File name '" << name << "' is converted to path '" << pathname << "'");
            return pathname.c_str();
        }

        std::string pathname;

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::ReadFilePath");

    }; // class Glesly::ReadFilePath

} // namespace Glesly

#endif /* __SRC_READ_FILE_PATH_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
