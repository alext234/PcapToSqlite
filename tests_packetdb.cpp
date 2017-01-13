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
        auto data = packet();
        ASSERT_THAT(equal(data.begin(), data.end(), vector<uint8_t>{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20}.begin()), Eq(true));
        
    }
    {
        vector<uint8_t> d(1024);
        iota(d.begin(), d.end(), 1);
        Packet packet (d);
        ASSERT_THAT (packet.len(), Eq(d.size()));
        
        auto data = packet();
        ASSERT_THAT(equal(data.begin(), data.end(), d.begin()), Eq(true));
        
    }
    
    

}


TEST(Packets, CopyPacket) {
    {
        vector<uint8_t> d(1024);
        iota(d.begin(), d.end(), 1);
        Packet packet (d);    
        
        
        Packet packet2(packet);//copy constructor
        
        // check len
        ASSERT_THAT (packet.len(), Eq(packet.len()));
        
        // check content
        ASSERT_THAT(equal(packet2().begin(), packet2().end(), packet().begin()), Eq(true));
        
        // change content of original packet
        for (int i=0; i< packet.len(); ++i) {
            packet[i]+=10;
        }
            
        // packet is now different from packet2
        ASSERT_THAT(equal(packet2().begin(), packet2().end(), packet().begin()), Eq(false));
        
    }
    {
        vector<uint8_t> d(1024);
        iota(d.begin(), d.end(), 1);
        Packet packet (d);    
        
        
        Packet packet2;
        packet2=packet; // assignment operator
        
        // check len
        ASSERT_THAT (packet.len(), Eq(packet.len()));
        
        // check content
        ASSERT_THAT(equal(packet2().begin(), packet2().end(), packet().begin()), Eq(true));
        
        // change content of original packet
        for (int i=0; i< packet.len(); ++i) {
            packet[i]+=10;
        }
            
        // packet is now different from packet2
        ASSERT_THAT(equal(packet2().begin(), packet2().end(), packet().begin()), Eq(false));
        
    }
    
}

TEST(Packets, getMacAddresses) {
    // TODO: generate 0 size packet  --> get mac address should have exception
    
    // TODO: get actual packet from 1.pcap --> verify the extracted mac address
}

TEST(PacketDb, clearAll) {
    PacketDb db("packets.db");
    db.clearAll(); // clear all tables
    
}

TEST(PacketDb, insertAndRetrieve) {
    vector<uint8_t> d(1024);
    iota(d.begin(), d.end(), 1);
    Packet packet (d);
    
    PacketDb db("packets.db");        
    db.insert (packet);
    // TODO: retrieve and verify

    
    
    

}
