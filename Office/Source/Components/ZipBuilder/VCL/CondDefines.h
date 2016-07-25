#ifndef CondDefinesH
#define CondDefinesH

// When we allow building archives above 2GB
#define ALLOW_2G
// When we want to use the build-in strings
#define USE_ALLZIPSTRINGS
//#define DEBUGCALLBACK
//#define DEBUG_PROGRESS
// When we want to build without disk spanning then define NO_SPAN
//#define NO_SPAN
//When we want to build without the code supporting the stream functions define NO_STREAM
//#define NO_STREAM
//When we want to build without self-extracting code then define NO_SFX
//#define NO_SFX
// when we want to build with the old internal self-extracting code then define INTERNAL_SFX
//#define INTERNAL_SFX


#ifdef INTERNAL_SFX
#include "BuildIntSfx.h"
#undef NO_SFX
#endif


#endif
