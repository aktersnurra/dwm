/* See LICENSE file for copyright and license details. */

/* Constants */
#define TERMINAL "st"
#define TERMCLASS "St"
#define BROWSER "firefox"

/* appearance */
static unsigned int borderpx  = 1;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;  /* 1 means swallow floating windows by default */
static const unsigned int gappih    = 20; /* horiz inner gap between windows */
static const unsigned int gappiv    = 10; /* vert inner gap between windows */
static const unsigned int gappoh    = 10; /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 30; /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;  /* 1 means no outer gap when there is only one window */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=8", "Symbols Nerd Font:style=2048-em:pixelsize=12:antialias=true:autohint=true" };
static const char dmenufont[]       = "monospace:size=8";
static char normbgcolor[] =     "#000000";
static char normbordercolor[] = "#000000";
static char normfgcolor[] =     "#eeeeee";
static char selfgcolor[] =      "#eeeeee";
static char selbordercolor[] =  "#373737";
static char selbgcolor[] =      "#373737";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {TERMINAL, "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {TERMINAL, "-n", "spcalc", "-f", "monospace:size=16", "-g", "50x20", "-e", "bc", "-lq", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spcalc",      spcmd2},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ BROWSER,   NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
	{ TERMCLASS, NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
  { MOD, XK_h, ACTION##stack, {.i = 0}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { TERMINAL, NULL };
static const char *passmenucmd[]  = { "passmenu", NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
    {"background", STRING, &normbordercolor},
    {"background", STRING, &selbordercolor},
    {"background", STRING, &normbgcolor},
    {"color7", STRING, &normfgcolor},
    {"color7", STRING, &selfgcolor},
    {"background", STRING, &selbgcolor},
    {"borderpx", INTEGER, &borderpx},
    {"snap", INTEGER, &snap},
    {"showbar", INTEGER, &showbar},
    {"topbar", INTEGER, &topbar},
    {"nmaster", INTEGER, &nmaster},
    {"resizehints", INTEGER, &resizehints},
    {"mfact", FLOAT, &mfact},
    {"gappih", INTEGER, &gappih},
    {"gappiv", INTEGER, &gappiv},
    {"gappoh", INTEGER, &gappoh},
    {"gappov", INTEGER, &gappov},
    {"swallowfloating", INTEGER, &swallowfloating},
    {"smartgaps", INTEGER, &smartgaps},
};

#include "shiftview.c"

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_space,  spawn,          {.v = dmenucmd } },
  { MODKEY|ShiftMask,		          XK_space,	 spawn,		       {.v = passmenucmd } },
	{ MODKEY,                       XK_t,      spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_b,      togglebar,      {0} },
  STACKKEYS(MODKEY,                          focus)
  STACKKEYS(MODKEY|ShiftMask,                push)
	{ MODKEY,                       XK_o,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_o,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_l,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_h,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY|ShiftMask,             XK_a,      defaultgaps,    {0} },
	{ MODKEY,                       XK_a,      togglegaps,     {0} },
  { MODKEY,                       XK_x,      incrgaps,       {.i = +3} },
  { MODKEY|ShiftMask,             XK_x,      incrgaps,       {.i = -3} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
  { MODKEY|ShiftMask,             XK_Tab,    shiftview,      {.i = -1}},
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_l,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_u,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ShiftMask,             XK_y,      setlayout,      {.v = &layouts[5]} },
  { MODKEY,                       XK_s,      togglesticky,   {0} },
  { MODKEY,                       XK_f,      setfullscreen,  {0} },
	{ MODKEY,                       XK_z,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_z,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
  { MODKEY,                       XK_BackSpace,      spawn,  SHCMD("sysact") },
  { MODKEY,                       XK_m,      spawn,          SHCMD("mailsync") },
  { MODKEY|ShiftMask,             XK_m,      spawn,          SHCMD("remaps") },
  { MODKEY,                       XK_p,      spawn,          SHCMD("scrot -s -q 100 screenshot.png") },
  { MODKEY|ShiftMask,             XK_s,      spawn,          SHCMD("slock") },
  { MODKEY,                       XK_w,      spawn,          SHCMD("$BROWSER") },
  { MODKEY,                       XK_r,      spawn,          SHCMD(TERMINAL " -e lf") },
  { MODKEY|ShiftMask,             XK_r,      spawn,          SHCMD(TERMINAL " -e btm") },
  { MODKEY,                       XK_n,      spawn,          SHCMD(TERMINAL " -e newsboat; pkill -RTMIN+6 dwmblocks") },
  { MODKEY,                       XK_e,      spawn,          SHCMD(TERMINAL " -e neomutt ; pkill -RTMIN+12 dwmblocks; rmdir ~/.abook") },
  { MODKEY,                       XK_i,      spawn,          SHCMD(TERMINAL " -e abook -C ~/.config/abook/abookrc --datafile ~/.config/abook/addressbook") },
  { MODKEY|ControlMask,           XK_l,      spawn,          SHCMD("layouthandler") },
  /* Bookmarks */
  { MODKEY,                       XK_b,      spawn,          SHCMD("bookmarkthis")},
  { MODKEY|XK_space,              XK_space,  spawn,          SHCMD("xdotool type $(grep -v '^#' ~/.local/share/snippets | dmenu -i -l 50 | cut -d' ' -f1)")},
  /* Audio */
  { MODKEY|ShiftMask,             XK_t,      spawn,          SHCMD("pamixer -t; kill -44 $(pidof dwmblocks)")},
  { MODKEY,                       XK_minus,  spawn,          SHCMD("pamixer --allow-boost -d 5; kill -44 $(pidof dwmblocks)") },
  { MODKEY|ShiftMask,             XK_minus,  spawn,          SHCMD("pamixer --allow-boost -d 15; kill -44 $(pidof dwmblocks)") },
  { MODKEY,                       XK_equal,  spawn,          SHCMD("pamixer --allow-boost -i 5; kill -44 $(pidof dwmblocks)") },
  { MODKEY|ShiftMask,             XK_equal,  spawn,          SHCMD("pamixer --allow-boost -i 15; kill -44 $(pidof dwmblocks)") },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
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

