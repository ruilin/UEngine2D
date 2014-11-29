/*
 * sysinfo.h
 *
 *  Created on: 2014-3-30
 *      Author: ryan
 */

#ifndef SYSINFO_H_
#define SYSINFO_H_

#include <base.h>

struct Sysinfo {
	char *dataDirectory;
	char *downloadCacheDirectory;
	char *externalStorageDirectoryName;
	char *rootDirectory;
	char *packageCodePath;
	char *packageName;
	char *packageResourcePath;
	char *fileDir;
	char *appBackupAgentName;
	char *appClassName;
	char *appDataDir;
	char *appNativeLibDir;
	char *appProcessName;
	char *appPublicSourceDir;
	char *appSourceDir;
	char *appCallingPackage;
	char *buildBoard;
	char *buildBootloader;
	char *buildBrand;
	char *buildCPUABI;
	char *buildCPUABI2;
	char *buildDevice;
	char *buildDisplay;
	char *buildFingerPrint;
	char *buildHardware;
	char *buildHost;
	char *buildID;
	char *buildManufacturer;
	char *buildModel;
	char *buildProduct;
	char *buildTags;
	char *buildType;
	char *buildUser;
	char *buildVersionCodename;
	char *buildVersionRelease;
	char *buildVersionIncremental;
	char *buildVersionCode;
	char *appName;

	BOOL isExternalStorageRemovable;
	int taskId;
	unsigned int appBuildVersion;

	/* keeps track the incoming/outgoing bandwidth used, as reported by the net thread */
	unsigned long long bandwidth_in;		/* bytes */
	unsigned long long bandwidth_out;		/* bytes */
};

struct Sysinfo *sysinfo_init();
void sysinfo_destroy();

#endif /* SYSINFO_H_ */
