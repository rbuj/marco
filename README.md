Marco [![Build Status](https://travis-ci.org/mate-desktop/marco.svg?branch=master)](https://travis-ci.org/mate-desktop/marco)
===

MATE Marco is a fork of GNOME Metacity.

COMPILING MARCO
===

You need GTK+ 3.22.  For startup notification to work you need
libstartup-notification at
http://www.freedesktop.org/software/startup-notification/.

SHRINKING MARCO
===

Not that marco is huge, but a substantial amount of code is in
preferences handling, in static strings that aren't essential, and in
the theme engine.

You can strip about 70K from the marco binary by compiling with
options such as:

 ```
 --disable-sm
 --disable-verbose-mode
 --disable-startup-notification
 ```

However the result is no good for desktop use, all prefs have to be
hardcoded in the binary, for example. If you wanted to make a really
small marco, here's some additional stuff you might consider
implementing:

 - add `--disable-themes`, which would replace theme.c and theme-parser.c
   with a hardcoded implementation of the interface in theme.h,
   should save about 80K. This should be fairly easy.

 - add `--disable-gtk`, which would implement the interface in ui.h
   without using GTK. This one is easier than you think because the
   main part of the window manager doesn't use GTK directly, but is
   still fairly hard to do.  You would probably have to give up some
   of the features, such as window menus, as menus are pretty complex
   to implement well.  So time may be better spent adding a GTK
   configure script feature to build GTK with only a small core set of
   functionality.

MARCO FEATURES
===

 - Boring window manager for the adult in you. Many window managers
   are like Marshmallow Froot Loops; Marco is like Cheerios.

 - Uses GTK+ 3.0 for drawing window frames. This means colors, fonts,
   etc. come from GTK+ theme.

 - Does not expose the concept of "window manager" to the user.  Some
   of the features in the MATE control panel and other parts of the
   desktop happen to be implemented in marco, such as changing your
   window border theme, or changing your window navigation shortcuts,
   but the user doesn't need to know this.

 - Includes only the window manager; does not try to be a desktop
   environment. The pager, configuration, etc. are all separate and
   modular. The "libwnck" library (which I also wrote) is available
   for writing marco extensions, pagers, and so on. (But libwnck
   isn't marco specific, or MATE-dependent; it requires only GTK,
   and should work with KWin, fvwm2, and other EWMH-compliant WMs.)

 - Has a simple theme system and a couple of extra themes come with it.
   Change themes via gsettings:
     ```
     gsettings set org.mate.Marco.general theme Menta
     gsettings set org.mate.Marco.general theme BlackMATE
     gsettings set org.mate.Marco.general theme TraditionalOk
     ```

   See theme-format.txt for docs on the theme format. Use
   marco-theme-viewer to preview themes.

 - Change number of workspaces via gsettings:
     ```
     gsettings set org.mate.Marco.general num-workspaces 5
     ```

   Can also change workspaces from MATE 2 pager.

 - Change focus mode:
     ```
     gsettings set org.mate.Marco.general focus-mode mouse
     gsettings set org.mate.Marco.general focus-mode sloppy
     gsettings set org.mate.Marco.general focus-mode click
     ```

 - Global keybinding defaults include:

   |                                                      |                                         |
   |------------------------------------------------------|-----------------------------------------|
   | <kbd>Alt</kbd>-<kbd>Tab</kbd>                        |forward cycle window focus               |
   | <kbd>Alt</kbd>-<kbd>Shift</kbd>-<kbd>Tab</kbd>       |backward cycle focus                     |
   | <kbd>Alt</kbd>-<kbd>Ctrl</kbd>-<kbd>Tab</kbd>        |forward cycle focus among panels         |
   | <kbd>Alt</kbd>-<kbd>Ctrl</kbd>-<kbd>Shift</kbd>-<kbd>Tab</kbd>|backward cycle focus among panels|
   | <kbd>Alt</kbd>-<kbd>Escape</kbd>                     |cycle window focus without a popup thingy|
   | <kbd>Ctrl</kbd>-<kbd>Alt</kbd>-<kbd>Left Arrow</kbd> |previous workspace                       |
   | <kbd>Ctrl</kbd>-<kbd>Alt</kbd>-<kbd>Right Arrow</kbd>|next workspace                           |
   | <kbd>Ctrl</kbd>-<kbd>Alt</kbd>-<kbd>D</kbd>          |minimize/unminimize all, to show desktop |

   Change keybindings for example:

     ```
     gsettings set org.mate.Marco.global-keybindings switch-to-workspace-1 '<Alt>F1'
     ```

   Also try the MATE keyboard shortcuts control panel.

   See marco.schemas for all available bindings.

 - Window keybindings:

    <kbd>Alt</kbd>-<kbd>space</kbd>         window menu

    Mnemonics work in the menu. That is, <kbd>Alt</kbd>-<kbd>space</kbd> then underlined
    letter in the menu item works.

    Choose Move from menu, and arrow keys to move the window.

    While moving, hold down Control to move slower, and
      Shift to snap to edges.

    Choose Resize from menu, and nothing happens yet, but
      eventually I might implement something.

    Keybindings for things like maximize window, vertical maximize,
    etc. can be bound, but may not all exist by default. See
    marco.schemas.

 - Window mouse bindings:

    Clicking anywhere on frame with button 1 will raise/focus window

    If you click a window control, such as the close button, then the
     control will activate on button release if you are still over it
     on release (as with most GUI toolkits)

    If you click and drag borders with button 1 it resizes the window

    If you click and drag the titlebar with button 1 it moves the
     window.

    If you click anywhere on the frame with button 2 it lowers the
     window.

    If you click anywhere on the frame with button 3 it shows the
     window menu.

    If you hold down Super (windows key) and click inside a window, it
     will move the window (buttons 1 and 2) or show menu (button 3).
     Or you can configure a different modifier for this.

    If you pick up a window with button 1 and then switch workspaces
     the window will come with you to the new workspace, this is
     a feature copied from Enlightenment.

    If you hold down Shift while moving a window, the window snaps
     to edges of other windows and the screen.

 - Session management:

    Marco connects to the session manager and will set itself up to
     be respawned. It theoretically restores sizes/positions/workspace
     for session-aware applications.

 - Marco implements much of the EWMH window manager specification
   from freedesktop.org, as well as the older ICCCM.  Please refer to
   the COMPLIANCE file for information on marco compliance with
   these standards.

 - Uses Pango to render text, so has cool i18n capabilities.
   Supports UTF-8 window titles and such.

 - There are simple animations for actions such as minimization,
   to help users see what is happening. Should probably
   have a few more of these and make them nicer.

 - if you have the proper X setup, set the GDK_USE_XFT=1
   environment variable to get antialiased window titles.

 - considers the panel when placing windows and maximizing
   them.

 - handles the window manager selection from the ICCCM. Will exit if
   another WM claims it, and can claim it from another WM if you pass
   the --replace argument. So if you're running another
   ICCCM-compliant WM, you can run "marco --replace" to replace it
   with Marco.

 - does basic colormap handling

 - and much more! well, maybe not a lot more.

HOW TO ADD EXTERNAL FEATURES
===

You can write a marco "plugin" such as a pager, window list, icon
box, task menu, or even things like "window matching" using the
Extended Window Manager Hints. See http://www.freedesktop.org for the
EWMH specification. An easy-to-use library called "libwnck" is
available that uses the EWMH and is specifically designed for writing
WM accessories.

You might be interested in existing accessories such as "Devil's Pie"
by Ross Burton, which add features to Marco (or other
EWMH-compliant WMs).

FAQ
===

Q: Will you add my feature?

A: If it makes sense to turn on unconditionally, or is genuinely a
   harmless preference that I would not be embarrassed to put in a
   simple, uncluttered, user-friendly configuration dialog.

   If the only rationale for your feature is that other window
   managers have it, or that you are personally used to it, or
   something like that, then I will not be impressed. Marco is
   firmly in the "choose good defaults" camp rather than the "offer 6
   equally broken ways to do it, and let the user pick one" camp.

   This is part of a "no crackrock" policy, despite some exceptions
   I'm mildly embarrassed about. For example, multiple workspaces
   probably constitute crackrock, they confuse most users and really
   are not that useful if you have a decent tasklist and so on. But I
   am too used to them to turn them off.  Or alternatively
   iconification/tasklist is crack, and workspaces/pager are good. But
   having both is certainly a bit wrong.  Sloppy focus is probably
   crackrock too.

   But don't think unlimited crack is OK just because I slipped up a
   little. No slippery slope here.

   Don't let this discourage patches and fixes - I love those. ;-)
   Just be prepared to hear the above objections if your patch adds
   some crack-ridden configuration option.

   http://pobox.com/~hp/free-software-ui.html
   http://pobox.com/~hp/features.html

Q: Why does Marco remember the workspace/position of some apps
   but not others across logout/login?

A: Marco only stores sizes/positions for apps that are session
   managed. As far as I can determine, there is no way to attempt to
   remember workspace/position for non-session-aware apps without
   causing a lot of weird effects.

   The reason is that you don't know which non-SM-aware apps were
   launched by the session. When you initially log in, Marco sees a
   bunch of new windows appear. But it can't distinguish between
   windows that were stored in your session, or windows you just
   launched after logging in. If Marco tried to guess that a window
   was from the session, it could e.g. end up maximizing a dialog, or
   put a window you just launched on another desktop or in a weird
   place. And in fact I see a lot of bugs like this in window managers
   that try to handle non-session-aware apps.

   However, for session-aware apps, Marco can tell that the
   application instance is from the session and thus restore it
   reliably, assuming the app properly restores the windows it had
   open on session save.

   So the correct way to fix the situation is to make apps
   session-aware. libSM has come with X for years, it's very
   standardized, it's shared by GNOME and KDE - even twm is
   session-aware. So anyone who won't take a patch to add SM is more
   archaic than twm - and you should flame them. ;-)

   Docs on session management:
    http://www.fifi.org/doc/xspecs/xsmp.txt.gz
    http://www.fifi.org/doc/xspecs/SMlib.txt.gz

   See also the ICCCM section on SM. For MATE apps, use the
   MateClient object. For a simple example of using libSM directly,
   twm/session.c in the twm source code is pretty easy to understand.

Q: How about adding viewports in addition to workspaces?

A: I could conceivably be convinced to use viewports _instead_ of
   workspaces, though currently I'm not thinking that. But I don't
   think it makes any sense to have both; it's just confusing. They
   are functionally equivalent.

   You may think this means that you won't have certain keybindings,
   or something like that. This is a misconception. The only
   _fundamental_ difference between viewports and workspaces is that
   with viewports, windows can "overlap" and appear partially on
   one and partially on another. All other differences that
   traditionally exist in other window managers are accidental -
   the features commonly associated with viewports can be implemented
   for workspaces, and vice versa.

   So I don't want to have two kinds of
   workspace/desktop/viewport/whatever, but I'm willing to add
   features traditionally associated with either kind if those
   features make sense.

Q: Why is the panel always on top?

A: Because it's a better user interface, and until we made this not
   configurable a bunch of apps were not getting fixed (the app
   authors were just saying "put your panel on the bottom" instead of
   properly supporting fullscreen mode, and such).

   rationales.txt has the bugzilla URL for some flamefesting on this,
   if you want to go back and relive the glory.
   Read these and the bugzilla stuff before asking/commenting:
     http://pobox.com/~hp/free-software-ui.html
     http://pobox.com/~hp/features.html

Q: Why is there no edge flipping?

A: This one is also in rationales.txt. Because "ouija board" UI, where
   you just move the mouse around and the computer guesses what you
   mean, has a lot of issues. This includes mouse focus, shade-hover
   mode, edge flipping, autoraise, etc. Marco has mouse focus and
   autoraise as a compromise, but these features are all confusing for
   many users, and cause problems with accessibility, fitt's law, and
   so on.

   Read these and the bugzilla stuff before asking/commenting:
     http://pobox.com/~hp/free-software-ui.html
     http://pobox.com/~hp/features.html

Q: Why does wireframe move/resize suck?

A: You can turn it on with the reduced_resources setting.

   But: it has low usability, and is a pain
   to implement, and there's no reason opaque move/resize should be a
   problem on any setup that can run a modern desktop worth a darn to
   begin with.

   Read these and the bugzilla stuff before asking/commenting:
     http://pobox.com/~hp/free-software-ui.html
     http://pobox.com/~hp/features.html

   The reason we had to add wireframe anyway was broken
   proprietary apps that can't handle lots of resize events.

Q: Why no XYZ?

A: You are probably getting the idea by now - check rationales.txt,
   query/search bugzilla, and read http://pobox.com/~hp/features.html
   and http://pobox.com/~hp/free-software-ui.html

   Then sit down and answer the question for yourself.  Is the feature
   good? What's the rationale for it? Answer "why" not just "why not."
   Justify in terms of users as a whole, not just users like
   yourself. How else can you solve the same problem? etc. If that
   leads you to a strong opinion, then please, post the rationale for
   discussion to an appropriate bugzilla bug, or to
   usability@gnome.org.

   Please don't just "me too!" on bugzilla bugs, please don't think
   flames will get you anywhere, and please don't repeat rationale
   that's already been offered.

Q: Your dumb web pages you made me read talk about solving problems in
   fundamental ways instead of adding preferences or workarounds.
   What are some examples where marco has done this?

A: There are quite a few, though many opportunities remain.  Sometimes
   the real fix involves application changes. The marco approach is
   that it's OK to require apps to change, though there are also
   plenty of workarounds in marco for battles considered too hard
   to fight.

   Here are some examples:

   - fullscreen mode was introduced to allow position constraints,
     panel-on-top, and other such things to apply to normal windows
     while still allowing video players etc. to "just work"

   - "whether to include minimized windows in Alt+Tab" was solved
     by putting minimized windows at the *end* of the tab order.

   - Whether to pop up a feedback display during Alt+Tab was solved by
     having both Alt+Tab and Alt+Esc

   - Whether to have a "kill" feature was solved by automatically
     detecting and offering to kill stuck apps. Better, marco
     actually does "kill -9" on the process, it doesn't just
     disconnect the process from the X server. You'll appreciate this
     if you ever did a "kill" on Netscape 4, and watched it keep
     eating 100% CPU even though the X server had booted it.

   - The workspaces vs. viewports mess was avoided by adding
     directional navigation and such to workspaces, see discussion
     earlier in this file.

   - Instead of configurable placement algorithms, there's just one
     that works fairly well most of the time.

   - To avoid excess CPU use during opaque move/resize, we rate limit
     the updates to the application window's size.

   - Instead of configurable "show size of window while resizing,"
     it's only shown for windows where it matters, such as terminals.
     (Only use-case given for all windows is for web designers
     choosing their web browser size, but there are web sites and
     desktop backgrounds that do this for you.)

   - Using startup notification, applications open on the workspace
     where you launched them, not the active workspace when their
     window is opened.

   - and much more.

Q: I think marco sucks.

A: Feel free to use any WM you like. The reason marco follows the
   ICCCM and EWMH specifications is that it makes marco a modular,
   interchangeable part in the desktop. libwnck-based apps such as the
   MATE window list will work just fine with any EWMH-compliant WM.

Q: Did you spend a lot of time on this?

A: Originally the answer was no. Sadly the answer is now yes.

Q: How can you claim that you are anti-crack, while still
   writing a window manager?

A: I have no comment on that.
