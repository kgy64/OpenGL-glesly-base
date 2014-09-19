/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     C++ wrapper for GL/ES - Smart Pointer for Shaders
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_SHADER_PTR_H_INCLUDED__
#define __GLESLY_SRC_SHADER_PTR_H_INCLUDED__

#include <Memory/Memory.h>

namespace Glesly
{
    class Shader;

    typedef MEM::shared_ptr<Shader> ShaderPtr;

} // namespace Glesly

#endif /* __GLESLY_SRC_SHADER_PTR_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
