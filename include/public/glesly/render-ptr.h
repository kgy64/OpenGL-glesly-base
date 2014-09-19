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

#include <Memory/Memory.h>

namespace Glesly
{
    class MenuRender;
    class Render;
    class CubeRender;
    class SurfaceRender;

    typedef MEM::shared_ptr<MenuRender> MenuRenderPtr;
    typedef MEM::shared_ptr<Render> RenderPtr;
    typedef MEM::shared_ptr<CubeRender> CubeRenderPtr;
    typedef MEM::shared_ptr<SurfaceRender> SurfaceRenderPtr;

} // namespace Glesly

#endif /* __GLESLY_SRC_READ_PTR_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
