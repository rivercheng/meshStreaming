#ifndef __PACKET_ID_H__
#define __PACKET_ID_H__
#include <iostream>
#include "vertexid.hh"
#include "ppmesh.hh"

/** @file  
 *  The interface to handle packetID.
 */

/**
 *The PacketID type. Currently is just an alias of VertexID. 
 */ 
typedef VertexID PacketID;

/**
 * This function generates an array of vertex ID which are all children of vertex with root_id at the given depth (level of root_id + level).
 * In total 2^level vertex IDs are be generated.
 */
void insert_vertexID(VertexID root_id, std::vector<VertexID>& vid_array, int level);


/**
 * Given a packet ID, generate all the vertex IDs belongs to this packet.
 */
void packetID_to_vertexID_array(PacketID pid, std::vector<VertexID>& vid_array
                                ,int level_count);


/**
 * Given a vertex ID, deduce its packet ID. 
 */
PacketID v_id_to_p_id(VertexID v_id, Ppmesh& ppmesh);

/**
 * read binary representation of packet ID from istream.
 */
PacketID readPacketID(std::istream& is);

/**
 * read binary representation of packet ID from buffer.
 */
PacketID readPacketID(const char* buffer);

/**
 * write packet ID in binary form to ostream.
 */
void writePacketID(PacketID pid, std::ostream& os);

/**
 * write packet ID in binary form to buffer.
 */
void writePacketID(PacketID pid, char* buffer);
#endif
