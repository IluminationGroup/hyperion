## Hyperion daemon initctl script

description "hyperion"
author "poljvd & tvdzwan"

start on (runlevel [2345])
stop on (runlevel [!2345])

respawn

exec /usr/bin/hyperiond /opt/hyperion/config/hyperion.config.json