#pragma once

#define SCF_WIN_API

#ifndef CALLBACK
#define CALLBACK __stdcall
#endif

#ifndef WINAPI
#define WINAPI __stdcall
#endif

#ifndef WINAPIV
#define WINAPIV __cdecl
#endif

#ifndef APIENTRY
#define APIENTRY WINAPI
#endif

#ifndef APIPRIVATE
#define APIPRIVATE __stdcall
#endif

#ifndef PASCAL
#define PASCAL __stdcall
#endif

#ifndef WINGDIAPI
#define WINGDIAPI __declspec(dllimport)
#endif