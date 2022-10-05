# Beta 0.4.1

- Updated Angular Velocity calculations to utilize quaternions rather than euler angles in the DIS Send Component. 
- Moved ESPDU variable calculations in DIS Send Component for linear velocity, linear acceleration, and angular velocity to a timer to give user more flexibility over frequency of them.
- Fixed small math errors in DIS_BPFL where ACos functions were missing where needed.
- Updated calculations for the body space linear acceleration in DISSendComponent.
- Updated DeadReckoning_BPFL to take into account the Endianness of the machine when necessary. 
- Added in enums for the Dead Reckoning algorithms.
- Added in setting of Entity Appearance and Capabilities within the DIS Sending Component. PDUs get emitted when changed as needed.
- Fixed issue with Dead Reckoning smoothing where it wouldn't take the shortest route when rotating.
- Bug fix to invert Linear Velocity Z direction on forming ESPDU using Body DR algorithms. Also fix to Body DR algorithm if Angular Velocity is 0.
- Fixed bug where Entity Marking would overflow when being converted from OpenDIS to UE4.

# Beta 0.4.0

- Separated DIS Component into two separate components (DIS Sender and DIS Receiver). 
	- DIS Sender has built in default functionality for sending ESPDUs. 
	- DIS Receiver has built in functionality for receiving DIS PDUs. Has associated event handlers for these events.	
- Moved Dead Reckoning functions into a Dead Reckoning BPFL.
- Fixed issue with Spawn from Network variable race case.
- Network DIS Entities are now spawned at proper ECEF/rotation as specified by the given ESPDU.
- Updated articulated parameters to look at the Record Type when converting to/from Open DIS. The Parameter Value changes depending on the Record Type parameter.

# Beta 0.3.0

- Updated variable categories to all be under the same parent GRILL DIS category.
- Removed project settings and toolbar button from the plugin.
- Added copyright header to all GRILL source files.
- Added in licensing files
- Removed unit tests

# Beta 0.2.0 -- Initial UE5 version

- Added in event handlers for start/resume PDUs and stop/freeze PDUs.
- Updated Ground Clamping to be an enum in DIS Component.
- Switch yaw and roll in DR3 calculation
- Add small offset to AngularVelocity if magnitude is 0
- Added Entity Marking and Entity ForceID as individual variables in DIS Component.
- Fix to CloseSendSocket in UDPSubsystem. If an item was not found, it would crash trying to dereference a null pointer
- Fixed error if send socket not initialized appropriately.
- Added in UDP functions to check if a connection is open and to get all IDs of connected send/receive sockets.
- Added in additional conversion functions for getting UE location/rotation from HPR, PTP, LLH, and ECEF.
- Updated Ground Clamping to use NED of UE coordinates to always clamp in appropriate direction.
- Added in switch statement for ground clamping to determine earth shape being used in the project (flat vs round). This is ties to the GeoReferencing plugin.
- Added connection types to UDP settings. Sending sockets now supports Multicast, Broadcast, and Unicast. Receiving sockets now support Multicast.
- Added in additional settings to the receive sockets to allow for packets coming from self (local) to be ignored.
- Added in option to enable/disable smoothing
- Added in culling for dead reckoning packets. If enabled, entities outside a range will no longer perform dead reckoning.
- Add ground clamping unit tests
- Add unit test metrics for DR, UDP, and PDU processes
- Modified PDUProcessor to not rely on OpenDIS PDU Bank. It has an incomplete implementation of the PDU types and is not needed for our use case.
- Update to take in the ExerciseID into account prior to using packets.
- Verify DeadReckoning is enabled/supported prior to trying to smooth.
- Updated UDP sockets to allow for multiple different sockets to be able to be open at once. Moved UDP auto connect settings to DIS Game Manager.

# Beta 0.1.2 -- Carried over from UE4 version

- Added in check for the DIS Class Enum Mapping to verify it has been set in the Game Manager prior to using.
- Added in entity location smoothing

