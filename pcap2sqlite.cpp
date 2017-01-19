#include <iostream>
#include <cxxopts.hpp>
#include <string>
#include <cpppcap.h>
#include "packetdb.h"

using namespace std;


/* 
* command line arguments: 
* -i input interface or .pcap file (optional, default is the first interface detected)
* -o db file (optional, default is "packets.db")
*/
int main (int argc, char* argv[]) 
{
    string input="";
    string dbfile="packets.db";
    
    cxxopts::Options options(argv[0], "Capture packet from interface or pcap files and store into sqlite database");
    options.add_options()
        ("h,help", "display this help");

    options.add_option ("","i", "input", "pcap file (.cap, .pcap), .lst storing list of pcap files, or network interface", cxxopts::value<string> (input),"",  false);
    options.add_option ("", "o", "database", "sqlite database file", cxxopts::value<string>(dbfile), "", false)  ;


    options.parse(argc, argv);  
    
    if (options.count("help"))
    {
      cout << options.help({""}) << endl;
      exit(0);
    } 
        
    cout << "input : " << (input==""?"default interface":input)<<endl;
    cout << "database : " << dbfile<<endl;
    
}
