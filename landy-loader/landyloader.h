#pragma once

#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <string>

SC_HANDLE installDriver(LPCSTR drivername, LPCSTR binaryPath);

int loadDriver(SC_HANDLE schandle);