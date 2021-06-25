#!/bin/bash

# This script installs all necessary dependencies,
# compiles the necessary programms
# and prepares all the analyses data

# constants
IPREGEL_DIR="iPregel"
LIGRA_DIR="ligra"

# navigate to iPregel root directory
cd "$(dirname "$0")/.."

echo "Compiling iPregel binaries..."
make &> /dev/null
echo

echo "----- installing graph converter -----"

# LIGRA CONVERTER
cd ..
if [ -d "$LIGRA_DIR" ]; then
    echo "Ligra installation found. Skipping install."
else
    ( # execute in subshell to catch errors and clean up afterwards
        set -e

        echo -n "install ligra to '$(pwd)'? (y/n): "
        read ANSWER
        if [ "$ANSWER" != "y" ]; then
            exit 1
        fi
        echo "Cloning ligra repository to $(pwd) ..."
        git clone https://github.com/jshun/ligra.git &> /dev/null

        echo "Compiling converter binaries..."
        make -C "$LIGRA_DIR/utils" SNAPtoAdj adjToBinary 1> /dev/null

        echo "Setting up SNAP to iPregel converter."
        "$IPREGEL_DIR/utility/Snap2iPregel.sh" -c "$LIGRA_DIR/utils/SNAPtoAdj" "$LIGRA_DIR/utils/adjToBinary"
    )
    if [ $? -ne 0 ]; then
        echo "Ligra installation failed. Exiting."
        rm -rf "$LIGRA_DIR"
        exit 1
    fi
fi
cd "$IPREGEL_DIR"
echo

# PYTHON & NETWORKIT
echo "----- installing python & networkit -----"
if [ -d ".venv" ]; then
    echo "Virtual python environment found. Skipping install."
    source .venv/bin/activate
else
    (
        set -e

        echo "Creating virtual python environment in $(pwd)"
        python3.9 -m venv .venv
        source .venv/bin/activate
        echo "Installing python dependencies..."
        pip install --upgrade pip 1> /dev/null
        pip install -r requirements.txt 1> /dev/null
        echo "Installing networkit..."
        pip install cmake cython 1> /dev/null
        pip install networkit 1> /dev/null
    )
    if [ $? -ne 0 ]; then
        echo "Python & networkit installation failed. Exiting."
        rm -rf ".venv"
        exit 1
    fi
fi
echo

# PREPARING DATA
echo
echo "----- downloading & preparing data -----"
cd analyses
echo "Preparing citation data..."
citation/prepare_data.sh
echo
echo "Preparing twitter data..."
twitter/prepare_data.sh
echo
echo "Preparing statkoverflow data..."
stackoverflow/prepare_data.sh
echo
echo "Preparing imdb data..."
imdb/prepare_data.sh
echo

# SUCCESS
echo
echo "Success!"