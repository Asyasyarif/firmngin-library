#!/bin/sh

# Default configuration
LIBRARY_NAME="Firmngin"
RELEASE_DIR="releases"
TEMP_DIR="$RELEASE_DIR/temp"

# Function to compare versions
version_gt() {
    test "$(printf '%s\n' "$@" | sort -V | head -n 1)" != "$1"
}

# Get current version from library.properties
CURRENT_VERSION=$(grep "version=" library.properties | cut -d'=' -f2)

# Parse command line arguments
VERSION=""
while [ "$1" != "" ]; do
    case $1 in
        -release | --release )
            shift
            VERSION=$1
            ;;
        * )
            echo "Error: Invalid parameter"
            echo "Usage: $0 -release <version>"
            echo "Example: $0 -release 1.1.0"
            exit 1
    esac
    shift
done

# Validate version parameter
if [ -z "$VERSION" ]; then
    echo "Error: Version parameter is mandatory"
    echo "Usage: $0 -release <version>"
    echo "Example: $0 -release 1.1.0"
    exit 1
fi

# Check if new version is greater than current
if ! version_gt "$VERSION" "$CURRENT_VERSION"; then
    echo "Error: New version ($VERSION) must be greater than current version ($CURRENT_VERSION)"
    exit 1
fi

echo "Creating release version $VERSION (current: $CURRENT_VERSION)"

# Create directories
mkdir -p "$RELEASE_DIR"
mkdir -p "$TEMP_DIR"

# Copy required files
cp -r src "$TEMP_DIR/"
cp -r examples "$TEMP_DIR/"
cp keywords.txt "$TEMP_DIR/"
cp library.properties "$TEMP_DIR/"
cp LICENSE "$TEMP_DIR/"
cp README.md "$TEMP_DIR/"

# Update version in library.properties
if [ -f "$TEMP_DIR/library.properties" ]; then
    sed -i.bak "s/version=.*/version=$VERSION/" "$TEMP_DIR/library.properties"
    rm "$TEMP_DIR/library.properties.bak"
fi

# Remove unwanted files
find "$TEMP_DIR" -name ".DS_Store" -delete
find "$TEMP_DIR" -name "*.pyc" -delete
find "$TEMP_DIR" -name "__pycache__" -type d -exec rm -r {} +

# Create ZIP
cd "$TEMP_DIR"
zip -r "../$LIBRARY_NAME-$VERSION.zip" ./*

# Cleanup
cd ../..
rm -rf "$TEMP_DIR"

echo "Release $VERSION created successfully!"
echo "ZIP file: $RELEASE_DIR/$LIBRARY_NAME-$VERSION.zip" 