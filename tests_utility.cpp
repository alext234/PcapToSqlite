#include "gmock/gmock.h"
#include "packetdb.h"
#include <cstdint>
#include <numeric>




using namespace testing;
using namespace std;
using namespace packetdb;

TEST(MacTypeAndStringConversion,MacTypeToString) {
    {
        MacType mac {0x11,0x22,0x33,0xAA,0xBB,0xCC};
    
        string macString = macTypeToString(mac);
        ASSERT_THAT (macString, Eq("11:22:33:AA:BB:CC"));
    }
    {
        MacType mac {0x01,0x02,0x03,0x0A,0x0B,0x0C};
    
        string macString = macTypeToString(mac);
        ASSERT_THAT (macString, Eq("01:02:03:0A:0B:0C"));        
    }
    
}


TEST(MacTypeAndStringConversion,StringToMacType) {
    {
        MacType mac  = stringToMacType("11:22:33:AA:BB:CC");
        ASSERT_THAT (mac  , Eq(MacType{0x11,0x22,0x33,0xAA,0xBB,0xCC}));
    }
    {
        try {
            stringToMacType("11:22:33:AA:BB:CC:EE:FF"); // an invalid one --> see exception
            ASSERT_THAT(1,Eq(0)); // should not reach here 
        } catch (const UtilityException& ex) {
        }
    }
    
    
    
}