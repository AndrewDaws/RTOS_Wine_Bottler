# Wine Bottler
Andrew Daws <br />
CEC 450-01 <br />
March 21, 2014 <br />

### Description
The objective of this project was to simulate the process of bottling wine. <br />

This is achieved through four stations.
1. The filling module is the first station that the bottle encounters. The objective of this station is to fill the bottle with the correct wine. In order to do so, a color sensor is used to determine weather the bottle is made of clear glass, or dark green glass. The dark green glass is used for red wine, while the clear glass contains white wine.
2. After the bottle is filled the conveyor belt moves the bottle to the next station. At the corking station an actuator seals the bottle by placing a cork in it.
3. When this is completed the conveyor belt moves the bottle to the necking station. At this station label is placed on the neck of the bottle. A color sensor is used in order to ensure the correct label is placed on the bottle.
4. Once this is completed the conveyor belt moves the bottle to the final station. At this station a label is placed on the main body of the bottle. A color sensor is also used at this station to ensure the correct label is placed the bottle.

### Purpose
The concepts of real-time software systems from the user, designer, and programmer viewpoint are introduced. The connection of an external process to a digital computer by means of hardware and software interface is discussed. The structure, programming, and basic properties of real-time systems are described with an overview of system software. Related topics as interrupts, concurrent task synchronization and communication, sharing resources, schedulability and reliability are discussed. Real-time software development in a host-target environment is presented.

### Requires:
1. WindRiver Tornado/VxWorks development and run-time suite
2. MyEasyVeep simulation
3. Arcom SBC-GX1 hardware
