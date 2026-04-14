#!/usr/bin/env python3
import os
import platform
import subprocess
import sys
from pathlib import Path
from build_complete import beep

ROOT = Path(__file__).resolve().parents[1] if (Path(__file__).parent.name.lower() in {"scripts", "script"}) else Path.cwd()
BUILD_DIR = ROOT / "BUILD"
PLUGIN_NAME = ROOT.name
TARGET = f"{PLUGIN_NAME}_Standalone"
BUILD_TYPE = os.environ.get("CMAKE_BUILD_TYPE", "Debug")

def run(cmd, cwd=None):
    print("+", " ".join(cmd))
    subprocess.run(cmd, cwd=cwd, check=True)

def regenerate_cmake_lists() -> None:
    """Regenerate CMAKE/SOURCES.cmake and CMAKE/TESTS.cmake from directory scan."""
    regen_script = Path(__file__).parent / "regenSource.py"
    if regen_script.exists():
        print("Regenerating CMake file lists...")
        subprocess.run([sys.executable, str(regen_script)], check=True)
    else:
        print("Warning: regenSource.py not found, skipping regeneration")

def is_multi_config_generator() -> bool:
    gen = (os.environ.get("CMAKE_GENERATOR") or "").lower()
    # Heuristic: VS/Xcode are multi-config; Ninja/Unix Makefiles typically single-config
    return ("visual studio" in gen) or ("xcode" in gen)

def main():
    # Regenerate CMake file lists before building
    regenerate_cmake_lists()

    BUILD_DIR.mkdir(exist_ok=True)

    # Configure (mirrors: cmake -DCMAKE_BUILD_TYPE=Debug .. from inside BUILD)
    cfg_cmd = ["cmake", "-S", str(ROOT), "-B", str(BUILD_DIR)]
    if not is_multi_config_generator():
        cfg_cmd += [f"-DCMAKE_BUILD_TYPE={BUILD_TYPE}"]
    run(cfg_cmd)

    # Build target
    build_cmd = ["cmake", "--build", str(BUILD_DIR), "--target", TARGET]
    if is_multi_config_generator():
        build_cmd += ["--config", BUILD_TYPE]
    run(build_cmd)

if __name__ == "__main__":
    try:
        main()
        beep(success=True)
    except subprocess.CalledProcessError as e:
        beep(success=False)
        sys.exit(e.returncode)
    except Exception:
        beep(success=False)
        raise
