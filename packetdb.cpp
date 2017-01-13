#include "packetdb.h"
#include <vector>

namespace packetdb {
    using namespace std;
    
    struct Packet::PacketPrivate {
        vector<uint8_t> data;
        PacketPrivate () {}        
        PacketPrivate (const PacketPrivate& packet) {
            data = packet.data;
        }
    };
    
    
    Packet::Packet() : pp(make_shared<PacketPrivate>()) {
    }
    
    Packet::Packet(const Packet& packet): pp(make_shared<PacketPrivate>(*packet.pp)){
        
       
    }
    Packet& Packet::operator=(const Packet& packet) {
        pp = make_shared<PacketPrivate> (*packet.pp);
        return *this;
    }
        
    Packet::Packet(const vector<uint8_t>& rawData): Packet()
    {
        pp->data=move(rawData);
    }
    Packet::~Packet() {}
    
    const vector<uint8_t>& Packet::operator() () {
        return pp->data;
    }
    
    uint16_t Packet::len() {
        return pp->data.size();
    }
    
    uint8_t& Packet::operator[] (int i) {
        return pp->data[i];
    }
    
 

}