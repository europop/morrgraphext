//this file disables a lot of warnings
#pragma once

#include "WinHeader.h"
#include "../log.h"

#pragma warning(disable:4244) //possible data loss warning
#pragma warning(disable:4267) //possible data loss warning

#pragma warning(disable:4313) //C4313: '%x' in format string conflicts with argument 1 of type  'VPSCRIPT'
#pragma warning(disable:4800)
#pragma warning(disable:4996) //depreciated command warning
