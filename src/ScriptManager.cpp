/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Joaqim $
   ======================================================================== */
#pragma once

#define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
#pragma warning(push)
#pragma warning(disable : 4996 4127) // Prevents c++17 Deprecation warnings //4127: conditional expression is constant
#include <sol.hpp>
#pragma warning(pop)

#include <Corrade/Utility/Directory.h>
#include <Corrade/Utility/Debug.h>
using namespace Corrade::Utility;

typedef sol::protected_function Function;

class ScriptManager {
public:

    bool executeScript(const std::string &scriptPath ){

        if(Directory::fileExists(scriptPath)) {
            _lua.script_file(scriptPath);
            return true;
        }
        Debug{} << "File not found: " << scriptPath;
        return false;
    }

    sol::table createTable(const std::string &tableName) {
        return _lua.create_named_table(tableName);
    }

    template <typename T>
    bool addFunction(const std::string &functionName, T &&function) {
        if(!_lua[functionName].valid()) {
            _lua[functionName] = function;
        }
        return _lua[functionName].valid();
    }

    Function getFunction(const std::string &functionName) {
        sol::protected_function f = _lua[functionName];
        return f;
    }
/*
        //f.error_handler = lua["handler"];

        sol::protected_function_result result = f(-500);
        if (result.valid()) {
                // Call succeeded
                int x = result;
        }
        else {
                // Call failed
                sol::error err = result;
                std::string what = err.what();
                // 'what' Should read
                // "Handled this message: negative number detected"
        }
        */

    private:
    sol::state _lua;
};

