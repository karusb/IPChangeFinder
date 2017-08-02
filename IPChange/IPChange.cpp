#include "stdafx.h"
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <locale>
#include <sstream>
#include <ctime>
#include <fstream>
#include <cmath>
#include <math.h>
//#define _WINSOCK_DEPRECATED_NO_WARNINGS_
using namespace std;
#pragma comment(lib,"ws2_32.lib")


char buffer[10000];
string website_HTML;
ofstream logfile;
////////////////////////
// Constant strings
const string LASTIPtext = "LASTIP:";
const string PATHtext = "PATH1:";
const string PATH2text = "PATH2:";
const string PATH3text = "PATH3:";
const string INTtext = "INTERVAL:";
const string STORJACtext = "STORJAC:";
const string STORJPATHtext = "STORJPATH:";
const char *start_text = "start runner.bat";
const char *batfilename = "runner.bat";
//****************************************************
// Functions
int getstringinputsize(string *inputfile, string tobefound);
int getstringinputsizeJSON(string *inputfile, string tobefound);
void reconfigureSTORJ(string IP, string PATH1, string PATH2 = "0", string PATH3 = "0");
void replacedatainpath(string PATH, string var, string data);
void get_Website(string url);
int main(void) {
	////////////////////////
	// Variables
	time_t timerstart;
	time_t timerend;
	string filename;
	string nicetime,formattedtime;
	formattedtime.resize(7);
	struct tm *y2k;
	
	string prev_ip;
	time(&timerstart);
	filename.append("LOG-");
	// Formatting time to MMMDDYY
	nicetime = asctime(localtime(&timerstart));
	for (int i = 4; i < 7; i++)formattedtime[i-4] = nicetime[i];
	for (int i = 8; i < 10; i++)formattedtime[i - 5] = nicetime[i];
	for (int i = 22; i < 24; i++)formattedtime[i - 17] = nicetime[i];
	filename.append(formattedtime);
	filename.append(".txt");
	cout << "Formatted Time:" << formattedtime << endl;
	cout << "LOG FILE: " << filename << endl;
	logfile.open(filename);
	if (!logfile.is_open())cout << "Error Opening Log File" << endl;
	filename.clear();
	ifstream configdata;
	ifstream appdata;
	ofstream oappdata;
	ofstream oconfigdata;
	ofstream runnerfile;

	string configfile;
	string appfile;
	char *timeint = "300000";
	char *storjac = "0";
	int storjac_int = 0;
	int timeint_int = 300000;
	string recprev_ip = "";
	string path;
	string path2;
	string path3;
	string storjpath = "0";
	storjpath = { " " };
	int storjpathsize = 0;
	int pathsize = 0;
	int path2size = 0;
	int path3size = 0;
	int timeintsize = 0;
	int recprev_ipsize = 0;
	int storjacsize = 0;

	size_t storjpathpos;
	size_t pathpos;
	size_t path2pos;
	size_t path3pos;
	bool storjpathposfound = false;
	bool pathposfound=false;
	bool pathposfound2=false;
	bool pathposfound3=false;
	size_t timepos;
	bool timeposfound=false;
	size_t prevIPpos;
	bool prevIPposfound=false;
	size_t storjacpos;
	bool storjacfound=false;

	bool appdatanotfound = false;
	bool configdatanotfound = false;

	////////////////////////////////////////////////////////////////
	// Reading previous appdata.ini which contains the last ip.
	appdata.open("appdata.ini"); // open previous app data
	if (!appdata.is_open())
	{
		logfile << "MAIN: APP data could not be opened, generating new app file" << endl;
		oappdata.open("appdata.ini");
		oappdata << "LASTIP: " << endl;
		oappdata.close();
		cout << "App data was not found, generated default appdata file." << endl;
		logfile << "MAIN: APP data output complete. Opening new app data." << endl;
		appdatanotfound = true;
		appdata.open("appdata.ini");
	}
	
	logfile << "MAIN: APP data opened successfully." << endl;
	appdata.seekg(0, std::ios::end);
	appfile.reserve(appdata.tellg());
	appdata.seekg(0, std::ios::beg);

	appfile.assign((std::istreambuf_iterator<char>(appdata)),
		std::istreambuf_iterator<char>());
	appdata.close();
	cout << appfile << endl;
	logfile << "MAIN: Getting previous IP size...";
	recprev_ipsize = getstringinputsize(&appfile, LASTIPtext);
	logfile << "IPSIZE: " << recprev_ipsize << endl;
	recprev_ip.resize(recprev_ipsize);// this plays important role in string comparison needs to be changed dynamically if possible
	logfile << "MAIN: Finding IP position in  APP file...";
	prevIPpos = appfile.find(LASTIPtext);
	//cout << prevIPpos << endl;
	//cout << appfile[prevIPpos] << endl;
	//cout << recprev_ip[prevIPpos] << endl;
	if (prevIPpos != std::string::npos)
	{
		logfile << "IP position found:" << prevIPpos << endl;
		//recprev_ip.clear();
		for (int i = 0; i < recprev_ipsize; i++) recprev_ip[i] = appfile[prevIPpos + i + 7]; // dynamically arrange limits??
		prevIPposfound = true;
	}
	cout << recprev_ip << endl;
	if (prevIPposfound) cout << "APP DATA Successfully Read" << endl;
	else cout << "APP Data not read" << endl;
	/////////////////////////////////////////////////////////////////////
	// Reading Config File Here
	logfile << "MAIN: Opening Config file..."<<endl;
	configdata.open("config.ini"); // open config data
	if (!configdata.is_open())
	{
		logfile << "MAIN: Config data could not be opened, generating new config file" << endl;
		oconfigdata.open("config.ini");
		oconfigdata << "STORJAC:0" << endl << "STORJPATH:C:/\"Program Files\"/\"Storj Share\"/\"Storj Share.exe\"" << endl << "PATH1:C:/Users/yourusername/.config/storjshare/configs/x.json " << endl << "PATH2:0 " << endl << "PATH3:0 " << endl << "INTERVAL:300000" << endl;
		oconfigdata.close();
		logfile << "MAIN: Config data output complete.Opening new config file." << endl;
		cout << "Config data could not be found. Generated default config file. Please edit paths for automatic STORJ configuration." << endl;
		configdatanotfound = true;
		configdata.open("config.ini");

	}

		logfile << "MAIN: Opening config file was successful." << endl;
		configdata.seekg(0, std::ios::end);
		configfile.reserve(configdata.tellg());
		configdata.seekg(0, std::ios::beg);

		configfile.assign((std::istreambuf_iterator<char>(configdata)),
			std::istreambuf_iterator<char>());
		configdata.close();

		logfile << "MAIN: Getting data sizes in config file..." << endl;
		storjacsize = getstringinputsize(&configfile, STORJACtext);
		storjpathsize = getstringinputsize(&configfile, STORJPATHtext);
		pathsize = getstringinputsize(&configfile, PATHtext);
		path2size = getstringinputsize(&configfile, PATH2text);
		path3size = getstringinputsize(&configfile, PATH3text);
		timeintsize = getstringinputsize(&configfile, INTtext);
		
		logfile << "MAIN: PATH SIZES- " <<storjacsize <<" "<< storjpathsize << " "<< pathsize << " " << path2size << " " << path3size << " " << timeintsize << " " << endl;
		
		storjpath.resize(storjpathsize);
		storjac = new char(storjacsize);
		path.resize(pathsize);
		path2.resize(path2size);
		path3.resize(path3size);
		timeint = new char(timeintsize);
		//storjac = new char(storjacsize);
		logfile << "Memory allocation for config variables complete..." << endl;
		//cout << "PATHSIZES:" << pathsize <<" " << path2size <<" " << path3size<< endl;
		//cout << "INTSIZE:" << timeintsize <<endl;
		logfile << "MAIN: Finding STORJAC variable...";
		storjacpos = configfile.find(STORJACtext);
		if (storjacpos != std::string::npos)
		{
			logfile << "STORJAC found" << endl;
			storjac[0] = char(configfile[storjacpos + 8]);
			storjacfound = true;

		}
		storjac_int = atoi(storjac);

		if (storjac_int == 1)
		{
			logfile << "MAIN: STORJAC is defined ON..." << endl;
			logfile << "MAIN: Starting to read PATH variables..." << endl;
			storjpathpos = configfile.find(STORJPATHtext);
			if (storjpathpos != std::string::npos)
			{
				logfile << "MAIN: STORJPATH found." << endl;

				for (int i = 0; i < storjpathsize; i++)
				{
					storjpath[i] = configfile[storjpathpos + i + 10];
					
				}
				storjpathposfound = true;
				logfile << "MAIN: Creating .bat file to run STORJ..." << endl;
				runnerfile.open(batfilename);
				runnerfile << "start " << storjpath << endl;
				runnerfile.close();
				logfile << "MAIN: .bat file created..." << endl;
			}
			pathpos = configfile.find(PATHtext);
			if (pathpos != std::string::npos)
			{
				logfile << "MAIN: PATH1 found." << endl;
				for (int i = 0; i < pathsize; i++)
				{
					path[i] = configfile[pathpos + i + 6];

				}
				pathposfound = true;
			}
			path2pos = configfile.find(PATH2text);
			if (path2pos != std::string::npos)
			{
				logfile << "MAIN: PATH2 found." << endl;
				for (int i = 0; i < path2size; i++)
				{
					path2[i] = configfile[path2pos + i + 6];

				}
				pathposfound2 = true;
			}
			path3pos = configfile.find(PATH3text);
			if (path3pos != std::string::npos)
			{
				logfile << "MAIN: PATH3 found." << endl;
				for (int i = 0; i < path3size; i++)
				{
					path3[i] = configfile[path3pos + i + 6];

				}
				pathposfound3 = true;
			}
		}
		timepos = configfile.find(INTtext);
		if (timepos != std::string::npos)
		{
			logfile << "MAIN: INTERVAL found." << endl;
			for (int i = 0; i < timeintsize; i++)
			{
				timeint[i] = configfile[timepos + i + 9];

			}

			timeposfound = true;

		}
		timeint_int = atoi(timeint);
		//cout << "Time int"<< timeint_int << endl;
		//  Check if required variables are found
		if ((pathposfound || !storjac_int) && timeposfound && (pathposfound2 || !storjac_int) && (pathposfound3 || !storjac_int) && storjacfound && (storjpathposfound || !storjac_int))
		{
			logfile << "MAIN: Config file passed integrity check." << endl;
			logfile << "STORJAC:" << storjac_int << endl << "STORJPATH:" << storjpath << endl << "PATH1:" << path << endl << "PATH2:" << path2 << endl << "PATH3:" << path3 << endl << "INTERVAL:" << timeint_int << endl;
			cout << "Config File Loaded Successfully" << endl;
			//cout << pathpos <<  endl;
			//cout << timepos << endl;
			cout << "STORJ AUTO CONFIGURATION: " << storjac_int << endl;
			if (storjpathposfound)cout << "STORJPATH: " << storjpath << endl;
			if (pathposfound)cout << "PATH1: " << path << endl;
			if (pathposfound2)cout << "PATH2: " << path2 << endl;
			if (pathposfound3)cout << "PATH3: " << path3 << endl;
			cout << "INTERVAL: " << timeint_int << endl;
		}
		else cout << "Config File Not Read Properly. Please delete config.ini to regenerate a default config file." << endl;
	

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Below is the main code for checking the IP in intervals.
	while (1)
	{
		website_HTML = "";
		y2k = 0;
		locale local;
		
		char lineBuffer[200][80] = { ' ' };
		char ip_address[16] = { "000.000.000.000"};

		int i = 0, bufLen = 0, j = 0, lineCount = 0;
		int lineIndex = 0, posIndex = 0;
		time(&timerstart);  /* get current time; same as: timer = time(NULL)  */
		time(&timerend);
		filename.append("LOG-");
		// Formatting time to MMMDDYY
		nicetime = asctime(localtime(&timerstart));
		for (int i = 4; i < 7; i++)formattedtime[i - 4] = nicetime[i];
		for (int i = 8; i < 10; i++)formattedtime[i - 5] = nicetime[i];
		for (int i = 22; i < 24; i++)formattedtime[i - 17] = nicetime[i];
		filename.append(formattedtime);
		filename.append(".txt");
		
		//if(!logfile.is_open())logfile.open(filename);
		filename.clear();
		//seconds = difftime(timerend, timerstart);
		cout << "\n\n\n";
		logfile << "MAIN: Starting API connection..." << endl;
		// CALL function to get IP using ipify.org API
		get_Website("api.ipify.org");
		for (size_t i = 0; i < website_HTML.length(); ++i) website_HTML[i] = tolower(website_HTML[i], local);

		istringstream ss(website_HTML);
		string stoken;
		
		logfile << "MAIN: Reading response..." << endl;
		while (getline(ss, stoken, '\n')) {

			cout <<"-->"<< stoken.c_str() << '\n';

			strcpy_s(lineBuffer[lineIndex], stoken.c_str());
			int dot = 0;
			for (int ii = 0; ii < strlen(lineBuffer[lineIndex]); ii++) {

				if (lineBuffer[lineIndex][ii] == '.') dot++;
				if (dot >= 3) {
					dot = 0;
					strcpy_s(ip_address, lineBuffer[lineIndex]);
				}
			}

			lineIndex++;
		}
		logfile << "Response read successfully." << endl;
		
		//cout << "Your IP Address is  " << ip_address << " \n\n";
		if (recprev_ip != ip_address)
		{
			logfile << "MAIN: IP address changed" << endl;
			cout << "IP Address Changed: " << ip_address << endl; // CALL CONFIGURATION FUNCTION HERE IF THE IP IS NOT THE SAME
			if (!appdatanotfound && !configdatanotfound)
			{
				logfile << "MAIN: Outputting new IP to appdata" << endl;
				oappdata.open("appdata.ini");
				oappdata << "LASTIP:" << ip_address << endl;
				oappdata.close();
			}
			else cout << "Configure config.ini and restart IPChange.exe" << endl;
			if(storjac_int == 1)reconfigureSTORJ(ip_address, path,path2,path3);
			else cout << "STORJ AUTO CONFIGURATION DISABLED" << endl;
		}
		else cout << "Last Updated: " << asctime(localtime(&timerstart)) << "IP: " << ip_address<<endl;
		logfile << "LAST UPDATED:" << asctime(localtime(&timerstart)) << endl;
		//recprev_ip.resize(16);
		recprev_ip = ip_address;
		Sleep(timeint_int);
		
		// USE BELOW METHOD IF YOU WANT THE CPU ACTIVE

		//while (difftime(timerend, timerstart) <= timeint_int/10 + 2)
		//{
		//	time(&timerend);
		//	//cout << difftime(timerend, timerstart) << endl;
		//}
		//free(lineBuffer);
	}
	//cout << "\nPress ANY key to close.\n\n";
	//cin.ignore(); cin.get();
	logfile.close();
	return 0;
}

