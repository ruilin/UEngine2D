/*
 * sysinfo.c
 *
 *  Created on: 2014-3-30
 *      Author: ryan
 */

#include "sysinfo.h"

struct Sysinfo s;

PUBLIC struct Sysinfo *sysinfo_init() {
	char **stringArray; long long *longArray;
	stringArray = (char **)MALLOC(sizeof(unsigned int) * 39);
	longArray = (long long *)MALLOC(sizeof(long long) * 3);
	jni_lib_helper_getSysInfo(stringArray, 39, longArray, 3);

	s.externalStorageDirectoryName = stringArray[0];
	s.rootDirectory = stringArray[1];
	s.dataDirectory = stringArray[2];
	s.downloadCacheDirectory = stringArray[3];
	FREE((void *)stringArray[4]);
	s.fileDir = stringArray[5];
	s.packageCodePath = stringArray[6];
	s.packageName = stringArray[7];
	s.packageResourcePath = stringArray[8];
	s.appBackupAgentName = stringArray[9];
	s.appClassName = stringArray[10];
	s.appDataDir = stringArray[11];
	s.appNativeLibDir = stringArray[12];
	s.appProcessName = stringArray[13];
	s.appPublicSourceDir = stringArray[14];
	s.appSourceDir = stringArray[15];
	s.appCallingPackage = stringArray[16];
	s.buildBoard = stringArray[17];
	s.buildBootloader = stringArray[18];
	s.buildBrand = stringArray[19];
	s.buildCPUABI = stringArray[20];
	s.buildCPUABI2 = stringArray[21];
	s.buildDevice = stringArray[22];
	s.buildDisplay = stringArray[23];
	s.buildFingerPrint = stringArray[24];
	s.buildHardware = stringArray[25];
	s.buildHost = stringArray[26];
	s.buildID = stringArray[27];
	s.buildManufacturer = stringArray[28];
	s.buildModel = stringArray[29];
	s.buildProduct = stringArray[30];
	s.buildTags = stringArray[31];
	s.buildType = stringArray[32];
	s.buildUser = stringArray[33];
	s.buildVersionCodename = stringArray[34];
	s.buildVersionRelease = stringArray[35];
	s.buildVersionIncremental = stringArray[36];
	s.buildVersionCode = stringArray[37];
	s.appName = stringArray[38];

	s.taskId = (int)longArray[0];
	s.isExternalStorageRemovable = (0 == longArray[1] ? FALSE : TRUE);
	s.appBuildVersion = (unsigned int)longArray[2];
#if SYSINFO
	LOGI("external: %s", s.externalStorageDirectoryName);
	LOGI("root dir: %s", s.rootDirectory);
	LOGI("data dir: %s", s.dataDirectory);
	LOGI("download cache dir: %s", s.downloadCacheDirectory);
	LOGI("isExternalStorageRemovable: %s", (TRUE == s.isExternalStorageRemovable ? "true" : "false"));
	LOGI("filedir: %s", s.fileDir);
	LOGI("packageCodePath: %s", s.packageCodePath);
	LOGI("packageName: %s", s.packageName);
	LOGI("packageResourcePath: %s", s.packageResourcePath);
	LOGI("appName: %s", s.appName);
	LOGI("appBackupAgentName: %s", s.appBackupAgentName);
	LOGI("appClassName: %s", s.appClassName);
	LOGI("appDataDir: %s", s.appDataDir);
	LOGI("appNativeLibDir: %s", s.appNativeLibDir);
	LOGI("appProcessName: %s", s.appProcessName);
	LOGI("appPublicSourceDir: %s", s.appPublicSourceDir);
	LOGI("appSourceDir: %s", s.appSourceDir);
	LOGI("appCallingPackage: %s", s.appCallingPackage);
	LOGI("buildBoard: %s", s.buildBoard);
	LOGI("buildBootloader: %s", s.buildBootloader);
	LOGI("buildBrand: %s", s.buildBrand);
	LOGI("buildCPUABI: %s", s.buildCPUABI);
	LOGI("buildCPUABI2: %s", s.buildCPUABI2);
	LOGI("buildDevice: %s", s.buildDevice);
	LOGI("buildDisplay: %s", s.buildDisplay);
	LOGI("buildFingerPrint: %s", s.buildFingerPrint);
	LOGI("buildHardware: %s", s.buildHardware);
	LOGI("buildHost: %s", s.buildHost);
	LOGI("buildID: %s", s.buildID);
	LOGI("buildManufacturer: %s", s.buildManufacturer);
	LOGI("buildModel: %s", s.buildModel);
	LOGI("buildProduct: %s", s.buildProduct);
	LOGI("buildTags: %s", s.buildTags);
	LOGI("buildType: %s", s.buildType);
	LOGI("buildUser: %s", s.buildUser);
	LOGI("buildVersionCodename: %s", s.buildVersionCodename);
	LOGI("buildVersionRelease: %s", s.buildVersionRelease);
	LOGI("buildVersionIncremental: %s", s.buildVersionIncremental);
	LOGI("buildVersionCode (SDK version): %s", s.buildVersionCode);
	LOGI("taskId: %d", s.taskId);
	LOGI("app build version: %u", s.appBuildVersion);
#endif
	FREE((void *)longArray);
	FREE((void *)stringArray);
	return &s;
}

PUBLIC void sysinfo_destroy() {
	FREE((void *)s.buildBoard);
	FREE((void *)s.buildBootloader);
	FREE((void *)s.buildBrand);
	FREE((void *)s.buildCPUABI);
	FREE((void *)s.buildCPUABI2);
	FREE((void *)s.buildDevice);
	FREE((void *)s.buildDisplay);
	FREE((void *)s.buildFingerPrint);
	FREE((void *)s.buildHardware);
	FREE((void *)s.buildHost);
	FREE((void *)s.buildID);
	FREE((void *)s.buildManufacturer);
	FREE((void *)s.buildModel);
	FREE((void *)s.buildProduct);
	FREE((void *)s.buildTags);
	FREE((void *)s.buildType);
	FREE((void *)s.buildUser);
	FREE((void *)s.buildVersionCodename);
	FREE((void *)s.buildVersionRelease);
	FREE((void *)s.buildVersionIncremental);
	FREE((void *)s.buildVersionCode);
	FREE((void *)s.appCallingPackage);
	FREE((void *)s.appSourceDir);
	FREE((void *)s.appPublicSourceDir);
	FREE((void *)s.appProcessName);
	FREE((void *)s.appNativeLibDir);
	FREE((void *)s.appDataDir);
	FREE((void *)s.appClassName);
	FREE((void *)s.appBackupAgentName);
	FREE((void *)s.packageResourcePath);
	FREE((void *)s.packageName);
	FREE((void *)s.packageCodePath);
	FREE((void *)s.fileDir);
	FREE((void *)s.dataDirectory);
	FREE((void *)s.downloadCacheDirectory);
	FREE((void *)s.rootDirectory);
	FREE((void *)s.externalStorageDirectoryName);
	FREE((void *)s.appName);

	return;
}
