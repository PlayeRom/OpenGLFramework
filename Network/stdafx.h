// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include "../OpenGLFramework/define.h"
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <winsock2.h>
#include <vector>
#include <tchar.h>
#include <process.h>
#include "../OpenGLFramework/UnicodeHelperFunc.h"
#include "../OpenGLFramework/WindowData.h"
#include "../Logger/Logger.h"
#include "../Draw/Vector.h"

// TODO: reference additional headers your program requires here
