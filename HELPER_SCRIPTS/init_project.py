#!/usr/bin/env python3
"""
Initialize a JUCE plugin project with git and JUCE submodule.

This script should be run from the project root directory after running setup_project.py.
It will:
1. Initialize a git repository
2. Add JUCE as a submodule in SUBMODULES/JUCE
3. Update and initialize all submodules

Usage:
    cd <project_directory>
    python SCRIPTS/init_project.py
"""

import subprocess
import sys
from pathlib import Path


def run(cmd: list[str], cwd: Path = None, check: bool = True) -> subprocess.CompletedProcess:
    """
    Run a command and print it.

    Args:
        cmd: Command to run as list of strings
        cwd: Working directory
        check: Whether to raise exception on non-zero exit code

    Returns:
        CompletedProcess result
    """
    print("+", " ".join(cmd))
    return subprocess.run(cmd, cwd=str(cwd) if cwd else None, check=check)


def is_git_repo(path: Path) -> bool:
    """Check if a directory is a git repository."""
    git_dir = path / ".git"
    return git_dir.exists()


def submodule_exists(path: Path, submodule_path: str) -> bool:
    """Check if a git submodule already exists."""
    submodule_dir = path / submodule_path
    return submodule_dir.exists() and any(submodule_dir.iterdir())


def main() -> int:
    """Main entry point."""
    project_root = Path.cwd()

    print(f"Initializing JUCE project: {project_root.name}")
    print(f"Location: {project_root}")
    print()

    # Check if SUBMODULES directory exists
    submodules_dir = project_root / "SUBMODULES"
    if not submodules_dir.exists():
        print("Error: SUBMODULES directory not found.")
        print("Are you running this from the project root directory?")
        return 1

    # Step 1: Initialize git repository
    if is_git_repo(project_root):
        print("Git repository already initialized.")
    else:
        print("Initializing git repository...")
        result = run(["git", "init"], cwd=project_root, check=False)
        if result.returncode != 0:
            print("\nError: Failed to initialize git repository.")
            print("Make sure git is installed and available in your PATH.")
            return 1
        print("Git repository initialized.")

    print()

    # Step 2: Add JUCE submodule
    juce_submodule_path = "SUBMODULES/JUCE"
    if submodule_exists(project_root, juce_submodule_path):
        print("JUCE submodule already exists.")
    else:
        print("Adding JUCE submodule (this may take a few minutes)...")
        result = run(
            ["git", "submodule", "add", "https://github.com/juce-framework/JUCE.git", juce_submodule_path],
            cwd=project_root,
            check=False
        )
        if result.returncode != 0:
            print("\nError: Failed to add JUCE submodule.")
            return 1
        print("JUCE submodule added.")

    print()

    # Step 3: Update submodules
    print("Updating submodules...")
    result = run(["git", "submodule", "update", "--init", "--recursive"], cwd=project_root, check=False)
    if result.returncode != 0:
        print("\nError: Failed to update submodules.")
        return 1

    print()
    print("=" * 70)
    print("Project initialization complete!")
    print("=" * 70)
    print()
    print("Next steps:")
    print("  1. Build the project: python SCRIPTS/rebuild_all.py")
    print("  2. Or build specific targets:")
    print("     python SCRIPTS/build_vst3.py")
    print("     python SCRIPTS/build_app.py")
    print()

    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except KeyboardInterrupt:
        print("\n\nInterrupted by user.")
        sys.exit(1)
    except Exception as e:
        print(f"\nUnexpected error: {e}")
        sys.exit(1)
