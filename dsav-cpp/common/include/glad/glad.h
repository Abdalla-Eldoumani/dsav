/*
 * GLAD - OpenGL Loader
 * Generated for OpenGL 3.3 Core Profile
 *
 * This is a simplified version containing essential functions for DSAV.
 * For a complete GLAD implementation, generate from: https://glad.dav1d.de/
 */

#ifndef __glad_h_
#define __glad_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <KHR/khrplatform.h>

/* OpenGL Type Definitions */
typedef void GLvoid;
typedef unsigned int GLenum;
typedef khronos_float_t GLfloat;
typedef int GLint;
typedef int GLsizei;
typedef unsigned int GLbitfield;
typedef double GLdouble;
typedef unsigned int GLuint;
typedef unsigned char GLboolean;
typedef khronos_uint8_t GLubyte;
typedef khronos_float_t GLclampf;
typedef double GLclampd;
typedef khronos_ssize_t GLsizeiptr;
typedef khronos_intptr_t GLintptr;
typedef char GLchar;
typedef khronos_int16_t GLshort;
typedef khronos_int8_t GLbyte;
typedef khronos_uint16_t GLushort;
typedef struct __GLsync *GLsync;
typedef khronos_uint64_t GLuint64;
typedef khronos_int64_t GLint64;

/* OpenGL Constants */
#define GL_VERSION_1_0 1
#define GL_VERSION_1_1 1
#define GL_VERSION_1_2 1
#define GL_VERSION_1_3 1
#define GL_VERSION_1_4 1
#define GL_VERSION_1_5 1
#define GL_VERSION_2_0 1
#define GL_VERSION_2_1 1
#define GL_VERSION_3_0 1
#define GL_VERSION_3_1 1
#define GL_VERSION_3_2 1
#define GL_VERSION_3_3 1

/* GLADloadproc type */
typedef void* (*GLADloadproc)(const char *name);

/* OpenGL Enum Values - Essential Subset */
#define GL_FALSE 0
#define GL_TRUE 1

/* String name constants */
#define GL_VENDOR 0x1F00
#define GL_RENDERER 0x1F01
#define GL_VERSION 0x1F02
#define GL_EXTENSIONS 0x1F03
#define GL_SHADING_LANGUAGE_VERSION 0x8B8C

/* Feature constants */
#define GL_MULTISAMPLE 0x809D
#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_STENCIL_BUFFER_BIT 0x00000400
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_TRIANGLES 0x0004
#define GL_TRIANGLE_STRIP 0x0005
#define GL_LINES 0x0001
#define GL_LINE_STRIP 0x0003
#define GL_POINTS 0x0000
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_BLEND 0x0BE2
#define GL_CULL_FACE 0x0B44
#define GL_DEPTH_TEST 0x0B71
#define GL_SCISSOR_TEST 0x0C11
#define GL_TEXTURE_2D 0x0DE1
#define GL_UNSIGNED_BYTE 0x1401
#define GL_UNSIGNED_INT 0x1405
#define GL_FLOAT 0x1406
#define GL_RGBA 0x1908
#define GL_RGB 0x1907
#define GL_LINEAR 0x2601
#define GL_NEAREST 0x2600
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_REPEAT 0x2901
#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_STATIC_DRAW 0x88E4
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_INFO_LOG_LENGTH 0x8B84
#define GL_FRAMEBUFFER 0x8D40
#define GL_FRAMEBUFFER_COMPLETE 0x8CD5
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_DEPTH_ATTACHMENT 0x8D00
#define GL_RENDERBUFFER 0x8D41
#define GL_DEPTH_COMPONENT 0x1902
#define GL_DEPTH_COMPONENT16 0x81A5
#define GL_DEPTH_COMPONENT24 0x81A6
#define GL_DEPTH_COMPONENT32 0x81A7
#define GL_POLYGON_MODE 0x0B40
#define GL_FILL 0x1B02
#define GL_LINE 0x1B01
#define GL_FRONT_AND_BACK 0x0408

/* OpenGL Function Pointers */
typedef void (*GLDEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);

