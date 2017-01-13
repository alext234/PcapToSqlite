#include "packetdb.h"
#include <sqlite_modern_cpp.h>
#include <vector>

namespace packetdb {
    using namespace std;
    
    struct Packet::PacketPrivate {
        vector<uint8_t> data;
    };
    
    
    Packet::Packet() : pp(make_shared<PacketPrivate>()) {
    }
    
    Packet::Packet(const vector<uint8_t>& rawData): Packet()
    {
        pp->data=move(rawData);
    }
    
    
    uint16_t Packet::len() {
        return pp->data.size();
    }
}