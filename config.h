/* See LICENSE file for copyright and license details. */

/* appearance */
static const float borderpt         = 1;        /* border pt of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=11" };
static const int fontpt             = 11;       /* used for calculating borderpx */
static const char dmenufont[]       = "monospace:size=11";

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
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
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
};
static const unsigned int itilelm = 0;  /* used by settile */
static const unsigned int itilerm = 1;

void tagview(const Arg *arg) {
	tag(arg);
	view(arg);
}

/* key definitions */
#define MOD1 Mod4Mask
#define MOD2 MOD1|ControlMask
#define MOD3 MOD2|ShiftMask
#define TAGKEYS(KEY,TAG) \
{ MOD1,				KEY,	view,		{.ui = 1 << TAG} },\
{ MOD2,				KEY,	tagview,	{.ui = 1 << TAG} },\
{ MOD3,				KEY,	tag,		{.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define SHELL "/bin/zsh"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", BASE01, "-nf", BASE04, "-sb", BASE02, "-sf", BASE05, NULL };
static char j4dmenuargs[] = "--dmenu=dmenu -i -m 0 -fn monospace:size=11 -nb \\"BASE01" -nf \\"BASE04" -sb \\"BASE02" -sf \\"BASE05; /* manipulated in spawn() */
static const int j4dmenuargsmonindex = 20;
static const char *j4dmenucmd[] = { "j4-dmenu-desktop", j4dmenuargs, NULL };
static const char *termcmd[]  = { "term", NULL };
static const char *browsercmd[] = { "browser", NULL };
static const char *volraisemiccmd[]  = { "volraisemic", NULL };
static const char *vollowermiccmd[]  = { "vollowermic", NULL };
static const char *volmicmutecmd[]  = { "volmutemic", NULL };
static const char *volmutecmd[]  = { "volmute", NULL };
static const char *vollowercmd[]  = { "vollower", NULL };
static const char *volraisecmd[]  = { "volraise", NULL };
static const char *backlightdeccmd[]  = { "xbacklight", "-10", NULL };
static const char *backlightinccmd[]  = { "xbacklight", "+10", NULL };

#include <X11/XF86keysym.h>
#include "unfloat.c"
#include "movestack.c"
static Key keys[] = {

	/* thumbs */
	{ MOD1, XK_Escape,			togglefullscr,	{0} },
	{ MOD2, XK_Escape,			togglefloating,	{0} },
	{ MOD1, XK_Tab,				spawn,			{.v = dmenucmd } },
	{ MOD2, XK_Tab,				spawn,			{.v = j4dmenucmd } },
	{ MOD1, XK_Delete,			spawn,			{.v = termcmd } },
	{ MOD2, XK_Delete,			spawn,			{.v = browsercmd } },
	{ MOD1, XK_Return,			focusstack,		{.i = +1 } },
	{ MOD2, XK_Return,			focusstack,		{.i = -1 } },
	{ MOD1, XK_space,			movestack,		{.i = +1 } },
	{ MOD2, XK_space,			movestack,		{.i = -1 } },

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

	{ MOD1, XK_q,				killclient,		{0} },
	{ MOD3, XK_q,				quit,			{0} },

	/* right */
	{ MOD3, XK_f,				spawn,			{.v = volraisemiccmd } },
	{ MOD3, XK_g,				spawn,			{.v = volraisecmd } },

	{ MOD3, XK_d,				spawn,			{.v = vollowermiccmd } },
	{ MOD1, XK_h,				setlayout,		{.v = &layouts[0]} }, /* left */
	{ MOD3, XK_h,				spawn,			{.v = vollowercmd } },
	{ MOD1, XK_t,				setlayout,		{.v = &layouts[2]} }, /* monocle */
	{ MOD1, XK_n,				setlayout,		{.v = &layouts[1]} }, /* right */
	{ MOD1, XK_s,				settile,		{0} },

	{ MOD3, XK_b,				spawn,			{.v = volmicmutecmd } },
	{ MOD3, XK_m,				spawn,			{.v = volmutecmd } },
	{ MOD1, XK_w,				focusmon,		{.i = -1 } },
	{ MOD2, XK_w,				tagmon,			{.i = -1 } },
	{ MOD1, XK_v,				focusmon,		{.i = +1 } },
	{ MOD2, XK_v,				tagmon,			{.i = +1 } },

	{ MOD1, XK_equal,			setmfact,		{.f = 0.5 } },
	{ MOD1, XK_minus,			setmfact,		{.f = -0.05 } },
	{ MOD2, XK_minus,			incnmaster,		{.i = -1 } },
	{ MOD1, XK_plus,			setmfact,		{.f = +0.05 } },
	{ MOD2, XK_plus,			incnmaster,		{.i = +1 } },

	/* keyboard function keys */
	{ 0,	XF86XK_AudioMute,			spawn,	{.v = volmutecmd } },
	{ 0,	XF86XK_AudioMicMute,		spawn,	{.v = volmicmutecmd } },
	{ 0,	XF86XK_AudioLowerVolume,	spawn,	{.v = vollowercmd } },
	{ 0,	XF86XK_AudioRaiseVolume,	spawn,	{.v = volraisecmd } },
	{ 0,	XF86XK_MonBrightnessDown,	spawn,	{.v = backlightdeccmd } },
	{ 0,	XF86XK_MonBrightnessUp,		spawn,	{.v = backlightinccmd } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click				event mask		button			function		argument */
	{ ClkLtSymbol,			0,				Button1,		setlayout,		{0} },
	{ ClkLtSymbol,			0,				Button3,		setlayout,		{.v = &layouts[0]} },
	{ ClkWinTitle,			0,				Button2,		zoom,			{0} },
	{ ClkClientWin,			MOD1,			Button1,		movemouse,		{0} },
	{ ClkClientWin,			MOD1,			Button2,		togglefloating	,{0} },
	{ ClkClientWin,			MOD1,			Button3,		resizemouse,	{0} },
	{ ClkTagBar,			0,				Button1,		view,			{0} },
	{ ClkTagBar,			0,				Button3,		toggleview,		{0} },
	{ ClkTagBar,			MOD1,			Button1,		tag,			{0} },
	{ ClkTagBar,			MOD1,			Button3,		toggletag,		{0} },
};

