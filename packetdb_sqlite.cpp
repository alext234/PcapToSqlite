#include "packetdb.h"
#include <sqlite_modern_cpp.h>
#include <vector>

namespace packetdb {
    using namespace std;
    
    
    struct PacketDb::PacketDbPrivate {
        
    };
    
    PacketDb::PacketDb() : pp(make_shared<PacketDbPrivate>())  { }
    PacketDb::~PacketDb(){}
    void PacketDb::clearAll() {
        
    }

}