
/******************************************************************************
*
*   FILE
*   ----
*   monitor.c
*
*   History
*   -------
*   2018-05-28   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_ressourcesMonitor
#define OV_COMPILE_LIBRARY_ressourcesMonitor
#endif


#include "ressourcesMonitor.h"
#include "libov/ov_macros.h"
#include "libov/ov_config.h"
#include "libov/ov_database.h"
#include "fb_database.h"

#if OV_SYSTEM_NT
#include <windows.h>
#endif

#if OV_SYSTEM_LINUX
#include <sys/utsname.h>
#endif

// Macro for server OS
// Copied from fb/fb_namedef.h
#if OV_SYSTEM_HPUX == 1
#define SERVER_SYSTEM "hpux"
#elif OV_SYSTEM_LINUX == 1
#define SERVER_SYSTEM "linux"
#elif OV_SYSTEM_NT == 1
#define SERVER_SYSTEM "nt"
#elif OV_SYSTEM_OPENVMS == 1
#define SERVER_SYSTEM "openvms"
#elif OV_SYSTEM_SOLARIS == 1
#define SERVER_SYSTEM "solaris"
#elif OV_SYSTEM_MC164 == 1
#define SERVER_SYSTEM "mc164"
#else
#define SERVER_SYSTEM "unknown"
#endif

// Macro for cpu architecture
// See https://sourceforge.net/p/predef/wiki/Architectures/ for reference
#if defined(__aarch64__)
#define SERVER_ARCH "arm64"
#elif defined (__arm__)
#define SERVER_ARCH "arm"
#elif defined(__amd64__) || defined(__x86_64__) || defined(_M_AMD64)
#define SERVER_ARCH "amd64"
#elif defined(__i386__) || defined(_X86_) || defined(__i686__) || defined(_M_ARM)
#define SERVER_ARCH "i386"
#else
#define SERVER_ARCH "unknown"
#endif

#define MAX_VERSION_STRING_LEN 255


/**
 * Uninlined function to update the list of currently loaded ov libraries on a monitor object.
 *
 * Code based on ov_vendortree_getlibraries()
 *
 * @param pinst The monitor instantce to update its library list
 */
static void monitor_update_libs(OV_INSTPTR_ressourcesMonitor_monitor pinst) {
	/* count associations */
	OV_UINT libs = 0;
	OV_INSTPTR_ov_library plib;
	Ov_ForEachChildEx(ov_instantiation, pclass_ov_library, plib, ov_library) {
		libs++;
	}
	OV_INSTPTR_ov_class pclass;
	Ov_ForEachChild(ov_inheritance, pclass_ov_library, pclass) {
		Ov_ForEachChildEx(ov_instantiation, pclass, plib, ov_library) {
			libs++;
		}
	}
	Ov_SetDynamicVectorLength(&pinst->v_ovLibs, libs, STRING);

	/* enter library paths into a string vector */
	libs = 0;
	Ov_ForEachChildEx(ov_instantiation, pclass_ov_library, plib, ov_library) {
		ov_string_setvalue(&(pinst->v_ovLibs.value[libs++]), plib->v_identifier);
	}
	Ov_ForEachChild(ov_inheritance, pclass_ov_library, pclass) {
		Ov_ForEachChildEx(ov_instantiation, pclass, plib, ov_library) {
			ov_string_setvalue(&(pinst->v_ovLibs.value[libs++]), plib->v_identifier);
		}
	}
}

#if OV_SYSTEM_NT && WINVER >= 0x0501
/**
 * Convert Windows API FILETIME structure to a simple uint64_t (alias unsigned long long)
 *
 * Source: https://stackoverflow.com/a/23148493
 */
static unsigned long long FileTimeToInt64(const FILETIME ft) {
	return (((unsigned long long)(ft.dwHighDateTime)) << 32) | ((unsigned long long)(ft.dwLowDateTime));
}
#endif



OV_DLLFNCEXPORT void ressourcesMonitor_monitor_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    OV_INSTPTR_ressourcesMonitor_monitor pinst = Ov_StaticPtrCast(ressourcesMonitor_monitor, pobj);

    /* do what the base class does first */
    fb_functionblock_startup(pobj);

    /* Initialize constant environment properties */
    ov_string_setvalue(&pinst->v_sysOS, SERVER_SYSTEM);
    ov_string_setvalue(&pinst->v_sysArch, SERVER_ARCH);

    /* Read Linux/Windows version */
#if OV_SYSTEM_LINUX
    struct utsname buf;
    uname(&buf);
    ov_string_setvalue(&pinst->v_sysOSVersion, buf.release);
