#include "gmock/gmock.h"
#include "packetdb.h"
#include <cstdint>
#include <numeric>
#include "cpppcap.h"
#include "cpp_observer.h"


using namespace testing;
using namespace std;
using namespace packetdb;
const string  TEST_DB ="packets_testdb.db";

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
    
    {
        Packet packet; 
        try {
            auto srcMac = packet.getSrcMac();
            auto dstMac = packet.getDstMac();            
            ASSERT_THAT(0, Eq(1));//  should not reach here
            srcMac = MacType{0}; // meaningless assignemt to avoid compiler warning
            dstMac= MacType{0};// meaningless assignemt to avoid compiler warning
        } catch (const PacketException& ex) {
            
        }
        
        
        
    }

    
    {
        std::string pcapFile{SAMPLE_PCAP_DIR};
        pcapFile+="1.pcap";
        
    
        auto dev = Pcap::openOffline(pcapFile);
        // register observer 
        dev->registerObserver([](const Pcap::Packet& p){
            packetdb::Packet packet (p.data());
            MacType dstMac = packet.getDstMac();            
            ASSERT_THAT(dstMac, Eq(MacType{0xbc, 0xdf, 0x20, 0x00, 0x01, 0x00}));
            MacType srcMac = packet.getSrcMac();
            ASSERT_THAT(srcMac, Eq(MacType{0x00, 0x00, 0x01, 0x00, 0x00, 0x00}));
        });

    
        dev->loop();        
    }
    
}

TEST(PacketDb, clearAll) {
    PacketDb db(TEST_DB);
    db.clearAll(); // clear all tables
    
}


TEST(PacketDb, insertAndRetrieve) {

    
    PacketDb db(TEST_DB);        
    {
        vector<uint8_t> d(1024);
        iota(d.begin(), d.end(), 1);
        Packet packet (d);
        auto insertedRowId = db.insert (packet);
        
        //  retrieve and verify
        
        Packet retrievedPacket = db.retrievePacketById(insertedRowId );
        ASSERT_THAT (retrievedPacket(), Eq(d));
        
    }

    {
        // retrieve nonexistent packet
        db.clearAll(); // clear all tables
        try {
            Packet retrievedPacket = db.retrievePacketById(1 );
            ASSERT_THAT(0, Eq(1));//  should not reach here
        } catch (const exception& ex) {
        }
        
    }
    {
        // retrieve sub packet with specified len
        vector<uint8_t> d(1024);
        iota(d.begin(), d.end(), 1);
        Packet packet (d);
        auto insertedRowId = db.insert (packet);
        
        //  retrieve and verify
        
        Packet retrievedPacket = db.retrievePacketById(insertedRowId,6, 20 );        
        auto pd = retrievedPacket ();
        
        ASSERT_THAT (equal(pd.cbegin(), pd.cend(), d.cbegin()+6, d.cbegin()+26), Eq(true));
        
    }
    
}


TEST(PacketDb, insertAndRetrieveMultiple) {
        std::string pcapFile{SAMPLE_PCAP_DIR};
        pcapFile+="2.pcap";
        
    
        map<long long, vector<uint8_t>> m;
        
        auto dev = Pcap::openOffline(pcapFile);
        
        PacketDb db(TEST_DB);        
        // register observer 
        dev->registerObserver([&m,&db](const Pcap::Packet& p){
            packetdb::Packet packet (p.data());
            auto _id = db.insert (p.data());
            m[_id ]= p.data();
        });

    
        dev->loop();       
        
        for(auto it = m.begin(); it != m.end(); ++it) {
            auto _id = it->first;
            auto rawData = it->second;
            
            Packet retrievedPacket = db.retrievePacketById(_id );
            ASSERT_THAT (retrievedPacket(), Eq(rawData));
            
        }
    
}