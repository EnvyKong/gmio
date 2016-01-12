/****************************************************************************
** gmio
** Copyright Fougue (2 Mar. 2015)
** contact@fougue.pro
**
** This software is a reusable library whose purpose is to provide complete
** I/O support for various CAD file formats (eg. STL)
**
** This software is governed by the CeCILL-B license under French law and
** abiding by the rules of distribution of free software.  You can  use,
** modify and/ or redistribute the software under the terms of the CeCILL-B
** license as circulated by CEA, CNRS and INRIA at the following URL
** "http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html".
****************************************************************************/

#include "stlb_infos_get.h"

#include "../../gmio_core/error.h"
#include "../../gmio_core/internal/byte_swap.h"

#include <string.h>

int gmio_stlb_infos_get(
        struct gmio_stl_infos_get_args* args,
        enum gmio_endianness byte_order,
        unsigned flags)
{
    if (flags != 0) {
        struct gmio_stl_infos* infos = &args->infos;
        uint32_t facet_count = 0;
        uint8_t buff[GMIO_STLB_HEADER_SIZE + sizeof(uint32_t)];

        { /* Read header and facet count into buff */
            const size_t read_size =
                    gmio_stream_read(&args->stream, buff, 1, sizeof(buff));
            if (read_size != sizeof(buff))
                return GMIO_ERROR_STREAM;
        }

        memcpy(&facet_count, buff + GMIO_STLB_HEADER_SIZE, sizeof(uint32_t));
        if (byte_order != GMIO_ENDIANNESS_HOST)
            facet_count = gmio_uint32_bswap(facet_count);

        if (flags & GMIO_STLB_INFO_FLAG_HEADER)
            memcpy(infos->stlb_header.data, buff, GMIO_STLB_HEADER_SIZE);
        if (flags & GMIO_STL_INFO_FLAG_FACET_COUNT)
            infos->facet_count = facet_count;
        if (flags & GMIO_STL_INFO_FLAG_SIZE) {
            infos->size =
                    GMIO_STLB_HEADER_SIZE
                    + sizeof(uint32_t)
                    + facet_count * GMIO_STLB_TRIANGLE_RAWSIZE;
        }
    }

    return GMIO_ERROR_OK;
}