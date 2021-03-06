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

#include "stl_utils.h"

#include "core_utils.h"
#include "../src/gmio_core/internal/min_max.h"
#include "../src/gmio_core/internal/numeric_utils.h"
#include "../src/gmio_core/internal/safe_cast.h"
#include "../src/gmio_core/internal/string.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void gmio_stl_nop_add_triangle(
        void *cookie, uint32_t tri_id, const struct gmio_stl_triangle *triangle)
{
    GMIO_UNUSED(cookie);
    GMIO_UNUSED(tri_id);
    GMIO_UNUSED(triangle);
}

struct gmio_stl_triangle_array gmio_stl_triangle_array_malloc(size_t tri_count)
{
    struct gmio_stl_triangle_array array = {0};
    if (tri_count > 0) {
        array.ptr =
                (struct gmio_stl_triangle*)calloc(
                    tri_count, sizeof(struct gmio_stl_triangle));
    }
    array.count = gmio_size_to_uint32(tri_count);
    array.capacity = array.count;
    return array;
}

void gmio_stl_triangle_array_free(struct gmio_stl_triangle_array *array)
{
    if (array != NULL) {
        free(array->ptr);
        array->ptr = NULL;
        array->count = 0;
        array->capacity = 0;
    }
}

static void gmio_stl_data__begin_solid(
        void* cookie, const struct gmio_stl_mesh_creator_infos* infos)
{
    struct gmio_stl_data* data = (struct gmio_stl_data*)cookie;

    if (infos->format == GMIO_STL_FORMAT_ASCII) {
        memset(data->solid_name, 0, sizeof(data->solid_name));
        if (infos->stla_solid_name != NULL) {
            gmio_cstr_copy(
                        data->solid_name,
                        sizeof(data->solid_name),
                        infos->stla_solid_name,
                        strlen(infos->stla_solid_name));
        }

        /* Try to guess how many vertices we could have assume we'll need
         * 200 bytes for each face */
        {
            const size_t facet_size = 200;
            const size_t facet_count =
                    gmio_streamsize_to_size(
                        GMIO_MAX(1, infos->stla_stream_size / facet_size));
            data->tri_array = gmio_stl_triangle_array_malloc(facet_count);
        }
    }
    else {
        memcpy(&data->header, infos->stlb_header, GMIO_STLB_HEADER_SIZE);
        data->tri_array =
                gmio_stl_triangle_array_malloc(infos->stlb_triangle_count);
    }
}

static void gmio_stl_data__add_triangle(
        void* cookie, uint32_t tri_id, const struct gmio_stl_triangle* triangle)
{
    struct gmio_stl_data* data = (struct gmio_stl_data*)cookie;
    if (tri_id >= data->tri_array.capacity) {
        uint32_t cap = data->tri_array.capacity;
        cap += cap >> 3; /* Add 12.5% more capacity */
        data->tri_array.ptr =
                realloc(data->tri_array.ptr, cap * sizeof(struct gmio_stl_triangle));
        data->tri_array.capacity = cap;
    }
    memcpy(&data->tri_array.ptr[tri_id], triangle, GMIO_STLB_TRIANGLE_RAWSIZE);
    data->tri_array.count = tri_id + 1;
}

static void gmio_stl_data__get_triangle(
        const void* cookie, uint32_t tri_id, struct gmio_stl_triangle* triangle)
{
    const struct gmio_stl_data* data = (const struct gmio_stl_data*)cookie;
    *triangle = data->tri_array.ptr[tri_id];
}

struct gmio_stl_mesh_creator gmio_stl_data_mesh_creator(struct gmio_stl_data *data)
{
    struct gmio_stl_mesh_creator creator = {0};
    creator.cookie = data;
    creator.func_begin_solid = &gmio_stl_data__begin_solid;
    creator.func_add_triangle = &gmio_stl_data__add_triangle;
    return creator;
}

struct gmio_stl_mesh gmio_stl_data_mesh(const struct gmio_stl_data *data)
{
    struct gmio_stl_mesh mesh = {0};
    mesh.cookie = data;
    mesh.triangle_count = data->tri_array.count;
    mesh.func_get_triangle = &gmio_stl_data__get_triangle;
    return mesh;
}

bool gmio_stl_triangle_equal(
        const struct gmio_stl_triangle *lhs,
        const struct gmio_stl_triangle *rhs,
        uint32_t max_ulp_diff)
{
    return gmio_vec3f_equal(&lhs->n, &rhs->n, max_ulp_diff)
            && gmio_vec3f_equal(&lhs->v1, &rhs->v1, max_ulp_diff)
            && gmio_vec3f_equal(&lhs->v2, &rhs->v2, max_ulp_diff)
            && gmio_vec3f_equal(&lhs->v3, &rhs->v3, max_ulp_diff)
            && lhs->attribute_byte_count == rhs->attribute_byte_count;
}

void gmio_stl_nop_get_triangle(
        const void *cookie, uint32_t tri_id, struct gmio_stl_triangle *triangle)
{
    GMIO_UNUSED(cookie);
    GMIO_UNUSED(tri_id);
    GMIO_UNUSED(triangle);
}
