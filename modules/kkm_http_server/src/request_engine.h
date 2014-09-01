#pragma once
#include <kkm_driver.h>

#include "kkm_driver_type.h"
#include "request_types.h"

namespace kkm_request
{
    TResult check(TSmartKKMControl spKKMControl, TCheck check, int& nErrCode);
	TResult return_check(TSmartKKMControl spKKMControl, TReturnCheck check, int& nErrCode);
	TResult cancellation_check(TSmartKKMControl spKKMControl, TCancellationCheck check, int& nErrCode);

	TResult report_x(TSmartKKMControl spKKMControl, const TReportX &report, int& nErrCode);
	TResult report_z(TSmartKKMControl spKKMControl, const TReportZ &report, int& nErrCode);

	TResult print_plain_text(TSmartKKMControl spKKMControl, const TPrintText& text, int& nErrCode);

	TResult check_kkm_state(TSmartKKMControl spKKMControl, int& nErrCode);
};
