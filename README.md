# Getting Started

- Clone this repository
- Put the unpacked plugin folder in the 'Plugins' folder for the desired project.
    - The plugins folder should be located in the root directory of the project. If not, create one.
- Launch the project and allow Unreal to rebuild needed modules.
- The GRILL DIS for Unreal plugin should be enabled upon the project opening.

# Required Actors
- Certain actors are required to be a given level in order for the plugin to work as desired. These are listed below:
	- _**NOTE:**_ Only one of each unique actor type listed below should be in a single level.
	- The GeoReferencingSystem actor is required in a level and can be found from the GeoReference Plugin by Epic Games. This plugin should be included when the GRILL DIS for Unreal plugin is installed and it should be enabled by default.
    	- This actor stores the Origin Location in real world coordinates in either LLA or in Projected CRS. This is used for conversions between real world coordinated and Unreal Engine coordinates.
        	- The website http://epsg.io/ can be used for getting Projected CRS locations.
	- The DIS Game Manager actor is also required in the level and is build into the GRILL DIS for Unreal plugin.
		- This actor stores the DIS Enumeration to class mappings and other various DIS information.
		- **DIS Enumeration Mappings**: Contains desired actor to DIS Enumeration mappings.
	        - _**NOTE:**_ The actors tied to DIS Enumerations have to implement the DIS Interface supplied by the GRILL DIS for Unreal plugin and also should have a DIS Component attached to them. Implement the DIS Interface functions to return the
			associated DIS component of the actor.
	    - **Exercise ID**: The exercise ID of the DIS sim this project will be associated with.
	    - **Site ID**: The site ID of this DIS sim.
	    - **Application ID**: The application ID of this DIS sim.

# Project Settings

- Access the GRILL DIS for Unreal project settings by clicking on the 'GRILL DIS' button in the Level Editor toolbar or go to Project Settings > GRILL DIS.
- Inside of these project settings the below can be set:
    - **Auto Connect Send**: Whether or not the UDP socket for sending DIS packets should be auto connected.
    - **Auto Send IP Address**: The IP address to send UDP packets to if Auto Connect Send is enabled.
    - **Auto Send Port**: The port for sending UDP packets on.
    - **Auto Connect Receive**: Whether or not the UDP socket for receiving DIS packets should be auto connected.
    - **Auto Receive IP Address**: The IP address to receive UDP packets from if Auto Connect Receive is enabled.
        - _**NOTE:**_ An IP address of 0.0.0.0 will listen to all incoming DIS packets.
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
- Contains event bindings for notifying when a specific PDU type has been processed for all different PDU types currently implemented.

# DIS Game Manager

- The DIS Game Manager is responsible for creating/removing DIS entities as packets are processed by the PDU Processor Subsystem. It also informs the appropriate DIS Entities when DIS packets are received that impact them. This is done through notifying their associated DIS Component.
- The DIS Game Manager contains:
    - Listing of DIS Entities and their associated enumeration. This is loaded from the DIS Class Enum Mappings that needs to be set on the DIS Game Manager once it is placed in the level.
    - Listing of Entity IDs and their active DIS Entities in the world. This is a living list that is added to/removed from as new packets are received.
    - The DIS exercise, site, and application IDs.
- Notable functions:
    - Events for handling every PDU type currently implemented.
    - Add DIS Entity to Map
    - Remove DIS Entity from Map
	- Event for managing dead reckoning on all entities in the level.

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
    - DIS Timeout
		- How long to wait after an Entity State PDU is received before deleting. Gets refreshed after an Entity State PDU is received.
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
- Utilizes the GeoReferencing plugin made by Epic Games for conversions to and from Unreal Engine coordinates.

# PDU Conversion Blueprint Function Library

- Contains functions for converting PDU types to bytes.
	- This allows for sending of the PDUs for example.