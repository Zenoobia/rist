#if !defined(DIRECTORIES_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Joaqim Planstedt $
   ======================================================================== */

#define DIRECTORIES_H

#include <string>
#include <vector>

#include <chrono>
#include <iomanip>
#include <fstream>
#include <filesystem>

#include <Corrade/Utility/Directory.h
#include "Corrade/Containers/Array.h"
#include "Corrade/Utility/String.h"

/* Unicode helpers for Windows */
#ifdef CORRADE_TARGET_WINDOWS
#include "Corrade/Utility/Unicode.h"
#ifdef __MINGW32__
#include <fcntl.h>
#include <ext/stdio_filebuf.h>
#endif
using Corrade::Utility::Unicode::widen;
using Corrade::Utility::Unicode::narrow;
#endif

using namespace Corrade;

namespace Utility { namespace Directory {

namespace fs = std::filesystem;
using namespace std::chrono_literals;

    struct file_handle {
      fs::path path;
      std::time_t last_write_time;
    };

} // End of namespace Directory
} // End of namespace Utility
//} // End of namespace Corrade

#endif
