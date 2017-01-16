#ifndef __PACKET_DB__
#define __PACKET_DB__
#include <memory>
#include <vector>
#include <array>
namespace packetdb {

    
    class PacketException: public std::exception {
    public:
        PacketException(std::string r): _reason(r) {}
        const char* what() const noexcept {
            return _reason.c_str();
        }
        private:
        std::string _reason;
    };

    using MacType= std::array<uint8_t,6>;
    
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
        
        MacType getSrcMac();
        MacType getDstMac();
        
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
