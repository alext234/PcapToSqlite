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
    
    PacketDb::PacketDb(string dbFile) : pp(make_shared<PacketDbPrivate>(dbFile))  { }
    PacketDb::~PacketDb(){}
    
    
    void PacketDb::clearAll() {
        
        pp->clearAll ();
        
        // recreate
        pp->createTables();
    }
    
    long long PacketDb::insert (const Packet& packet){
        
        MacType srcMac = packet.getSrcMac();
        MacType dstMac = packet.getDstMac();
        
        
        
        *pp->db << "insert into packets(srcmac, dstmac, rawdata) values(?,?,?)"<< macTypeToString(srcMac)<<macTypeToString(dstMac)<<packet();
        return pp->db-> last_insert_rowid();
    };
}