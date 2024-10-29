#!/usr/bin/env bash
FULL_PATH=$(realpath $0)
DIR_PATH=$(dirname $FULL_PATH)
ENV="${DIR_PATH}/venv"
ACTIVE="${ENV}/bin/activate"
REQ="${DIR_PATH}/requirements.txt"

python3 -m venv $ENV
source $ACTIVE
pip install -r $REQ
deactivate
