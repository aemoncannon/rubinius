#include "vm.hpp"
#include "call_frame.hpp"
#include "vmmethod.hpp"
#include "trace_info.hpp"
#include "builtin/symbol.hpp"
#include "trace.hpp"


namespace rubinius {


	TraceInfo::TraceInfo()
		: exit_ip(-1),
		  exit_stack(NULL),
			exit_call_frame(NULL),
			entry_call_frame(NULL),
			expected_exit_ip(-1),
			nestable(0),
			recording(0)
	{}


}
