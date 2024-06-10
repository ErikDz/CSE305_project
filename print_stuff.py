import os

# Get the current directory
current_directory = os.getcwd()

# Create a new text file to store the file contents
output_file = "file_contents.txt"

# Open the output file in write mode
with open(output_file, "w") as file:
    # Iterate over all files in the current directory
    for filename in os.listdir(current_directory):
        # Check if the current item is a file (not a directory)
        if os.path.isfile(filename):
            # Open the file in read mode
            with open(filename, "r") as input_file:
                if filename.endswith(".cpp") or filename.endswith(".h"):
                    # Read the contents of the file
                    file_contents = input_file.read()

                    # Write the file name and contents to the output file
                    file.write(f"// {filename}\n")
                    file.write(file_contents)
                    file.write("\n\n------------------------\n\n")

print(f"File contents have been written to '{output_file}'.")