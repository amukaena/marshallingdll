#include "SystemController.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <algorithm>
#include <sys/statvfs.h>
#include <fstream>
#include <vector>

int doResultShell(std::string shell, std::string& result);
int doResultShell2(std::string cmd, std::string& result);
int SetNetworkInfo(NetworkInfo* info);
void TestEcho(const char* msg)
{
	std::string cmd;
	cmd = "echo \"";
	cmd += msg;
	cmd += "\"";
	system(cmd.c_str());
}

EXT_API int SystemReboot()
{
	std::string cmd;
	cmd = "sudo reboot -f";
	return system(cmd.c_str());
}

EXT_API int SystemExcuteShell(char* file)
{
	std::string cmd;
	cmd = "sudo sh ";
	cmd += file;
	return system(cmd.c_str());
}

EXT_API int SystemFactoryResetNetwork()
{
	NetworkInfo prevInfo;
	SystemGetNetworkInfo(&prevInfo);

	NetworkInfo resetInfo;
	sprintf(resetInfo.dhcp, "static");
	sprintf(resetInfo.ip, "172.16.160.60");
	sprintf(resetInfo.netmask, "255.255.0.0");
	sprintf(resetInfo.gateway, "172.16.100.1");

	int ret = SetNetworkInfo(&resetInfo);
	if (ret != 0)
		SetNetworkInfo(&prevInfo);

	return ret;
}

EXT_API void SystemGetNetworkInfo(NetworkInfo* info)
{
	std::string answer = "";
	if (doResultShell2("cat /etc/network/interfaces | awk '/iface eth0 inet/ {gsub(\"iface\", \"\", $4); print $4}'", answer) != 0)
	{
		sprintf(info->dhcp, "%s", "None");
	}
	else
	{
		sprintf(info->dhcp, "%s", answer.c_str());
	}

	answer = "";
	if (doResultShell2("ifconfig eth0 | awk '/inet addr/ {gsub(\"addr:\", \"\", $2); print $2}'", answer) != 0)
	{
		sprintf(info->ip, "%s", "None");
	}
	else
	{
		sprintf(info->ip, "%s", answer.c_str());
	}

	answer = "";
	if (doResultShell2("ifconfig eth0 | awk '/inet addr/ {gsub(\"Mask:\", \"\", $4); print $4}'", answer) != 0)
	{
		sprintf(info->netmask, "%s", "None");
	}
	else
	{
		sprintf(info->netmask, "%s", answer.c_str());
	}

	answer = "";
	if (doResultShell2("route | awk '/default/ { print $2 }'", answer) != 0)
	{
		sprintf(info->gateway, "%s", "None");
	}
	else
	{
		sprintf(info->gateway, "%s", answer.c_str());
	}
}

EXT_API int SystemSetNetworkInfo(NetworkInfo* info)
{
	NetworkInfo prevInfo;
	SystemGetNetworkInfo(&prevInfo);

	int ret = SetNetworkInfo(info);
	if (ret != 0)
		SetNetworkInfo(&prevInfo);

	return ret;
}

EXT_API char* SystemGetSoundVolume()
{
	std::string answer = "";
	char* result = (char*)malloc(5 * sizeof(char));
	if (doResultShell2("amixer | awk '/Front Left:/ {gsub(\"Left: \", \"\", $4); print $4}'", answer) != 0)
	{
		sprintf(result, "%s", "None");
		return result;
	}
	else
	{
		answer.erase(0, 1);
		answer.erase(answer.find('%'), 2);
		sprintf(result, "%s", answer.c_str());
		return result;
	}
}

EXT_API int SystemSetSoundVolume(char* vol)
{
	std::string cmd;
	cmd = "amixer -c 0 sset 'softmaster1',0 ";
	cmd += vol;
	cmd += "% ";
	cmd += vol;
	cmd += "% on";
	return system(cmd.c_str());
}

EXT_API void SystemGetResourceInfo(ResourceInfo* info)
{
	// disk
	struct statvfs sv;
	statvfs("/", &sv);

	long totalDisk, availDisk;

	totalDisk = ((long long)sv.f_blocks * sv.f_bsize / 1024);
	availDisk = ((long long)sv.f_bavail * sv.f_bsize / 1024);

	info->disktotal = totalDisk;
	info->diskusage = totalDisk - availDisk;

	// memory
	std::string token;
	std::ifstream file("/proc/meminfo");

	long totalMemory = 0;
	long availMemory = 0;
	if (file.is_open())
	{
		while (file >> token)
		{
			if (token == "MemTotal:")
			{
				file >> totalMemory;
				continue;
			}

			if (token == "MemFree:")
			{
				file >> availMemory;
				break;;
			}
		}
		file.close();
	}

	info->memorytotal = totalMemory;
	info->memoryusage = totalMemory - availMemory;

	// cpu
	std::string line;
	file.open("/proc/stat");

	long totalCpu = 0;
	long usedCpu = 0;
	if (file.is_open())
	{
		getline(file, line);

		std::vector<std::string> output;
		std::string::size_type prev_pos = 0, pos = 0;

		while ((pos = line.find(' ', pos)) != std::string::npos)
		{
			std::string substring(line.substr(prev_pos, pos - prev_pos));

			if (substring.length() > 0)
				output.push_back(substring);

			prev_pos = ++pos;
		}
		output.push_back(line.substr(prev_pos, pos - prev_pos));

		for (int i = 1; i < output.size(); i++)
			totalCpu += std::stol(output[i]);

		usedCpu = std::stol(output[1]) + std::stol(output[2]) + std::stol(output[3]);

		file.close();
	}

	info->cputotal = totalCpu;
	info->cpuusage = usedCpu;

	// mac
	file.open("/sys/class/net/eth0/address");

	line = "";
	if (file.is_open())
	{
		getline(file, line);
		file.close();

		sprintf(info->mac, "%s", line.c_str());
	}
	else
		sprintf(info->mac, "%s", "None");

	// wirelessmac
	file.open("/sys/class/net/wlan0/address");

	line = "";
	if (file.is_open())
	{
		getline(file, line);
		file.close();

		sprintf(info->wirelessmac, "%s", line.c_str());
	}
	else
		sprintf(info->wirelessmac, "%s", "None");
}

