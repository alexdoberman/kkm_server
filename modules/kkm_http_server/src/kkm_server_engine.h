#pragma once
#include <ostream>

#include <types.h>
#include "request_types.h"
#include "config.h"

namespace kkm_request_engine
{
	TResult process_request_check(const TKKMConfig& cfg, const THeader &header, const TCheck & check, std::ostream& ssOut);
	TResult process_request_cancel_check(const TKKMConfig& cfg, const THeader &header, const TCancellationCheck & check, std::ostream& ssOut);
	TResult process_request_return_check(const TKKMConfig& cfg, const THeader &header, const TReturnCheck & check, std::ostream& ssOut);
	TResult process_request_report_x(const TKKMConfig& cfg, const THeader &header, const TReportX & report, std::ostream& ssOut);
	TResult process_request_report_z(const TKKMConfig& cfg, const THeader &header, const TReportZ & report, std::ostream& ssOut);
	TResult process_request_get_device(const TKKMConfig& cfg, const THeader &header, std::ostream& ssOut);

	TResult process_request_get_mode(const TKKMConfig& cfg, const THeader &header, std::ostream& ssOut);
	TResult process_request_reset_mode(const TKKMConfig& cfg, const THeader &header, std::ostream& ssOut);
	TResult process_request_annulate_check(const TKKMConfig& cfg, const THeader &header, std::ostream& ssOut);
	TResult process_request_exec_command(const TKKMConfig& cfg, const THeader &header, const TExecCommand & cmd , std::ostream& ssOut);
	TResult process_request_get_kkm_state(const TKKMConfig& cfg, const THeader &header, std::ostream& ssOut);

	TResult process_request_print_plain_text(const TKKMConfig& cfg, const THeader &header, const TPrintText& text,  std::ostream& ssOut);
	TResult process_request_jpos_print_text(const TKKMConfig& cfg, const THeader &header, const TPrintText& text,  std::ostream& ssOut);
}
