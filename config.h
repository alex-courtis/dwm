/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpt  = 2;        /* border pt of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=11" };
static const int fontpt             = 11;       /* used for calculating borderpx */
static const char dmenufont[]       = "monospace:size=11";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";

/* base16-default-dark */
static const char base00[] = "#181818"; // Default Background
static const char base01[] = "#282828"; // Lighter Background (Used for status bars)
static const char base02[] = "#383838"; // Selection Background
static const char base03[] = "#585858"; // Comments, Invisibles, Line Highlighting
static const char base04[] = "#b8b8b8"; // Dark Foreground (Used for status bars)
static const char base05[] = "#d8d8d8"; // Default Foreground, Caret, Delimiters, Operators
static const char base06[] = "#e8e8e8"; // Light Foreground (Not often used)
static const char base07[] = "#f8f8f8"; // Light Background (Not often used)

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { base04,    base01,    base02 },
	[SchemeSel]  = { base05,    base02,    base04 },
};

/* systray */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.60; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[M]",      monocle },	/* first entry is default */
	{ "=[]",      tilermaster },
	{ "[]=",      tilelmaster },
	{ "><>",      NULL },		/* no layout function means floating behavior */
};
static const unsigned int itilelm = 2;  /* used by settile */
static const unsigned int itilerm = 1;

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define SHELL "/bin/zsh"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { SHELL, "-c", "term &", NULL };
static const char *browsercmd[] = { SHELL, "-c", "browser &", NULL };
static const char *lockcmd[] = { "xautolock", "-locknow", NULL };
static const char *audiomutecmd[]  = { "pulseaudio-ctl", "mute", NULL };
static const char *audiomicmutecmd[]  = { "pulseaudio-ctl", "mute-input", NULL };
static const char *audiolowervolumecmd[]  = { "pulseaudio-ctl", "down", NULL };
static const char *audioraisevolumecmd[]  = { "pulseaudio-ctl", "up", NULL };
static const char *backlightdeccmd[]  = { "xbacklight", "-10", NULL };
static const char *backlightinccmd[]  = { "xbacklight", "+10", NULL };
static const char *twiddledisplayscmd[]  = { "xlayoutdisplay", NULL };
static const char *twiddledisplaysmirrorcmd[]  = { "xlayoutdisplay", "-m", NULL };
static const char *grabscreencmd[]  = { "scrot", "/tmp/%Y-%m-%d-%H%M%S_$wx$h_scrot.png",               "--exec", "echo -n '$f' | xsel --input --clipboard", NULL };
static const char *grabwindowcmd[]  = { "scrot", "/tmp/%Y-%m-%d-%H%M%S_$wx$h_scrot.png", "--focussed", "--exec", "echo -n '$f' | xsel --input --clipboard", NULL };

#include <X11/XF86keysym.h>
#include "unfloat.c"
static Key keys[] = {
	/* left */
	{ MODKEY,                       XK_semicolon,               tag,            {.ui = 1 << 0 } },
	{ MODKEY,                       XK_comma,                   tag,            {.ui = 1 << 1 } },
	{ MODKEY,                       XK_period,                  tag,            {.ui = 1 << 2 } },
	{ MODKEY,                       XK_p,                       tag,            {.ui = 1 << 3 } },
	{ MODKEY,                       XK_y,                       tag,            {.ui = 1 << 4 } },

	{ MODKEY,                       XK_a,                       view,           {.ui = 1 << 0 } },
	{ MODKEY,                       XK_o,                       view,           {.ui = 1 << 1 } },
	{ MODKEY,                       XK_e,                       view,           {.ui = 1 << 2 } },
	{ MODKEY,                       XK_u,                       view,           {.ui = 1 << 3 } },
	{ MODKEY,                       XK_i,                       view,           {.ui = 1 << 4 } },

	/* right */
	{ MODKEY,                       XK_g,                       setlayout,      {.v = &layouts[0]} }, /* monocle */
	{ MODKEY,                       XK_c,                       settile,        {0} },
	{ MODKEY,                       XK_r,                       setlayout,      {.v = &layouts[3]} }, /* float */
	{ MODKEY,                       XK_l,                       setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_l,                       incnmaster,     {.i = -1 } },

	{ MODKEY,                       XK_d,                       killclient,     {0} },
	{ MODKEY,                       XK_h,                       setmfact,       {.f = -0.05} },
	{ MODKEY|ShiftMask,             XK_h,                       incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_t,                       spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_n,                       spawn,          {.v = browsercmd } },
	{ MODKEY,                       XK_s,                       spawn,          {.v = dmenucmd } },

	{ MODKEY,                       XK_b,                       togglebar,      {0} },
	{ MODKEY,                       XK_m,                       unfloatvisible, {0} }, 
	{ MODKEY,                       XK_w,                       focusmon,       {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_w,                       tagmon,         {.i = -1 } },
	{ MODKEY,                       XK_v,                       focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_v,                       tagmon,         {.i = +1 } },

	/* thumbs */
	{ MODKEY,                       XK_Escape,                  view,           {0} },
	{ MODKEY,                       XK_Return,                  zoom,           {0} },
	{ MODKEY,                       XK_space,                   focusstack,     {.i = -1 } },

	/* big chordy mnemonic things */
	{ MODKEY|ShiftMask|ControlMask, XK_q,                       quit,           {0} },
	{ MODKEY|ShiftMask|ControlMask, XK_l,                       spawn,          {.v = lockcmd} },
	{ MODKEY|ShiftMask|ControlMask, XK_d,                       spawn,          {.v = twiddledisplayscmd} },
	{ MODKEY|ShiftMask|ControlMask, XK_m,                       spawn,          {.v = twiddledisplaysmirrorcmd} },
	{ MODKEY|ShiftMask|ControlMask, XK_w,                       spawn,          {.v = grabwindowcmd} },
	{ MODKEY|ShiftMask|ControlMask, XK_s,                       spawn,          {.v = grabscreencmd} },

	/* keyboard function keys */
	{ 0,                            XF86XK_AudioMute,           spawn,          {.v = audiomutecmd } },
	{ 0,                            XF86XK_AudioMicMute,        spawn,          {.v = audiomicmutecmd } },
	{ 0,                            XF86XK_AudioLowerVolume,    spawn,          {.v = audiolowervolumecmd } },
	{ 0,                            XF86XK_AudioRaiseVolume,    spawn,          {.v = audioraisevolumecmd } },
	{ 0,                            XF86XK_MonBrightnessDown,   spawn,          {.v = backlightdeccmd } },
	{ 0,                            XF86XK_MonBrightnessUp,     spawn,          {.v = backlightinccmd } },
	{ 0,                            XF86XK_Display,             spawn,          {.v = twiddledisplayscmd } },
	{ 0,                            XK_Print,                   spawn,          {.v = grabscreencmd } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[0]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

