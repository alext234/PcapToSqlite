#ifndef __PACKET_DB__
#define __PACKET_DB__
#include <memory>
#include <vector>
namespace packetdb {

    class Packet {
    public:
        Packet();
        Packet(const std::vector<uint8_t>& rawData) ;
        
        uint16_t len();
        const std::vector<uint8_t>& operator() () ;
    
    private:
        class PacketPrivate;
        std::shared_ptr<PacketPrivate> pp;
        
    };

    class PacketDb {
        
    };
}
#endif //__PACKET_DB__
