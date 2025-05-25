#!/bin/bash
SETENV_DIR="$(dirname "$(realpath "${BASH_SOURCE[0]}")")"

PROJECT_DIR="$(dirname "$(realpath "$SETENV_DIR/../")")"
echo $PROJECT_DIR