#!/usr/bin/env python3
import sys
import pathlib

version_file = pathlib.Path(sys.argv[1])
header_file  = pathlib.Path(sys.argv[2])

if version_file.exists():
    raw = version_file.read_text().strip()
    parts = raw.split(".")
    if len(parts) == 3 and all(p.isdigit() for p in parts):
        major, minor, patch = map(int, parts)
    else:
        major, minor, patch = 0, 0, 1
else:
    major, minor, patch = 0, 0, 1

patch += 1
new_version = f"{major}.{minor}.{patch}"

version_file.write_text(new_version + "\n")
header_file.parent.mkdir(parents=True, exist_ok=True)
header_file.write_text(f"""#pragma once
#define BUILD_VERSION_MAJOR {major}
#define BUILD_VERSION_MINOR {minor}
#define BUILD_VERSION_PATCH {patch}
#define BUILD_VERSION_STRING "{new_version}"
""")