# Beta 0.1.1 -- Carried over from UE4 version

- Small update to add in friendly names to the DIS Enumeration mappings.
- Updated additional std::map variable I missed to utilize the std::map functions to avoid creating null entries.
- Modified PDUs to be contained in structs. Reworked file hierarchy to contain folders for each PDU family type. Fixed various bugs and errors. Added in PDUConversions_BPFL to help with accessing PDU struct functions.
- Small update to Game Manager tick to utilize the DISInterface when getting DIS Component.
- Removed trying to spawn an entity from an Entity State Update PDU. These PDUs do not contain an Entity Type, so we can't key off of it in our map.
- Move dead reckoning from component tick to manager tick
- Removed unneeded DISEntity_Base from project and all header inclusions it had.
- Updated plugin enumeration mappings to be stored in a custom Unreal Engine asset named DISClassEnumMappings.
- Updated DIS Game State to be DIS Game Manager and it is now of the AInfo class type rather than game state.

# Beta 0.1.0 -- Carried over from UE4 version

- Added UPROPERTY tag to the PDU variables for MostRecentEntityState and DeadReckonedPDU. Fixes them being auto garbage collected.
- Fixed race case with dead reckoning. Added in IsValid checks prior to using any UObject PDU.
- Fix 64 bit entity type representation
- Update to make the Entity State PDU and Dead Reckoning PDU getters blueprint pure.
- Updated accessibility of the Entity State and Dead Reckoning info in the DIS Component to have to go through getters/setters.
- Changed structs to Classes for all PDU types. Allows for functions and variables to be inherited across PDUs of like-type
- Add STL maps for 64 bit mapping
- Added in log warning if the user has multiple instances of the same enumeration.

# Beta 0.0.1 -- Carried over from UE4 version

- Update functions to use hierarchical structs
- Added missing DIS PDU variables and missing structs for inheritance. Added in additional PDU to bytes functions.
- Add edge cases to struct packing
- Consolidate struct->DIS packing
- Added ground clamping. Only happens after Entity State PDUs received, after Entity State Update PDUs received, and after dead reckoning is performed.
- Add Unreal->HeadingPitchRoll conversion
- Added Unreal->NED/ENU conversion
- Added ENU<->NED conversions
- Added engine version and support link to the plugin file.
- Added in automatic loading of OpenDIS6 dll in built versions of the project.
- Added GeoReferencing Plugin. Updated conversion functions going to and from Unreal to utilize the plugin.
- Add conversion to unreal rotations
- Added in structs for HeadingPitchRoll, ECEF, PsiThetaPhi, LLH, ENU, and NED.
- Added use of Dead Reckoning other parameters
- Added World Origin variables for LLA and ENU into the OpenDIS project settings.
- Implemented dead reckoning algorithms
- Added DIS unit conversions in float and double precision
- Added in converting received Stop/Freeze PDUs and Start/Resume PDUs to a supported struct. They are not utilized anywhere, but events are broadcasted from the PDUProcessor when they are received and processed into corresponding structs.
- Added in handling of Entity State Update PDUs.
- Added in SpawnedFromNetwork flag into the OpenDISComponent to check if an entity is owned by the network or not.
- Updated names of OpenDIS network settings in the project settings to better reflect how they are utilized.
- Moved UDP and PDU processing into GameInstance subsystems. 
- Moved DIS related structs and enums into their own class to separate them from the game state.
- Added in check to make sure receive socket is set prior to binding.
- Added in a base class to use for the DIS Entities.
- Implemented UDP functionality and began wrapping it in with the UEOpenDISGameState.
- Added in ECEF<->LLH conversions
- Added in checks to verify that the RemoveEntityPDU was meant for our sim prior to processing it.
- Added in check to see if the EntityStatePDU indicates the receiving entity has been deactivated.
- Added in heartbeat to delete an entity if it has not received an associated EntityStatePDU in so long.
- Added in DIS<->Unreal conversions
