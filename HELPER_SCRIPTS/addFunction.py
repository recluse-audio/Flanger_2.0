import os
import sys

SOURCE_DIR = "./SOURCE"

def find_insertion_point(header_lines, class_name):
    """Finds the insertion point for function declarations inside the class."""
    inside_class = False
    for i, line in enumerate(header_lines):
        if f"class {class_name}" in line:
            inside_class = True
        if inside_class and "};" in line:  # End of class
            return i  # Insert before the closing brace
    return None

def add_function_to_header(header_path, class_name, function_signature):
    """Adds a function declaration to the class in the header file."""
    if not os.path.exists(header_path):
        print(f"Error: Header file {header_path} not found.")
        return
    
    with open(header_path, "r") as file:
        lines = file.readlines()

    insert_index = find_insertion_point(lines, class_name)
    if insert_index is None:
        print(f"Error: Could not find class {class_name} in {header_path}.")
        return

    declaration = f"    {function_signature};\n"

    if declaration in lines:
        print(f"Warning: Function already exists in {header_path}. Skipping...")
        return

    lines.insert(insert_index, declaration)

    with open(header_path, "w") as file:
        file.writelines(lines)

    print(f"Added function declaration to {header_path}")

def add_function_to_cpp(cpp_path, class_name, function_signature):
    """Adds a function definition to the .cpp file."""
    if not os.path.exists(cpp_path):
        print(f"Error: Source file {cpp_path} not found.")
        return
    
    return_type, function_name = function_signature.split(" ", 1)
    definition = f"{return_type} {class_name}::{function_name}\n{{\n    // TODO: Implement\n}}\n\n"

    with open(cpp_path, "r") as file:
        content = file.read()

    if definition in content:
        print(f"Warning: Function already exists in {cpp_path}. Skipping...")
        return

    with open(cpp_path, "a") as file:
        file.write(definition)

    print(f"Added function definition to {cpp_path}")

def main():
    if len(sys.argv) < 3:
        print("Usage: python3 addFunction.py ClassName 'returnType functionName(args)'")
        sys.exit(1)

    class_name = sys.argv[1]
    function_signature = sys.argv[2]

    header_path = os.path.join(SOURCE_DIR, f"{class_name}.h")
    cpp_path = os.path.join(SOURCE_DIR, f"{class_name}.cpp")

    add_function_to_header(header_path, class_name, function_signature)
    add_function_to_cpp(cpp_path, class_name, function_signature)

if __name__ == "__main__":
    main()



#############
### USAGE ###
# ryandevens@Ryans-iMac GrainMaker % python3 addFunction.py Granulator "void prepare()"                                                                           
# Class must exist in file.
# TODO: Doesn't work with files in subdirectories of SOURCE yet