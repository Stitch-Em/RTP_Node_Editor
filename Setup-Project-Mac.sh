#!/bin/bash

# Get the directory of the script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "Script directory: $SCRIPT_DIR"

# Run Premake with the Lua file for Xcode
"$SCRIPT_DIR/vendors/bin/premake/macOS/premake5" --file="$SCRIPT_DIR/RTP_Node_Editor.lua" xcode4