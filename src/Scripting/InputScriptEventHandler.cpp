#include "InputScriptEventHandler.h"

#include <sol/sol.hpp>

#include "Library/Logger/Logger.h"

#include "ScriptingSystem.h"

InputScriptEventHandler::InputScriptEventHandler(sol::state_view solState) : PlatformEventFilter({ EVENT_KEY_PRESS }), _solState(solState) {}

bool InputScriptEventHandler::keyPressEvent(const PlatformKeyEvent *event) {
    sol::safe_function function = _solState["_globalOnKeyPress"];
    if (function.valid()) {
        try {
            sol::function errorHandler = _solState["_globalErrorHandler"];
            if (errorHandler.valid()) {
                function.set_error_handler(errorHandler);
            }
            auto result = function(event->key);
            if (result.valid()) {
                return result;
            }
        } catch (const sol::error &e) {
            logger->error(ScriptingSystem::ScriptingLogCategory, "An unexpected error has occurred: {}", e.what());
        }
    }

    return false;
}
