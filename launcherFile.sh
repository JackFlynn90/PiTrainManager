#!/bin/bash
set -x #echo on
cd ~ #go to pi root
cd /share/djangoTrain #go to our shared development folder
source myvenv/bin/activate #activate the virtual environement
nohup sudo /share/djangoTrain/myvenv/bin/python manage.py runserver 0:80 &> Djangoserver.log& #run django in backround 
cd python_scripts # move to python scripts store
nohup python ArduinoEffectsManager.py &> ArdEffMan.log& #run our Effects manager script
nohup python DccManager.py &> DccMan.log& #run our Dcc manager script