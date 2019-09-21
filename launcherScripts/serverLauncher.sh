#!/bin/bash
#
# Simple script to start compassWorker.py.
echo "$(date): Starting django server"

set -x #echo on
cd ~ #go to pi root
cd /share/djangoTrain #go to our shared development folder
source myvenv/bin/activate #activate the virtual environement
while true
do
  sudo /share/djangoTrain/myvenv/bin/python manage.py runserver 0:80 &> Djangoserver.log& #run django in backround
  wait $!
  echo "$(date): manage.py Exited"
  sleep 5
done
