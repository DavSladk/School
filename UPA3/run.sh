#!/usr/bin/env bash
FULL_PATH=$(realpath $0)
DIR_PATH=$(dirname $FULL_PATH)
ENV="${DIR_PATH}/venv"
ACTIVE="${ENV}/bin/activate"
REQ="${DIR_PATH}/requirements.txt"
URLS="${DIR_PATH}/urls.py"
DATA="${DIR_PATH}/data.py"

source $ACTIVE
python3 $URLS -n 20 | python3 $DATA
deactivate
