#!/usr/bin/env python3

import os
import shutil
import json
from datetime import datetime

# Configuration
LIBRARY_NAME = "Firmngin"
VERSION = "1.0.0"
RELEASE_DIR = "releases"

# Files and directories to include
INCLUDE_FILES = [
    "src/",
    "examples/",
    "keywords.txt",
    "library.properties",
    "LICENSE",
    "README.md"
]

# Files and directories to exclude
EXCLUDE_PATTERNS = [
    ".git",
    ".github",
    "__pycache__",
    "*.pyc",
    ".DS_Store",
    "tools",
    "test",
    "docs"
]


def create_release():
    # Create release directory
    release_path = os.path.join(RELEASE_DIR, f"{LIBRARY_NAME}-{VERSION}")
    if os.path.exists(release_path):
        shutil.rmtree(release_path)
    os.makedirs(release_path, exist_ok=True)

    # Copy files
    for item in INCLUDE_FILES:
        src = item
        dst = os.path.join(release_path, os.path.basename(item))

        if os.path.isdir(src):
            shutil.copytree(
                src, dst, ignore=shutil.ignore_patterns(*EXCLUDE_PATTERNS))
        else:
            shutil.copy2(src, dst)

    # Create ZIP file
    zip_name = f"{LIBRARY_NAME}-{VERSION}.zip"
    shutil.make_archive(
        os.path.join(RELEASE_DIR, f"{LIBRARY_NAME}-{VERSION}"),
        'zip',
        release_path
    )

    print(f"Release {VERSION} created successfully!")
    print(f"ZIP file: {os.path.join(RELEASE_DIR, zip_name)}")


def update_library_properties():
    with open('library.properties', 'r') as f:
        content = f.read()

    # Update version
    content = content.replace(
        'version=',
        f'version={VERSION}'
    )

    # Update date
    today = datetime.now().strftime('%Y-%m-%d')
    content = content.replace(
        'date=',
        f'date={today}'
    )

    with open('library.properties', 'w') as f:
        f.write(content)


if __name__ == "__main__":
    # Create releases directory if not exists
    if not os.path.exists(RELEASE_DIR):
        os.makedirs(RELEASE_DIR)

    # Update library.properties
    update_library_properties()

    # Create release
    create_release()
