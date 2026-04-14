from __future__ import annotations
import shutil
import sys
from pathlib import Path


def find_cmake() -> str:
    """Return the cmake executable path, checking common install locations on macOS."""
    if found := shutil.which("cmake"):
        return found

    if sys.platform == "darwin":
        candidates = [
            "/opt/homebrew/bin/cmake",           # Homebrew (Apple Silicon)
            "/usr/local/bin/cmake",              # Homebrew (Intel)
            "/Applications/CMake.app/Contents/bin/cmake",
        ]
        for path in candidates:
            if Path(path).exists():
                return path

    raise FileNotFoundError(
        "cmake not found. Install it via 'brew install cmake' or https://cmake.org/download/"
    )


def beep(*, success: bool = True) -> None:
    """Play an audible notification. Best-effort cross-platform."""
    try:
        if sys.platform.startswith("win"):
            import winsound
            sounds_dir = Path(__file__).parent / "SOUNDS"
            sound_file = sounds_dir / ("success_sound.wav" if success else "failure_sound.wav")
            if not sound_file.exists() and not success:
                sound_file = sounds_dir / "success_sound.wav"
            if sound_file.exists():
                winsound.PlaySound(str(sound_file), winsound.SND_FILENAME)
            else:
                winsound.Beep(880 if success else 220, 120 if success else 400)
        else:
            sys.stdout.write("\a")
            sys.stdout.flush()
    except Exception as e:
        print(f"Error playing sound: {e}")
