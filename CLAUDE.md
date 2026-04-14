# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Flanger 2.0 is a JUCE-based stereo flanger audio plugin (VST3/AU/Standalone) built with CMake. It uses a custom wavetable LFO to modulate two independent delay lines with feedback.

## Build Commands

All build scripts are run from the **project root** using Python 3.

```bash
# Build VST3 (also regenerates CMAKE/SOURCES.cmake)
python HELPER_SCRIPTS/build_vst3.py --config Debug

# Build Standalone app
python HELPER_SCRIPTS/build_standalone.py --config Debug

# Build AU plugin (macOS only)
python HELPER_SCRIPTS/build_au.py --config Debug

# Build and run tests (requires Catch2, fetched automatically)
python HELPER_SCRIPTS/build_tests.py --config Debug

# Full clean rebuild
python HELPER_SCRIPTS/rebuild_all.py --config Debug --clean
```

### Manual CMake Build

```bash
cmake -B BUILD -DCMAKE_BUILD_TYPE=Debug
cmake --build BUILD --target Flanger_2_0_VST3 --config Debug
cmake --build BUILD --target Flanger_2_0_Standalone --config Debug
cmake --build BUILD --target Tests --config Debug  # requires -DBUILD_TESTS=ON
```

### Adding/Removing Source Files

After adding or removing `.h`/`.cpp` files in `Source/`, regenerate the CMake source lists:

```bash
python HELPER_SCRIPTS/regenSource.py
```

This updates `CMAKE/SOURCES.cmake` and `CMAKE/TESTS.cmake`. Failure to do this will cause missing/stale source errors at build time.

### Version Management

Version is defined in `VERSION.txt` (format: `major.minor.patch`). CMake automatically invokes `HELPER_SCRIPTS/update_version.py` before each build to regenerate `Source/Util/Version.h` with `BUILD_VERSION_*` macros. Never edit `Version.h` directly.

## Architecture

### Audio Processing Pipeline

```
processBlock()
  └── per-channel, per-sample:
        DelayBase::processSample()
          ├── WaveTable LFO → modulation offset
          ├── DelayLine::pushSample() (with feedback)
          └── DelayLine::popSample() (Lagrange 3rd-order interpolation)
```

**`FlangerAudioProcessor`** (`Source/Processor/PluginProcessor.h/cpp`) owns two `DelayBase` instances (one per channel) in an `OwnedArray<DelayBase>`. Parameters are managed via JUCE's `AudioProcessorValueTreeState` (APVTS). Preset save/load uses APVTS serialization to XML.

**`DelayBase`** (`Source/Processor/DelayBase.h`, header-only) is the core DSP unit. Key internals:
- `juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Lagrange3rd>` — max delay 75ms
- `juce::SmoothedValue<float>` — 0.5s ramp for delay time changes (prevents clicks)
- `WaveTable` LFO — modulates delay time in samples
- LFO frequency multiplier controlled by mode: Low (0.1×), Normal (1×), FM (10×)

**`WaveTable`** (`Source/Util/WaveTable.h`, header-only) — sine wavetable LFO with 2048 samples and linear interpolation. Provides `getNextSample()` for sample-accurate modulation.

### UI Architecture

**`FlangerAudioProcessorEditor`** (`Source/Components/PluginEditor.h/cpp`) uses a `juce::Timer` callback for animation. Child components:
- `TapeReel` × 2 — animated rotating reel visualization (speed tied to LFO rate)
- `LFOSliders` × 2 — knob groups for Rate, Depth, Delay, Mix, Feedback per channel
- `PresetMenu` — preset browser with save/load/delete
- Mode buttons (Low/Mid/High) — set LFO frequency multiplier per channel

### Custom Look & Feel

- `MixFeel` (`Source/LookNFeel/`) — rotary sliders, labels, channel strips
- `MenuFeel` (`Source/LookNFeel/`) — combo box / preset menu styling
- Color palette: lavender, pink, yellow, baby blue (defined in editor)

## Key Constraints

- **C++20 required** — `CMakeLists.txt` sets `CMAKE_CXX_STANDARD 20 REQUIRED`
- **JUCE is a git submodule** at `SUBMODULES/JUCE/` — run `git submodule update --init` if missing
- **Python 3 required** for all helper scripts
- Tests use **Catch2 v3.1.0** fetched automatically via CMake `FetchContent` when `-DBUILD_TESTS=ON`
- Build output goes to `BUILD/` (gitignored except `.gitkeep`)
- Plugin identifiers: Bundle ID `com.recluseaudio.flanger2`, Manufacturer Code `Rcls`, Plugin Code `Fl2g`
