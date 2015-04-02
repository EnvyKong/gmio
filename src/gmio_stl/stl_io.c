/****************************************************************************
** GeomIO Library
** Copyright FougSys (2 Mar. 2015)
** contact@fougsys.fr
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

#include "stl_io.h"

#include "stl_error.h"
#include "stl_format.h"
#include "../gmio_core/error.h"
#include "../gmio_core/stream.h"
#include "../gmio_core/transfer.h"
#include "../gmio_core/internal/helper_stream.h"

int gmio_stl_read_file(
        const char* filepath,
        gmio_stl_mesh_creator_t* creator)
{
    int error = GMIO_ERROR_OK;
    FILE* file = NULL;

    file = fopen(filepath, "rb");
    if (file != NULL) {
        gmio_transfer_t trsf = { 0 };
        trsf.stream = gmio_stream_stdio(file);
        trsf.buffer = gmio_buffer_default();

        error = gmio_stl_read(&trsf, creator);
        fclose(file);
        gmio_buffer_deallocate(&trsf.buffer);
    }
    else {
        error = GMIO_ERROR_UNKNOWN;
    }

    return error;
}

int gmio_stl_read(gmio_transfer_t *trsf, gmio_stl_mesh_creator_t *creator)
{
    int error = GMIO_ERROR_OK;

    if (trsf != NULL) {
        const gmio_stl_format_t stl_format = gmio_stl_get_format(&trsf->stream);

        switch (stl_format) {
        case GMIO_STL_FORMAT_ASCII: {
            error = gmio_stla_read(trsf, creator);
            break;
        }
        case GMIO_STL_FORMAT_BINARY_BE: {
            error = gmio_stlb_read(trsf, creator, GMIO_ENDIANNESS_BIG);
            break;
        }
        case GMIO_STL_FORMAT_BINARY_LE: {
            error = gmio_stlb_read(trsf, creator, GMIO_ENDIANNESS_LITTLE);
            break;
        }
        case GMIO_STL_FORMAT_UNKNOWN: {
            error = GMIO_STL_ERROR_UNKNOWN_FORMAT;
        }
        } /* end switch() */
    }
    else {
        error = GMIO_ERROR_NULL_TRANSFER;
    }

    return error;
}
