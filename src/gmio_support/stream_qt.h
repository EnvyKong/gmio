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

/*! \file stream_qt.h
 *  Support of Qt's \c QIODevice (requires at least Qt4)
 *
*  \addtogroup gmio_support
 *  @{
 */

#ifndef __cplusplus
#  error C++ compiler required
#endif

#pragma once

#include "support_global.h"
#include "../gmio_core/stream.h"
#include <QtCore/QtGlobal>

QT_BEGIN_NAMESPACE
class QIODevice;
QT_END_NAMESPACE

/*! Returns a gmio_stream for \c QIODevice* (cookie will hold \p device) */
struct gmio_stream gmio_stream_qiodevice(
#ifndef DOXYGEN
        QT_PREPEND_NAMESPACE(QIODevice)* device
#else
        QIODevice* device
#endif /* !DOXYGEN */
        );

/*! @} */
