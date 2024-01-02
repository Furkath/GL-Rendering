#!/bin/bash

# Set the directory path
directory="/home/jim/TEST/OpenGL_Final_PROJ/new/GL-Rendering/demos"

# Set the common prefix to be removed
prefix="xxx "


# Change to the directory
cd "$directory" || exit

# Rename files in the directory
for file in "$prefix"*; do
    newname="${file#$prefix}"
    mv "$file" "$newname"
done

