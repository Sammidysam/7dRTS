#ifndef OSDETECT_H
#define OSDETECT_H

#ifdef _WIN64
#define OS 0
#elif _WIN32
#define OS 0
#elif __APPLE__
#define OS 2
#elif __linux
#define OS 1
#elif __unix
#define OS 1
#elif __posix
#define OS 1
#else
#define OS -1
#endif

#define WINDOWS 0
#define LINUX 1
#define MAC 2

#if OS == WINDOWS
#define PATH_SEP "\\"
#else
#define PATH_SEP "/"
#endif

#endif
