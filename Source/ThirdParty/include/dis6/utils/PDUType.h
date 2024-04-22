#ifndef _dtdis_pdu_type_h_
#define _dtdis_pdu_type_h_

namespace DIS
{
   enum PDUType
   {
      PDU_OTHER = 0,
      PDU_ENTITY_STATE = 1,
      PDU_FIRE = 2,
      PDU_DETONATION = 3,
      PDU_COLLISION = 4,
      PDU_SERVICE_REQUEST = 5,
      PDU_RESUPPLY_OFFER = 6,
      PDU_RESUPPLY_RECEIVED = 7,
      PDU_RESUPPLY_CANCEL = 8,
      PDU_REPAIR_COMPLETE = 9,
      PDU_REPAIR_RESPONSE = 10,
      PDU_CREATE_ENTITY = 11,
      PDU_REMOVE_ENTITY = 12,
      PDU_START_RESUME = 13,
      PDU_STOP_FREEZE = 14,
      PDU_ACKNOWLEDGE = 15,
      PDU_ACTION_REQUEST = 16,
      PDU_ACTION_RESPONSE = 17,
      PDU_DATA_QUERY = 18,
      PDU_SET_DATA = 19,
      PDU_DATA = 20,
      PDU_EVENT_REPORT = 21,
      PDU_COMMENT = 22,
      PDU_ELECTRONIC_EMMISIONS = 23,
      PDU_DESIGNATOR = 24,
      PDU_TRANSMITTER = 25,
      PDU_SIGNAL = 26,
      PDU_RECEIVER = 27,
      PDU_IFF = 28,
      PDU_UNDERWATER_ACOUSTIC = 29,
      PDU_SUPPLEMENTAL_EMISSION_ENTITY = 30,
      PDU_INTERCOM_SIGNAL = 31,
      PDU_INTERCOM_CONTROL = 32,
      PDU_AGGREGATE_STATE = 33,
      PDU_ISGROUPOF = 34,
      PDU_TRANSFER_OWNERSHIP = 35,
      PDU_ISPARTOF = 36,
      PDU_MINEFIELD_STATE = 37,
      PDU_MINEFIELD_QUERY = 38,
      PDU_MINEFIELD_DATA = 39,
      PDU_MINEFIELD_RESPONSE_NACK = 40,
      PDU_ENVIRONMENTAL_PROCESS = 41,
      PDU_GRIDDED_DATA = 42,
      PDU_POINT_OBJECT_STATE = 43,
      PDU_LINEAR_OBJECT_STATE = 44,
      PDU_AREAL_OBJECT_STATE = 45,
      PDU_TSPI = 46,
      PDU_APPEARANCE = 47,
      PDU_ARTICULATED_PARTS = 48,
      PDU_LE_FIRE = 49,
      PDU_LE_DETONATION = 50,
      PDU_CREATE_ENTITY_R = 51,
      PDU_REMOVE_ENTITY_R = 52,
      PDU_START_RESUME_R = 53,
      PDU_STOP_FREEZE_R = 54,
      PDU_ACKNOWLEDGE_R = 55,
      PDU_ACTION_REQUEST_R = 56,
      PDU_ACTION_RESPONSE_R = 57,
      PDU_DATA_QUERY_R = 58,
      PDU_SET_DATA_R = 59,
      PDU_DATA_R = 60,
      PDU_EVENT_REPORT_R = 61,
      PDU_COMMENT_R = 62,
      PDU_RECORD_R = 63,
      PDU_SET_RECORD_R = 64,
      PDU_RECORD_QUERY_R = 65,
      PDU_COLLISION_ELASTIC = 66,
      PDU_ENTITY_STATE_UPDATE = 67,
      PDU_DIRECTED_ENERGY_FIRE = 68,
      PDU_ENTITY_DAMAGE_STATUS = 69,
      PDU_INFORMATION_OPERATIONS_ACTION = 70,
      PDU_INFORMATION_OPERATIONS_REPORT = 71,
      PDU_ATTRIBUTE = 72
   };
}

#endif // _dtdis_pdu_type_h_