#include "packetid.hh"
#include <cstring>


void insert_vertexID(VertexID root_id, std::vector<VertexID>& vid_array, int level)
{
    int total = static_cast<int>(pow(2, level));
    VertexID base = root_id << level;
    for (int i = 0; i< total; i++)
    {
        vid_array.push_back(base+i);
    }
}



void packetID_to_vertexID_array(PacketID pid, std::vector<VertexID>& vid_array
                                ,int level_count)
{
    vid_array.clear();
    int total = static_cast<int>(pow(2, level_count));
    for (int j = 0; j<total; j++)
    {
        VertexID root_id = (pid<<level_count)+j;
        for (int i = 0; i<level_count;i++)
        {
            insert_vertexID(root_id, vid_array, i);
        }
    }
}

PacketID v_id_to_p_id(VertexID v_id, Ppmesh& ppmesh)
{
    PacketID pid = 0;
    VertexID root_id = 0;
    unsigned int level = ppmesh.id2level(v_id);
    unsigned int shift = (level - 1) % 4;
    root_id = v_id >> shift;
    pid = root_id >> 4;
    return pid;
}

PacketID readPacketID(std::istream& is)
{
    PacketID id;
    is.read((char *)&id, sizeof(id));
    return id;
}

PacketID readPacketID(const char* buffer)
{
    PacketID id;
    memcpy(&id, buffer, sizeof(id));
    return id;
}

void writePacketID(PacketID pid, std::ostream& os)
{
    os.write((const char *)&pid, sizeof(pid));
}

void writePacketID(PacketID pid, char* buffer)
{
    memcpy(buffer, (const char*)&pid, sizeof(pid));
}

