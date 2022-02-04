# Getting Started

- Clone this repository
- Put the unpacked plugin folder in the 'Plugins' folder for the desired project.
    - The plugins folder should be located in the root directory of the project. If not, create one.
- Launch the project and allow Unreal to rebuild needed modules.
- The GRILL DIS for Unreal plugin should be enabled upon the project opening.

# Project Settings

- Access the GRILL DIS for Unreal project settings by clicking on the 'GRILL DIS' button in the Level Editor toolbar or go to Project Settings > GRILL DIS.
- Inside of these project settings the below can be set:
    - **DIS Enumeration Mappings**: Contains desired actor to DIS Enumeration mappings.
        - NOTE: Currently the actors tied to DIS Enumerations have to inherit from the DISEntity_Base actor supplied by the GRILL DIS for Unreal plugin.
    - **Exercise ID**: The exercise ID of the DIS sim this project will be associated with.
    - **Site ID**: The site ID of this DIS sim.
    - **Application ID**: The application ID of this DIS sim.
    - **Auto Connect Send**: Whether or not the UDP socket for sending DIS packets should be auto connected.
    - **Auto Send IP Address**: The IP address to send UDP packets to if Auto Connect Send is enabled.
    - **Auto Send Port**: The port for sending UDP packets on.
    - **Auto Connect Receive**: Whether or not the UDP socket for receiving DIS packets should be auto connected.
    - **Auto Receive IP Address**: The IP address to receive UDP packets from if Auto Connect Receive is enabled.
        - NOTE: An IP address of 0.0.0.0 will listen to all incoming DIS packets.
    - **Auto Receive Port**: The port for receiving UDP packets on.

# UDP Subsystem

- The UDP Subsystem is what is used to control UDP socket connections.
- It can be accessed via blueprints through getting the 'UDPSubsystem'.
- Notable functions:
    - Open Receive Socket
    - Close Receive Socket
    - Open Send Socket
    - Close Send Socket
    - Get UDP Receive Settings
    - Get UDP Send Settings
Contains event bindings for:
    - On Receive Socket Opened
    - On Receive Socket Closed
    - On Send Socket Opened
    - On Send Socket Closed
    - On Received Bytes

# PDU Processor Subsystem

- The PDU Processor Subsystem is what is used to convert received bytes to their appropriate DIS PDU struct.
- It can be accessed via blueprints through getting the 'PDUProcessor'
- Notable functions:
    - Process DIS Packet
    - Functions to convert PDUs to bytes for all different PDU types currently implemented.
- Contains event bindings for notifying when a specific PDU type has been processed for all different PDU types currently implemented.

# DIS Game State

- The DIS Game State is responsible for creating/removing DIS entities as packets are processed by the PDU Processor Subsystem. It also informs the appropriate DIS Entities when DIS packets are received that impact them. This is done through notifying their associated DIS Component.
- The DIS Game State contains:
    - Listing of DIS Entities and their associated enumeration. This is loaded from the project settings on play.
    - Listing of Entity IDs and their active DIS Entities in the world. This is a living list that is added to/removed from as new packets are received.
    - The DIS exercise, site, and application IDs. These are loaded from project settings on play. These can be modified during runtime.
- Notable functions:
    - Events for handling every PDU type currently implemented.
    - Add DIS Entity to Map
    - Remove DIS Entity from Map

# DIS Entity Base Actor

- The base Actor for DIS Entities that should be used.
- Has a DIS Component added to it that handles DIS PDU updates.
- Used by the GRILL DIS for Unreal project settings to setup DIS Enumeration mappings.

# DIS Actor Component

- The DIS Component is responsible for handling all DIS functionality and DIS PDU updates for its associated DIS Entity.
- Handles dead reckoning and ground clamping updates.
    - Automatically handles both of these in C++.
- Contains various DIS related variables.
- Notable functions:
    - Simple Ground Clamping.
        - Can be overriden in blueprints for a custom implementation.
- Contains event bindings for:
    - Receiving each type of DIS Entity PDU currently implemented.
    - Dead reckoning update
- Has variables for:
    - Most Recent Entity State PDU
    - Dead Reckoning Entity State PDU
        - This is an Entity State PDU whose information has been updated with the most recent Dead Reckoning information.
    - Latest Entity State PDU Timestamp
    - Spawned From Network
        - Whether or not this entity was spawned from the network.
    - DIS Heartbeat
    - Entity Type
    - Entity ID
    - Perform Dead Reckoning
        - Whether or not dead reckoning should be performed.
    - Perform Ground Clamping
        - Whether or not ground clamping should be perfoemd.
    - Ground Clamping Collision Channel
        - The collision channel that should be used for ground clamping.

# DIS Blueprint Function Library

- Contains functions for converting between geospatial coordinates and Unreal Engine coordinates.
- Currently utilizes the GeoReferencing plugin made by Epic Games for conversions to and from Unreal Engine coordinates.
