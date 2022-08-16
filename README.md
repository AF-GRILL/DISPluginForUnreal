[![DISPluginLogo](Resources/Icon128.png)](https://www.af-grill.com/)

# Getting Started

- This plugin was made for Unreal Engine 4.27
- Clone this repository
- Put the unpacked plugin folder in the 'Plugins' folder for the desired project.
    - The plugins folder should be located in the root directory of the project. If not, create one.
- Launch the project and allow Unreal to rebuild needed modules.
- The GRILL DIS for Unreal plugin should be enabled upon the project opening.

# Required Actors
- Certain actors are required to be in a given level in order for the plugin to work as desired. These are listed below:
	- _**NOTE:**_ Only one of each unique actor type listed below should be in a single level.

![GeoReferencingSystemActor](Resources/ReadMeImages/GeoReferencingSystemActor.png)

- The GeoReferencingSystem actor is required in a level and can be found from the GeoReference Plugin by Epic Games. This plugin should be included when the GRILL DIS for Unreal plugin is installed and it should be enabled by default.
    - This actor stores the Origin Location in real world coordinates in either LLA or in Projected CRS. This is used for conversions between real world coordinated and Unreal Engine coordinates.
        - The website http://epsg.io/ can be used for getting Projected CRS locations.
        - More information on the actor itself can be retrieved at [Unreal Engine's Georeferencing Documentation](https://docs.unrealengine.com/4.27/en-US/BuildingWorlds/Georeferencing/)

![DISGameManagerActor](Resources/ReadMeImages/DISGameManagerActor.png)

- The DIS Game Manager actor is also required in the level and is built into the GRILL DIS for Unreal plugin.
	- This actor stores the DIS Enumeration to class mappings and other various DIS information.
	- More information on the actor itself can be found in the **DIS Game Manager section** below

# UDP Subsystem

- The UDP Subsystem is what is used to control UDP socket connections.
- It can be accessed via blueprints through getting the 'UDPSubsystem'.
- Notable functions:
    - Open Receive Socket
    - Close Receive Socket
    - Open Send Socket
    - Close Send Socket
	- Close All Send Sockets
	- Close All Receive Sockets
	- Get Connected Receive Socket IDs
	- Get Connected Send Socket IDs
	- Any Connected Sockets

![UDPFunctions](Resources/ReadMeImages/UDPFunctions.png)

- Contains event bindings for:
    - On Receive Socket Opened
    - On Receive Socket Closed
    - On Send Socket Opened
    - On Send Socket Closed
    - On Received Bytes

![UDPEvents](Resources/ReadMeImages/UDPEvents.png)

# PDU Processor Subsystem

- The PDU Processor Subsystem is what is used to convert received bytes to their appropriate DIS PDU struct.
- It can be accessed via blueprints through getting the 'PDUProcessor'
- Notable functions:
    - Process DIS Packet

![PDUFunctions](Resources/ReadMeImages/PDUFunctions.png)

- Contains event bindings for notifying when a specific PDU type has been processed for all different PDU types currently implemented.

![PDUEvents](Resources/ReadMeImages/PDUEvents.png)

# DIS Game Manager

- The DIS Game Manager is responsible for creating/removing DIS entities as packets are processed by the PDU Processor Subsystem. It also informs the appropriate DIS Entities when DIS packets are received that impact them. This is done through notifying their associated DIS Component.
- The DIS Game Manager has the following settings:
    - **DIS Enumeration Mappings**: Contains desired actor to DIS Enumeration mappings.
        - _**NOTE:**_ The actors tied to DIS Enumerations have to implement the DIS Interface supplied by the GRILL DIS for Unreal plugin and also should have a DIS Component attached to them. Implement the DIS Interface functions to return the associated DIS component of the actor. Refer to the _**DIS Interface**_ section below.
    - **Exercise ID**: The exercise ID of the DIS sim this project will be associated with.
    - **Site ID**: The site ID of this DIS sim.
    - **Application ID**: The application ID of this DIS sim. 
    - **Auto Connect Send Addresses**: Whether or not the UDP socket(s) for sending DIS packets should be auto connected.
    - **Auto Connect Send Sockets**: The send sockets to automatically setup if 'Auto Connect Send Addresses' is enabled.
        - IP Address
            - The IP address to send DIS packets on. Should be a Multicast address if the socket connection type is set to Multicast.
        - Port
            - The port to send DIS packets on.
        - Send Socket Connection Type
            - The type of send socket to use. Options are: Broadcast, Multicast, and Unicast.
        - Socket Description
            - Friendly description for the the socket.
        - Buffer Size
            - The max buffer size for this socket.
    - **Auto Connect Receive Addresses**: Whether or not the UDP socket(s) for receiving DIS packets should be auto connected.
    - **Auto Connect Receive Sockets**: The receive sockets to automatically setup if 'Auto Connect Receive Addresses' is enabled.
        - IP Address
            - The IP address to receive DIS packets on. Should be a Multicast address if the socket connection type is set to Multicast.
            - _**NOTE:**_ An IP address of 0.0.0.0 will listen to all incoming DIS packets for broadcast connections.
        - Port
            - The port to receive DIS packets on.
        - Socket Description
            - Friendly description for the the socket.
        - Buffer Size
            - The max buffer size for this socket.
        - Use Multicast
            - Whether or not this socket will be receiving Multicast connections.
		- Allow Loopback
			- Whether or not to receive packets that originate from our local IP.
        - Receive Data on Game Thread
            - Whether or not this socket should receive data on the game thread. Will receive on its own thread if set to false.

![DISGameManagerSettings](Resources/ReadMeImages/DISGameManagerSettings.png)

- The DIS Game Manager can be accessed via a static function called 'GetDISGameManager'.
    - _**NOTE**_: This function performs a 'Get All Actors of Class' behind the scenes. Perform this function as little as possible.
    - This function returns the DIS Game Manager in the current level if one is found. If none or multiple are found, null is returned and an appropriate message is logged.
- The DIS Game Manager contains:
    - Listing of DIS Entities and their associated enumeration. This is loaded from the DIS Class Enum Mappings that needs to be set on the DIS Game Manager once it is placed in the level.
    - Listing of Entity IDs and their active DIS Entities in the world. This is a living list that is added to/removed from as new packets are received.
    - The DIS exercise, site, and application IDs.
- Notable functions:
    - Events for handling every PDU type currently implemented.
    - Add DIS Entity to Map
    - Remove DIS Entity from Map
	- Event for managing dead reckoning on all entities in the level.

![DISGameManagerFunctions](Resources/ReadMeImages/DISGameManagerFunctions.png)

# DIS Receive Actor Component

![DISReceiveComponent](Resources/ReadMeImages/DISReceiveComponent.png)

- The DIS Receive Component is responsible for handling all receive DIS functionality and DIS PDU updates for its associated DIS Entity.
- Handles dead reckoning and ground clamping updates.
    - Automatically handles both of these in C++.
- Contains various DIS related variables.
- Notable functions:
    - Ground Clamping
        - Can be overriden in blueprints for a custom implementation.
- Contains event bindings for:
    - Receiving each type of DIS Entity PDU currently implemented.
    - Dead reckoning update
	
![DISReceiveComponentEvents](Resources/ReadMeImages/DISReceiveComponentEvents.png)
	
- Has variables for:
    - Most Recent Entity State PDU
    - Dead Reckoning Entity State PDU
        - This is an Entity State PDU whose information has been updated with the most recent Dead Reckoning information.
    - Latest Entity State PDU Timestamp
    - Spawned From Network
        - Whether or not this entity was spawned from the network.
    - Entity Type _(Read Only)_
		- This record specifies the kind of entity, the country of design, the domain, the specific identification of the entity, and any extra information necessary for describing the entity.
        - This value gets set when an Entity State PDU or Entity State Update PDU is received for the associated entity.
    - Entity ID _(Read Only)_
		- This record specifies the site ID, application ID, and entity ID fields. They combine to form a unique identifier of the entity in the exercise.
        - This value gets set when an Entity State PDU or Entity State Update PDU is received for the associated entity.
    - Entity Force ID _(Read Only)_
		- This field distinguishes the different teams or sides in a DIS exercise.
        - This value gets set when an Entity State PDU or Entity State Update PDU is received for the associated entity.
    - Entity Marking _(Read Only)_
		- This record is used to specify the friendly name of the entity to be interpreted for display.
        - This value gets set when an Entity State PDU or Entity State Update PDU is received for the associated entity.
    - DIS Timeout
		- How long to wait in seconds after an Entity State PDU is received before deleting. Gets refreshed after an Entity State PDU is received.
	- DIS Culling Mode
		- Culls DIS packets based on settings
			- Options:
				- None
				- Cull Dead Reckoning
					- Cull Dead Reckoning updates. Distance updates get culled at is dictated by the 'DIS Culling Distance' variable.
				- Cull All
                    - Currently only culls Dead Reckoning updates.
	- DIS Culling Distance
		- The distance away from the camera that entities will start to have DIS packets culled.
    - Perform Dead Reckoning
        - Whether or not dead reckoning should be performed.
	- Perform Dead Reckoning Smoothing
		- Whether or not to smooth location/rotation changes after receiving a new Entity State update. Prevents entity teleporting.
	- Dead Reckoning Smoothing Period Seconds
		- Time in seconds that smoothing should take place.
    - Perform Ground Clamping
        - Performs ground clamping based on settings
			- Options:
				- None
				- Ground Clamp with DIS Options
					- Perform ground clamping. Ignore ground clamping for munitions and non-ground entity types.
				- Always Ground Clamp
					- Always perform ground clamping regardless of entity type.
    - Ground Clamping Collision Channel
        - The collision channel that should be used for ground clamping.

![DISReceiveComponentSettings](Resources/ReadMeImages/DISReceiveComponentSettings.png)

# DIS Send Actor Component

![DISSendComponent](Resources/ReadMeImages/DISSendComponent.png)

- The DIS Send Component handles basic sending DIS functionality its associated DIS Entity.
- Contains various DIS related variables.
- Notable functions:
	- Form Entity State PDU
		- Uses all known information to form an ESPDU for the associated DIS Entity.
    - Send Entity State PDU
        - Can be overriden in blueprints for a custom implementation.
	- Set Dead Reckoning Algorithm
		- Used to update the dead reckoning algorithm during runtime.
	
- Has variables for:
    - Most Recent Entity State PDU
    - Dead Reckoning Entity State PDU
        - This is an Entity State PDU whose information has been updated with the most recent Dead Reckoning information.
    - Entity Type
		- This record specifies the kind of entity, the country of design, the domain, the specific identification of the entity, and any extra information necessary for describing the entity.
        - This value should be set on the component and will be used when sending automatic PDU updates.
    - Entity ID
		- This record specifies the site ID, application ID, and entity ID fields. They combine to form a unique identifier of the entity in the exercise.
        - This value should be set on the component and will be used when sending automatic PDU updates.
    - Entity Force ID
		- This field distinguishes the different teams or sides in a DIS exercise.
        - This value should be set on the component and will be used when sending automatic PDU updates.
    - Entity Marking
		- This record is used to specify the friendly name of the entity to be interpreted for display.
        - This value should be set on the component and will be used when sending automatic PDU updates.
    - DIS Heartbeat Seconds
		- How often a new PDU update should be sent out.
		- Utilized if Dead Reckoning Thresholds are not clipped.
	- Entity State PDU Sending Mode
		- Mode that the send component should be in.
			- Options:
				- None
					- Don't send any automatic Entity State or Entity State Update PDU updates.
				- Entity State PDU
					- Automatically send out Entity State PDU updates.
					- Will send out a new PDU when a Dead Reckoning Threshold is clipped, the DIS heartbeat expires, when the Dead Reckoning algoritm is changed, or when the entity expires in the world.
				- Entity State Update PDU
					- Automatically send out Entity State Update PDU updates.
					- Will send out a new PDU when a Dead Reckoning Threshold is clipped, the DIS heartbeat expires, or when the entity expires in the world.
	- Dead Reckoning Algorithm
		- The dead reckoning algorithm to use.
    - Dead Reckoning Position Threshold Meters
        - The position threshold in meters to use for dead reckoning. If the dead reckoning position deviates more than this value away from the actual position in any axis, a new Entity State PDU will be sent.
    - Dead Reckoning Orientation Threshold Degrees
        - The orientation threshold in degrees to use for dead reckoning. If the dead reckoning orientation deviates more than this value away from the actual orientation, a new Entity State PDU will be sent.

![DISSendComponentSettings](Resources/ReadMeImages/DISSendComponentSettings.png)

# DIS Interface

- The DIS Interface is required to implement for any actor that is desired to be a DIS entity.
- The DIS Interface allows for actors to appear in the Entity section of the DIS Enumeration Mappings UAsset described below.
- After adding the DIS Interface, make sure to implement the below functions as needed:
	- Its 'GetActorDISReceiveComponent' function and pass the DIS Receive Component of the actor as the return value.
	- Its 'GetActorDISSendComponent' function and pass the DIS Send Component of the actor as the return value

![DISInterface](Resources/ReadMeImages/DISInterface.png)

# DIS Enumeration Mappings

![DISEnumMappingsUAsset](Resources/ReadMeImages/DISEnumMappingsUAsset.png)

- The DIS Enumeration Mappings UAsset allows for mappings between DIS Enumerations and Unreal Engine actors to be made.
- To create a new DIS Enumeration Mapping UAsset:
    - Right click in the 'Content Browser' of Unreal Engine.
    - Hover over 'Blueprints' of the menu that appears.
    - Select the 'DIS Enumeration Mappings' option.
- Once created, mappings can be created by opening up the UAsset.
- The settings it contains are:
    - DIS Class Enumeration Mappings
        - Mapping of all available Actor to DIS Enumeration mappings.
    - Friendly Name
        - Friendly name of this mapping for easy user lookup
    - DIS Entity
        - The Unreal Engine actor that this mapping should point to.
        - _**NOTE**_: In order for actors to appear in this section, they need to implement the DIS Interface. See the _**DIS Interface**_ section above.
    - Associated DIS Enumerations
        - All desired DIS Enumerations that should point to this actor.
        - _**NOTE**_: If duplicate enumerations are found across multiple entities, an appropriate message is logged and the most recent encountered actor to enumeration mapping is used.
        - _**NOTE**_: If a DIS Enumeration is received on the network and no mapping exists for it, an appropriate message is logged and the packet is ignored.
        - For a breakdown of the individual elements of a DIS Enumeration, refer to the [Naval Postgraduate School's Documentation](http://faculty.nps.edu/brutzman/vrtp/mil/navy/nps/disenumerations/jdbehtmlfiles/pdu/28.htm#:~:text=Description%3A%20The%20type%20of%20entity,necessary%20for%20describing%20the%20entity.).

![DISEnumMappingsSettings](Resources/ReadMeImages/DISEnumMappingsSettings.png)

# DIS Blueprint Function Library

- Contains functions for converting between geospatial coordinates and Unreal Engine coordinates.
- Utilizes the GeoReferencing plugin made by Epic Games for conversions to and from Unreal Engine coordinates.

![BPFLFunctions](Resources/ReadMeImages/BPFLFunctions.png)

# PDU Conversion Blueprint Function Library

- Contains functions for converting PDU types to bytes.
	- This allows for sending of the PDUs for example.

![PDUConversions](Resources/ReadMeImages/PDUConversions.png)

# Dead Reckoning Blueprint Function Library

- Contains functions for performing Dead Reckoning

![DeadReckoningBPFL](Resources/ReadMeImages/DeadReckoningBPFL.png)