//****************************************************

/*reconfigureSTORJ
@Author - Baris TANYERI
@Description - Reconfigures STORJ configuration files with the given IP address and .json paths.
@Dependencies - getstringinputsizeJSON(string *inputfile,string tobefound),replacedatainpath(string PATH, string var, string data), logfile, start_text
@libDependencies - fstream,iostream,sstream,windows.h
@param string IP - new IP address to be replaced with the existing IP address
@param string PATH1 - Path to .json file to be changed.
@param string PATH2 - Same as above but optional.
@param string PATH3 - Same as above but optional.
@return void

*/
void reconfigureSTORJ(string IP, string PATH1, string PATH2, string PATH3)
{
	logfile << "reconfigureSTORJ: Reconfiguration started..." << endl;
	ifstream storjconf1;
	ofstream ostorjconf1;
	string storjfile1;
	string storjfile2;
	size_t storjpos1;
	int datasize;
	size_t initsize, sizedif;
	const string RPCtext = "\"rpcAddress\": \"";
	const string ENDtext = "\",\n";
	logfile << "reconfigureSTORJ: Calling system to kill Storj Share GUI..." << endl;
	system("Taskkill /IM \"Storj Share.exe\" /F"); // kills storj share
	system("Taskkill /IM \"cmd.exe\" /F"); // this closes previous command windows that was created everytime storj gui is run

	//////////////////////////////////////////////////////////
	// READING GIVEN STORJ PATHS
	logfile << "reconfigureSTORJ: Opening PATHS..." << endl;
	replacedatainpath(PATH1, RPCtext, IP);
	replacedatainpath(PATH2, RPCtext, IP);
	replacedatainpath(PATH3, RPCtext, IP);
	
	logfile << "reconfigureSTORJ: Calling system to start STORJPATH" << endl;
	// Start_text must be defined!
	system(start_text);
}
/* int getstringinputsize (string inputfile, string tobefound)
	@Description  Counts the data size of the variable given in the config file
	@Dependencies - string.h
	@param string *inputfile - A reference to the string to be searched
	@param string tobefound - A string that is to be found in inputfile
	@return int - Returns the data size after the given variable to the end of line

	@example &inputfile where EOL represents end of line 
	LASTIP:192.168.1.1 EOL
	@example tobefound = "LASTIP:"
	@example return = 11

*/
int getstringinputsize(string *inputfile, string tobefound)
{
	size_t strpos;
	int size = 0;
	size_t eolpos;
	size_t datastartpos;
	strpos = inputfile->find(tobefound);
	datastartpos = inputfile->find(":",strpos);
	eolpos = inputfile->find("\n", datastartpos);
	bool eol = false;
	for (int i = datastartpos+1; i < inputfile->size() && !eol ; i++)
	{
		
		//cout << i;
		if (i == eolpos)eol = true;
		else
		{
			size += 1;
		}
	}
	return size;
}
/* int getstringinputsizeJSON (string inputfile, string tobefound)
@Description  Counts the data size of the variable given in the config file
@libDependencies - string.h
@param string *inputfile - A reference to the string to be searched
@param string tobefound - A string that is to be found in inputfile
@return int - Returns the data size after the given variable to the end of line

@notice - Doesn't work with integer variables, create another function and change find variable for eolpos to "," only.
@example &inputfile 
"rpcAddress": "192.168.1.1",
@example tobefound = \"rpcAddress\": \"
@example return = int 11

*/
int getstringinputsizeJSON(string *inputfile, string tobefound)
{
	size_t strpos;
	int size = 0;
	size_t eolpos;
	size_t datastartpos;
	strpos = inputfile->find(tobefound);
	datastartpos = inputfile->find(":", strpos);
	eolpos = inputfile->find("\",", datastartpos);
	bool eol = false;
	for (int i = datastartpos + 3; i < inputfile->size() && !eol; i++)
	{

		//cout << i;
		if (i == eolpos)eol = true;
		else
		{
			size += 1;
		}
	}
	return size;
}
/*
@Description - Replaces the given variable data with the given string data in the given PATH.
@Dependencies - logfile, getstringinputsizeJSON(string *inputfile, string tobefound)
@libDependencies - string.h, ifstream
@param string PATH - Path of the file to be read and re-written
@param string var - variable name in string such as "\"rpcAddress\": \""
@param string data - Data that is to be replaced with the data in the file
@notice - skips the whole process if path is set to char(0) 
*/
void replacedatainpath(string PATH, string var, string data)
{
	ifstream ifspath;
	ofstream oifspath;
	string sfile1;
	string sfile2;
	size_t datapos;
	size_t initsize, sizedif;
	int datasize;

	if (PATH != "0")
	{
		// PATH
		ifspath.open(PATH);
		if (ifspath.is_open())
		{
			logfile << "replacedatainpath: PATH Exists starting to read..." << endl;
			cout << "Reading PATH..." << endl;
			ifspath.seekg(0, std::ios::end);
			initsize = ifspath.tellg();
			sfile1.reserve(initsize);
			ifspath.seekg(0, std::ios::beg);

			sfile1.assign((std::istreambuf_iterator<char>(ifspath)),
				std::istreambuf_iterator<char>());
			ifspath.close();
			datapos = sfile1.find(var);
			//////////////////////////////////////////////////
			// CREATE YOUR OWN FUNCTION WHERE NECESSARY FOR DIFFERENT FORMATS
			datasize = getstringinputsizeJSON(&sfile1, var);
			//datasize = getstringinputsize(&sfile1, var);
			//////////////////////////////////////////////////
			sizedif = data.size() - datasize;
			sfile2.resize(initsize+sizedif);
			if (datapos != std::string::npos)
			{

				for (int i = 0; i < datapos; i++)sfile2[i] = sfile1[i];
				for (int i = datapos; i < var.size() + datapos; i++)sfile2[i] = var[i - datapos];
				for (int i = datapos + var.size(); i < var.size() + datapos + data.size(); i++)sfile2[i] = data[i - datapos - var.size()];
				//for (int i = datapos + var.size() + data.size(); i < var.size() + datapos + data.size() + ENDtext.size(); i++)sfile2[i] = ENDtext[i - datapos - data.size() - var.size()];
				for (int i = datapos + var.size() + data.size(); i < sfile1.size() + sizedif; i++)
				{
					if (sfile1[i - sizedif] != '\0')	sfile2[i] = sfile1[i - sizedif];
					//if (sfile1[i - sizedif] = 'NUL') sfile2[i] = char();
				}
				int exspace = 0;
				
				for (int lastpos = datapos + var.size() + data.size(); lastpos < sfile2.size(); lastpos++)
				{
					if (sfile2[lastpos] == '\0')exspace += 1;
				}
				ifspath.close();
				sfile2.resize(sfile2.size() - exspace);

				oifspath.open(PATH);
				oifspath << sfile2;
				oifspath.close();
				
			}
			cout << "PATH data variable changed." << endl;
			logfile << "replacedatainpath: PATH variable changed." << endl;
		}
		else
		{
			cout << "PATH Not Found." << endl;
			logfile << "replacedatainpath: PATH was not found or not accessible." << endl;
		}
	}
}
// Winsock function that sends http request to the given string url and reads to response to char buffer that is defined globally.
void get_Website(string url) {
	WSADATA wsaData;
	SOCKET Socket;
	SOCKADDR_IN SockAddr;
	int lineCount = 0;
	int rowCount = 0;
	struct hostent *host;
	string get_http;


	get_http = "GET / HTTP/1.1\r\nHost: " + url + "\r\nConnection: close\r\n\r\n";

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cout << "WSAStartup failed.\n";
		system("pause");
		//return 1;
	}

	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	host = gethostbyname(url.c_str());

	SockAddr.sin_port = htons(80);
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);

	while (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0) {
		cout << "Could not connect, trying again in 30 seconds.";
		Sleep(30000);
		//system("pause");
		//return 1;
	}
	send(Socket, get_http.c_str(), strlen(get_http.c_str()), 0);

	int nDataLength;
	while ((nDataLength = recv(Socket, buffer, 10000, 0)) > 0) {
		int i = 0;
		while (buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r') {

			website_HTML += buffer[i];
			i += 1;
		}
	}

	closesocket(Socket);
	WSACleanup();

}