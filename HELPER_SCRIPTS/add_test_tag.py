#!/usr/bin/env python3
"""
Adds a tag to every TEST_CASE in a Catch2 test file.

Usage:
    python add_test_tag.py <file_path> <tag>

Example:
    python add_test_tag.py TESTS/test_Granulator.cpp Granulator

This will add [Granulator] to every TEST_CASE in the file.
"""

import sys
import re

def add_tag_to_test_cases(file_path: str, tag: str) -> int:
    """
    Adds a tag to all TEST_CASE declarations in a file.
    Returns the number of test cases modified.
    """
    with open(file_path, 'r', encoding='utf-8') as f:
        content = f.read()

    # Pattern matches TEST_CASE("name") or TEST_CASE("name", "[existing][tags]")
    # Group 1: TEST_CASE("name"
    # Group 2: existing tags (optional, includes the comma and quotes)
    # Group 3: just the tags inside quotes (optional)
    pattern = r'(TEST_CASE\s*\(\s*"[^"]*")\s*(,\s*"(\[[^\]]*\]*)"\s*)?\)'

    modified_count = 0

    def replace_func(match):
        nonlocal modified_count
        test_case_name = match.group(1)  # TEST_CASE("name"
        existing_tags = match.group(3)   # [existing][tags] or None

        tag_formatted = f"[{tag}]"

        if existing_tags:
            # Check if tag already exists
            if tag_formatted in existing_tags:
                return match.group(0)  # No change needed
            # Append new tag to existing tags
            new_tags = existing_tags + tag_formatted
        else:
            # No existing tags, add new one
            new_tags = tag_formatted

        modified_count += 1
        return f'{test_case_name}, "{new_tags}")'

    new_content = re.sub(pattern, replace_func, content)

    if modified_count > 0:
        with open(file_path, 'w', encoding='utf-8') as f:
            f.write(new_content)

    return modified_count

def main():
    if len(sys.argv) != 3:
        print("Usage: python add_test_tag.py <file_path> <tag>")
        print("Example: python add_test_tag.py TESTS/test_Granulator.cpp Granulator")
        sys.exit(1)

    file_path = sys.argv[1]
    tag = sys.argv[2]

    # Remove brackets if user included them
    tag = tag.strip("[]")

    try:
        count = add_tag_to_test_cases(file_path, tag)
        print(f"Added [{tag}] to {count} test case(s) in {file_path}")
    except FileNotFoundError:
        print(f"Error: File not found: {file_path}")
        sys.exit(1)
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
