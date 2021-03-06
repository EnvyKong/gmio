/****************************************************************************
** Copyright (c) 2017, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
**
**     1. Redistributions of source code must retain the above copyright
**        notice, this list of conditions and the following disclaimer.
**
**     2. Redistributions in binary form must reproduce the above
**        copyright notice, this list of conditions and the following
**        disclaimer in the documentation and/or other materials provided
**        with the distribution.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
****************************************************************************/

/*! \file global.h
 *  Fundamental global declarations, included by almost all other header/source
 *  files
 *
 *  \defgroup gmio_core gmioCore
 *  All other gmio modules rely on this module
 *
 *  \addtogroup gmio_core
 *  @{
 */

#pragma once

/* "config.h" is generated by cmake, it should reside in the out-of-source
 * build dir.
 * In CMakeFiles.txt, the directory where resides "config.h" is added to the
 * include path list
 */
#include "config.h"

/* GMIO_OS_WIN */
#if defined(WIN64) || defined(_WIN64) || defined(__WIN64__) \
      || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) \
      || defined(__NT__)
#  define GMIO_OS_WIN
#endif

/* GMIO_OS_LINUX */
#if defined(__linux) || defined(__linux__) || defined(linux)
#  define GMIO_OS_LINUX
#endif

/* GMIO_OS_MAC */
#if defined(__APPLE__)
#  define GMIO_OS_MAC
#endif

/* GMIO_DECL_IMPORT */
/* GMIO_DECL_EXPORT */
#ifdef GMIO_OS_WIN
#  define GMIO_DECL_EXPORT __declspec(dllexport)
#  define GMIO_DECL_IMPORT __declspec(dllimport)
#else
/*! Expands to the C compiler extension to export functions to a DLL */
#  define GMIO_DECL_EXPORT
/*! Expands to the C compiler extension to import functions from a DLL */
#  define GMIO_DECL_IMPORT
#endif

/* GMIO_API */
#ifdef GMIO_DLL
#  ifdef GMIO_MAKING_DLL
#    define GMIO_API GMIO_DECL_EXPORT
#  else
#    define GMIO_API GMIO_DECL_IMPORT
#  endif /* GMIO_MAKING_DLL */
#else
/*! Expands either to GMIO_DECL_EXPORT or GMIO_DECL_IMPORT when respectively
 *  compiling/using the DLL */
#  define GMIO_API
#endif

/* GMIO_HAVE_INT64_TYPE */
#if defined(GMIO_HAVE_INT64_T) \
    || defined(GMIO_HAVE_MSVC_INT64) \
    || defined(GMIO_HAVE_LONG_LONG)
#  define GMIO_HAVE_INT64_TYPE
#endif

/* Typedefs for specific width integers */
#ifdef GMIO_HAVE_STDINT_H
#  include <stdint.h>
#else
typedef char          int8_t;
typedef unsigned char uint8_t;

#  if GMIO_SIZEOF_SHORT == 2
typedef short          int16_t;
typedef unsigned short uint16_t;
#  else
#    error Not supported: sizeof(short) != 2
#  endif

#  if GMIO_SIZEOF_INT == 4
typedef int          int32_t;
typedef unsigned int uint32_t;
#  elif GMIO_SIZEOF_LONG == 4
typedef long          int32_t;
typedef unsigned long uint32_t;
#  else
#    error Failed to find a 32bit integer type with 'int' and 'long'
#  endif

#  ifndef GMIO_HAVE_INT64_T
#    if defined(GMIO_HAVE_MSVC_INT64)
typedef __int64_t          int64_t;
typedef unsigned __int64_t uint64_t;
#    elif defined(GMIO_HAVE_LONG_LONG)
typedef long long          int64_t;
typedef unsigned long long uint64_t;
#    endif
#endif

