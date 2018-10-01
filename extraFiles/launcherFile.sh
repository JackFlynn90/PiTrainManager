#!/bin/bash
set -x #echo on

cd ~
cd /share/djangoTrain
source myvenv/bin/activate
nohup python manage.py runserver 0:8080 &
cd python_scripts
nohup python ArduinoEffectsManager.py &
