#include "vm.hpp"
#include "call_frame.hpp"
#include "vmmethod.hpp"
#include "builtin/symbol.hpp"
#include "trace.hpp"
#include "trace_info.hpp"


namespace rubinius {

	TraceInfo::TraceInfo()
		:  exit_trace_node(NULL)
	{}

}
