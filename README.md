# LagCompensation

https://developer.valvesoftware.com/wiki/Lag_Compensation

https://doc.photonengine.com/en-us/pun/current/gameplay/lagcompensation

https://docs-multiplayer.unity3d.com/netcode/0.1.0/reference/glossary/network-latency-management/index.html

Added custom TrackComponent, which is used to create a Frame Snapshot on the server side.

RTT client is used for compensation. 
The server "rolls back" to the amount of client delay and simulates the world at that point in time, then returns to the actual state.

An example of implementation of compensation for a shooting vector can be found in BP FirstPersonCharacter.
![](https://media.discordapp.net/attachments/190175905824374784/984504074600861706/unknown.png?width=1280&height=688)
