/****************************************************************************
** gmio benchmarks
** Copyright Fougue (2 Mar. 2015)
** contact@fougue.pro
**
** This software provides performance benchmarks for the gmio library
** (https://github.com/fougue/gmio)
**
** This software is governed by the CeCILL-B license under French law and
** abiding by the rules of distribution of free software.  You can  use,
** modify and/ or redistribute the software under the terms of the CeCILL-B
** license as circulated by CEA, CNRS and INRIA at the following URL
** "http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html".
****************************************************************************/

#include "benchmark_tools.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#ifdef WIN32
#  include <windows.h>
#  define BENCHMARK_TIMER_WINDOWS
#else
#  include <time.h>
#  define BENCHMARK_TIMER_LIBC
#endif

/* Benchmark timers */

struct benchmark_timer
{
#ifdef BENCHMARK_TIMER_WINDOWS
    LARGE_INTEGER start_time;
    LARGE_INTEGER frequency;
#elif defined(BENCHMARK_TIMER_LIBC)
    clock_t start_tick;
#endif
};
typedef struct benchmark_timer benchmark_timer_t;

static void benchmark_timer_start(benchmark_timer_t* timer)
{
#ifdef BENCHMARK_TIMER_WINDOWS
    QueryPerformanceFrequency(&timer->frequency);
    QueryPerformanceCounter(&timer->start_time);
#elif defined(BENCHMARK_TIMER_LIBC)
    timer->start_tick = clock();
#endif
}

static int64_t benchmark_timer_elapsed_ms(const benchmark_timer_t* timer)
{
#ifdef BENCHMARK_TIMER_WINDOWS
    LARGE_INTEGER end_time = {0};
    LARGE_INTEGER elapsed = {0};
    QueryPerformanceCounter(&end_time);
    elapsed.QuadPart = end_time.QuadPart - timer->start_time.QuadPart;

    /*
     We now have the elapsed number of ticks, along with the
     number of ticks-per-second. We use these values
     to convert to the number of elapsed microseconds.
     To guard against loss-of-precision, we convert
     to milliseconds *before* dividing by ticks-per-second.
     */

    elapsed.QuadPart *= 1000;
    elapsed.QuadPart /= timer->frequency.QuadPart;
    return elapsed.QuadPart;
#elif defined(BENCHMARK_TIMER_LIBC)
    const clock_t elapsed_ticks = clock() - timer->start_tick;
    const double elapsed_ms = (elapsed_ticks * 1000) / ((double)CLOCKS_PER_SEC);
    return (int64_t)elapsed_ms;
#endif
}

/* Wraps around formatted printing functions */

/*! Wrapp around sprintf() to be used with gprintf_func_exec_time() */
static void sprintf_wrap(void* cookie, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vsprintf((char*)cookie, fmt, args);
    va_end(args);
}

