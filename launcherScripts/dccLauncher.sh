#!/bin/bash
#
# Simple script to start compassWorker.py.
echo "$(date): Starting DCCManager"

set -x #echo on
cd ~ #go to pi root
cd /share/djangoTrain #go to our shared development folder
source myvenv/bin/activate #activate the virtual environement
cd python_scripts # move to python scripts store
while true
do
  python DccManager.py &> DccMan.log& #run our Dcc manager script
  wait $!
  echo "$(date): DCCManager.py Exited"
  sleep 5
done
