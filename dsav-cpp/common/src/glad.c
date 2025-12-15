/*
 * GLAD - OpenGL Loader Implementation
 * Generated for OpenGL 3.3 Core Profile
 */

#include <glad/glad.h>
#include <stdlib.h>

/* Function pointer definitions */
PFNGLCULLFACEPROC gladglCullFace = NULL;
PFNGLFRONTFACEPROC gladglFrontFace = NULL;
PFNGLHINTPROC gladglHint = NULL;
PFNGLLINEWIDTHPROC gladglLineWidth = NULL;
PFNGLPOINTSIZEPROC gladglPointSize = NULL;
PFNGLPOLYGONMODEPROC gladglPolygonMode = NULL;
PFNGLSCISSORPROC gladglScissor = NULL;
PFNGLTEXPARAMETERFPROC gladglTexParameterf = NULL;
PFNGLTEXPARAMETERFVPROC gladglTexParameterfv = NULL;
PFNGLTEXPARAMETERIPROC gladglTexParameteri = NULL;
PFNGLTEXPARAMETERIVPROC gladglTexParameteriv = NULL;
PFNGLTEXIMAGE1DPROC gladglTexImage1D = NULL;
PFNGLTEXIMAGE2DPROC gladglTexImage2D = NULL;
PFNGLDRAWBUFFERPROC gladglDrawBuffer = NULL;
PFNGLCLEARPROC gladglClear = NULL;
PFNGLCLEARCOLORPROC gladglClearColor = NULL;
PFNGLCLEARSTENCILPROC gladglClearStencil = NULL;
PFNGLCLEARDEPTHPROC gladglClearDepth = NULL;
PFNGLSTENCILMASKPROC gladglStencilMask = NULL;
PFNGLCOLORMASKPROC gladglColorMask = NULL;
PFNGLDEPTHMASKPROC gladglDepthMask = NULL;
PFNGLDISABLEPROC gladglDisable = NULL;
PFNGLENABLEPROC gladglEnable = NULL;
PFNGLFINISHPROC gladglFinish = NULL;
PFNGLFLUSHPROC gladglFlush = NULL;
PFNGLBLENDFUNCPROC gladglBlendFunc = NULL;
PFNGLLOGICOPPROC gladglLogicOp = NULL;
PFNGLSTENCILFUNCPROC gladglStencilFunc = NULL;
PFNGLSTENCILOPPROC gladglStencilOp = NULL;
PFNGLDEPTHFUNCPROC gladglDepthFunc = NULL;
PFNGLPIXELSTOREFPROC gladglPixelStoref = NULL;
PFNGLPIXELSTOREIPROC gladglPixelStorei = NULL;
PFNGLREADBUFFERPROC gladglReadBuffer = NULL;
PFNGLREADPIXELSPROC gladglReadPixels = NULL;
PFNGLGETBOOLEANVPROC gladglGetBooleanv = NULL;
PFNGLGETDOUBLEVPROC gladglGetDoublev = NULL;
PFNGLGETERRORPROC gladglGetError = NULL;
PFNGLGETFLOATVPROC gladglGetFloatv = NULL;
PFNGLGETINTEGERVPROC gladglGetIntegerv = NULL;
PFNGLGETSTRINGPROC gladglGetString = NULL;
PFNGLGETTEXIMAGEPROC gladglGetTexImage = NULL;
PFNGLGETTEXPARAMETERFVPROC gladglGetTexParameterfv = NULL;
PFNGLGETTEXPARAMETERIVPROC gladglGetTexParameteriv = NULL;
PFNGLGETTEXLEVELPARAMETERFVPROC gladglGetTexLevelParameterfv = NULL;
PFNGLGETTEXLEVELPARAMETERIVPROC gladglGetTexLevelParameteriv = NULL;
PFNGLISENABLEDPROC gladglIsEnabled = NULL;
PFNGLDEPTHRANGEPROC gladglDepthRange = NULL;
PFNGLVIEWPORTPROC gladglViewport = NULL;
PFNGLDRAWARRAYSPROC gladglDrawArrays = NULL;
PFNGLDRAWELEMENTSPROC gladglDrawElements = NULL;
PFNGLGETPOINTERVPROC gladglGetPointerv = NULL;
PFNGLPOLYGONOFFSETPROC gladglPolygonOffset = NULL;
PFNGLCOPYTEXIMAGE1DPROC gladglCopyTexImage1D = NULL;
PFNGLCOPYTEXIMAGE2DPROC gladglCopyTexImage2D = NULL;
PFNGLCOPYTEXSUBIMAGE1DPROC gladglCopyTexSubImage1D = NULL;
PFNGLCOPYTEXSUBIMAGE2DPROC gladglCopyTexSubImage2D = NULL;
PFNGLTEXSUBIMAGE1DPROC gladglTexSubImage1D = NULL;
PFNGLTEXSUBIMAGE2DPROC gladglTexSubImage2D = NULL;
PFNGLBINDTEXTUREPROC gladglBindTexture = NULL;
PFNGLDELETETEXTURESPROC gladglDeleteTextures = NULL;
PFNGLGENTEXTURESPROC gladglGenTextures = NULL;
PFNGLISTEXTUREPROC gladglIsTexture = NULL;
PFNGLGENBUFFERSPROC gladglGenBuffers = NULL;
PFNGLDELETEBUFFERSPROC gladglDeleteBuffers = NULL;
PFNGLBINDBUFFERPROC gladglBindBuffer = NULL;
PFNGLBUFFERDATAPROC gladglBufferData = NULL;
PFNGLBUFFERSUBDATAPROC gladglBufferSubData = NULL;
PFNGLATTACHSHADERPROC gladglAttachShader = NULL;
PFNGLCOMPILESHADERPROC gladglCompileShader = NULL;
PFNGLCREATEPROGRAMPROC gladglCreateProgram = NULL;
PFNGLCREATESHADERPROC gladglCreateShader = NULL;
PFNGLDELETEPROGRAMPROC gladglDeleteProgram = NULL;
PFNGLDELETESHADERPROC gladglDeleteShader = NULL;
PFNGLDETACHSHADERPROC gladglDetachShader = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC gladglEnableVertexAttribArray = NULL;
PFNGLGETPROGRAMIVPROC gladglGetProgramiv = NULL;
PFNGLGETPROGRAMINFOLOGPROC gladglGetProgramInfoLog = NULL;
PFNGLGETSHADERIVPROC gladglGetShaderiv = NULL;
PFNGLGETSHADERINFOLOGPROC gladglGetShaderInfoLog = NULL;
PFNGLLINKPROGRAMPROC gladglLinkProgram = NULL;
PFNGLSHADERSOURCEPROC gladglShaderSource = NULL;
PFNGLUSEPROGRAMPROC gladglUseProgram = NULL;
PFNGLUNIFORM1FPROC gladglUniform1f = NULL;
PFNGLUNIFORM2FPROC gladglUniform2f = NULL;
PFNGLUNIFORM3FPROC gladglUniform3f = NULL;
PFNGLUNIFORM4FPROC gladglUniform4f = NULL;
PFNGLUNIFORM1IPROC gladglUniform1i = NULL;
PFNGLUNIFORMMATRIX4FVPROC gladglUniformMatrix4fv = NULL;
PFNGLVERTEXATTRIBPOINTERPROC gladglVertexAttribPointer = NULL;
PFNGLGETUNIFORMLOCATIONPROC gladglGetUniformLocation = NULL;
PFNGLGETATTRIBLOCATIONPROC gladglGetAttribLocation = NULL;
PFNGLGENVERTEXARRAYSPROC gladglGenVertexArrays = NULL;
PFNGLDELETEVERTEXARRAYSPROC gladglDeleteVertexArrays = NULL;
PFNGLBINDVERTEXARRAYPROC gladglBindVertexArray = NULL;
PFNGLGENFRAMEBUFFERSPROC gladglGenFramebuffers = NULL;
PFNGLDELETEFRAMEBUFFERSPROC gladglDeleteFramebuffers = NULL;
PFNGLBINDFRAMEBUFFERPROC gladglBindFramebuffer = NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC gladglFramebufferTexture2D = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSPROC gladglCheckFramebufferStatus = NULL;
PFNGLGENRENDERBUFFERSPROC gladglGenRenderbuffers = NULL;
PFNGLBINDRENDERBUFFERPROC gladglBindRenderbuffer = NULL;
PFNGLRENDERBUFFERSTORAGEPROC gladglRenderbufferStorage = NULL;
PFNGLFRAMEBUFFERRENDERBUFFERPROC gladglFramebufferRenderbuffer = NULL;

