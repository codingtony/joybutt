#!/bin/sh
[ -z "$1" ] && echo "you need to specify the joystick device" && exit 1
./joybutt -f $1 |  while read l
do 
	but=`echo $l  | grep ^0 | cut -b3-` 
	[ -z "$but" ] && continue
	case $but in
		0)
			echo "button 0";
			;;
		1)
			echo "button 1";
			;;
		2)
			echo "button 2";
			;;
		3)
			echo "button 3";
			;;
		4)	
			echo "button 4";
			;;
		*)
			echo "other button :"$but
			;;
	esac
		
done

