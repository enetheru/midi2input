// Copyright (c) 2011 Seiya Tokui <beam.web@gmail.com>. All Rights Reserved.
// This source code is distributed under MIT License in LICENSE file.

#ifndef ELOG_THREAD_H_
#define ELOG_THREAD_H_

#ifdef _WIN32
# include "thread_win32.h"
#else
# include "thread_posix.h"
#endif

#endif  // ELOG_THREAD_H_
