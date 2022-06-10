static const struct arg args[] = {
	/* function format          argument */
	{battery_perc, " ^b#81a1c1^^c#eceff4^  ^b#5e81ac^ %s%% ", "BAT0"},
	{disk_perc, "^b#2e3440^^c#81a1c1^  ^c#eceff4^%s%% ", "/"},
	{cpu_perc, "^b#a3be8c^^c#2e3440^ CPU ^b#2e3440^^c#a3be8c^ %s%% ", NULL},
	{ram_perc, "^c#81a1c1^  %s%% ", NULL },
	{wifi_essid, "^b#81a1c1^^c#2e3440^   ^b#2e3440^^c#81a1c1^ %s ", "wlan0"},
	{ datetime, "^b#81a1c1^^c#eceff4^  ^b#5e81ac^ %s ",           "%F %T" },  
};
