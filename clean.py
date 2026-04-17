import os

ROOT_DIR = "."

TARGET_FOLDER_NAME = "resources"
DELETE_EXTENSIONS = (".cpp", ".hpp")


def clean_project():
    removed = 0

    for root, dirs, files in os.walk(ROOT_DIR):

        # only act inside "resources" folders
        if os.path.basename(root) != TARGET_FOLDER_NAME:
            continue

        for file in files:
            if file.lower().endswith(DELETE_EXTENSIONS):
                path = os.path.join(root, file)

                try:
                    os.remove(path)
                    print(f"Deleted: {path}")
                    removed += 1
                except Exception as e:
                    print(f"Failed: {path} -> {e}")

    print(f"\nClean complete. Removed {removed} files.")


if __name__ == "__main__":
    clean_project()