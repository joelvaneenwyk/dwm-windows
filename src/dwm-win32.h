#pragma once

/* See LICENSE file for copyright and license details.
 *
 * This is a port of the popular X11 window manager dwm to Microsoft Windows.
 * It was originally started by Marc Andre Tanner <mat at brain-dump dot org>
 *
 * Each child of the root window is called a client. Clients are organized
 * in a global linked client list, the focus history is remembered through
 * a global stack list. Each client contains a bit array to indicate the
 * tags of a client.
 *
 * Keys and tagging rules are organized as arrays and defined in config.h.
 *
 * To understand everything else, start reading WinMain().
 */

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0600

#if _MSC_VER
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "dwmapi.lib")
#endif

#include <compat-5.3.h>
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

#ifndef LUAJIT
#include "../extern/luabitop/bit.c"
#endif

#include <dwmapi.h>
#include <shellapi.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <winuser.h>

#include "mods/client.h"
#include "mods/display.h"
#include "mods/dwm.h"
#include "mods/eventemitter.h"
#include "mods/hotkey.h"

#define NAME L"dwm-win32" /* Used for window name/class */

#define ISVISIBLE(x) ((x)->tags & tagset[seltags])
#define ISFOCUSABLE(x)                                                         \
  (!(x)->isminimized && ISVISIBLE(x) && IsWindowVisible((x)->hwnd))
#define LENGTH(x) (sizeof x / sizeof x[0])
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAXTAGLEN 16
#define WIDTH(x) ((x)->w + 2 * (x)->bw)
#define HEIGHT(x) ((x)->h + 2 * (x)->bw)
#define TAGMASK ((int)((1LL << LENGTH(tags)) - 1))
#define TEXTW(x) (textnw(x, wcslen(x)))

#ifdef NDEBUG
#define debug(...)                                                             \
  do {                                                                         \
  } while (false)
#else
#define debug(...) eprint(false, __VA_ARGS__)
#endif

#define die(...)                                                               \
  if (TRUE) {                                                                  \
    eprint(true, __VA_ARGS__);                                                 \
    eprint(true, L"Win32 Last Error: %d", GetLastError());                     \
    cleanup(NULL);                                                             \
    exit(EXIT_FAILURE);                                                        \
  }

#define EVENT_OBJECT_CLOAKED 0x8017
#define EVENT_OBJECT_UNCLOAKED 0x8018

enum { CurNormal, CurResize, CurMove, CurLast };             /* cursor */
enum { ColBorder, ColFG, ColBG, ColLast };                   /* color */
enum { ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle }; /* clicks */

typedef struct {
  int x, y, w, h;
  unsigned long norm[ColLast];
  unsigned long sel[ColLast];
  HDC hdc;
} DC; /* draw context */

typedef union {
  int i;
  unsigned int ui;
  float f;
  void *v;
} Arg;

typedef struct {
  unsigned int click;
  unsigned int button;
  unsigned int key;
  void (*func)(const Arg *arg);
  const Arg arg;
} Button;

typedef struct Client Client;
struct Client {
  HWND hwnd;
  HWND parent;
  HWND root;
  int x, y, w, h;
  int bw; // XXX: useless?
  unsigned int tags;
  bool isminimized;
  bool isfloating;
  bool isalive;
  bool ignore;
  bool ignoreborder;
  bool border;
  bool wasvisible;
  bool isfixed, isurgent; // XXX: useless?
  bool iscloaked;         // WinStore apps
  Client *next;
  Client *snext;
};

typedef struct {
  unsigned int mod;
  unsigned int key;
  void (*func)(const Arg *);
  const Arg arg;
} Key;

typedef struct {
  const wchar_t *symbol;
  void (*arrange)(void);
} Layout;

typedef struct {
  const wchar_t *class;
  const wchar_t *title;
  const wchar_t *processname;
  unsigned int tags;
  bool isfloating;
  bool ignoreborder;
} Rule;
