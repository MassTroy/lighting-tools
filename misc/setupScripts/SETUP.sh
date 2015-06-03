#!/bin/bash
#
#	COPYRIGHT 2010, 2011 TROY MASS
#	
#	This file is part of lightning-format.
#
#	lightning-format is free software: you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	(at your option) any later version.
#
#	lightning-format is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with lightning-format. If not, see <http://www.gnu.org/licenses/>.
#

# This is the instalation script for lightning-format on linux
#
# COPYRIGHT TROY MASS 2010
#
# License under the GNU/GPL V3

if [ $UID != 0 ] ; then
	echo -e "\nInstalling lighting-format for $USER"
	echo "This script requires reoot permissions. Typing sudo for you: "
	
	sudo cp -u -f "./lightning-format" "/usr/bin/lightning-format"""
	sudo chmod +rx+rx+rx "/usr/bin/lightning-format"
	sudo ln -f "/usr/bin/lightning-format" "/usr/bin/lformat"
	sudo chmod +rx+rx+rx "/usr/bin/lformat"
	#sudo ln -f "/usr/bin/lightning-format" "/usr/bin/lightf"
	#sudo chmod +rx+rx+rx "/usr/bin/lightf"
	
	mkdir -p "$HOME/.lightning-format"
	cp -u -f "./lformat-prefs.txt" "$HOME/.lightning-format/lformat-prefs.txt"
	sudo chmod +rw+rw+r "$HOME/.lightning-format/lformat-prefs.txt"
	
	touch "$HOME/.lightning-format/lightning-format.conf"
	echo "$HOME/.lightning-format/lformat-prefs.txt" > "$HOME/.lightning-format/lightning-format.conf"""
	echo -e "Installed:\n\t/usr/bin/lightning-format\n\t/usr/bin/lformat\n\t$HOME/.lightning-format/lformat-prefs.txt\n\t$HOME/.lightning-format/lightning-format.conf"
else
	echo -e "\nInstalling lighting-format for all users"
	
	cp -u -f "./lightning-format" "/usr/bin/lightning-format"
	chmod +rx+rx+rx "/usr/bin/lightning-format"
	ln -f "/usr/bin/lightning-format" "/usr/bin/lformat"
	chmod +rx+rx+rx "/usr/bin/lformat"
	#ln -f "/usr/bin/lightning-format" "/usr/bin/lightf"
	#chmod +rx+rx+rx "/usr/bin/lightf"
	
	mkdir -p "/etc/lightning-tools"
	cp -u -f "./lformat-prefs.txt" "/etc/lightning-tools/lformat-prefs.txt"
	chmod +r+r+r "/etc/lightning-tools/lformat-prefs.txt"
	
	touch "/etc/lightning-tools/lightning-format.conf"
	echo "/etc/lightning-tools/lformat-prefs.txt" > "/etc/lightning-format.conf"
	chmod +r+r+r "/etc/lightning-tools/lightning-format.conf"
	echo -e "Installed:\n\t/usr/bin/lightning-format\n\t/usr/bin/lformat\n\t/etc/lightning-tools/lightning-format.conf\n\t/etc/lightning-tools/lformat-prefs.txt"
fi

echo -e "\nDone installing"
echo -e "\nUse lightining format by typing \n\t\"lightning-format [source] [destination]\" or\n\t\"lformat [source] [destination]\""
