/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Class to execute one OpenGL program with Renders
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_READ_PTR_H_INCLUDED__
#define __GLESLY_SRC_READ_PTR_H_INCLUDED__

#include <boost/shared_ptr.hpp>

namespace Glesly
{
    class Render;

    typedef boost::shared_ptr<Render> RenderPtr;

} // namespace Glesly

#endif /* __GLESLY_SRC_READ_PTR_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