#elif OV_SYSTEM_NT
    // See https://msdn.microsoft.com/de-de/library/windows/desktop/ms724451(v=vs.85).aspx
    // and https://msdn.microsoft.com/de-de/library/windows/desktop/ms724833(v=vs.85).aspx
    OSVERSIONINFO osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osvi);
    ov_string_print(&pinst->v_sysOSVersion, "%u.%u", osvi.dwMajorVersion, osvi.dwMinorVersion);
#endif

    // TODO find cpu type
    // TODO find mem size on Windows
}

OV_DLLFNCEXPORT void ressourcesMonitor_monitor_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    OV_INSTPTR_ressourcesMonitor_monitor pinst = Ov_StaticPtrCast(ressourcesMonitor_monitor, pfb);

    /* Update database size and usage */
    pinst->v_ovDBSize = ov_database_getsize() / 1024;
    pinst->v_ovDBUsed = ov_database_getused() / 1024;

    /* Update fb Urtask timings */
    pinst->v_ovFbUrCycTime = ((OV_INSTPTR_fb_task)fb_database_geturtask())->v_cyctime;
    pinst->v_ovFbUrCalcTime = ((OV_INSTPTR_fb_task)fb_database_geturtask())->v_calctime;

    /* Update OV libs */
    monitor_update_libs(pinst);

    /* Update CPU and memory usage */
#if OV_SYSTEM_LINUX
    /* CPU usage on Linux (read /proc/stat) */
    FILE *fp = fopen("/proc/stat", "r");
    if (fp) {
		char name[31];
		OV_UINT totalIdle, totalNonIdle, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
		// See https://www.kernel.org/doc/Documentation/filesystems/proc.txt
		// Section 1.8
		fscanf(fp, "%30s %u %u %u %u %u %u %u %u %u %u",
				name, &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal, &guest, &guest_nice);
		fclose(fp);
		// See https://stackoverflow.com/questions/23367857
		// or https://github.com/Leo-G/DevopsWiki/wiki/How-Linux-CPU-Usage-Time-and-Percentage-is-calculated
		totalIdle = idle + iowait;
		totalNonIdle = user + nice + system + irq + softirq + steal;

		pinst->v_cpuUsage = (OV_SINGLE)(totalNonIdle - pinst->v_cpuLastTicks + pinst->v_cpuLastIdleTicks)
				/ (OV_SINGLE)(totalNonIdle + totalIdle - pinst->v_cpuLastTicks);
		pinst->v_cpuLastTicks = totalIdle + totalNonIdle;
		pinst->v_cpuLastIdleTicks = totalIdle;
    }

    /* Memory usage on Linux (read /proc/meminfo) */
    fp = fopen("/proc/meminfo", "r");
	if (fp) {
	    OV_UINT memTotal = 0, memAvail = 0;
	    while (TRUE) {
			char name[33];
			unsigned long val;
			int res = fscanf(fp, "%32s %lu kB", name, &val);
			if (res <= 0)
				break;
			if (strcmp(name, "MemTotal:") == 0) {
				memTotal = val;
			} else if (strcmp(name, "MemAvailable:") == 0) {
				memAvail = val;
			}
	    }
		fclose(fp);

		pinst->v_memSize = memTotal;
		pinst->v_memUsed = memTotal - memAvail;
	}

#elif OV_SYSTEM_NT && WINVER >= 0x0501
	/* CPU usage on Windows (using GetSystemTimes()) */
	unsigned long long scale_factor = 25000;
	// See https://msdn.microsoft.com/de-de/library/windows/desktop/ms724400(v=vs.85).aspx
	// and https://stackoverflow.com/a/23148493
	// Requires WINVER >= 0x0501 (>= WinXP) to be set as compiler define flag
	FILETIME idleTime, kernelTime, userTime;
    GetSystemTimes(&idleTime, &kernelTime, &userTime);
    OV_UINT totalIdle = FileTimeToInt64(idleTime) / scale_factor;
    OV_UINT totalNonIdle = (FileTimeToInt64(kernelTime) - FileTimeToInt64(idleTime) + FileTimeToInt64(userTime)) / scale_factor;

	pinst->v_cpuUsage = (OV_SINGLE)(totalNonIdle - pinst->v_cpuLastTicks + pinst->v_cpuLastIdleTicks)
			/ (OV_SINGLE)(totalNonIdle + totalIdle - pinst->v_cpuLastTicks);
	pinst->v_cpuLastTicks = totalIdle + totalNonIdle;
	pinst->v_cpuLastIdleTicks = totalIdle;


	/* Memory usage on Windows (using GlobalMemoryStatusEx()) */
	// See https://msdn.microsoft.com/de-de/library/windows/desktop/aa366770(v=vs.85).aspx
	// Requires WINVER >= 0x0501 (>= WinXP) to be set as compiler define flag
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	pinst->v_memSize = memInfo.ullTotalPhys / 1024;
	pinst->v_memUsed = (memInfo.ullTotalPhys - memInfo.ullAvailPhys) / 1024;
#endif
}

