// landyloader.cpp : Defines the entry point for the console application.
//

#include "landyloader.h"

SC_HANDLE installDriver(LPCWSTR drivername, LPCWSTR binarypath) {
	SC_HANDLE scmHandle = NULL;
	SC_HANDLE svcHandle = NULL;

	//Grab a handle to the scm manager that will allow us to interact with services
	scmHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (scmHandle == NULL) {
		wprintf(L"Could not get handle to SCM\n");
		return NULL;
	}

	printf("%s    %s\n", drivername, binarypath);
	//Create our service using the CreateService Call
	svcHandle = CreateService(
		scmHandle,
		drivername,
		drivername,
		SERVICE_ALL_ACCESS,
		SERVICE_KERNEL_DRIVER,
		SERVICE_DEMAND_START,
		SERVICE_ERROR_NORMAL,
		binarypath,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
	);

	//check to make sure that our service was installed correctly
	if (svcHandle == NULL) {
		if (GetLastError() == ERROR_SERVICE_EXISTS) {
			//service already exsists
			wprintf(L"Service Already Exsists\n");
			CloseServiceHandle(scmHandle);
			return NULL;
		}
		else {
			//some error occured
			wprintf(L"Error Creating service: 0x%08lx\n", GetLastError());
			CloseServiceHandle(scmHandle);
		}
	}
	else {

	}

	//driver installed successfully
	wprintf(L"Driver %s Installed Correctly\n", drivername);
	CloseServiceHandle(scmHandle);
	
	return svcHandle;

}

int LoadDriver(SC_HANDLE schandle) {
	
	int status = StartService(schandle, 0, NULL);

	if (status == 0) {
		if (GetLastError() == ERROR_SERVICE_ALREADY_RUNNING) {
			wprintf(L"Service already running\n");
			return 1;
		}
		else {
			wprintf(L"Error starting service: 0x%08lx\n", GetLastError());
			return 1;
		}

		wprintf(L"Driver service has been started\n");
		return 0;
	}
	else {
		return 1;
	}

}


int main(int argc, wchar_t *argv[])
{
	if (argc != 3) {

		wprintf(L"Usage: landy-loader <driver name> <binary path>");

		return 0;
	}
	else {

		SC_HANDLE svchandle = NULL;

		std::wstring name = argv[1];
		std::wstring binpath = argv[2];

		svchandle = installDriver((LPCWSTR)name.c_str(), (LPCWSTR)binpath.c_str());

		if (svchandle != NULL) {
			LoadDriver(svchandle);
		}
	}

	return 0;
}
