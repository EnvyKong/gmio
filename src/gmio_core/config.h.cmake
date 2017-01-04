/****************************************************************************
** Copyright (c) 2016, Fougue Ltd. <http://www.fougue.pro>
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

/* Generated by CMake */

#ifndef GMIO_CONFIG_H_CMAKE
#define GMIO_CONFIG_H_CMAKE

/* Build type */
#cmakedefine GMIO_DEBUG_BUILD

/* Size(in bytes) of integer types */
#cmakedefine GMIO_SIZEOF_SHORT @GMIO_SIZEOF_SHORT@
#cmakedefine GMIO_SIZEOF_INT   @GMIO_SIZEOF_INT@
#cmakedefine GMIO_SIZEOF_LONG  @GMIO_SIZEOF_LONG@

/* 64b integer type */
#cmakedefine GMIO_HAVE_INT64_T
#cmakedefine GMIO_HAVE_MSVC_INT64
#cmakedefine GMIO_HAVE_LONG_LONG

/* C99 */
#ifndef GMIO_HAVE_STDINT_H
#cmakedefine GMIO_HAVE_STDINT_H
#endif

#ifndef GMIO_HAVE_C99_BOOL
#cmakedefine GMIO_HAVE_C99_BOOL
#endif

#cmakedefine GMIO_HAVE_STRTOF_FUNC
#cmakedefine GMIO_HAVE_POWF_FUNC
#cmakedefine GMIO_HAVE_SNPRINTF_FUNC
#cmakedefine GMIO_HAVE_WIN__SNPRINTF_FUNC
#cmakedefine GMIO_HAVE_VSNPRINTF_FUNC
#cmakedefine GMIO_HAVE_WIN__VSNPRINTF_FUNC

#cmakedefine GMIO_HAVE_ISFINITE_SYM
#cmakedefine GMIO_HAVE_WIN__FINITE_SYM
#cmakedefine GMIO_HAVE_ISNAN_SYM
#cmakedefine GMIO_HAVE_WIN__ISNAN_SYM

/* POSIX */
#cmakedefine GMIO_HAVE_SYS_TYPES_H
#cmakedefine GMIO_HAVE_SYS_STAT_H
#cmakedefine GMIO_HAVE_POSIX_FILENO_FUNC
#cmakedefine GMIO_HAVE_POSIX_FSTAT64_FUNC
#cmakedefine GMIO_HAVE_WIN__FSTAT64_FUNC

/* Compiler byte-swap functions */
#cmakedefine GMIO_HAVE_GCC_BUILTIN_BSWAP16_FUNC
#cmakedefine GMIO_HAVE_GCC_BUILTIN_BSWAP32_FUNC

#cmakedefine GMIO_HAVE_MSVC_BUILTIN_BSWAP_FUNC

/* Target architecture */
#cmakedefine GMIO_HOST_IS_BIG_ENDIAN

#if defined(__APPLE__)
#  if defined(__i386__) || defined(__ppc__)
#    define GMIO_TARGET_ARCH_BIT_SIZE  32
#  elif defined(__x86_64__) || defined(__ppc64__)
#    define GMIO_TARGET_ARCH_BIT_SIZE  64
#  else
#    error "Unknown architecture!"
#  endif
#else
#  define GMIO_TARGET_ARCH_BIT_SIZE  @GMIO_TARGET_ARCH_BIT_SIZE@
#endif

/* Select the string-to-float library to be used */
#define GMIO_STR2FLOAT_LIB_STD 0
#define GMIO_STR2FLOAT_LIB_IRRLICHT 1
#define GMIO_STR2FLOAT_LIB_DOUBLE_CONVERSION 2
#define GMIO_STR2FLOAT_LIB @GMIO_STR2FLOAT_LIBCODE@

/* Select the float-to-string library to be used */
#define GMIO_FLOAT2STR_LIB_STD 0
#define GMIO_FLOAT2STR_LIB_DOUBLE_CONVERSION 2
#define GMIO_FLOAT2STR_LIB @GMIO_FLOAT2STR_LIBCODE@

/* Header: gmio_core/internal/string_ascii_utils.h */
/*#define GMIO_STRING_ASCII_UTILS_CTYPE_H*/

#endif /* GMIO_CONFIG_H_CMAKE */
