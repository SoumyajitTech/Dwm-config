/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const char *upvol[]   = { "/usr/bin/pactl", "set-sink-volume", "0", "+5%",     NULL };
static const char *downvol[] = { "/usr/bin/pactl", "set-sink-volume", "0", "-5%",     NULL };
static const char *mutevol[] = { "/usr/bin/pactl", "set-sink-mute",   "0", "toggle",  NULL };
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "system san francisco display:size=10:antialias=true:autohint=true",
                                         "Hack:size=8:antialias=true:autohint=true",
                                         "JoyPixels:size=10:antialias=true:autohint=true"
                                                                                };
static const char dmenufont[]       = "monospace:size=10";


static const char col_gray1[]         = "#292d3e";
static const char col_gray2[]         = "#292d3e"; /* border color unfocused windows */
static const char col_gray3[]         = "#96b5b4";
static const char col_gray4[]         = "#d7d7d7";
static const char col_gray5[]         = "#f7f9f9";
static const char col_cyan[]          = "#0a90c9"; /* border color focused windows and tags */
static const char title_bg_dark[]     = "#020000";
static const char title_bg_light[]    = "#fdfdfd";
static const int color_ptrs[][3]  = {
	/*                              fg         bg         border    */
	[SchemeNorm]                = { -1,         -1,        1 },
	[SchemeSel]                 = { -1,        -1,        11 },
	[SchemeTagsNorm]            = { 7,         0,         0 },
	[SchemeTagsSel]             = { 0,         6,         1 },
	[SchemeTitleNorm]           = { 6,         -1,        -1 },
	[SchemeTitleSel]            = { 6,         -1,        -1 },
	[SchemeStatus]              = { 2,         0,         0 },
};

static char colors[][3][8]          = {
	/*                              fg         bg         border    */
	[SchemeNorm]                = { "#000000", "#000000", "#000000" },
	[SchemeSel]                 = { "#000000", "#000000", "#000000" },
	[SchemeTagsNorm]            = { "#000000", "#000000", "#000000" },
	[SchemeTagsSel]             = { "#000000", "#000000", "#000000" },
	[SchemeTitleNorm]           = { "#000000", "#000000", "#000000" },
	[SchemeTitleSel]            = { "#000000", "#000000", "#000000" },
	[SchemeStatus]              = { "#000000", "#000000", "#000000" },
 };

/* tagging */
static const char *tags[] = { "1:", "2:", "3:", "4:", "5:", "6:", "7:", "8:", "9:" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Pavucontrol",    NULL,       NULL,       0,            1,           -1 },
        { "Brave-browser",    NULL,       NULL,       1 << 0,       0,           -1 },
        { "Terminator",     NULL,       NULL,       1 << 1,       0,           -1 },
        { "Thunar",         NULL,       NULL,       1 << 3,       0,           -1 },

};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
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

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray5, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "terminator", NULL };
static const char *webcmd[] = { "brave-browser", NULL };
static const char *filescmd[] = { "thunar", NULL };
static const char *lockcmd[] = { "i3lock", NULL };

