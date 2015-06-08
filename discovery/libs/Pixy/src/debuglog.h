//
// begin license header
//
// This file is part of Pixy CMUcam5 or "Pixy" for short
//
// All Pixy source code is provided under the terms of the
// GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).
// Those wishing to use Pixy source code, software and/or
// technologies under different licensing terms should contact us at
// cmucam@cs.cmu.edu. Such licensing terms are available for
// all portions of the Pixy codebase presented here.
//
// end license header
//
#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdarg.h>
#include <stdio.h>

static void log(const char *format, ...)
{
#ifdef DEBUG
  va_list elements;

  // Send debug message to stdout //
  va_start(elements, format);
  vfprintf(stderr,format, elements);
  fflush(stderr);
  va_end(elements);
 #else
    (void)format;
#endif 
}

#endif