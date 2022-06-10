#!/bin/bash
iwddir="/var/lib/iwd"	
station="wlan0"
consym=""
strsym1=" "
strsym2=" "
strsym3=" "
function dmenuopt () { echo '-l 10'
}	#Dmenu options, so as not to clutter the rest of the script. These are the options that I use, change them if you'd like
function passlogin () {
	pass="'$(echo "" | dmenu $(dmenuopt) -p Password: | sed 's/ /\ /g')'" || exit 0	#This is called when doing a user-entered passphrase. Exits the program if a null value is entered, as that means no password, user is probably trying to leave
	echo $pass
	[[ $pass == "" ]] && exit 0		#Exits if [Enter] is pressed, thus selecting an empty character. different from null character
	iwctl station $station connect $selname --passphrase $pass --dont-ask && kill -43 $(pidof dwmblocks) || echo "Password incorrect, please try again" | dmenu $(dmenuopt)	#Attempts authentication, could run a script but that's optional, then exits, OR, if authentication failed, exits and gives error message
	echo $pass
}
while :; do

	[[ -z $(iwctl station $station get-networks | grep '>') ]] && iwctl station $station scan		#Refreshes nearby network cache, only works properly when disconnected from network, not fixable

	nets=$(iwctl station $station get-networks | sed -r '1,+3 d; $ d; s/\x1B\[(1;30|37|0)m(|\*{1,3}\x1B\[0m)//g; s/(^.{2})//')	#The backend of this script. Lists all networks nearby

	netsorted=""

	echo "$nets"
	for x in $(seq $(grep -c '*' <<< "$nets")); do #For as many numbers as there are networks detected;

		conname="$(sed -r "$x"' { s/>/'"$consym"' /; s/(^.{34}).*/\1/; s/ *$//}; '"$x"'!d' <<< "$nets")"	#Finds name. Dependent on whether we've reached the connection line yet

		name="$(sed -r 's/(^.{2})//' <<< $conname)"
		
		sec=$(sed -r "$x"' { s/(^.{38}).*/\1/; s/(^.{34})//; s/ *$//}; '"$x"'!d' <<< "$nets")	#Finds security

		str="$strsym1"	#Assumes strength is weak. If it gets changed later, it isn't weak.

		[[ "$(sed -r "$x"' { s/(^.{48}).*/\1/; s/(^.{44})//; s/ *$//}; '"$x"'!d' <<< "$nets")" == '***' ]] && str="$strsym2"	#Sets connection symbol/string

		[[ "$(sed -r "$x"' { s/(^.{48}).*/\1/; s/(^.{44})//; s/ *$//}; '"$x"' !d' <<< "$nets")" == '****' ]] && str="$strsym3"	#Same as before

		[[ "$x" != "1" ]] && netsorted="$netsorted\n"	#If this isn't the first line, add a separator.

		netsorted="$netsorted$str  $conname   [ $sec ]"	#If we've reached the connection line, and we haven't already done this, add a check mark to the current line, and then add all of the info to the end of the sorted variable.

	done


	sel=$(echo -e "$netsorted" | dmenu $(dmenuopt)) || exit 1	#Allow user to select line (network)
	selname="$(sed -r 's/\[ (psk|open|wep) \]//; s/ *$//; s/(^.{2} *)//' <<< "$sel")"	#Parses the selected name for future use
	echo "$selname" > ~/wifi
	selsec=$(awk '{ FS = " " ; print $4 }' <<< "$sel")	#Parses the selected security standard for future use 
	[[ -n "$(grep "$consym" <<< "$sel")" ]] && [[ "$(echo -e "N\nY" | dmenu $(dmenuopt) -p "Do you want to disconnect from your current network? [N/y]" | awk '{ print tolower($0) }')" == y ]] && iwctl station $station disconnect && killall -43 $(pidof dwmblocks)

	if [[ "$selsec" == "psk" ]] || [[ "$selsec" == wep ]]; then	#If the security standard is psk or wep;

		if [[ -f "$iwddir"'/'"$selname"'.psk' ]]; then	#If we have a saved password;

			if [[ "$(echo -e "Y\nN" | dmenu $(dmenuopt) -p "Use the saved password? [Y/n]" | awk '{ print tolower($0) }')" == "y" ]]	#If the user wants to use the saved password;

			then

				iwctl station $station connect "$selname" --dont-ask && kill -43 $(pidof dwmblocks) || echo "The saved password is incorrect" | dmenu $(dmenuopt)	#Connect!

			else

				passlogin	#Otherwise, login with user password

			fi

		else

			passlogin

		fi

	fi

	[[ "$selsec" == "open" ]] && iwctl station $station connect "$selname" && kill -43 $(pidof dwmblocks)	#Log in to network automatically if its open

done

echo "You have tried to connect to a network that isn't open, psk, or wep. Please file a bug report, thanks! :)" | dmenu $(dmenuopt) && exit 1		#debug message
