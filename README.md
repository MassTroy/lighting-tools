--------------------------------------------------------------------------
	General Information
--------------------------------------------------------------------------
lightning-format is a tool for formatting text file source code.
It supports languages such as C, C++, C#, and Java.
Other Languages with similar formats can also be formatted.
More info at <http://code.google.com/p/lightning-tools>

Usage:
	lformat [source]
		-formats and overwrites source file leaving a backup file
	lformat [source] [dest]
		-formats source file and saves in dest

	--copying 	-Displays copyright information
	--display 		-Displays current options
	--edit 		-Edit the options file
	--help 		-Displays this help content
	
	Please give credit where credit is due, and not remove the signature 
	line from your source code. It looks like: 
	/*This code was formatted using lightning-format V2.0*/


--------------------------------------------------------------------------
	Installation Instructions - Linux
--------------------------------------------------------------------------
	Easy Instructions (single user):
		1. Open the linux folder and run the file "SETUP.sh"
		2. Type in your password to allow root access
	Easy Instructions (all users):
		1. Open a terminal in the linux folder and run 
			"sudo SETUP.sh"
	Manual Installation (single user):
		1. Copy "lightning-format" to /usr/bin/
		2. Open your home folder and create a folder called 
			".lightning-format"
		3. Copy "lformat-prefs.txt" to /home/username/.lightning-format/
	Manual Installation (all users):
		1. Copy "lightning-format" to /usr/bin/
		2. Make the folder "/etc/lightning-tools/"
		3. Copy "lformat-prefs.txt" to /etc/lightning-tools/
		4. Navigate to /etc
		5. Create file called "lightning-format.conf"
		6. Open the file and type the exact path to the prefs file
			(ie. "/etc/lighting-tools/lformat-prefs.txt")
	Bonus
		1. To add the "Open in Terminal" right click menu entry
			(not required but extremely usefull!), open a 
			terminal window and type: 
			"sudo apt-get install nautilus-open-terminal"
	Tips
		- lightning-format will always use the prefs file in the current 
			folder if it exists.
		- You can move the default prefs file to any location. Just edit 
			the lightning-format.conf file located in "/etc" or 
			"~/.lightning-format" so that it has the exact path.


--------------------------------------------------------------------------
	Installation Instructions - Windows
--------------------------------------------------------------------------
		1. Install linux.
		2. Follow the linux instructions (above).
	Easy Instructions:
		1. Decide that you are too stuborn to upgrade to linux.
		2. Open the windows folder and run setup. Note, you must
			run the setup as aministrator.
				"SETUP_XP.bat" for systems up to windows XP
				"SETUP_7.bat" for windows vista and above
	Manual Installation:
		1. Create a folder called "bin" in your system drive
			(ie. "C:\bin").
		2. Copy "lightning-format.exe" and "lformat.bat" to bin.
		3. Create a folder called "ltools" in the bin folder.
		4. Copy "lformat-prefs.txt" to the ltools folder.
		5.1 Windows Vista and 7:
			Open command prompt and type 
				setx PATH="%PATH%;%SYSTEMDRIVE%\bin"
		5.2 Windows XP and before:
			Press WindowsKey + R and type
				%SystemDrive%\AUTOEXEC.bat
			And add the line (save and close):
				SET Path=%PATH%;%SystemDrive%\bin
	Bonus
		1. To add the ability to open CMD from any menu 
			(not required but extremely usefull!), open
			the file "OpenCommandPrompt.reg" and select yes.


--------------------------------------------------------------------------
	COPYRIGHT INFORMATION
--------------------------------------------------------------------------
	COPYRIGHT 2010, 2011 TROY MASS

	lightning-format is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	lightning-format is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with lightning-format. If not, see <http://www.gnu.org/licenses/>.
	
	Please give credit where credit is due, and not remove the signature 
	line from your source code. It looks like: 
	/*This code was formatted using lightning-format V2.0*/

		lightning-format version: 2.0
		COPYRIGHT (C) 2010, 2011 Troy Mass
		License GPLv3+: <http://www.gnu.org/licenses/gpl.html>

