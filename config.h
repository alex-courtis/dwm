/* See LICENSE file for copyright and license details. */

/* appearance */
static const float borderpt         = 1;        /* border pt of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=12" };
static const int fontpt             = 12;       /* used for calculating borderpx */
static const char dmenufont[]       = "monospace:size=12";

/* base16-bright */
#define BASE00 "#000000" // Default Background
#define BASE01 "#303030" // Lighter Background (Used for status bars)
#define BASE02 "#505050" // Selection Background
#define BASE0203 "#808080" // Window selection border
#define BASE03 "#b0b0b0" // Comments, Invisibles, Line Highlighting
#define BASE04 "#d0d0d0" // Dark Foreground (Used for status bars)
#define BASE05 "#e0e0e0" // Default Foreground, Caret, Delimiters, Operators
#define BASE06 "#f5f5f5" // Light Foreground (Not often used)
#define BASE07 "#ffffff" // Light Background (Not often used)

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { BASE04,    BASE01,    BASE01 },
	[SchemeSel]  = { BASE05,    BASE02,    BASE04 },
};

/* systray */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor  borderpx */
	{ "Steam",    NULL,       NULL,       0,            0,           -1,      0  },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tilelmaster },	/* first entry is default */
	{ "=[]",      tilermaster },
	{ "[M]",      monocle },
	{ "><>",      NULL },    /* no layout function means floating behavior */
};

void tagview(const Arg *arg) {
	tag(arg);
	view(arg);
}

/* key definitions */
#define MOD1 Mod4Mask
#define MOD2 MOD1|ShiftMask
#define MOD3 MOD2|ControlMask
#define TAGKEYS(KEY,TAG) \
{ MOD1,				KEY,	view,		{.ui = 1 << TAG} },\
{ MOD2,				KEY,	tagview,	{.ui = 1 << TAG} },\
{ MOD3,				KEY,	tag,		{.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", BASE01, "-nf", BASE04, "-sb", BASE02, "-sf", BASE05, NULL };

#include <X11/XF86keysym.h>
#include "unfloat.c"
static Key keys[] = {

	/* thumbs */
	{ MOD1, XK_Tab,				togglefullscr,	{0} },
	{ MOD1, XK_space,			setlayoutlm,	{0} },
	{ MOD1, XK_Delete,			togglefloating,	{0} },
	{ MOD1, XK_Return,			view,			{ .ui = 0 } },

	/* left */
	TAGKEYS(					XK_a,			0)
	TAGKEYS(					XK_o,			1)
	TAGKEYS(					XK_e,			2)
	TAGKEYS(					XK_u,			3)
	TAGKEYS(					XK_i,			4)
	TAGKEYS(					XK_semicolon,	5)
	TAGKEYS(					XK_comma,		6)
	TAGKEYS(					XK_period,		7)
	TAGKEYS(					XK_p,			8)
	TAGKEYS(					XK_y,			9)

	// bemenu doens't obey x font scale
	{ MOD1, XK_apostrophe,		spawn,			SHCMD("bemenu-run --fn 'monospace 22' > /dev/null 2>&1 &!") },
	{ MOD2, XK_apostrophe,		spawn,			SHCMD("exec $(menjar -d ~/.local/share/applications -m 'bemenu -i -p# --fn \"monospace 22\"') > /dev/null 2>&1 &!") },

	{ MOD1, XK_q,				view,			{ .ui = 1 << 9 } }, // annoying, to get out of the cmd-Q habit for macOS

	{ MOD1, XK_j,				spawn,			SHCMD("term") },

	{ MOD1, XK_k,				spawn,			SHCMD("b") },
	{ MOD2, XK_k,				spawn,			SHCMD("bp") },

	/* right */

	{ MOD1, XK_f,				setlayout,		{.v = &layouts[3]} }, /* float */
	{ MOD3, XK_f,				spawn,			SHCMD("volraisemic") },

	{ MOD1, XK_g,				incnmaster,		{.i = +1 } },
	{ MOD3, XK_g,				spawn,			SHCMD("volraise") },

	{ MOD1, XK_c,				setmfact,		{.f = +0.025 } },
	{ MOD2, XK_c,				setmfact,		{.f = 1.5 } },

	{ MOD1, XK_r,				incnmaster,		{.i = -1 } },

	{ MOD3, XK_l,				spawn,			SHCMD("slock") },

	{ MOD3, XK_slash,			spawn,			SHCMD("pa-mute-all") },

	{ MOD1, XK_d,				setlayout,		{.v = &layouts[0]} }, /* left */
	{ MOD3, XK_d,				spawn,			SHCMD("vollowermic") },

	{ MOD1, XK_h,				focusstack,		{.i = -1 } },
	{ MOD2, XK_h,				pushup,			{ 0 } },
	{ MOD3, XK_h,				spawn,			SHCMD("vollower") },

	{ MOD1, XK_t,				setmfact,		{.f = -0.025 } },
	{ MOD2, XK_t,				setmfact,		{.f = 1.5 } },

	{ MOD1, XK_n,				focusstack,		{.i = +1 } },
	{ MOD2, XK_n,				pushdown,		{ 0 } },

	{ MOD3, XK_s,				spawn,			SHCMD("sss") },

	{ MOD1, XK_b,				setlayout,		{.v = &layouts[2]} }, /* monocle */

	{ MOD1, XK_w,				killclient,		{0} },
	{ MOD3, XK_w,				quit,			{0} },

	{ MOD3, XK_v,				spawn,			SHCMD("display-init") },

	{ MOD3, XK_z,				spawn,			SHCMD("xlayoutdisplay --mirror") },

	{ MOD3, XK_backslash,		spawn,			SHCMD("sudo systemctl suspend") },

	{ MOD1, XK_Left,			focusmon,		{.i = -1 } },
	{ MOD2, XK_Left,			tagmon,			{.i = -1 } },
	{ MOD1, XK_Right,			focusmon,		{.i = +1 } },
	{ MOD2, XK_Right,			tagmon,			{.i = +1 } },

	/* keyboard function keys */
	{ 0,	XF86XK_AudioMute,			spawn,	SHCMD("volmute") },
	{ 0,	XF86XK_AudioMicMute,		spawn,	SHCMD("volmutemic") },
	{ 0,	XF86XK_AudioLowerVolume,	spawn,	SHCMD("vollower") },
	{ 0,	XF86XK_AudioRaiseVolume,	spawn,	SHCMD("volraise") },
	{ 0,	XF86XK_MonBrightnessDown,	spawn,	SHCMD("xbacklight -10") },
	{ 0,	XF86XK_MonBrightnessUp,		spawn,	SHCMD("xbacklight +10") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click				event mask		button			function		argument */
	{ ClkLtSymbol,			0,				Button1,		setlayout,		{0} },
	{ ClkLtSymbol,			0,				Button3,		setlayout,		{.v = &layouts[0]} },
	{ ClkWinTitle,			0,				Button1,		copytitle,		{0} },
	{ ClkWinTitle,			0,				Button2,		zoom,			{0} },
	{ ClkClientWin,			MOD1,			Button1,		movemouse,		{0} },
	{ ClkClientWin,			MOD1,			Button2,		togglefloating,	{0} },
	{ ClkClientWin,			MOD1,			Button3,		resizemouse,	{0} },
	{ ClkTagBar,			0,				Button1,		view,			{0} },
	{ ClkTagBar,			0,				Button3,		toggleview,		{0} },
	{ ClkTagBar,			MOD1,			Button1,		tag,			{0} },
	{ ClkTagBar,			MOD1,			Button3,		toggletag,		{0} },
};

