https://github.com/karusb/IPChangeFinder

# IPChangeFinder
Detects when the router obtains a new IP. Useful for configuring applications automatically that require a static IP.

Has auto configuration option for STORJ Share GUI ready to use.
Autoconfiguration of STORJ Share is done by changing the configuration files of the STORJ Share GUI.
The algoritm finds the IP address line and replaces with the current IP address and restarts STORJ Share GUI for the re-configuration to take place.
When the STORJAC is NOT ON, the algorithm only reports the IP and saves the last known IP to appdata.ini.

# -INSTRUCTIONS
## Download
[IPChange V1.0 for Windows 64bit x64](https://github.com/karusb/IPChangeFinder/raw/master/IPChange-V1.0-x64.zip)
or
[IPChange V1.0 for Windows 32bit x86](https://github.com/karusb/IPChangeFinder/raw/master/IPChange-V1.0-x86.zip)

- Start IPChange.exe
- Close IPChange.exe
- Look at config.ini that was generated
- Change config.ini to your purposes described below
- Start IPChange.exe
- Your IP will be written to appdata.ini and keep the window open

# -CONFIGURATION
	*There are 6 variables in the configuration file and they need to be present for the program to work.
- STORJAC: Automatic configuration of STORJ "0" is off "1" is on.

- STORJPATH: File path to the Storj Share.exe GUI.
	*BE CAREFUL PATHS WITH SPACES MUST BE WRITTEN AS GIVEN WHICH ALSO APPLIES TO PATH1,PATH2,PATH3:
	*SIMPLY, QUOTE ANY PATH WITH SPACES
	*C:/"Program Files"/"Storj Share"/"Storj Share.exe"
- PATH1: File paths to STORJ node .json files. "0" means that path is not used. Currently capped at 3 configuration files.
	Usually .json files reside in:
	C:/Users/yourusername/.config/storjshare/configs/
	Make sure to give full path to the .json file you want to configure such as:
	C:/Users/yourusername/.config/storjshare/configs/dec1c096d71cb528c9e7f08e8f99187ee733e285.json
- PATH2: Same as PATH1 but your second node .json path.
- PATH3: Same as PATH1 but your third node .json path. 
- INTERVAL: Interval to check the public IP in milliseconds. Default is 300000 (5mins).

# -LOGGING
	Each day is logged in a separate text file to keep track of your previous IP's,
	 as well as some extra information for me to debug if you have any problems.
	 
# -TODO:
- MAKEFILE for easier compilation.
- Preprocessor definitions to compile with success = _WINSOCK_DEPRECATED_NO_WARNINGS;_CRT_SECURE_NO_WARNINGS
