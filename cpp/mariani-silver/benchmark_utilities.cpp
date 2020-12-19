#include "benchmark_utilities.h"

#ifdef _WIN32

#include <Windows.h>

void trySetRealtime() {
	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
}

#else
#include <iostream>

using namespace std;

void trySetRealtime() {
	cout << "Realtime process is not supported on this platform" << endl;
}
#endif
