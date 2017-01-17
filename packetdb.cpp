#include "packetdb.h"
#include <vector>
#include <sstream>
#include <iomanip>
#include <iostream>

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
    
    const vector<uint8_t>& Packet::operator() () const {
        return pp->data;
    }
    
    uint16_t Packet::len() const {
        return pp->data.size();
    }
    
    uint8_t& Packet::operator[] (int i) {
        return pp->data[i];
    }
    
    MacType Packet::getSrcMac() const {
        if (len()<12) throw PacketException("Packet does not have source MAC address");;
        auto d = pp->data;
        return MacType{d[6],d[7],d[8],d[9],d[10],d[11]};
    }
    
    MacType Packet::getDstMac() const {
        if (len()<6) throw PacketException("Packet does not have destionation MAC address");;
        auto d = pp->data;
        return MacType{d[0],d[1],d[2],d[3],d[4],d[5]};
        
    }

        
    std::string macTypeToString (MacType mac) {
        ostringstream ss;
        
        for (auto it =mac.cbegin(); it!=mac.cend(); ++it) {
           if (it!=mac.cbegin()) ss<<":";
           ss<<setfill('0')<<setw(2)<<uppercase<<hex<<int(*it);
        }
        return ss.str();
    }
    
    MacType stringToMacType (string mac) {
        istringstream ss(mac);
        string s;
        MacType ret;
        auto retIt = ret.begin();
        while (getline(ss, s, ':')) {
            if (retIt==ret.end()) {
                throw UtilityException("Invalid MAC string: "+mac);
            }
            *retIt  = stoi (s, 0, 16);
            ++retIt;
            
        }
        return ret;

    }
    
 

}