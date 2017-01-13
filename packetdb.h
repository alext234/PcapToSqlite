#ifndef __PACKET_DB__
#define __PACKET_DB__
#include <memory>
#include <vector>
namespace packetdb {

    class Packet {
    public:        
        Packet();
        Packet(const Packet& packet);
        Packet(const std::vector<uint8_t>& rawData) ;
        ~Packet() ;
        
        uint16_t len();
        const std::vector<uint8_t>& operator() () ;
        Packet& operator=(const Packet& packet);
        uint8_t& operator[] (int i) ;          
    
    private:
        class PacketPrivate;
        std::shared_ptr<PacketPrivate> pp;
        
    };

    class PacketDb {
    public:
        PacketDb(std::string dbFile);
        ~PacketDb();
        PacketDb()=delete;
        PacketDb(const PacketDb&) = delete;
        PacketDb& operator= (const PacketDb&) = delete;
        void insert (const Packet& packet);
        
        void clearAll() ; 
        
    private:
        class PacketDbPrivate;
        std::shared_ptr<PacketDbPrivate> pp;
        
    };
}
#endif //__PACKET_DB__