/*! Wrap around printf() to be used with gprintf_func_exec_time() */
static void printf_wrap(void* cookie, const char* fmt, ...)
{
    va_list args;
    GMIO_UNUSED(cookie);
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

/*! Typedef on pointer to printf-wrap functions(eg. sprintf_wrap()) */
typedef void (*func_gprintf_t)(void*, const char*, ...);


/* Utilities */

/*! Calls printf(str) \p n times */
static void print_string_n(const char* str, size_t n)
{
    size_t i; /* for-loop index*/
    for (i = 0; i < n; ++i)
        printf(str);
}

/*! Safe wrapper around strlen() for NULL strings */
GMIO_INLINE size_t safe_strlen(const char* str)
{
    return str != NULL ? strlen(str) : 0;
}

/*! Returns the maximum of two size_t values */
GMIO_INLINE size_t size_t_max(size_t lhs, size_t rhs)
{
    return lhs > rhs ? lhs : rhs;
}

/*! String representation of the unit used for execution time */
static const char* unit_time_str = "ms";

/*! Generic formatted print of some execution time */
static void gprintf_func_exec_time(
        /* Annex data for func_gprintf (ex: char* for sprintf()) */
        void* cookie,
        /* Function ptr on a printf wrap (ex: sprintf_wrap()) */
        func_gprintf_t func_gprintf,
        /* Width of the print column, if any (can be == 0) */
        size_t width_column,
        /* Execution time, in ms */
        size_t time_ms,
        /* Valid execution time ? */
        gmio_bool_t has_time)
{
    if (has_time) {
        char str_time[128] = {0};
        /* TODO: %ull is not accepted by mingw, find a fix(maybe %ul64) */
        sprintf(str_time, "%u%s", (unsigned)time_ms, unit_time_str);
        if (width_column > 0)
            func_gprintf(cookie, "%-*s", width_column, str_time);
        else
            func_gprintf(cookie, "%s", str_time);
    }
    else {
        const char n_a[] = "N/A";
        if (width_column > 0)
            func_gprintf(cookie, "%-*s", width_column, n_a);
        else
            func_gprintf(cookie, "%s", n_a);
    }
}

/*! Helper for printf() around gprintf_func_exec_time() */
static void printf_func_exec_time(
        size_t width_column,
        size_t time_ms,
        gmio_bool_t has_time)
{
    gprintf_func_exec_time(
                NULL, &printf_wrap, width_column, time_ms, has_time);
}

/*! Returns the strlen of the longest tag string */
static size_t find_maxlen_cmp_result_tag(benchmark_cmp_result_array_t res_array)
{
    size_t max_len = 0;
    size_t i;
    for (i = 0; i < res_array.count; ++i) {
        const size_t len = safe_strlen(res_array.ptr[i].tag);
        max_len = size_t_max(len, max_len);
    }
    return max_len;
}

/*! Writes in output args the func1 execution informations */
static void select_cmp_result_func1_exec_infos(
        const benchmark_cmp_result_t* cmp, size_t* time, gmio_bool_t* has_time)
{
    *time = cmp->func1_exec_time_ms;
    *has_time = cmp->has_func1_exec_time;
}

/*! Writes in output args the func2 execution informations */
static void select_cmp_result_func2_exec_infos(
        const benchmark_cmp_result_t* cmp, size_t* time, gmio_bool_t* has_time)
{
    *time = cmp->func2_exec_time_ms;
    *has_time = cmp->has_func2_exec_time;
}

/*! Typedef on pointer to functions like select_cmp_result_funcX_exec_infos() */
typedef void (*func_select_cmp_result_func_exec_infos_t)(
        const benchmark_cmp_result_t*, size_t*, gmio_bool_t*);

/*! Returns the strlen of the longest execution time string */
static size_t find_maxlen_cmp_result_func_exec_time(
        benchmark_cmp_result_array_t res_array,
        func_select_cmp_result_func_exec_infos_t func_select_exec_infos)
{
    char strbuff[1024] = {0};
    size_t max_len = 0;
    size_t i;
    for (i = 0; i < res_array.count; ++i) {
        size_t time = 0;
        gmio_bool_t has_time = GMIO_FALSE;
        size_t len = 0;
        func_select_exec_infos(&res_array.ptr[i], &time, &has_time);
        gprintf_func_exec_time(strbuff, &sprintf_wrap, 0, time, has_time);
        len = safe_strlen(strbuff);
        max_len = size_t_max(len, max_len);
    }
    return max_len;
}

/* Implementation */

benchmark_cmp_result_t benchmark_cmp(benchmark_cmp_arg_t arg)
{
    benchmark_cmp_result_t result = {0};
    result.tag = arg.tag;

    if (arg.func1 != NULL) {
        benchmark_timer_t timer = {0};
        benchmark_timer_start(&timer);
        (*arg.func1)(arg.func1_filepath);
        result.func1_exec_time_ms = benchmark_timer_elapsed_ms(&timer);
        result.has_func1_exec_time = GMIO_TRUE;
    }
    if (arg.func2 != NULL) {
        benchmark_timer_t timer = {0};
        benchmark_timer_start(&timer);
        (*arg.func2)(arg.func2_filepath);
        result.func2_exec_time_ms = benchmark_timer_elapsed_ms(&timer);
        result.has_func2_exec_time = GMIO_TRUE;
    }

    return result;
}

void benchmark_print_results(
        benchmark_print_format_t format,
        benchmark_cmp_result_header_t header,
        benchmark_cmp_result_array_t result_array)
{
    if (format == BENCHMARK_PRINT_FORMAT_MARKDOWN) {
        const char* header_comp1 =
                header.component_1 != NULL ?  header.component_1 : "";
        const char* header_comp2 =
                header.component_2 != NULL ?  header.component_2 : "";
        const size_t width_tag_col =
                find_maxlen_cmp_result_tag(result_array);
        const size_t width_func1_col =
                size_t_max(
                    find_maxlen_cmp_result_func_exec_time(
                        result_array, &select_cmp_result_func1_exec_infos),
                    safe_strlen(header_comp1));
        const size_t width_func2_col =
                size_t_max(
                    find_maxlen_cmp_result_func_exec_time(
                        result_array, &select_cmp_result_func2_exec_infos),
                    safe_strlen(header_comp2));
        size_t i; /* for-loop index*/

        /* Print table header */
        printf("%*s | ", (int)width_tag_col, "");
        printf("%-*s | ", (int)width_func1_col, header_comp1);
        printf("%-*s\n", (int)width_func2_col, header_comp2);

        /* Print separation between header and results */
        print_string_n("-", width_tag_col + 1);
        printf("|");
        print_string_n("-", width_func1_col + 2);
        printf("|");
        print_string_n("-", width_func2_col + 2);
        printf("\n");

        /* Print benchmark result lines */
        for (i = 0; i < result_array.count; ++i) {
            const benchmark_cmp_result_t result = result_array.ptr[i];
            printf("%-*s | ", (int)width_tag_col, result.tag);
            printf_func_exec_time(
                        width_func1_col,
                        result.func1_exec_time_ms,
                        result.has_func1_exec_time);
            printf(" | ");
            printf_func_exec_time(
                        width_func2_col,
                        result.func2_exec_time_ms,
                        result.has_func2_exec_time);
            printf("\n");
        }
    }
}

void benchmark_cmp_batch(
        size_t run_count,
        const benchmark_cmp_arg_t *arg_array,
        benchmark_cmp_result_t *result_array,
        void (*func_init)(),
        void (*func_cleanup)())
{
    size_t run; /* for-loop index */
    size_t array_size = 0;
    while (arg_array[array_size].tag != NULL) {
        ++array_size;
    }

    for (run = 0; run < run_count; ++run) {
        size_t i; /* for-loop index */
        /* Init */
        if (func_init)
            (*func_init)();

        for (i = 0; i < array_size; ++i) {
            const benchmark_cmp_result_t ires = benchmark_cmp(arg_array[i]);
            benchmark_cmp_result_t* fres = &result_array[i];
            if  (run != 0) {
                if (fres->func1_exec_time_ms > ires.func1_exec_time_ms)
                    fres->func1_exec_time_ms = ires.func1_exec_time_ms;
                if (fres->func2_exec_time_ms > ires.func2_exec_time_ms)
                    fres->func2_exec_time_ms = ires.func2_exec_time_ms;
            }
            else {
                *fres = ires;
            }
        }

        /* Cleanup */
        if (func_cleanup)
            (*func_cleanup)();
    }
}
