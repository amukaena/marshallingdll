#pragma once

#define  EXT_API extern "C" 

typedef struct NetworkInfo
{
	char dhcp[8];
	char ip[16];
	char netmask[16];
	char gateway[16];	
}NetworkInfo;

typedef struct ResourceInfo
{
	long disktotal;
	long diskusage;
	long memorytotal;
	long memoryusage;
	long cputotal;
	long cpuusage;
	char mac[18];
	char wirelessmac[18];
}ResourceInfo;


EXT_API int SystemReboot();
EXT_API int SystemExcuteShell(char* file);

EXT_API int SystemFactoryResetNetwork();
EXT_API void SystemGetNetworkInfo(NetworkInfo* info);
EXT_API int SystemSetNetworkInfo(NetworkInfo* info);

EXT_API char* SystemGetSoundVolume();
EXT_API int SystemSetSoundVolume(char* vol);

EXT_API void SystemGetResourceInfo(ResourceInfo* info);

EXT_API int SystemFactoryResetNetworkShell();
EXT_API void SystemGetNetworkInfoShell(NetworkInfo* info);