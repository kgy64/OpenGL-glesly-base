/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     C++ wrapper for GL/ES - Error Handler Classes
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_ERROR_H_INCLUDED__
#define __GLESLY_SRC_ERROR_H_INCLUDED__

#include <Exceptions/Exceptions.h>

namespace Glesly
{
    /// Generic error within Glesly
    typedef EX::Error Error;
} // namespace Glesly

#endif /* __GLESLY_SRC_ERROR_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
