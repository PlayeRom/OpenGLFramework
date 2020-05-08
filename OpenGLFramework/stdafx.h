// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include "define.h"
#include <windows.h>
#include <winsock2.h>
#include <crtdbg.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glaux.h>
#include "include/wglext.h"		//WGL extensions
#include "include/glext.h"		//GL extensions
#include <vector>
#include <tchar.h>
#include <math.h>
#include <mmsystem.h>
#include <time.h>
#include <process.h>
#include "../Draw/Vector.h"
#include "../MatrixOperations/MatrixOperations.h"
#include "../Lighting/Lighting.h"
#include "../SpeedControl/SpeedControl.h"
#include "../SpeedControl/Profiler.h"
#include "../Multilanguage/Multilanguage.h"
#include "../Draw/StencilShadow.h"
#include "../Draw/Billboard.h"
#include "../Draw/TextureLoader.h"
#include "../Draw/Particles.h"
#include "../Draw/Brightness.h"

#if _USE_SOUND_OPENAL_
#include "../Sound/OpenALManager.h"
#else
#include "../Sound/SoundFMOD.h"
#endif

#include "../Logger/Logger.h"
#include "RMessageBox.h"
#include "UnicodeHelperFunc.h"
#include "../Network/NetworkManager.h"
#include "SingletonBase.h"