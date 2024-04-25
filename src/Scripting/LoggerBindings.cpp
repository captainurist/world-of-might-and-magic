#include "LoggerBindings.h"

#include <Library/Logger/Logger.h>
#include <Library/Logger/DistLogSink.h>

#include <memory>

#include "ScriptLogSink.h"

LoggerBindings::LoggerBindings(const sol::state_view &solState, DistLogSink &distLogSink) : _solState(solState) {
    distLogSink.addLogSink(std::make_unique<ScriptLogSink>(solState));
}

sol::table LoggerBindings::getBindingTable() {
    if(!_bindingTable) {
        _bindingTable = _solState.create_table_with(
            "info", [](std::string_view message) { logger->info("{}", message); },
            "trace", [](std::string_view message) { logger->trace("{}", message); },
            "debug", [](std::string_view message) { logger->debug("{}", message); },
            "warning", [](std::string_view message) { logger->warning("{}", message); },
            "error", [](std::string_view message) { logger->error("{}", message); },
            "critical", [](std::string_view message) { logger->critical("{}", message); }
        );
    }
    return _bindingTable;
}
