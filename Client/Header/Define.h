#ifndef Define_h__
#define Define_h__

#include "Typedef.h"
#include "Function.h"
#include "Macro.h"
#include "Struct.h"
#include "Enum.h"

extern HINSTANCE g_hInst;
extern HWND g_hWnd;

using namespace Client;

constexpr int STATE_IDLE =			0x00000001;
constexpr int STATE_ATTACK =		0x00000002;
constexpr int STATE_INVINCIBLE =	0x00000004;
constexpr int STATE_HIT =			0x00000008;
constexpr int STATE_DEAD =			0x00000010;

#endif