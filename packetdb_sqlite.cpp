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
        
        shared_ptr<database> db;
    };
    
    PacketDb::PacketDb(string dbFile) : pp(make_shared<PacketDbPrivate>(dbFile))  { }
    PacketDb::~PacketDb(){}
    
    
    void PacketDb::clearAll() {
        pp->deleteTable ("packets");
    }

}