/* Load OpenGL function pointers */
int gladLoadGLLoader(GLADloadproc load) {
    if (load == NULL) {
        return 0;
    }

    /* OpenGL 1.0 */
    gladglCullFace = (PFNGLCULLFACEPROC)load("glCullFace");
    gladglFrontFace = (PFNGLFRONTFACEPROC)load("glFrontFace");
    gladglHint = (PFNGLHINTPROC)load("glHint");
    gladglLineWidth = (PFNGLLINEWIDTHPROC)load("glLineWidth");
    gladglPointSize = (PFNGLPOINTSIZEPROC)load("glPointSize");
    gladglPolygonMode = (PFNGLPOLYGONMODEPROC)load("glPolygonMode");
    gladglScissor = (PFNGLSCISSORPROC)load("glScissor");
    gladglTexParameterf = (PFNGLTEXPARAMETERFPROC)load("glTexParameterf");
    gladglTexParameterfv = (PFNGLTEXPARAMETERFVPROC)load("glTexParameterfv");
    gladglTexParameteri = (PFNGLTEXPARAMETERIPROC)load("glTexParameteri");
    gladglTexParameteriv = (PFNGLTEXPARAMETERIVPROC)load("glTexParameteriv");
    gladglTexImage1D = (PFNGLTEXIMAGE1DPROC)load("glTexImage1D");
    gladglTexImage2D = (PFNGLTEXIMAGE2DPROC)load("glTexImage2D");
    gladglDrawBuffer = (PFNGLDRAWBUFFERPROC)load("glDrawBuffer");
    gladglClear = (PFNGLCLEARPROC)load("glClear");
    gladglClearColor = (PFNGLCLEARCOLORPROC)load("glClearColor");
    gladglClearStencil = (PFNGLCLEARSTENCILPROC)load("glClearStencil");
    gladglClearDepth = (PFNGLCLEARDEPTHPROC)load("glClearDepth");
    gladglStencilMask = (PFNGLSTENCILMASKPROC)load("glStencilMask");
    gladglColorMask = (PFNGLCOLORMASKPROC)load("glColorMask");
    gladglDepthMask = (PFNGLDEPTHMASKPROC)load("glDepthMask");
    gladglDisable = (PFNGLDISABLEPROC)load("glDisable");
    gladglEnable = (PFNGLENABLEPROC)load("glEnable");
    gladglFinish = (PFNGLFINISHPROC)load("glFinish");
    gladglFlush = (PFNGLFLUSHPROC)load("glFlush");
    gladglBlendFunc = (PFNGLBLENDFUNCPROC)load("glBlendFunc");
    gladglLogicOp = (PFNGLLOGICOPPROC)load("glLogicOp");
    gladglStencilFunc = (PFNGLSTENCILFUNCPROC)load("glStencilFunc");
    gladglStencilOp = (PFNGLSTENCILOPPROC)load("glStencilOp");
    gladglDepthFunc = (PFNGLDEPTHFUNCPROC)load("glDepthFunc");
    gladglPixelStoref = (PFNGLPIXELSTOREFPROC)load("glPixelStoref");
    gladglPixelStorei = (PFNGLPIXELSTOREIPROC)load("glPixelStorei");
    gladglReadBuffer = (PFNGLREADBUFFERPROC)load("glReadBuffer");
    gladglReadPixels = (PFNGLREADPIXELSPROC)load("glReadPixels");
    gladglGetBooleanv = (PFNGLGETBOOLEANVPROC)load("glGetBooleanv");
    gladglGetDoublev = (PFNGLGETDOUBLEVPROC)load("glGetDoublev");
    gladglGetError = (PFNGLGETERRORPROC)load("glGetError");
    gladglGetFloatv = (PFNGLGETFLOATVPROC)load("glGetFloatv");
    gladglGetIntegerv = (PFNGLGETINTEGERVPROC)load("glGetIntegerv");
    gladglGetString = (PFNGLGETSTRINGPROC)load("glGetString");
    gladglGetTexImage = (PFNGLGETTEXIMAGEPROC)load("glGetTexImage");
    gladglGetTexParameterfv = (PFNGLGETTEXPARAMETERFVPROC)load("glGetTexParameterfv");
    gladglGetTexParameteriv = (PFNGLGETTEXPARAMETERIVPROC)load("glGetTexParameteriv");
    gladglGetTexLevelParameterfv = (PFNGLGETTEXLEVELPARAMETERFVPROC)load("glGetTexLevelParameterfv");
    gladglGetTexLevelParameteriv = (PFNGLGETTEXLEVELPARAMETERIVPROC)load("glGetTexLevelParameteriv");
    gladglIsEnabled = (PFNGLISENABLEDPROC)load("glIsEnabled");
    gladglDepthRange = (PFNGLDEPTHRANGEPROC)load("glDepthRange");
    gladglViewport = (PFNGLVIEWPORTPROC)load("glViewport");

    /* OpenGL 1.1 */
    gladglDrawArrays = (PFNGLDRAWARRAYSPROC)load("glDrawArrays");
    gladglDrawElements = (PFNGLDRAWELEMENTSPROC)load("glDrawElements");
    gladglGetPointerv = (PFNGLGETPOINTERVPROC)load("glGetPointerv");
    gladglPolygonOffset = (PFNGLPOLYGONOFFSETPROC)load("glPolygonOffset");
    gladglCopyTexImage1D = (PFNGLCOPYTEXIMAGE1DPROC)load("glCopyTexImage1D");
    gladglCopyTexImage2D = (PFNGLCOPYTEXIMAGE2DPROC)load("glCopyTexImage2D");
    gladglCopyTexSubImage1D = (PFNGLCOPYTEXSUBIMAGE1DPROC)load("glCopyTexSubImage1D");
    gladglCopyTexSubImage2D = (PFNGLCOPYTEXSUBIMAGE2DPROC)load("glCopyTexSubImage2D");
    gladglTexSubImage1D = (PFNGLTEXSUBIMAGE1DPROC)load("glTexSubImage1D");
    gladglTexSubImage2D = (PFNGLTEXSUBIMAGE2DPROC)load("glTexSubImage2D");
    gladglBindTexture = (PFNGLBINDTEXTUREPROC)load("glBindTexture");
    gladglDeleteTextures = (PFNGLDELETETEXTURESPROC)load("glDeleteTextures");
    gladglGenTextures = (PFNGLGENTEXTURESPROC)load("glGenTextures");
    gladglIsTexture = (PFNGLISTEXTUREPROC)load("glIsTexture");

    /* OpenGL 1.5 - VBO */
    gladglGenBuffers = (PFNGLGENBUFFERSPROC)load("glGenBuffers");
    gladglDeleteBuffers = (PFNGLDELETEBUFFERSPROC)load("glDeleteBuffers");
    gladglBindBuffer = (PFNGLBINDBUFFERPROC)load("glBindBuffer");
    gladglBufferData = (PFNGLBUFFERDATAPROC)load("glBufferData");
    gladglBufferSubData = (PFNGLBUFFERSUBDATAPROC)load("glBufferSubData");

    /* OpenGL 2.0 - Shaders */
    gladglAttachShader = (PFNGLATTACHSHADERPROC)load("glAttachShader");
    gladglCompileShader = (PFNGLCOMPILESHADERPROC)load("glCompileShader");
    gladglCreateProgram = (PFNGLCREATEPROGRAMPROC)load("glCreateProgram");
    gladglCreateShader = (PFNGLCREATESHADERPROC)load("glCreateShader");
    gladglDeleteProgram = (PFNGLDELETEPROGRAMPROC)load("glDeleteProgram");
    gladglDeleteShader = (PFNGLDELETESHADERPROC)load("glDeleteShader");
    gladglDetachShader = (PFNGLDETACHSHADERPROC)load("glDetachShader");
    gladglEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)load("glEnableVertexAttribArray");
    gladglGetProgramiv = (PFNGLGETPROGRAMIVPROC)load("glGetProgramiv");
    gladglGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)load("glGetProgramInfoLog");
    gladglGetShaderiv = (PFNGLGETSHADERIVPROC)load("glGetShaderiv");
    gladglGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)load("glGetShaderInfoLog");
    gladglLinkProgram = (PFNGLLINKPROGRAMPROC)load("glLinkProgram");
    gladglShaderSource = (PFNGLSHADERSOURCEPROC)load("glShaderSource");
    gladglUseProgram = (PFNGLUSEPROGRAMPROC)load("glUseProgram");
    gladglUniform1f = (PFNGLUNIFORM1FPROC)load("glUniform1f");
    gladglUniform2f = (PFNGLUNIFORM2FPROC)load("glUniform2f");
    gladglUniform3f = (PFNGLUNIFORM3FPROC)load("glUniform3f");
    gladglUniform4f = (PFNGLUNIFORM4FPROC)load("glUniform4f");
    gladglUniform1i = (PFNGLUNIFORM1IPROC)load("glUniform1i");
    gladglUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)load("glUniformMatrix4fv");
    gladglVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)load("glVertexAttribPointer");
    gladglGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)load("glGetUniformLocation");
    gladglGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)load("glGetAttribLocation");

    /* OpenGL 3.0 - VAO */
    gladglGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)load("glGenVertexArrays");
    gladglDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)load("glDeleteVertexArrays");
    gladglBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)load("glBindVertexArray");

    /* OpenGL 3.0 - Framebuffer Objects */
    gladglGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)load("glGenFramebuffers");
    gladglDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)load("glDeleteFramebuffers");
    gladglBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)load("glBindFramebuffer");
    gladglFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)load("glFramebufferTexture2D");
    gladglCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)load("glCheckFramebufferStatus");
    gladglGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)load("glGenRenderbuffers");
    gladglBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)load("glBindRenderbuffer");
    gladglRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)load("glRenderbufferStorage");
    gladglFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)load("glFramebufferRenderbuffer");

    return 1;
}
