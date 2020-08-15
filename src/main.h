#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <stdio.h>
#include <fstream>
#include <cstring>
#include <utility>
using namespace std;
#include "core.h"
typedef void(*logprintf_t)(char* format, ...);

logprintf_t logprintf;
#define CHECK_PARAMS(m, n) \
	if (params[0] != (m * 4)) \
	{ \
		logprintf("%s: Expecting %d parameter(s), but found %d.", n, m, params[0] / sizeof(cell)); \
		return 0; \
	}