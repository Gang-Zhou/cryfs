#include "ProgramOptions.h"
#include <cstring>
#include <messmer/cpp-utils/assert/assert.h>

using namespace cryfs::program_options;
using std::string;
using std::vector;
using boost::optional;

ProgramOptions::ProgramOptions(const string &baseDir, const string &mountDir, const optional<string> &configFile,
                               bool foreground, const optional<double> &unmountAfterIdleMinutes,
                               const optional<string> &logFile, const optional<string> &cipher,
                               const optional<string> &extPass, const vector<char*> &fuseOptions)
    :_baseDir(baseDir), _mountDir(new char[mountDir.size()+1]), _configFile(configFile), _foreground(foreground),
     _cipher(cipher), _unmountAfterIdleMinutes(unmountAfterIdleMinutes), _logFile(logFile), _extPass(extPass),
     _fuseOptions(fuseOptions) {
    std::memcpy(_mountDir, mountDir.c_str(), mountDir.size()+1);
    // Fuse needs the mountDir passed as first option (first option = position 1, since 0 is the executable name)
    ASSERT(_fuseOptions.size() >= 1, "There has to be one parameter at least for the executable name");
    _fuseOptions.insert(_fuseOptions.begin()+1, _mountDir);
}

ProgramOptions::ProgramOptions(ProgramOptions &&rhs)
    :_baseDir(std::move(rhs._baseDir)), _mountDir(std::move(rhs._mountDir)), _configFile(std::move(rhs._configFile)),
     _foreground(std::move(rhs._foreground)), _cipher(std::move(rhs._cipher)),
     _unmountAfterIdleMinutes(std::move(rhs._unmountAfterIdleMinutes)), _logFile(std::move(rhs._logFile)),
     _extPass(std::move(rhs._extPass)), _fuseOptions(std::move(rhs._fuseOptions)) {
    rhs._mountDir = nullptr;
}

ProgramOptions::~ProgramOptions() {
    if (_mountDir != nullptr) {
        delete[] _mountDir;
    }
}

const string &ProgramOptions::baseDir() const {
    return _baseDir;
}

string ProgramOptions::mountDir() const {
    return string(_mountDir);
}

const optional<string> &ProgramOptions::configFile() const {
    return _configFile;
}

bool ProgramOptions::foreground() const {
    return _foreground;
}

const optional<double> &ProgramOptions::unmountAfterIdleMinutes() const {
    return _unmountAfterIdleMinutes;
}

const optional<string> &ProgramOptions::logFile() const {
    return _logFile;
}

const optional<string> &ProgramOptions::cipher() const {
    return _cipher;
}

const optional<string> &ProgramOptions::extPass() const {
    return _extPass;
}

const vector<char *> &ProgramOptions::fuseOptions() const {
    return _fuseOptions;
}
