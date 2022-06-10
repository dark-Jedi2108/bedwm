static const struct arg args[] = {
	/* function format          argument */
	{battery_perc, " ^b#83a598^^c#ebdbb2^  ^b#458588^ %s%% ", "BAT0"},
	{disk_perc, "^b#282828^^c#83a598^  ^c#ebdbb2^%s%% ", "/"},
	{cpu_perc, "^b#8ec07c^^c#282828^ CPU ^b#282828^^c#8ec07c^ %s%% ", NULL},
	{ram_perc, "^c#83a598^  %s%% ", NULL },
	{wifi_essid, "^b#83a598^^c#282828^   ^b#282828^^c#83a598^ %s ", "wlan0"},
	{ datetime, "^b#83a598^^c#ebdbb2^  ^b#458588^ %s ",           "%F %T" },  
};
