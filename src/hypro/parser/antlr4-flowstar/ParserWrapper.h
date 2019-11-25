#pragma once

#include "ErrorListener.h"
#include "HybridAutomatonLexer.h"
#include "HybridAutomatonParser.h"
#include "HyproHAVisitor.h"


namespace hypro {

	template<typename Number>
	std::pair<HybridAutomaton<Number>, ReachabilitySettings> parseFlowstarFile(const std::string& filename);

} // namespace
