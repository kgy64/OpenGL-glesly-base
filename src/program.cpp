/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Class to represent one OpenGL program
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <glesly/shader.h>
#include <glesly/error.h>

#include "program.h"

using namespace Glesly;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *     class Program:                                                                    *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Program::Program(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 myProgram = glCreateProgram();

 SYS_DEBUG(DL_INFO1, "KGY Created program: " << myProgram);
}

Program::~Program(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
}

void Program::Attach(ShaderPtr shader)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 myShaders.push_back(shader);
 glAttachShader(GetProgramID(), shader->GetShaderID());
}

void Program::Link(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 SYS_DEBUG(DL_INFO3, "KGY: linking program " << GetProgramID());

 glLinkProgram(GetProgramID());

 GLint result;
 glGetProgramiv(GetProgramID(), GL_LINK_STATUS, &result);
 if (!result) {
    throw Error("Could not link shaders") << GetLogInfo();
 }
}

std::string Program::GetLogInfo(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 int log_length = -1;
 glGetProgramiv(GetProgramID(), GL_INFO_LOG_LENGTH, &log_length);

 if (log_length <= 0) {
    return "(No log info available)";
 }

 char log[log_length];
 int real_length;
 glGetProgramInfoLog(GetProgramID(), log_length, &real_length, log);
 return log;
}

GLint Program::GetUniformLocationSafe(const char * name) const
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 int result = GetUniformLocation(name);
 if (result < 0) {
    throw Error("glGetUniformLocation() failed for variable name") << name;
 }
 return result;
}

GLint Program::GetAttribLocationSafe(const char * name) const
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 int result = GetAttribLocation(name);
 if (result < 0) {
    throw Error("glGetAttribLocation() failed for variable name") << name;
 }
 return result;
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
