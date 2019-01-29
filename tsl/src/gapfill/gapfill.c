/*
 * This file and its contents are licensed under the Timescale License.
 * Please see the included NOTICE for copyright information and
 * LICENSE-TIMESCALE for a copy of the license.
 */
#include <postgres.h>
#include <fmgr.h>
#include <utils/lsyscache.h>

#include "license.h"
#include "time_bucket.h"
#include "gapfill/gapfill.h"

Datum
gapfill_marker(PG_FUNCTION_ARGS)
{
	license_print_expiration_warning_if_needed();
	ereport(ERROR,
			(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
			 errmsg("%s can only be used in an aggregation query with time_bucket_gapfill", get_func_name(fcinfo->flinfo->fn_oid))));
	pg_unreachable();
}

#define GAPFILL_TIMEBUCKET_WRAPPER(datatype) \
TS_FUNCTION_INFO_V1(ts_gapfill_ ## datatype ## _bucket); \
Datum \
gapfill_ ## datatype ## _time_bucket(PG_FUNCTION_ARGS) \
{ \
	/* \
	 * since time_bucket is STRICT and time_bucket_gapfill \
	 * is not we need to add explicit checks for NULL here \
	 */ \
	if (PG_ARGISNULL(0) || PG_ARGISNULL(1)) \
		PG_RETURN_NULL(); \
	return DirectFunctionCall2(ts_ ## datatype ## _bucket, PG_GETARG_DATUM(0), PG_GETARG_DATUM(1)); \
}

GAPFILL_TIMEBUCKET_WRAPPER(int16);
GAPFILL_TIMEBUCKET_WRAPPER(int32);
GAPFILL_TIMEBUCKET_WRAPPER(int64);
GAPFILL_TIMEBUCKET_WRAPPER(date);
GAPFILL_TIMEBUCKET_WRAPPER(timestamp);
GAPFILL_TIMEBUCKET_WRAPPER(timestamptz);