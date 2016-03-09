/////////////////////////////////////////////////////////////////////////////
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		      // Exclude rarely-used stuff from Windows headers
#endif

// Disable warning C4100: 'identifier' : unreferenced formal parameter
#pragma warning( disable:4100 )

// Rhino plug-ins must always use the release MFC that Rhino uses.
// Rhino plug-ins that require debugging information need to be built
// with RHINO_DEBUG_PLUGIN defined.
#if defined(RHINO_DEBUG_PLUGIN) && defined(_DEBUG)
// V4 PseudoDebug plug-ins should define RHINO_DEBUG_PLUGIN, 
// but not define _DEBUG in the .vcproj file.
#error Do not define _DEBUG - use RHINO_DEBUG_PLUGIN instead
#endif

// Rhino SDK Preamble
#include "C:\Program Files (x86)\Rhino 5.0 x64 SDK\Inc\RhinoSdkStdafxPreamble.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#include <afxwin.h>           // MFC core and standard components
#include <afxext.h>           // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>           // MFC OLE classes
#include <afxodlgs.h>         // MFC OLE dialog classes
#include <afxdisp.h>          // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			      // MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			      // MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		      // MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			      // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//
// TODO: include additional commonly used header files here
//

#if defined(_M_X64) && defined(WIN32) && defined(WIN64)
//   The afxwin.h includes afx.h, which includes afxver_.h, 
//   which unconditionally defines WIN32  This is a bug.
//   Note, all Windows builds (32 and 64 bit) define _WIN32.
//   Only 64-bit builds define _WIN64. Never define/undefine
//   _WIN32 or _WIN64.  Only define EXACTLY one of WIN32 or WIN64.
//   See the MSDN "Predefined Macros" help file for details.
#undef WIN32
#endif




// Rhino Plug-in
#include "C:\Program Files (x86)\Rhino 5.0 x64 SDK\Inc\RhinoSdk.h"

#include <Win32_GLAppUtil.h>

// Render Development Kit.
#include "C:\Program Files (x86)\Rhino 5.0 x64 SDK\Inc\RhRdkHeaders.h"

#if defined(RHINO_DEBUG_PLUGIN)
// Now that all the system headers are read, we can
// safely define _DEBUG so the developers can test
// for _DEBUG in their code.
#define _DEBUG
#endif


//#include "../../OculusRoomTiny_Advanced/Common/Win32_GLAppUtil.h"
#include "Kernel/OVR_System.h"

// Rhino Plug-in Linking Pragmas
#include "C:\Program Files (x86)\Rhino 5.0 x64 SDK\Inc\rhinoSdkPlugInLinkingPragmas.h"

// OVR
//#include <OVR_CAPI.h>
#include <OVR_CAPI_GL.h>
#include <OVR_Math.h>

#include <SOIL.h>

#include "rhinoEscapeKey.h"

// I think a new approach to setting up the rendering is needed. Check the examples, but this file below
//#include "../../OculusSDK/Samples/OculusRoomTiny_Advanced/Common//Win32_GLAppUtil.h"
// is a list of headers for things which SHOULD already exist in Rhino SDK / OpenGL or in OVR_CAPI_GL
// check out McNeel, OVR tutorials. And finish that table.

// try adding new .h file - 4 u 4 graphics - and add from GlAppUtil.h to extend rhino's opengl 
// where you cannot find something suitable in the existing implementation

// rhinosdkdisplayengine_ogl.h at 485, 584, 559

//#ifndef __gl_h_
//#define __gl_h_
//#include "LibOVRKernel\Src\GL\CAPI_GLE_GL.h"
//#endif