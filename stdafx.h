// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <string>
#include <vector>
#include <algorithm>

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9tex.h>
#include <Commdlg.h>
#include <shlwapi.h>
#include <comdef.h>
//#include <crtdbg.h>

//#include <vld.h>

#ifdef _DEBUG
	#define new DEBUG_CLIENTBLOCK 
   #define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
#else
   #define DEBUG_CLIENTBLOCK
#endif // _DEBUG

// http://stackoverflow.com/questions/1454407/macros-to-disallow-class-copy-and-assignment-google-vs-qt
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);   \
  void operator=(const TypeName&)

