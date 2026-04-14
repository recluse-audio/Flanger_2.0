#!/usr/bin/env python3
"""
Regenerates CMAKE/SOURCES.cmake and CMAKE/TESTS.cmake by scanning directories.

Run this whenever you add or remove source files to update the CMake file lists.
"""

import os
from pathlib import Path


def generate_files_list(root_folders, output_file, variable_name):
    files_list = []
    for root_folder in root_folders:
        if not os.path.exists(root_folder):
            continue
        for folder, subfolders, files in os.walk(root_folder):
            for filename in files:
                if filename.endswith('.cpp') or filename.endswith('.h'):
                    file_path = os.path.join(folder, filename).replace('\\', '/')
                    files_list.append(file_path)
    files_list.sort()
    if files_list:
        files_list_str = "\n    ".join(files_list)
        output = 'set({}\n    {}\n)'.format(variable_name, files_list_str)
    else:
        output = 'set({}\n    # No files found\n)'.format(variable_name)
    with open(output_file, 'w+') as f:
        f.write(output)
    print(f"Generated {output_file} with {len(files_list)} file(s)")


def discover_source_folders():
    folders = []
    if os.path.exists('SOURCE'):
        folders.append('SOURCE')
    submodules_dir = Path('SUBMODULES')
    if submodules_dir.exists():
        for item in submodules_dir.iterdir():
            if item.is_dir() and item.name != 'JUCE':
                source_dir = item / 'SOURCE'
                if source_dir.exists():
                    folders.append(str(source_dir).replace('\\', '/'))
    return folders


def discover_test_folders():
    folders = []
    if os.path.exists('TESTS'):
        folders.append('TESTS')
    return folders


def main():
    source_folders = discover_source_folders()
    if source_folders:
        print(f"Scanning source folders: {', '.join(source_folders)}")
    else:
        print("Warning: No SOURCE folders found")
    generate_files_list(source_folders, 'CMAKE/SOURCES.cmake', 'SOURCES')

    test_folders = discover_test_folders()
    if test_folders:
        print(f"Scanning test folders: {', '.join(test_folders)}")
    generate_files_list(test_folders, 'CMAKE/TESTS.cmake', 'TEST_SOURCES')


if __name__ == '__main__':
    main()
