#ifndef __VERTEX_ID_H__
#define __VERTEX_ID_H__
#include <string>
#include <bitset>
#include <iostream>
#include "bitstring.hh"
/** @file  
 *  The interface to handle vertexID.
 */

/**
 * the VertexID type. Currently is just an alias of unsigned int.
 * But do not rely on this. It can be changed in the future.
 */
typedef unsigned int VertexID;
//Exceptions.
class   DecodeIdError{};
class   EndOfBitString{};

/**
 * to check a given positon of ID is set or not.
 */
bool    idIsSet(VertexID id, size_t pos);

/** 
 * return the length of the ID, ignoring the leading 1.
 */
size_t      len_of_id(VertexID id);

/**
 * convert to a string representation.
 */
std::string to_string(VertexID id);

/**
 * Encode an array of ID into a bitstring. How many bits are used as tree_bits needs to be given.
 */
void        id_encode(const std::vector<VertexID>& id_array, size_t tree_bits, BitString& bs);

/**
 * Decode a bitstring to an array of IDs. How many bits are used as tree_id needs to be given.
 */
void        id_decode(const BitString& bs, size_t tree_bits, std::vector<VertexID>& id_array);

/**
 * Write a ID into ostream in binary form.
 */
void        write_id_binary(std::ostream& os, VertexID id);

/**
 * Read a ID from an istream in binary form.
 */
VertexID        read_id_binary(std::istream& is);
#endif
