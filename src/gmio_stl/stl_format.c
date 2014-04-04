#include "stl_format.h"

#include "stl_triangle.h"

#include "../gmio_core/endian.h"
#include "../gmio_core/internal/byte_codec.h"
#include "../gmio_core/internal/byte_swap.h"

#include <ctype.h>
#include <string.h>

#define _INTERNAL_GMIO_FIXED_BUFFER_SIZE  512

/*! \brief Returns the format of the STL data in \p stream
 *
 *  It will try to read 512 bytes from \p stream into a buffer and then analyses this data to guess
 *  the format.
 *
 *  Parameter \p data_size must provide the total size (in bytes) of the stream data (e.g. file
 *  size), it is required to guess endianness in case of binary format.
 *
 *  Returns GMIO_STL_UNKNOWN_FORMAT in case of error.
 */
gmio_stl_format_t gmio_stl_get_format(gmio_stream_t *stream, size_t data_size)
{
  char fixed_buffer[_INTERNAL_GMIO_FIXED_BUFFER_SIZE];
  size_t read_size = 0;

  if (stream == NULL || data_size == 0)
    return GMIO_STL_UNKNOWN_FORMAT;

  /* Read a chunk of bytes from stream, then try to find format from that */
  memset(fixed_buffer, 0, _INTERNAL_GMIO_FIXED_BUFFER_SIZE);
  read_size = gmio_stream_read(stream, &fixed_buffer, 1, _INTERNAL_GMIO_FIXED_BUFFER_SIZE);
  read_size = read_size < _INTERNAL_GMIO_FIXED_BUFFER_SIZE ? read_size :
                                                             _INTERNAL_GMIO_FIXED_BUFFER_SIZE;

  /* Binary STL ? */
  if (read_size >= (GMIO_STLB_HEADER_SIZE + 4)) {
    /* Try with little-endian format */
    uint32_t facet_count = gmio_decode_uint32_le((const uint8_t*)fixed_buffer + 80);

    if ((GMIO_STLB_HEADER_SIZE + 4 + facet_count*GMIO_STLB_TRIANGLE_RAWSIZE) == data_size)
      return GMIO_STL_BINARY_LE_FORMAT;

    /* Try with byte-reverted facet count */
    facet_count = gmio_uint32_bswap(facet_count);
    if ((GMIO_STLB_HEADER_SIZE + 4 + facet_count*GMIO_STLB_TRIANGLE_RAWSIZE) == data_size)
      return GMIO_STL_BINARY_BE_FORMAT;
  }

  /* ASCII STL ? */
  {
    /* Skip spaces at beginning */
    size_t pos = 0;
    while (pos < read_size && isspace(fixed_buffer[pos]))
      ++pos;

    /* Next token (if exists) must match "solid " */
    if (pos < _INTERNAL_GMIO_FIXED_BUFFER_SIZE
        && strncmp(fixed_buffer + pos, "solid ", 6) == 0)
    {
      return GMIO_STL_ASCII_FORMAT;
    }
  }

  /* Fallback case */
  return GMIO_STL_UNKNOWN_FORMAT;
}