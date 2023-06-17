// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here

#define WIN32_LEAN_AND_MEAN

#include "framework.h"
#include <Windows.h>
#include <detours.h>

#include <cstdint>
#include <cstdio>

#include <INIReader.h>
#include <Helpers.h>
#include "./rangers-api/rangers-api/rangers-api.h"
#include "./Signature.h"

//internal
#include "BlackboardStatus.h"
#include "sigscans.h"
#include "Frontiers-util.h"
#include "util.h"
#include "mod.h"


#endif //PCH_H
