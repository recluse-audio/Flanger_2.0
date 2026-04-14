import os
import sys
import subprocess

SOURCE_DIR = "./SOURCE"
TEST_DIR = "./TESTS"

def create_file(filepath, content):
    """Creates a file if it does not already exist."""
    os.makedirs(os.path.dirname(filepath), exist_ok=True)  # Ensure subdirectory exists

    if os.path.exists(filepath):
        print(f"Warning: File {filepath} already exists. Skipping...")
        return
    
    with open(filepath, "w") as file:
        file.write(content)
    
    print(f"Created {filepath}")

def main():
    if len(sys.argv) < 2:
        print("Usage: python3 addFile.py [subdir] ClassName")
        sys.exit(1)

    if len(sys.argv) == 2:  # No subdirectory, just class name
        class_name = sys.argv[1]
        sub_dir = ""
    else:
        sub_dir = sys.argv[1]  # Subdirectory path
        class_name = sys.argv[2]

    source_subdir = os.path.join(SOURCE_DIR, sub_dir) if sub_dir else SOURCE_DIR
    header_path = os.path.join(source_subdir, f"{class_name}.h")
    cpp_path = os.path.join(source_subdir, f"{class_name}.cpp")
    test_path = os.path.join(TEST_DIR, f"test_{class_name}.cpp")

    # Header guard format: Convert subdir path to uppercase and replace slashes
    header_guard = f"{sub_dir.upper()}_{class_name.upper()}_H".replace("/", "_") if sub_dir else f"{class_name.upper()}_H"

    # Default file contents
    header_content = f"""#pragma once

#ifndef {header_guard}
#define {header_guard}

class {class_name}
{{
public:
    {class_name}();
    ~{class_name}();
}};

#endif // {header_guard}
"""
    cpp_content = f"""#include "{class_name}.h"

{class_name}::{class_name}()
{{
}}

{class_name}::~{class_name}()
{{
}}
"""
    test_content = f"""#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "{class_name}.h"

TEST_CASE("{class_name} Test", "[{class_name}]")
{{
    {class_name} obj;
    REQUIRE(true);
}}
"""

    # Create files
    create_file(header_path, header_content)
    create_file(cpp_path, cpp_content)
    create_file(test_path, test_content)

    # Call regenSource.py
    try:
        print("Running regenSource.py...")
        subprocess.run(["python3", "regenSource.py"], check=True)
        print("regenSource.py executed successfully.")
    except subprocess.CalledProcessError as e:
        print(f"Error running regenSource.py: {e}")

if __name__ == "__main__":
    main()



#################
### USAGE
###
# python3 addClass.py effects/layer2/coolguy EffectClass 
# allows class creation in nested directories.  Assumes there is a TESTS and SOURCE directory and being called from root of repo.