#include "selfrestart.c"
#include "shiftview.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ 0,              XF86XK_AudioLowerVolume, spawn, {.v = downvol } },
        { 0,                     XF86XK_AudioMute, spawn, {.v = mutevol } },
        { 0,              XF86XK_AudioRaiseVolume, spawn, {.v = upvol   } },
        { MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
        { MODKEY,                       XK_t,      spawn,          {.v = termcmd } },
        { MODKEY,                       XK_w,      spawn,          {.v = webcmd } },
        { MODKEY,                       XK_e,      spawn,          {.v = filescmd } },
        { MODKEY,                       XK_b,      togglebar,      {0} },
        { MODKEY,                       XK_Right,  focusstack,     {.i = +1 } },   /*Focus*/
        { MODKEY,                       XK_Left,   focusstack,     {.i = -1 } },
        { MODKEY|ShiftMask,             XK_m,      incnmaster,     {.i = +1 } },
        { MODKEY|ShiftMask,             XK_n,      incnmaster,     {.i = -1 } },
        { MODKEY|ShiftMask,             XK_Left,   setmfact,       {.f = -0.05} },   /*Increase*/
        { MODKEY|ShiftMask,             XK_Right,  setmfact,       {.f = +0.05} },
	{ MODKEY|ControlMask,  		XK_j,      incrigaps,      {.i = +1 } },
	{ MODKEY|ControlMask,  		XK_h,      incrigaps,      {.i = -1 } },
	{ MODKEY,	                XK_0,      togglegaps,     {0} },
	{ MODKEY|ControlMask,   	XK_0,      defaultgaps,    {0} },
	{ MODKEY|ControlMask,           XK_l,      incrohgaps,     {.i = +1 } },
	{ MODKEY|ControlMask,           XK_k,      incrohgaps,     {.i = -1 } },
	{ MODKEY,           		XK_l,      incrivgaps,     {.i = +1 } },
        { MODKEY,           		XK_k,      incrivgaps,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_l,      incrovgaps,     {.i = +1 } },
        { MODKEY|ShiftMask,             XK_k,      incrovgaps,     {.i = -1 } },
	{ ControlMask|Mod1Mask,         XK_Right,  shiftview,      {.i = +1} },    /*Workspace Shift*/
        { ControlMask|Mod1Mask,         XK_Left,   shiftview,      {.i = -1} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
        { MODKEY|ShiftMask,             XK_u,      setlayout,      {.v = &layouts[0]} },  /*Layout*/
        { MODKEY|ShiftMask,             XK_i,      setlayout,      {.v = &layouts[1]} },
        { MODKEY|ShiftMask,             XK_o,      setlayout,      {.v = &layouts[2]} },
        { MODKEY,                       XK_space,  setlayout,      {0} },
        { MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
        { MODKEY|Mod1Mask,              XK_0,      view,           {.ui = ~0 } },
        { MODKEY|ShiftMask|ControlMask, XK_0,      tag,            {.ui = ~0 } },
        { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
        { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
        TAGKEYS(                        XK_1,                      0)              /*Workspace*/
        TAGKEYS(                        XK_2,                      1)
        TAGKEYS(                        XK_3,                      2)
        TAGKEYS(                        XK_4,                      3)
        TAGKEYS(                        XK_5,                      4)
        TAGKEYS(                        XK_6,                      5)
        TAGKEYS(                        XK_7,                      6)
        TAGKEYS(                        XK_8,                      7)
        TAGKEYS(                        XK_9,                      8)
 	{ MODKEY|ShiftMask,             XK_x,      spawn,          {.v = lockcmd } },
	{ MODKEY|ShiftMask,             XK_r,      self_restart,   {0} },
        { MODKEY|ShiftMask,             XK_q,      quit,           {0} },

};

/* Apps Launched with SUPER + ALT + KEY */
/*        { MODKEY|Mod1Mask,        XK_g,    spawn,          CMD("st -e surf www.google.com") },
        { MODKEY|Mod1Mask,        XK_c,    spawn,          CMD("st -e cmus") },
        { MODKEY|Mod1Mask,        XK_y,    spawn,          CMD("st -e surf www.youtube.com") },
        { MODKEY|Mod1Mask,        XK_f,    spawn,          CMD("st -e vifm") },
        { MODKEY|Mod1Mask,        XK_h,    spawn,          CMD("st -e htop") },
        { MODKEY|Mod1Mask,        XK_i,    spawn,          CMD("st -e irssi") },
        { MODKEY|Mod1Mask,        XK_l,    spawn,          CMD("st -e lynx gopher://distro.tube") },
        { MODKEY|Mod1Mask,        XK_n,    spawn,          CMD("st -e newsboat") },
        { MODKEY|Mod1Mask,        XK_r,    spawn,          CMD("st -e rtv") },*/

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
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
