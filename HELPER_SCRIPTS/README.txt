HELPER_SCRIPTS — Build utilities for JUCE plugins
===================================================

Run all scripts from the project root directory.

PLUGIN_NAME is derived automatically from the repo directory name.

Scripts
-------
build_vst3.py       Build VST3 target          [--config Debug|Release]
build_standalone.py Build Standalone target     [--config Debug|Release]
build_au.py         Build AU target (macOS)     [--config Debug|Release]
build_tests.py      Build and run Catch2 tests  [--config Debug|Release]
rebuild_all.py      Full clean + rebuild        [--config --clean --target --generator]
regenSource.py      Regenerate CMAKE/SOURCES.cmake and CMAKE/TESTS.cmake
update_version.py   Bump patch version and write SOURCE/Util/Version.h

Shared utilities
----------------
build_complete.py   Provides find_cmake() and beep() — imported by all build scripts
SOUNDS/             WAV files for success/failure audio feedback (Windows)

Notes
-----
- find_cmake() checks PATH first, then common macOS install locations
- beep() plays SOUNDS/success_sound.wav or failure_sound.wav on Windows,
  falls back to system beep if WAV is missing, terminal bell on macOS/Linux
- regenSource.py scans SOURCE/ and any non-JUCE SUBMODULES/*/SOURCE/ directories
