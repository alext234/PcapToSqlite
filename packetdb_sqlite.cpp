#include "packetdb.h"
#include <sqlite_modern_cpp.h>
#include <vector>

namespace packetdb {
    using namespace std;
    using namespace  sqlite;

    
    struct PacketDb::PacketDbPrivate {
        PacketDbPrivate()=delete;
        PacketDbPrivate(string dbFile) : db(make_shared<database>(dbFile)) {
        
        }
        
        // some db helper 
        void deleteTable(string name)  { 
            *db << "drop table if exists "+name;
            
        }
        void clearAll() {
            deleteTable("packets");
        }
        void createTables() {
            *db <<  "create table if not exists packets ("
                    " _id integer primary key autoincrement not null,"
                    "srcmac char(12),"
                    "dstmac char(12),"
                    "rawdata blob"
                    ");"
                    ;
        }
        shared_ptr<database> db;
    };
    
    PacketDb::PacketDb(string dbFile) : pp(make_shared<PacketDbPrivate>(dbFile))  { 
        pp->createTables();
    }
    PacketDb::~PacketDb(){}
    
    
    void PacketDb::clearAll() {
        
        pp->clearAll ();
        
        // recreate
        pp->createTables();
    }
    
    long long PacketDb::insert (const Packet& packet){
        
        MacType srcMac = packet.getSrcMac();
        MacType dstMac = packet.getDstMac();
                        
        uint16_t retries = 100;
        while (retries >0 ) {
            try {
                *pp->db << "insert into packets(srcmac, dstmac, rawdata) values(?,?,?)"<< macTypeToString(srcMac)<<macTypeToString(dstMac)<<packet();            
                
            } catch (const sqlite::exceptions::locked& ex) {
                --retries;
            } catch (const sqlite::exceptions::busy& ex) {
                --retries;
            }
            return pp->db-> last_insert_rowid();
        }
        throw PacketDbException("Unable to insert. database seems to locked for long time");
         
    };
    
    Packet PacketDb::retrievePacketById(long long _id, uint16_t offset, int16_t n) {
        vector<uint8_t> rawData;        
        // select substr(rawdata,2,10) from packets;
        offset +=1; // sqlite offset starts from 1, not 0
        if (n==-1) {
            *pp->db <<"select substr(rawdata,?) from packets where _id=?"<< (int)offset<< _id  >> rawData;     
        } else {
            *pp->db <<"select substr(rawdata,?,?) from packets where _id=?"<< (int)offset << (int)n << _id  >> rawData;     
        }
            
        
        
        return Packet(rawData);
    }
}