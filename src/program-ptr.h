/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     C++ wrapper for GL/ES - Smart Pointer for Programs
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_PROGRAM_PTR_H_INCLUDED__
#define __GLESLY_SRC_PROGRAM_PTR_H_INCLUDED__

#include <boost/shared_ptr.hpp>

namespace Glesly
{
    class Program;

    typedef boost::shared_ptr<Program> ProgramPtr;

} // namespace Glesly

#endif /* __GLESLY_SRC_PROGRAM_PTR_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
