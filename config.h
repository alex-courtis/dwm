/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=11" };
static const char dmenufont[]       = "monospace:size=11";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

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
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "><>",      NULL },    /* no layout function means floating behavior */
};

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
	{ MODKEY,                       XK_comma,                   spawn,          {.v = browsercmd } },
	{ MODKEY,                       XK_period,                  spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_p,                       spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_y,                       killclient,     {0} },

	{ MODKEY,                       XK_a,                       view,           {.ui = 1 << 0 } },
	{ MODKEY,                       XK_o,                       view,           {.ui = 1 << 1 } },
	{ MODKEY,                       XK_e,                       view,           {.ui = 1 << 2 } },
	{ MODKEY,                       XK_u,                       view,           {.ui = 1 << 3 } },
	{ MODKEY,                       XK_i,                       view,           {.ui = 1 << 4 } },

	{ MODKEY,                       XK_apostrophe,              tag,            {.ui = 1 << 0 } },
	{ MODKEY,                       XK_q,                       tag,            {.ui = 1 << 1 } },
	{ MODKEY,                       XK_j,                       tag,            {.ui = 1 << 2 } },
	{ MODKEY,                       XK_k,                       tag,            {.ui = 1 << 3 } },
	{ MODKEY,                       XK_x,                       tag,            {.ui = 1 << 4 } },

	/* right */
	//{ MODKEY,                       XK_f,                       togglermaster,  {0} },
	{ MODKEY,                       XK_g,                       setlayout,      {.v = &layouts[0]} }, /* tile */
	{ MODKEY,                       XK_c,                       setlayout,      {.v = &layouts[1]} }, /* monocle */
	{ MODKEY,                       XK_r,                       setlayout,      {.v = &layouts[2]} }, /* float */
	{ MODKEY,                       XK_l,                       setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_l,                       incnmaster,     {.i = -1 } },

	{ MODKEY,                       XK_h,                       setmfact,       {.f = -0.05} },
	{ MODKEY|ShiftMask,             XK_h,                       incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_t,                       focusmon,       {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_t,                       tagmon,         {.i = -1 } },
	{ MODKEY,                       XK_n,                       focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_n,                       tagmon,         {.i = +1 } },

	{ MODKEY,                       XK_b,                       togglebar,      {0} },
	{ MODKEY,                       XK_m,                       unfloatvisible, {0} }, 

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
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[1]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

