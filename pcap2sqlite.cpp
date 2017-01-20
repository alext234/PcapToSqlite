#include <iostream>
#include <cxxopts.hpp>
#include <string>
#include <cpppcap.h>
#include "packetdb.h"
#include <csignal>


using namespace std;
shared_ptr<Pcap::Dev> dev; // global variable so it can be accessed from sigIntHandler

shared_ptr<Pcap::Dev> getDev(string input) {
    if (input=="") {
        
        // get default interface
        return  Pcap::lookUpDev();  
        
    } else {
        auto dotPos = input.find_last_of('.');
   
        if (dotPos ==  string::npos) { 
            // not a pcap file, sshould be an interface
            return Pcap::openLive(input);
        } else {
            
            // pcap file; open offile
            return Pcap::openOffline (input);
        }
    }
    return nullptr;
}

void sigIntHandler( int signum ) {
    cout <<endl;
    cout << "stopping..." <<endl;
    dev->breakLoop();
}

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

    options.add_option ("","i", "input", "pcap file (.cap, .pcap), or network interface", cxxopts::value<string> (input),"",  false);
    options.add_option ("", "o", "database", "sqlite database file", cxxopts::value<string>(dbfile), "", false)  ;


    options.parse(argc, argv);  
    
    if (options.count("help"))
    {
      cout << options.help({""}) << endl;
      exit(0);
    } 
        
    cout << "input : " << (input==""?"default interface":input)<<endl;
    cout << "database : " << dbfile<<endl;
    
    dev = getDev (input);
    
    if (dev==nullptr) {        
        exit(1);
    }
    packetdb::PacketDb db(dbfile);
    struct Stat {
        uint64_t packetCount=0;
    } stat;
    dev->registerObserver([&stat,&db](const Pcap::Packet& p){
        packetdb::Packet packet (p.data());
        
        db.insert (packet);        
        
        auto stat = dev -> getStats();
        cout << "\r";
        cout << "  received : "<< stat.recv;
        cout << "  dropped: "<< stat.drop;
        cout << "  ifdropped: "<< stat.ifdrop;
        cout <<flush        ;
        
    });
    
    signal(SIGINT, sigIntHandler);  

    dev->loop();
        
    cout <<endl;
    

}
