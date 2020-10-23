using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace SystemController
{
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct NetworkInfo
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 8)]
        public string dhcp;

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 16)]
        public string ip;

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 16)]
        public string netmask;

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 16)]
        public string gateway;
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct ResourceInfo
    {
        public int disktotal;
        public int diskusage;
        public int memorytotal;
        public int memoryusage;
        public int cputotal;
        public int cpuusage;

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 18)]
        public string mac;

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 18)]
        public string wirelessmac;
    }

    public class SystemController
    {
        [DllImport("SystemController.so", CallingConvention = CallingConvention.Cdecl)]
        extern public static int SystemReboot();

        [DllImport("SystemController.so", CallingConvention = CallingConvention.Cdecl)]
        extern public static int SystemExcuteShell(string file);

        [DllImport("SystemController.so", CallingConvention = CallingConvention.Cdecl)]
        extern public static int SystemFactoryResetNetwork();

        [DllImport("SystemController.so", CallingConvention = CallingConvention.Cdecl)]
        extern public static void SystemGetNetworkInfo([MarshalAs(UnmanagedType.Struct), In, Out] ref NetworkInfo info);

        [DllImport("SystemController.so", CallingConvention = CallingConvention.Cdecl)]
        extern public static int SystemSetNetworkInfo([MarshalAs(UnmanagedType.Struct), In, Out] ref NetworkInfo info);

        [DllImport("SystemController.so", CallingConvention = CallingConvention.Cdecl)]
        extern public static IntPtr SystemGetSoundVolume();

        [DllImport("SystemController.so", CallingConvention = CallingConvention.Cdecl)]
        extern public static int SystemSetSoundVolume(string vol);

        [DllImport("SystemController.so", CallingConvention = CallingConvention.Cdecl)]
        extern public static void SystemGetResourceInfo([MarshalAs(UnmanagedType.Struct), In, Out] ref ResourceInfo info);

        [DllImport("SystemController.so", CallingConvention = CallingConvention.Cdecl)]
        extern public static int SystemFactoryResetNetworkShell();

        public static void DoSystemReboot()
        {
            int result = -1;
            result = SystemReboot();
            Console.WriteLine("Result : " + result);
        }

        public static void DoSystemExcuteShell(string file)
        {
            int result = -1;
            result = SystemExcuteShell(file);
            Console.WriteLine("Result : " + result);
        }

        public static void DoSystemFactoryResetNetwork()
        {
            int result = -1;
            result = SystemFactoryResetNetwork();
            Console.WriteLine("Result : " + result);
        }

        public static void DoSystemGetNetworkInfo()
        {
            NetworkInfo info = new NetworkInfo();
            SystemGetNetworkInfo(ref info);

            Console.WriteLine("DHCP : " + info.dhcp);
            Console.WriteLine("ADDRESS : " + info.ip);
            Console.WriteLine("NETMASK : " + info.netmask);
            Console.WriteLine("GATEWAY : " + info.gateway);
        }

        public static void DoSystemSetNetworkInfo(NetworkInfo info)
        {
            int result = -1;
            result = SystemSetNetworkInfo(ref info);
            Console.WriteLine("Result : " + result);
        }

        public static void DoSystemGetSoundVolume()
        {
            IntPtr ptr = SystemGetSoundVolume();
            string str = Marshal.PtrToStringAnsi(ptr);
            Marshal.FreeHGlobal(ptr);
            Console.WriteLine("Volume : " + str);
        }

        public static void DoSystemSetSoundVolume(string vol)
        {
            int result = -1;
            result = SystemSetSoundVolume(vol);
            Console.WriteLine("Result : " + result);
        }

        public static void DoSystemGetResourceInfo()
        {
            ResourceInfo info = new ResourceInfo();
            SystemGetResourceInfo(ref info);

            Console.WriteLine("DiskTotal : " + info.disktotal);
            Console.WriteLine("DiskUsage : " + info.diskusage);
            Console.WriteLine("MemoryTotal : " + info.memorytotal);
            Console.WriteLine("MemoryUsage : " + info.memoryusage);
            Console.WriteLine("CPUTotal : " + info.cputotal);
            Console.WriteLine("CPUUsage : " + info.cpuusage);
            Console.WriteLine("MAC : " + info.mac);
            Console.WriteLine("WirelessMAC : " + info.wirelessmac);
        }

        public static void DoSystemFactoryResetNetworkShell()
        {
            int result = -1;
            result = SystemFactoryResetNetworkShell();
            Console.WriteLine("Result : " + result);
        }
    }
}