EXT_API int SystemFactoryResetNetworkShell()
{
	std::string cmd;
	cmd = "sudo sh /home/cubox/K_Face_XT/SystemController/FactoryResetNetwork.sh";
	return system(cmd.c_str());
}

EXT_API void SystemGetNetworkInfoShell(NetworkInfo* info)
{
	std::string answer = "";
	if (doResultShell("/home/cubox/K_Face_XT/SystemController/GetNetworkDHCPInfo.sh", answer) != 0)
	{
		sprintf(info->dhcp, "%s", "None");
	}
	else
	{
		sprintf(info->dhcp, "%s", answer.c_str());
	}

	answer = "";
	if (doResultShell("/home/cubox/K_Face_XT/SystemController/GetNetworkIP.sh", answer) != 0)
	{
		sprintf(info->ip, "%s", "None");
	}
	else
	{
		sprintf(info->ip, "%s", answer.c_str());
	}

	answer = "";
	if (doResultShell("/home/cubox/K_Face_XT/SystemController/GetNetworkNetmask.sh", answer) != 0)
	{
		sprintf(info->netmask, "%s", "None");
	}
	else
	{
		sprintf(info->netmask, "%s", answer.c_str());
	}

	answer = "";
	if (doResultShell("/home/cubox/K_Face_XT/SystemController/GetNetworkGateway.sh", answer) != 0)
	{
		sprintf(info->gateway, "%s", "None");
	}
	else
	{
		sprintf(info->gateway, "%s", answer.c_str());
	}
}

int doResultShell(std::string shell, std::string& result)
{
	std::string cmd;
	cmd = "sh ";
	cmd += shell;

	FILE* pipe = popen(cmd.c_str(), "r");
	if (!pipe)
	{
		return -1;
	}

	char buffer[128];
	std::string res = "";
	while (fgets(buffer, sizeof(buffer), pipe))
		res += buffer;

	pclose(pipe);

	res.erase(std::remove(res.begin(), res.end(), '\n'), res.end());
	result = res;

	return 0;
}

int doResultShell2(std::string cmd, std::string& result)
{
	FILE* pipe = popen(cmd.c_str(), "r");
	if (!pipe)
	{
		return -1;
	}

	char buffer[128];
	std::string res = "";
	while (fgets(buffer, sizeof(buffer), pipe))
		res += buffer;

	pclose(pipe);

	res.erase(std::remove(res.begin(), res.end(), '\n'), res.end());
	result = res;

	return 0;
}

int SetNetworkInfo(NetworkInfo* info)
{
	std::string cmd;

	if (strcmp(info->dhcp, "dhcp") == 0)
	{
		cmd = "sed -i \"s/eth0 inet static/eth0 inet dhcp/g\" /etc/network/interfaces";
	}
	else
		cmd = "sed -i \"s/eth0 inet dhcp/eth0 inet static/g\" /etc/network/interfaces";
	int result = system(cmd.c_str());
	if (result != 0)
		return result;

	cmd = "sed -i \"6s/.*/address ";
	cmd += info->ip;
	cmd += "/g\" /etc/network/interfaces";
	result = system(cmd.c_str());
	if (result != 0)
		return result;

	cmd = "sed -i \"7s/.*/netmask ";
	cmd += info->netmask;
	cmd += "/g\" /etc/network/interfaces";
	result = system(cmd.c_str());
	if (result != 0)
		return result;

	cmd = "sed -i \"8s/.*/gateway ";
	cmd += info->gateway;
	cmd += "/g\" /etc/network/interfaces";
	result = system(cmd.c_str());
	if (result != 0)
		return result;

	cmd = "sudo ifdown eth0";
	result = system(cmd.c_str());
	if (result != 0)
		return result;

	cmd = "sudo ifup eth0";
	result = system(cmd.c_str());
	if (result != 0)
		return result;

	cmd = "ifconfig eth0 inet ";
	cmd += info->ip;
	cmd += " netmask ";
	cmd += info->netmask;
	result = system(cmd.c_str());
	if (result != 0)
		return result;

	cmd = "route add default gw ";
	cmd += info->gateway;
	cmd += " eth0";
	result = system(cmd.c_str());
	if (result == 1792)
		result = 0;

	return result;
}

EXT_API int SystemKillAPI()
{
	std::string cmd;
	cmd = "kill $(ps -ef | grep K_Face_XT_API | awk '/home/ {gsub(\"root\", \"\", $2); print $2}')";
	return system(cmd.c_str());
}