#ifndef __PACKET_DB__
#define __PACKET_DB__
#include <memory>
#include <vector>
#include <array>
namespace packetdb {

    
    class Exception: public std::exception {
    public:
        Exception(std::string r): _reason(r) {}
        const char* what() const noexcept {
            return _reason.c_str();
        }
        private:
        std::string _reason;
    };
    
    class PacketException: public Exception {  using Exception::Exception;  };
    class UtilityException: public Exception { using Exception::Exception;   };    
    
    
    using MacType= std::array<uint8_t,6>;
    
    class Packet {
    public:        
        Packet();
        Packet(const Packet& packet);
        Packet(const std::vector<uint8_t>& rawData) ;
        ~Packet() ;
        
        uint16_t len() const ;
        const std::vector<uint8_t>& operator() () const ;
        Packet& operator=(const Packet& packet);
        uint8_t& operator[] (int i) ;          
        
        MacType getSrcMac() const;
        MacType getDstMac() const;
        
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
        long long insert (const Packet& packet);
        Packet retrievePacketById(long long _id);
        void clearAll() ; 
        
    private:
        class PacketDbPrivate;
        std::shared_ptr<PacketDbPrivate> pp;
        
    };
    
    std::string macTypeToString (MacType mac);
    MacType stringToMacType (std::string  mac);
}
#endif //__PACKET_DB__
