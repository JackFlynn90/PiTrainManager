#!/bin/bash
set -x #echo on
cd ~ #go to pi root
cd /share/djangoTrain #go to our shared development folder
source myvenv/bin/activate #activate the virtual environement
nohup python manage.py runserver 0:8080 & #run django in backround 
cd python_scripts # move to python scripts store
nohup python ArduinoEffectsManager.py & #run our manager script