#  ifdef GMIO_HAVE_INT64_TYPE
typedef int64_t  intmax_t;
typedef uint64_t uintmax_t;
#  else
typedef int32_t  intmax_t;
typedef uint32_t uintmax_t;
#  endif

#  define INT16_MAX  0x7FFF
#  define UINT16_MAX 0xFFFF
#  define INT32_MAX  0x7FFFFFFF
#  define UINT32_MAX 0xFFFFFFFF

#endif

/* GMIO_HAVE_C99_BOOL */
#ifndef __cplusplus
#  if defined(GMIO_HAVE_C99_BOOL)
#    include <stdbool.h>
#  endif
#  if !defined(__bool_true_false_are_defined) && !defined(DOXYGEN)
typedef int bool;
enum gmio_bool_value
{
    false = 0,
    true = 1
};
#  define __bool_true_false_are_defined 1
#  endif
#endif

/* GMIO_UNUSED */
/*! Tells the compiler that a parameter is not used in the body of a function */
#define GMIO_UNUSED(x) (void)x;

/* GMIO_INLINE */
#ifndef GMIO_INLINE
#  if defined(__GNUC__)
#    define GMIO_INLINE __inline__ static  /* Compatible with C90 */
#  elif defined(_MSC_VER)
#    define GMIO_INLINE __inline static
#  elif !defined(DOXYGEN)
#    define GMIO_INLINE static
#  else
/*! Expands to the C compiler specific inline keyword (if any) */
#    define GMIO_INLINE
#  endif
#endif

/* GMIO_RESTRICT */
#ifndef GMIO_RESTRICT
#  if defined(__GNUC__)
#    define GMIO_RESTRICT __restrict__  /* Compatible with C90 */
#  elif defined(_MSC_VER)
#    define GMIO_RESTRICT __restrict
#  elif defined(GMIO_HAVE_C99_RESTRICT) /* TODO: add cmake detection */
#    define GMIO_RESTRICT restrict
#  else
/*! Expands to the C compiler specific restrict keyword (if any) */
#    define GMIO_RESTRICT
#  endif
#endif

/* GMIO_C_LINKAGE_BEGIN */
/* GMIO_C_LINKAGE_END */
#ifdef __cplusplus
#  define GMIO_C_LINKAGE_BEGIN extern "C" {
#  define GMIO_C_LINKAGE_END   }
#else
/*! Expands to <tt>extern "C" {</tt> when building with a C++ compiler */
#  define GMIO_C_LINKAGE_BEGIN
/*! Expands to \c } when building with a C++ compiler */
#  define GMIO_C_LINKAGE_END
#endif

/* GMIO_PRAGMA_MSVC_WARNING_PUSH_AND_DISABLE */
/* GMIO_PRAGMA_MSVC_WARNING_POP */
#if defined(_MSC_VER) && _MSC_VER >= 1400 /* Visual C++ 2008 */
#  define GMIO_PRAGMA_MSVC_WARNING_PUSH_AND_DISABLE(__code__) \
     __pragma(warning(push)) \
     __pragma(warning(disable: __code__))
#  define GMIO_PRAGMA_MSVC_WARNING_POP() \
     __pragma(warning(pop))
#else
/*! MSVC specific macro that disable the compiler warning of code \p __code__
 *
 * With Visual C++, expands to :
 *    \code{.c}
 *      #pragma warning(push)
 *      #pragma warning(disable: __code__)
 *    \endcode
 */
#  define GMIO_PRAGMA_MSVC_WARNING_PUSH_AND_DISABLE(__code__)
/*! MSVC specific macro that pop the changes made after last warning(pop)
 *
 * With Visual C++, expands to :
 *    \code{.c}
 *      #pragma warning(pop)
 *    \endcode
 */
#  define GMIO_PRAGMA_MSVC_WARNING_POP()
#endif

/*! Expands to the size(item count) of an array */
#define GMIO_ARRAY_SIZE(array) sizeof(array) / sizeof(*array)

/*! @} */
