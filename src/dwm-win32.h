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

#ifndef DWM_WIN32_H__
#define DWM_WIN32_H__

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0600

#if _MSC_VER
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "dwmapi.lib")
#endif

#include <lua.h>
#include <lualib.h>

//
#include <lauxlib.h>

///
#include <compat-5.3.h>

#include <dwmapi.h>
#include <shellapi.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <winuser.h>

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

/* function declarations */
extern void applyrules(Client* c);
extern void arrange(void);
extern void attach(Client* c);
extern void attachstack(Client* c);
extern void cleanup(lua_State* L);
extern void clearurgent(Client* c);
extern void detach(Client* c);
extern void detachstack(Client* c);
extern void drawbar(void);
extern void drawsquare(bool filled, bool empty, bool invert, unsigned long col[ColLast]);
extern void drawtext(const wchar_t* text, unsigned long col[ColLast], bool invert);
extern void drawborder(Client* c, COLORREF color);
extern void eprint(bool premortem, const wchar_t* errstr, ...);
extern void focus(Client* c);
extern void focusstack(const Arg* arg);
extern void movestack(const Arg *arg);
extern void forcearrange(const Arg *arg);
extern Client *getclient(HWND hwnd);
extern LPWSTR getclientclassname(HWND hwnd);
extern LPWSTR getclienttitle(HWND hwnd);
extern LPWSTR getclientprocessname(HWND hwnd);
extern HWND getroot(HWND hwnd);
extern void grabkeys(HWND hwnd);
extern void killclient(const Arg* arg);
extern Client* manage(HWND hwnd);
extern void monocle(void);
extern Client *nextchild(Client *p, Client *c);
extern Client *nexttiled(Client *c);
extern void quit(const Arg* arg);
extern void resize(Client *c, int x, int y, int w, int h);
extern void restack(void);
extern BOOL CALLBACK scan(HWND hwnd, LPARAM lParam);
extern void setborder(Client* c, bool border);
extern void setvisibility(HWND hwnd, bool visibility);
extern void setlayout(const Arg* arg);
extern void setmfact(const Arg* arg);
extern void setup(lua_State* L, HINSTANCE hInstance);
extern void setupbar(HINSTANCE hInstance);
extern void showclientinfo(const Arg* arg);
extern void showhide(Client* c);
extern void spawn(const Arg* arg);
extern void tag(const Arg* arg);
extern int textnw(const wchar_t* text, unsigned int len);
extern void tile(void);
extern void togglebar(const Arg* arg);
extern void toggleborder(const Arg* arg);
extern void toggleexplorer(const Arg* arg);
extern void togglefloating(const Arg* arg);
extern void toggletag(const Arg* arg);
extern void toggleview(const Arg* arg);
extern void writelog(const Arg* arg);
extern void unmanage(Client* c);
extern void updatebar(void);
extern void updategeom(void);
extern void view(const Arg* arg);
extern void zoom(const Arg* arg);
extern bool iscloaked(HWND hwnd);

/* overrides */
void bstack(void);
void gaplessgrid(void);
void grid(void);
void fibonacci(int s);
void spiral(void);
void dwindle(void);

extern Client* clients;
/* bar geometry x, y, height and layout symbol width */
static int bx, by, bh, blw;
/* window area geometry x, y, width, height, bar excluded */
extern int wx, wy, ww, wh;

/* factor of master area size [0.05..0.95] */
extern float mfact;

typedef struct lua_State lua_State;
extern void luaL_openlibs(lua_State *L);
extern int luaopen_bit(lua_State *L);

#endif /* DWM_WIN32_H__ */
