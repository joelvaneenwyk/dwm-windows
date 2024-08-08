# `dwm-windows`

`dwm-windows` (aka. `dwm-win32`) is a port of the well-known X11 window manager [`dwm`](https://dwm.suckless.org/) to [`Windows 11`](https://en.wikipedia.org/wiki/Windows_11).

It is recommended to run `dwm-win32` as `Administrator` so it will catch all windows including those you ran as `Administrator`.

![dwm-win32 screenshot](./screenshot.png)

## Description

`dwm` is a dynamic window manager for `Windows 11`. It manages windows
in `tiled`, `monocle` and `floating` layouts. Either layout can be applied
dynamically, optimising the environment for the application in use and
the task performed.

- In `tiled` layouts, windows are managed in a master and stacking area. The
master area contains the window which currently needs most attention,
whereas the stacking area contains all other windows.
- In `monocle` layout
all windows are maximised to the screen size.
- In `floating` layout windows
can be resized and moved freely. Dialog windows are always managed
floating, regardless of the layout applied.

All windows are grouped by tags and each window can be tagged with one or more tags. Selecting certain tags displays all windows with these tags.

`dwm` contains a small status bar which displays all available tags, the
layout and the title of the focused window. A floating window is indicated
with an empty square and a maximised floating window is indicated with a
filled square before the windows title. The selected tags are indicated
with a different color. The tags of the focused window are indicated with
a filled square in the top left corner.  The tags which are applied to
one or more windows are indicated with an empty square in the top left
corner.

`dwm` draws a small border around windows to indicate the focus state.

## Usage

## Keyboard

<!-- markdownlint-disable MD033 -->

dwm uses a modifier key by default this is <kbd>ALT</kbd>.

| Keybinding                                                      | Description                                                                      |
|-----------------------------------------------------------------|----------------------------------------------------------------------------------|
| <kbd>MOD</kbd> + <kbd>Control</kbd> + <kbd>b</kbd>              | Toggles bar on and off.                                                          |
| <kbd>MOD</kbd> + <kbd>e</kbd>                                   | Toggles windows explorer and taskbar on and off.                                 |
| <kbd>MOD</kbd> + <kbd>t</kbd>                                   | Sets tiled layout.                                                               |
| <kbd>MOD</kbd> + <kbd>f</kbd>                                   | Sets floating layout.                                                            |
| <kbd>MOD</kbd> + <kbd>m</kbd>                                   | Sets monocle layout.                                                             |
| <kbd>MOD</kbd> + <kbd>Control</kbd> + <kbd>Space</kbd>          | Toggles between current and previous layout.                                     |
| <kbd>MOD</kbd> + <kbd>j</kbd>                                   | Focus next window.                                                               |
| <kbd>MOD</kbd> + <kbd>k</kbd>                                   | Focus previous window.                                                           |
| <kbd>MOD</kbd> + <kbd>h</kbd>                                   | Decrease master area size.                                                       |
| <kbd>MOD</kbd> + <kbd>l</kbd>                                   | Increase master area size.                                                       |
| <kbd>MOD</kbd> + <kbd>Control</kbd> + <kbd>Return</kbd>         | Zooms/cycles focused window to/from master area (tiled layouts only).            |
| <kbd>MOD</kbd> + <kbd>Shift</kbd> + <kbd>c</kbd>                | Close focused window.                                                            |
| <kbd>MOD</kbd> + <kbd>Shift</kbd> + <kbd>a</kbd>                | Force rearrange.                                                                 |
| <kbd>MOD</kbd> + <kbd>Shift</kbd> + <kbd>Space</kbd>            | Toggle focused window between tiled and floating state.                          |
| <kbd>MOD</kbd> + <kbd>n</kbd>                                   | Toggles border of currently focused window.                                      |
| <kbd>MOD</kbd> + <kbd>i</kbd>                                   | Display classname of currently focused window, useful for writing tagging rules. |
| <kbd>MOD</kbd> + <kbd>Tab</kbd>                                 | Toggles to the previously selected tags.                                         |
| <kbd>MOD</kbd> + <kbd>Shift</kbd> + <kbd>[1..n]</kbd>           | Apply nth tag to focused window.                                                 |
| <kbd>MOD</kbd> + <kbd>Shift</kbd> + <kbd>0</kbd>                | Apply all tags to focused window.                                                |
| <kbd>MOD</kbd> + <kbd>Control</kbd> + Shift + <kbd>[1..n]</kbd> | Add/remove nth tag to/from focused window.                                       |
| <kbd>MOD</kbd> + <kbd>Shift</kbd> + <kbd>j</kbd>                | Move stack +1                                                                    |
| <kbd>MOD</kbd> + <kbd>Shift</kbd> + <kbd>k</kbd>                | Move stack -1                                                                    |
| <kbd>MOD</kbd> + <kbd>[0..n]</kbd>                              | View all windows with nth tag.                                                   |
| <kbd>MOD</kbd> + <kbd>0</kbd>                                   | View all windows with any tag.                                                   |
| <kbd>MOD</kbd> + <kbd>Control</kbd> + <kbd>[1..n]</kbd>         | Add/remove all windows with nth tag to/from the view.                            |
| <kbd>MOD</kbd> + <kbd>Control</kbd> + <kbd>q</kbd>              | Quit dwm.                                                                        |
| <kbd>MOD</kbd> + <kbd>Control</kbd> + <kbd>l</kbd>              | Log all window state.                                                            |

## Mouse

- <kbd>Left Button</kbd> click on a tag label to display all windows with that tag, click
      on the layout label toggles between tiled and floating layout.
- <kbd>Right Button</kbd> click on a tag label adds/removes all windows with that tag to/from
      the view.
- <kbd>Alt</kbd> + <kbd>Left Button</kbd> click on a tag label applies that tag to the focused window.
- <kbd>Alt</kbd> + <kbd>Right Button</kbd> click on a tag label adds/removes that tag to/from the focused window.

## How it works

A `ShellHook` is registered which is notified upon window creation and
destruction, however it is important to know that this only works for
unowned top level windows. This means we will not get notified when child
windows are created/destroyed. Therefore we scan the currently active top
level window upon activation to collect all associated child windows.
This information is for example used to tag all windows and not just
the top-level one when tag changes occur.

This is all kind of messy and we might miss some child windows in certain
situations. A better approach would probably be to introduce a `CBTProc`
function and register it with `SetWindowsHookEx(WH_CBT, ...)` with this we
would get notified by all and every windows including toolbars etc.
which we would have to filter out.

Unfortunately the `SetWindowsHookEx` thingy seems to require a separate
dll because it will be loaded into each process address space.

## Compiling

To compile, `dwm-win32` requires:

- [Visual Studio C/C++ Build Tools and Windows SDK](https://visualstudio.microsoft.com/downloads/) (You can use the Visual Studio Installer to download only headless c/c++ build tools and windows sdk without the full IDE)
- [zig](https://ziglang.org/)

Source code for `dwm-win32` is written in C and uses `zig cc` to compile C to native code.
You can install the Zig compiler by using [scoop](https://scoop.sh) as `scoop install zig`.

```cmd
call build.cmd
```

- Remove `-DNDEBUG` and `-O2 -s` from `build.cmd` to build debug version.
- Update version in `build.cmd` before each release.

## TODO

- Support `luajit`
- Show/hide child windows upon tag switch, in theory this should already work but in practice we need to tweak `ismanageable()` so that it recognises child windows but doesn't generate false positives.
- Fullscreen windows, mstsc for example doesn't resize properly when maximized.
- Screensaver?
- System dialogs from desktop window
- Urgent flag?
- Window border isn't yet perfect
- Status text via stdin or a separate tool
- Crash handler which makes all windows visible restores borders etc
- Use `BeginDeferWindowPos`, `DeferWindowPos` and `EndDeferWindowPos`
- Optimize for speed
- Code cleanups all over the place
- Multi-head support?
- Introduce a `CBTProc` function and register it with `SetWindowsHookEx(WH_CBT, ...)` to handle window events instead of the current mechanism in `WndProc` which is based on the `shellhookid` and `WH_SHELL` because this only works for top-level windows. See also the "How it works" section.
