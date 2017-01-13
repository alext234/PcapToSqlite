#include "gmock/gmock.h"
#include "packetdb.h"
#include <cstdint>
#include <numeric>
using namespace testing;
using namespace std;
using namespace packetdb;

TEST(Packets, CreatePacket) {
    {
        Packet packet;
        
        ASSERT_THAT (packet.len(),Eq(0));
    }
    {
        
        Packet packet({0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20});
        ASSERT_THAT (packet.len(), Eq(21));
        // TODO: verify the content of the packet
    }
    {
        vector<uint8_t> d(1024);
        iota(d.begin(), d.end(), 1);
        Packet packet (d);
        ASSERT_THAT (packet.len(), Eq(d.size()));
        // TODO: verify the content of the packet
        
    }
    
    

}

TEST(PacketDb, StoreAndRetrievePacket) {
    PacketDb db;
    // TODO: make namespace packetdb; Packet class which allow initializing packets from raw data; store packet into db, read out to verify

}
