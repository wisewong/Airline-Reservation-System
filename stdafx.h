// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS
#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件: 
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string.h>
#include <stdio.h>

// 包括 SDKDDKVer.h 将定义可用的最高版本的 Windows 平台。
#include <SDKDDKVer.h>

// TODO:  在此处引用程序需要的其他头文件
#include<commctrl.h>
#include "resource.h"

#pragma comment(lib, "comctl32.lib")