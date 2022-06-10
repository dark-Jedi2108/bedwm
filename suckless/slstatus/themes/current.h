static const struct arg args[] = {
	/* function format          argument */
	{battery_perc, " ^b#bd93f9^^c#282a36^  ^b#282a36^^c#bd93f9^ %s%% ", "BAT0"},
	{disk_perc, "^b#282a36^^c#bd93f9^  ^c#ebdbb2^%s%% ", "/"},
	{cpu_perc, "^b#50fa7b^^c#282a36^ CPU ^b#282a36^^c#50fa7b^ %s%% ", NULL},
	{ram_perc, "^c#bd93f9^  %s%% ", NULL },
	{wifi_essid, "^b#bd93f9^^c#282a36^   ^b#282a36^^c#bd93f9^ %s ", "wlan0"},
	{ datetime, "^b#bd93f9^^c#282a36^  ^b#282a36^^c#bd93f9^ %s ",           "%F %T" },  
};
