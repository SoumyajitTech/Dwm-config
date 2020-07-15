//Modify this file to change what commands output to your statusbar, and recompile using the make command.
static const char font[] = "fontawesome:size=15";
static const Block blocks[] = {
	/*Icon*/	/*Command*/		/*Update Interval*/	/*Update Signal*/
	{"", "cat ~/.pacupdate | sed /📦0/d",		0,		9},

	{"", "~/.local/bin/statusbar/battery",		120,		0},

	{"", "~/.local/bin/statusbar/brightness",	60,		10},

	{"", "~/.local/bin/statusbar/cpu",		5,		0},

	{"", "~/.local/bin/statusbar/disk",		3600,		11},

	{""," ~/.local/bin/statusbar/memory",		5,		0},

	{""," ~/.local/bin/statusbar/music",            1800,             0},

 	{""," ~/.local/bin/statusbar/volume",           2,             0},

	{""," ~/.local/bin/statusbar/wifi",              5,             0},

 	{""," ~/.local/bin/statusbar/weather",          3600,             0},

	{""," ~/.local/bin/statusbar/clock",            60,             0},
};

//sets delimeter between status commands. NULL character ('\0') means no delimeter.
static char delim = '|';
