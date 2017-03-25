# photon-tcpserver-tcpclients
Particle Photon code that enables one Particle Photon to act as a server that sends messages to other Particle Photons acting as clients. 

The first Photon is controlled by a momentary push button.
The second Photon is responsible for turning on/off an LED, when the server tells it on/off.

## Setup:
Install [particle-cli](https://github.com/spark/particle-cli).

## To run code:

`particle flash [SERVER_DEVICE_ID] server`

`particle flash [CLIENT_DEVICE_ID] client`

## To debug:
See all the running USB devices here:

`particle serial monitor list`

Then guess which one you want to monitor (or just monitor both):

`particle serial monitor 1`

`particle serial monitor 2`

