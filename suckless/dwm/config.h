/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
#include "/home/namish/bedwm/suckless/dwm/themes/current.h"
/* appearance */
static const unsigned int gappx     = 9;
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "JetBrains Mono Nerd Font:size=12" };
//static const char col_gray1[]       = "#282828";
//static const char col_gray2[]       = "#444444";
//static const char col_gray3[]       = "#bbbbbb";
//static const char col_gray4[]       = "#e5e9f0";
static const int user_bh            = 25;
//static const char col_cyan[]        = "#fbf1c7";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};
static const char *dmenucmd[] = { "dmenu_run", "-l", "20", NULL };
static const char dmenufont[]       = "Fira Code:size=11";
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
/* tagging */
/*static const char *tagsel[][2] = {
       { "#fb4935", "#3c3835" },
       { "#d3869b", "#3c3835" },
       { "#8ec07c", "#3c3835" },
       { "#fabd2f", "#3c3835" },
       { "#83a598", "#3c3835" },
       { "#fe8019", "#3c3835" },
       { "#b8bb26", "#3c3835" },

};
*/
static const char *tags[] = { "cmd", "www", "dev", "doc", "chat", "med", "sys" };
static const char *alttags[] = { "[cmd]", "[www]", "[dev]", "[doc]", "[chat]", "[med]", "[sys]" };
static const unsigned int ulinepad     = 4;  /* horizontal padding between the underline and tag */
static const unsigned int ulinestroke  = 1;    /* thickness / height of the underline */
static const unsigned int ulinevoffset = 0;    /* how far above the bottom of the bar the line should appear */
static const int ulineall              = 0;    /* 1 to show underline on all tags,
0 for just the active ones */

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
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "|T|",      tile },    /* first entry is default */
	{ "|F|",      NULL },    /* no layout function means floating behavior */
	{ "|M|",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/usr/bin/bash", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[]  = { "st", NULL };
static const char *mutecmd[] = {"changevolume", "mute", NULL};
static const char *volupcmd[] = {"changevolume", "up", NULL};
static const char *voldowncmd[] = {"changevolume", "down", NULL};
static const char  *brupcmd[] = {"changebrightness", "up", NULL};
static const char  *brdowncmd[] = {"changebrightness", "down", NULL};
static const char *screenshotcmd[] = {"screenshot", NULL};
static const char *wifimenu[]= {"wifimenu.sh", NULL};
static const char *dashcmd[] = {"/usr/bin/eww", "open", "--toggle", "dashboard", NULL};
static const char *powermenu[] = {"/usr/bin/eww", "open", "--toggle", "pmenu", NULL};
static const char *appmenu[] = {"rofi", "-show", "drun", NULL};
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_a,      spawn,          {.v = appmenu } },
	{ MODKEY,                       XK_x,      spawn,          {.v = powermenu } },
	{ MODKEY,                       XK_n,      spawn,          {.v = wifimenu } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_p, spawn,          {.v = dashcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY|ShiftMask,             XK_s,      spawn,           {.v = screenshotcmd} },
	{ 0, XF86XK_AudioMute, spawn, {.v = mutecmd } },
	{ 0, XF86XK_AudioLowerVolume, spawn, {.v = voldowncmd } },
	{ 0, XF86XK_AudioRaiseVolume, spawn, {.v = volupcmd } },
	{ 0, XF86XK_MonBrightnessUp, spawn, {.v = brupcmd} },
	{ 0, XF86XK_MonBrightnessDown, spawn, {.v = brdowncmd} },

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

