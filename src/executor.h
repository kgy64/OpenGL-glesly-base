/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Class to execute a number of OpenGL programs
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_EXECUTOR_H_INCLUDED__
#define __GLESLY_SRC_EXECUTOR_H_INCLUDED__

#include <glesly/program.h>

SYS_DECLARE_MODULE(DM_GLESLY);

namespace Glesly
{
    class Executor
    {
     protected:
        Executor(void);

    }; // class Executor

} // namespace Glesly

#endif /* __GLESLY_SRC_EXECUTOR_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
