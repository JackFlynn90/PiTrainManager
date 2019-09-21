#!/bin/bash
#
# Simple script to start compassWorker.py.
echo "$(date): Master Boot Script. Launching all startup scripts"
echo "$(date): Starting server.sh"
/bin/bash /share/djangoTrain/launcherScripts/serverLauncher.sh >> /share/djangoTrain/logs/server.log &
echo "$(date): Starting ArduinoEffects Manager"
/bin/bash /share/djangoTrain/launcherScripts/arduinoEffectsLauncher.sh > /dev/null 2>&1 &
echo "$(date): Starting DCC Manager"
/bin/bash /share/djangoTrain/launcherScripts/dccLauncher.sh > /dev/null 2>&1 &
echo "$(date): Done..."
