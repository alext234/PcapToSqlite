[![Build Status](https://travis-ci.org/alext234/PcapToSqlite.svg?branch=master)](https://travis-ci.org/alext234/PcapToSqlite)

# pcap2sqlite
A utility to capture network traffic and store into sqlite database.


## Build

Clone the code and update submodules:
```
git clone https://github.com/alext234/PcapToSqlite.git
cd PcapToSqlite
git submodule update --init --recursive
```

Build with cmake
```
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

```

Run unit tests 
```
./runtests

```
## Usage
Display the help page:
```
./pcap2sqlite -h

Capture packet from interface or pcap files and store into sqlite database
Usage:
  ./pcap2sqlite [OPTION...]

  -h, --help          display this help
  -i, --input arg     pcap file (.cap, .pcap), or network interface
  -o, --database arg  sqlite database file
```

## A sample run
The following screenshot shows the program in action. Here we use [SqliteBrowser](https://github.com/sqlitebrowser/sqlitebrowser) to view the database while capturing. 

![Screenshot](examples/screenshot1.png?raw=true)
