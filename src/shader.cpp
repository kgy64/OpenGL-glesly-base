/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     C++ wrapper for GL/ES - Shader handler classes
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <glesly/error.h>

#include "shader.h"

using namespace Glesly;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *     class Shader:                                                                     *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Shader::Shader(GLenum type, const char * source, GLint length):
    myType(type)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 myShader = glCreateShader(myType);
 glShaderSource(myShader, 1, &source, &length);
 glCompileShader(myShader);

 GLint result;
 glGetShaderiv(myShader, GL_COMPILE_STATUS, &result);
 if (!result) {
    throw Error("Could not compile shader") << GetLogInfo();
 }

 SYS_DEBUG(DL_INFO1, "Shader " << myShader << " compilation ready, result=" << result);
}

Shader::~Shader()
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 glDeleteShader(GetShaderID());
}

std::string Shader::GetLogInfo(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 int log_length;
 glGetShaderiv(myShader, GL_INFO_LOG_LENGTH, &log_length);
 char log[log_length];
 int real_length;
 glGetShaderInfoLog(myShader, log_length, &real_length, log);
 return log;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *     class Shader::Reader:                                                             *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool Shader::Reader::fromFile = true;

Shader::Reader::Reader(const char * source)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 if (fromFile) {
    myFile.reset(new FILES::FileMap(source));
    mySource = reinterpret_cast<const char *>(myFile->GetData());
    myLength = myFile->GetSize();
 } else {
    mySource = source;
    myLength = strlen(source);
 }
}

Shader::Reader::Reader(const Glesly::ShaderSource & source)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 if (fromFile) {
    myFile.reset(new FILES::FileMap(source.filename));
    mySource = reinterpret_cast<const char *>(myFile->GetData());
    myLength = myFile->GetSize(); // It is not null-terminated
 } else {
    mySource = source.content;
    myLength = strlen(mySource);
 }
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