/* OpenGL 1.0 */
typedef void (KHRONOS_APIENTRY *PFNGLCULLFACEPROC)(GLenum mode);
typedef void (KHRONOS_APIENTRY *PFNGLFRONTFACEPROC)(GLenum mode);
typedef void (KHRONOS_APIENTRY *PFNGLHINTPROC)(GLenum target, GLenum mode);
typedef void (KHRONOS_APIENTRY *PFNGLLINEWIDTHPROC)(GLfloat width);
typedef void (KHRONOS_APIENTRY *PFNGLPOINTSIZEPROC)(GLfloat size);
typedef void (KHRONOS_APIENTRY *PFNGLPOLYGONMODEPROC)(GLenum face, GLenum mode);
typedef void (KHRONOS_APIENTRY *PFNGLSCISSORPROC)(GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (KHRONOS_APIENTRY *PFNGLTEXPARAMETERFPROC)(GLenum target, GLenum pname, GLfloat param);
typedef void (KHRONOS_APIENTRY *PFNGLTEXPARAMETERFVPROC)(GLenum target, GLenum pname, const GLfloat *params);
typedef void (KHRONOS_APIENTRY *PFNGLTEXPARAMETERIPROC)(GLenum target, GLenum pname, GLint param);
typedef void (KHRONOS_APIENTRY *PFNGLTEXPARAMETERIVPROC)(GLenum target, GLenum pname, const GLint *params);
typedef void (KHRONOS_APIENTRY *PFNGLTEXIMAGE1DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void (KHRONOS_APIENTRY *PFNGLTEXIMAGE2DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void (KHRONOS_APIENTRY *PFNGLDRAWBUFFERPROC)(GLenum buf);
typedef void (KHRONOS_APIENTRY *PFNGLCLEARPROC)(GLbitfield mask);
typedef void (KHRONOS_APIENTRY *PFNGLCLEARCOLORPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (KHRONOS_APIENTRY *PFNGLCLEARSTENCILPROC)(GLint s);
typedef void (KHRONOS_APIENTRY *PFNGLCLEARDEPTHPROC)(GLdouble depth);
typedef void (KHRONOS_APIENTRY *PFNGLSTENCILMASKPROC)(GLuint mask);
typedef void (KHRONOS_APIENTRY *PFNGLCOLORMASKPROC)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
typedef void (KHRONOS_APIENTRY *PFNGLDEPTHMASKPROC)(GLboolean flag);
typedef void (KHRONOS_APIENTRY *PFNGLDISABLEPROC)(GLenum cap);
typedef void (KHRONOS_APIENTRY *PFNGLENABLEPROC)(GLenum cap);
typedef void (KHRONOS_APIENTRY *PFNGLFINISHPROC)(void);
typedef void (KHRONOS_APIENTRY *PFNGLFLUSHPROC)(void);
typedef void (KHRONOS_APIENTRY *PFNGLBLENDFUNCPROC)(GLenum sfactor, GLenum dfactor);
typedef void (KHRONOS_APIENTRY *PFNGLLOGICOPPROC)(GLenum opcode);
typedef void (KHRONOS_APIENTRY *PFNGLSTENCILFUNCPROC)(GLenum func, GLint ref, GLuint mask);
typedef void (KHRONOS_APIENTRY *PFNGLSTENCILOPPROC)(GLenum fail, GLenum zfail, GLenum zpass);
typedef void (KHRONOS_APIENTRY *PFNGLDEPTHFUNCPROC)(GLenum func);
typedef void (KHRONOS_APIENTRY *PFNGLPIXELSTOREFPROC)(GLenum pname, GLfloat param);
typedef void (KHRONOS_APIENTRY *PFNGLPIXELSTOREIPROC)(GLenum pname, GLint param);
typedef void (KHRONOS_APIENTRY *PFNGLREADBUFFERPROC)(GLenum src);
typedef void (KHRONOS_APIENTRY *PFNGLREADPIXELSPROC)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels);
typedef void (KHRONOS_APIENTRY *PFNGLGETBOOLEANVPROC)(GLenum pname, GLboolean *data);
typedef void (KHRONOS_APIENTRY *PFNGLGETDOUBLEVPROC)(GLenum pname, GLdouble *data);
typedef GLenum (KHRONOS_APIENTRY *PFNGLGETERRORPROC)(void);
typedef void (KHRONOS_APIENTRY *PFNGLGETFLOATVPROC)(GLenum pname, GLfloat *data);
typedef void (KHRONOS_APIENTRY *PFNGLGETINTEGERVPROC)(GLenum pname, GLint *data);
typedef const GLubyte *(KHRONOS_APIENTRY *PFNGLGETSTRINGPROC)(GLenum name);
typedef void (KHRONOS_APIENTRY *PFNGLGETTEXIMAGEPROC)(GLenum target, GLint level, GLenum format, GLenum type, void *pixels);
typedef void (KHRONOS_APIENTRY *PFNGLGETTEXPARAMETERFVPROC)(GLenum target, GLenum pname, GLfloat *params);
typedef void (KHRONOS_APIENTRY *PFNGLGETTEXPARAMETERIVPROC)(GLenum target, GLenum pname, GLint *params);
typedef void (KHRONOS_APIENTRY *PFNGLGETTEXLEVELPARAMETERFVPROC)(GLenum target, GLint level, GLenum pname, GLfloat *params);
typedef void (KHRONOS_APIENTRY *PFNGLGETTEXLEVELPARAMETERIVPROC)(GLenum target, GLint level, GLenum pname, GLint *params);
typedef GLboolean (KHRONOS_APIENTRY *PFNGLISENABLEDPROC)(GLenum cap);
typedef void (KHRONOS_APIENTRY *PFNGLDEPTHRANGEPROC)(GLdouble n, GLdouble f);
typedef void (KHRONOS_APIENTRY *PFNGLVIEWPORTPROC)(GLint x, GLint y, GLsizei width, GLsizei height);

/* OpenGL 1.1 */
typedef void (KHRONOS_APIENTRY *PFNGLDRAWARRAYSPROC)(GLenum mode, GLint first, GLsizei count);
typedef void (KHRONOS_APIENTRY *PFNGLDRAWELEMENTSPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices);
typedef void (KHRONOS_APIENTRY *PFNGLGETPOINTERVPROC)(GLenum pname, void **params);
typedef void (KHRONOS_APIENTRY *PFNGLPOLYGONOFFSETPROC)(GLfloat factor, GLfloat units);
typedef void (KHRONOS_APIENTRY *PFNGLCOPYTEXIMAGE1DPROC)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
typedef void (KHRONOS_APIENTRY *PFNGLCOPYTEXIMAGE2DPROC)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
typedef void (KHRONOS_APIENTRY *PFNGLCOPYTEXSUBIMAGE1DPROC)(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
typedef void (KHRONOS_APIENTRY *PFNGLCOPYTEXSUBIMAGE2DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (KHRONOS_APIENTRY *PFNGLTEXSUBIMAGE1DPROC)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels);
typedef void (KHRONOS_APIENTRY *PFNGLTEXSUBIMAGE2DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
typedef void (KHRONOS_APIENTRY *PFNGLBINDTEXTUREPROC)(GLenum target, GLuint texture);
typedef void (KHRONOS_APIENTRY *PFNGLDELETETEXTURESPROC)(GLsizei n, const GLuint *textures);
typedef void (KHRONOS_APIENTRY *PFNGLGENTEXTURESPROC)(GLsizei n, GLuint *textures);
typedef GLboolean (KHRONOS_APIENTRY *PFNGLISTEXTUREPROC)(GLuint texture);

/* OpenGL 1.5 - VBO Support */
typedef void (KHRONOS_APIENTRY *PFNGLGENBUFFERSPROC)(GLsizei n, GLuint *buffers);
typedef void (KHRONOS_APIENTRY *PFNGLDELETEBUFFERSPROC)(GLsizei n, const GLuint *buffers);
typedef void (KHRONOS_APIENTRY *PFNGLBINDBUFFERPROC)(GLenum target, GLuint buffer);
typedef void (KHRONOS_APIENTRY *PFNGLBUFFERDATAPROC)(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
typedef void (KHRONOS_APIENTRY *PFNGLBUFFERSUBDATAPROC)(GLenum target, GLintptr offset, GLsizeiptr size, const void *data);

/* OpenGL 2.0 - Shaders */
typedef void (KHRONOS_APIENTRY *PFNGLATTACHSHADERPROC)(GLuint program, GLuint shader);
typedef void (KHRONOS_APIENTRY *PFNGLCOMPILESHADERPROC)(GLuint shader);
typedef GLuint (KHRONOS_APIENTRY *PFNGLCREATEPROGRAMPROC)(void);
typedef GLuint (KHRONOS_APIENTRY *PFNGLCREATESHADERPROC)(GLenum type);
typedef void (KHRONOS_APIENTRY *PFNGLDELETEPROGRAMPROC)(GLuint program);
typedef void (KHRONOS_APIENTRY *PFNGLDELETESHADERPROC)(GLuint shader);
typedef void (KHRONOS_APIENTRY *PFNGLDETACHSHADERPROC)(GLuint program, GLuint shader);
typedef void (KHRONOS_APIENTRY *PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint index);
typedef void (KHRONOS_APIENTRY *PFNGLGETPROGRAMIVPROC)(GLuint program, GLenum pname, GLint *params);
typedef void (KHRONOS_APIENTRY *PFNGLGETPROGRAMINFOLOGPROC)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (KHRONOS_APIENTRY *PFNGLGETSHADERIVPROC)(GLuint shader, GLenum pname, GLint *params);
typedef void (KHRONOS_APIENTRY *PFNGLGETSHADERINFOLOGPROC)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (KHRONOS_APIENTRY *PFNGLLINKPROGRAMPROC)(GLuint program);
typedef void (KHRONOS_APIENTRY *PFNGLSHADERSOURCEPROC)(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
typedef void (KHRONOS_APIENTRY *PFNGLUSEPROGRAMPROC)(GLuint program);
typedef void (KHRONOS_APIENTRY *PFNGLUNIFORM1FPROC)(GLint location, GLfloat v0);
typedef void (KHRONOS_APIENTRY *PFNGLUNIFORM2FPROC)(GLint location, GLfloat v0, GLfloat v1);
typedef void (KHRONOS_APIENTRY *PFNGLUNIFORM3FPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (KHRONOS_APIENTRY *PFNGLUNIFORM4FPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (KHRONOS_APIENTRY *PFNGLUNIFORM1IPROC)(GLint location, GLint v0);
typedef void (KHRONOS_APIENTRY *PFNGLUNIFORMMATRIX4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (KHRONOS_APIENTRY *PFNGLVERTEXATTRIBPOINTERPROC)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
typedef GLint (KHRONOS_APIENTRY *PFNGLGETUNIFORMLOCATIONPROC)(GLuint program, const GLchar *name);
typedef GLint (KHRONOS_APIENTRY *PFNGLGETATTRIBLOCATIONPROC)(GLuint program, const GLchar *name);

/* OpenGL 3.0 - VAO */
typedef void (KHRONOS_APIENTRY *PFNGLGENVERTEXARRAYSPROC)(GLsizei n, GLuint *arrays);
typedef void (KHRONOS_APIENTRY *PFNGLDELETEVERTEXARRAYSPROC)(GLsizei n, const GLuint *arrays);
typedef void (KHRONOS_APIENTRY *PFNGLBINDVERTEXARRAYPROC)(GLuint array);

/* OpenGL 3.0 - Framebuffer Objects */
typedef void (KHRONOS_APIENTRY *PFNGLGENFRAMEBUFFERSPROC)(GLsizei n, GLuint *framebuffers);
typedef void (KHRONOS_APIENTRY *PFNGLDELETEFRAMEBUFFERSPROC)(GLsizei n, const GLuint *framebuffers);
typedef void (KHRONOS_APIENTRY *PFNGLBINDFRAMEBUFFERPROC)(GLenum target, GLuint framebuffer);
typedef void (KHRONOS_APIENTRY *PFNGLFRAMEBUFFERTEXTURE2DPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef GLenum (KHRONOS_APIENTRY *PFNGLCHECKFRAMEBUFFERSTATUSPROC)(GLenum target);
typedef void (KHRONOS_APIENTRY *PFNGLGENRENDERBUFFERSPROC)(GLsizei n, GLuint *renderbuffers);
typedef void (KHRONOS_APIENTRY *PFNGLBINDRENDERBUFFERPROC)(GLenum target, GLuint renderbuffer);
typedef void (KHRONOS_APIENTRY *PFNGLRENDERBUFFERSTORAGEPROC)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (KHRONOS_APIENTRY *PFNGLFRAMEBUFFERRENDERBUFFERPROC)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);

/* Function pointers */
extern PFNGLCULLFACEPROC gladglCullFace;
#define glCullFace gladglCullFace
extern PFNGLFRONTFACEPROC gladglFrontFace;
#define glFrontFace gladglFrontFace
extern PFNGLHINTPROC gladglHint;
#define glHint gladglHint
extern PFNGLLINEWIDTHPROC gladglLineWidth;
#define glLineWidth gladglLineWidth
extern PFNGLPOINTSIZEPROC gladglPointSize;
#define glPointSize gladglPointSize
extern PFNGLPOLYGONMODEPROC gladglPolygonMode;
#define glPolygonMode gladglPolygonMode
extern PFNGLSCISSORPROC gladglScissor;
#define glScissor gladglScissor
extern PFNGLTEXPARAMETERFPROC gladglTexParameterf;
#define glTexParameterf gladglTexParameterf
extern PFNGLTEXPARAMETERFVPROC gladglTexParameterfv;
#define glTexParameterfv gladglTexParameterfv
extern PFNGLTEXPARAMETERIPROC gladglTexParameteri;
#define glTexParameteri gladglTexParameteri
extern PFNGLTEXPARAMETERIVPROC gladglTexParameteriv;
#define glTexParameteriv gladglTexParameteriv
extern PFNGLTEXIMAGE1DPROC gladglTexImage1D;
#define glTexImage1D gladglTexImage1D
extern PFNGLTEXIMAGE2DPROC gladglTexImage2D;
#define glTexImage2D gladglTexImage2D
extern PFNGLDRAWBUFFERPROC gladglDrawBuffer;
#define glDrawBuffer gladglDrawBuffer
extern PFNGLCLEARPROC gladglClear;
#define glClear gladglClear
extern PFNGLCLEARCOLORPROC gladglClearColor;
#define glClearColor gladglClearColor
extern PFNGLCLEARSTENCILPROC gladglClearStencil;
#define glClearStencil gladglClearStencil
extern PFNGLCLEARDEPTHPROC gladglClearDepth;
#define glClearDepth gladglClearDepth
extern PFNGLSTENCILMASKPROC gladglStencilMask;
#define glStencilMask gladglStencilMask
extern PFNGLCOLORMASKPROC gladglColorMask;
#define glColorMask gladglColorMask
extern PFNGLDEPTHMASKPROC gladglDepthMask;
#define glDepthMask gladglDepthMask
extern PFNGLDISABLEPROC gladglDisable;
#define glDisable gladglDisable
extern PFNGLENABLEPROC gladglEnable;
#define glEnable gladglEnable
extern PFNGLFINISHPROC gladglFinish;
#define glFinish gladglFinish
extern PFNGLFLUSHPROC gladglFlush;
#define glFlush gladglFlush
extern PFNGLBLENDFUNCPROC gladglBlendFunc;
#define glBlendFunc gladglBlendFunc
extern PFNGLLOGICOPPROC gladglLogicOp;
#define glLogicOp gladglLogicOp
extern PFNGLSTENCILFUNCPROC gladglStencilFunc;
#define glStencilFunc gladglStencilFunc
extern PFNGLSTENCILOPPROC gladglStencilOp;
#define glStencilOp gladglStencilOp
extern PFNGLDEPTHFUNCPROC gladglDepthFunc;
#define glDepthFunc gladglDepthFunc
extern PFNGLPIXELSTOREFPROC gladglPixelStoref;
#define glPixelStoref gladglPixelStoref
extern PFNGLPIXELSTOREIPROC gladglPixelStorei;
#define glPixelStorei gladglPixelStorei
extern PFNGLREADBUFFERPROC gladglReadBuffer;
#define glReadBuffer gladglReadBuffer
extern PFNGLREADPIXELSPROC gladglReadPixels;
#define glReadPixels gladglReadPixels
extern PFNGLGETBOOLEANVPROC gladglGetBooleanv;
#define glGetBooleanv gladglGetBooleanv
extern PFNGLGETDOUBLEVPROC gladglGetDoublev;
#define glGetDoublev gladglGetDoublev
extern PFNGLGETERRORPROC gladglGetError;
#define glGetError gladglGetError
extern PFNGLGETFLOATVPROC gladglGetFloatv;
#define glGetFloatv gladglGetFloatv
extern PFNGLGETINTEGERVPROC gladglGetIntegerv;
#define glGetIntegerv gladglGetIntegerv
extern PFNGLGETSTRINGPROC gladglGetString;
#define glGetString gladglGetString
extern PFNGLGETTEXIMAGEPROC gladglGetTexImage;
#define glGetTexImage gladglGetTexImage
extern PFNGLGETTEXPARAMETERFVPROC gladglGetTexParameterfv;
#define glGetTexParameterfv gladglGetTexParameterfv
extern PFNGLGETTEXPARAMETERIVPROC gladglGetTexParameteriv;
#define glGetTexParameteriv gladglGetTexParameteriv
extern PFNGLGETTEXLEVELPARAMETERFVPROC gladglGetTexLevelParameterfv;
#define glGetTexLevelParameterfv gladglGetTexLevelParameterfv
extern PFNGLGETTEXLEVELPARAMETERIVPROC gladglGetTexLevelParameteriv;
#define glGetTexLevelParameteriv gladglGetTexLevelParameteriv
extern PFNGLISENABLEDPROC gladglIsEnabled;
#define glIsEnabled gladglIsEnabled
extern PFNGLDEPTHRANGEPROC gladglDepthRange;
#define glDepthRange gladglDepthRange
extern PFNGLVIEWPORTPROC gladglViewport;
#define glViewport gladglViewport
extern PFNGLDRAWARRAYSPROC gladglDrawArrays;
#define glDrawArrays gladglDrawArrays
extern PFNGLDRAWELEMENTSPROC gladglDrawElements;
#define glDrawElements gladglDrawElements
extern PFNGLGETPOINTERVPROC gladglGetPointerv;
#define glGetPointerv gladglGetPointerv
extern PFNGLPOLYGONOFFSETPROC gladglPolygonOffset;
#define glPolygonOffset gladglPolygonOffset
extern PFNGLCOPYTEXIMAGE1DPROC gladglCopyTexImage1D;
#define glCopyTexImage1D gladglCopyTexImage1D
extern PFNGLCOPYTEXIMAGE2DPROC gladglCopyTexImage2D;
#define glCopyTexImage2D gladglCopyTexImage2D
extern PFNGLCOPYTEXSUBIMAGE1DPROC gladglCopyTexSubImage1D;
#define glCopyTexSubImage1D gladglCopyTexSubImage1D
extern PFNGLCOPYTEXSUBIMAGE2DPROC gladglCopyTexSubImage2D;
#define glCopyTexSubImage2D gladglCopyTexSubImage2D
extern PFNGLTEXSUBIMAGE1DPROC gladglTexSubImage1D;
#define glTexSubImage1D gladglTexSubImage1D
extern PFNGLTEXSUBIMAGE2DPROC gladglTexSubImage2D;
#define glTexSubImage2D gladglTexSubImage2D
extern PFNGLBINDTEXTUREPROC gladglBindTexture;
#define glBindTexture gladglBindTexture
extern PFNGLDELETETEXTURESPROC gladglDeleteTextures;
#define glDeleteTextures gladglDeleteTextures
extern PFNGLGENTEXTURESPROC gladglGenTextures;
#define glGenTextures gladglGenTextures
extern PFNGLISTEXTUREPROC gladglIsTexture;
#define glIsTexture gladglIsTexture
extern PFNGLGENBUFFERSPROC gladglGenBuffers;
#define glGenBuffers gladglGenBuffers
extern PFNGLDELETEBUFFERSPROC gladglDeleteBuffers;
#define glDeleteBuffers gladglDeleteBuffers
extern PFNGLBINDBUFFERPROC gladglBindBuffer;
#define glBindBuffer gladglBindBuffer
extern PFNGLBUFFERDATAPROC gladglBufferData;
#define glBufferData gladglBufferData
extern PFNGLBUFFERSUBDATAPROC gladglBufferSubData;
#define glBufferSubData gladglBufferSubData
extern PFNGLATTACHSHADERPROC gladglAttachShader;
#define glAttachShader gladglAttachShader
extern PFNGLCOMPILESHADERPROC gladglCompileShader;
#define glCompileShader gladglCompileShader
extern PFNGLCREATEPROGRAMPROC gladglCreateProgram;
#define glCreateProgram gladglCreateProgram
extern PFNGLCREATESHADERPROC gladglCreateShader;
#define glCreateShader gladglCreateShader
extern PFNGLDELETEPROGRAMPROC gladglDeleteProgram;
#define glDeleteProgram gladglDeleteProgram
extern PFNGLDELETESHADERPROC gladglDeleteShader;
#define glDeleteShader gladglDeleteShader
extern PFNGLDETACHSHADERPROC gladglDetachShader;
#define glDetachShader gladglDetachShader
extern PFNGLENABLEVERTEXATTRIBARRAYPROC gladglEnableVertexAttribArray;
#define glEnableVertexAttribArray gladglEnableVertexAttribArray
extern PFNGLGETPROGRAMIVPROC gladglGetProgramiv;
#define glGetProgramiv gladglGetProgramiv
extern PFNGLGETPROGRAMINFOLOGPROC gladglGetProgramInfoLog;
#define glGetProgramInfoLog gladglGetProgramInfoLog
extern PFNGLGETSHADERIVPROC gladglGetShaderiv;
#define glGetShaderiv gladglGetShaderiv
extern PFNGLGETSHADERINFOLOGPROC gladglGetShaderInfoLog;
#define glGetShaderInfoLog gladglGetShaderInfoLog
extern PFNGLLINKPROGRAMPROC gladglLinkProgram;
#define glLinkProgram gladglLinkProgram
extern PFNGLSHADERSOURCEPROC gladglShaderSource;
#define glShaderSource gladglShaderSource
extern PFNGLUSEPROGRAMPROC gladglUseProgram;
#define glUseProgram gladglUseProgram
extern PFNGLUNIFORM1FPROC gladglUniform1f;
#define glUniform1f gladglUniform1f
extern PFNGLUNIFORM2FPROC gladglUniform2f;
#define glUniform2f gladglUniform2f
extern PFNGLUNIFORM3FPROC gladglUniform3f;
#define glUniform3f gladglUniform3f
extern PFNGLUNIFORM4FPROC gladglUniform4f;
#define glUniform4f gladglUniform4f
extern PFNGLUNIFORM1IPROC gladglUniform1i;
#define glUniform1i gladglUniform1i
extern PFNGLUNIFORMMATRIX4FVPROC gladglUniformMatrix4fv;
#define glUniformMatrix4fv gladglUniformMatrix4fv
extern PFNGLVERTEXATTRIBPOINTERPROC gladglVertexAttribPointer;
#define glVertexAttribPointer gladglVertexAttribPointer
extern PFNGLGETUNIFORMLOCATIONPROC gladglGetUniformLocation;
#define glGetUniformLocation gladglGetUniformLocation
extern PFNGLGETATTRIBLOCATIONPROC gladglGetAttribLocation;
#define glGetAttribLocation gladglGetAttribLocation
extern PFNGLGENVERTEXARRAYSPROC gladglGenVertexArrays;
#define glGenVertexArrays gladglGenVertexArrays
extern PFNGLDELETEVERTEXARRAYSPROC gladglDeleteVertexArrays;
#define glDeleteVertexArrays gladglDeleteVertexArrays
extern PFNGLBINDVERTEXARRAYPROC gladglBindVertexArray;
#define glBindVertexArray gladglBindVertexArray
extern PFNGLGENFRAMEBUFFERSPROC gladglGenFramebuffers;
#define glGenFramebuffers gladglGenFramebuffers
extern PFNGLDELETEFRAMEBUFFERSPROC gladglDeleteFramebuffers;
#define glDeleteFramebuffers gladglDeleteFramebuffers
extern PFNGLBINDFRAMEBUFFERPROC gladglBindFramebuffer;
#define glBindFramebuffer gladglBindFramebuffer
extern PFNGLFRAMEBUFFERTEXTURE2DPROC gladglFramebufferTexture2D;
#define glFramebufferTexture2D gladglFramebufferTexture2D
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC gladglCheckFramebufferStatus;
#define glCheckFramebufferStatus gladglCheckFramebufferStatus
extern PFNGLGENRENDERBUFFERSPROC gladglGenRenderbuffers;
#define glGenRenderbuffers gladglGenRenderbuffers
extern PFNGLBINDRENDERBUFFERPROC gladglBindRenderbuffer;
#define glBindRenderbuffer gladglBindRenderbuffer
extern PFNGLRENDERBUFFERSTORAGEPROC gladglRenderbufferStorage;
#define glRenderbufferStorage gladglRenderbufferStorage
extern PFNGLFRAMEBUFFERRENDERBUFFERPROC gladglFramebufferRenderbuffer;
#define glFramebufferRenderbuffer gladglFramebufferRenderbuffer

/* GLAD initialization function */
int gladLoadGLLoader(GLADloadproc load);

#ifdef __cplusplus
}
#endif

#endif /* __glad_h_ */
