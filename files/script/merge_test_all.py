import os

def get_files_recursive(directory):
    files = []
    for root, dirs, filenames in os.walk(directory):
        for filename in filenames:
            files.append(os.path.join(root, filename))
    return files

def merge_files(directory):
    files = get_files_recursive(directory)
    contents = []
    for file in files:
        with open(file, 'r') as f:
            contents.append(f.read())
    with open('test/test_all_one.nl', 'w') as f:
        f.write('\n'.join(contents))

def main():
    directory = "test"
    merge_files(directory)

if __name__ == "__main__":
    main()
