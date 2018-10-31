# Introduction

This will build **either** an new kind of XLX reflector **or** a tri-mode XRF reflector. The XLX is new because it can support out-going DExtra links, by adding a new DExtra Peer type *and* it has many changes designed to increase reliability and stability. The XRF reflector built from this source supports inbound DExtra, DPlus **and DCS connections**. Like XLX, XRF also supports out-going DExtra linking. Please note that for now, only one DExtra peer link per node is supported.

This is an improved version of the multi-protocol Reflector. Nearly all std::vector containers have been replaced with std::list containers. This is a far better choice for any collection where it is common to delete elements that are not at the end of the collection. In most cases, Also in this package, no classes are derived from any standard containers. Because standard containers don't have a virtual desctructor, this is highly ill-advised and while the origin XLX server worked using such derivations, it represents a possible serious problem when considering future development. Also, the clean-up routines designed to be executed when shutting down were unreachable as designed and this has been fixed. Please note that it can take several seconds to complete the shutdown while some execution threads complete their i/o cycle.

The Makefiles have been improved to provide automatically generated dependencies. This significantly speeds up updating time and eliminates build errors. Be sure to do a `make clean` before you switch your build type between XLX and XRF.

These reflectors no longer has a daemon-mode. It is unnecessary for systemd. Only systemd-based operating systems are supported. Debian 9 or Ubuntu 18 is recommended. If you want to install this on a non-systemd based OS, you are on your own. Also, by default, ambed and xlxd or xrfd are built without gdb support. If you want to add it, modify the `Makefile` in each build directory. Finally, this repository is designed so that you don't have to modify any file in the repository when you build your system. Any file you need to modify to properly configure your reflector will be a file you copy from you locally cloned repo. This makes it easier to update the source code when this repository is updated. Follow the instructions below to build your transcoding XLX reflector or tri-mode XRF reflector.

- 73 de n7tae

# Usage

The packages which are described in this document are designed to install server software which is used for the D-Star network infrastructure. It requires a 24/7 internet connection which can support 20 voice streams or more to connect repeaters and hot-spot dongles!

- The server requires a static IP-address!
- The public IP address should have a DNS record which must be published in the common host files.

If you want to run this software please make sure that you can provide this service free of charge, like the developer team provides the software and they network infrastructure free of charge! If you don't need transcoding support, you don't need to build or install `ambed`.

# Installation

Below are instructions for building either an XLX or XRF reflector. It is assumed that the XLX system will support transcoding. If this is not the case, you can safely ignore commands associated with building and installing `ambed`.

#### After a clean installation of Debian make sure to run update and upgrade
```
sudo apt update
sudo apt upgrade
```
#### Required packages (some of these will probably already be installed)
```
sudo apt install git git-core
sudo apt install apache2 php5
sudo apt install build-essential
sudo apt install g++
```

#### Download the sources
```
# git clone https://github.com/n7tae/xlxd.git
```

#### Create and edit the xlxd `main.h` file
Go to the xlxd/src directory and
```
cd xlxd/src
cp main.example.h main.h
```
Use your favorite editor to modify `main.h`. **You will need to change the parameters at the begining of this file!** REFLECTOR_CALLSIGN must be set as well as the MY_IP_ADDRESS. If you are building an XLX reflector, by default, the DMR ID file is downloaded from XLX950 every three hours. If you want to provide your own source, build a cron-based script that will download a suitable file. You also need to modify the `DMRIDDB_USE_RLX_SERVER` and `DMRIDDB_PATH` variables.

You will also want to set NB_OF_MODULES to the number of modules you need. The max is 26. If you want to use module Z you need to set `NB_OF_MODULES` to 26.

#### Create and edit your blacklist, whitelist and linking files
If you are building an XLX reflector:
```
cp ../config/xlx.blacklist .
cp ../config/xlx.whitelist .
cp ../config/xlx.interlink .
```
If you are building an XRF reflector (please note the name changes, especially for the interlink file):
```
cp ../config/xlx.blacklist xrf.blacklist
cp ../config/xlx.whitelist xrf.whitelist
cp ../config/xlx.interlink xrf.linklist
```
Use your favorite editor to modify each of these files. If you want a totally open network, the blacklist and whitelist files are ready to go. The blacklist determine which callsigns can't use the reflector. The whitelist determines which callsigns can use the reflector and the interlink or linklist file sets up the XLX<--->XLX linking and/or out-going XRF linking. When building your network, remember that XLX only supports a single hop, so each XLX reflector needs to be interlinked with all the reflectors for that module's network. Along with multi-protocol support, this is the outstanding feature of the XLX design! The down-side is that a Brand Meister link is of the same Peer group as XLX, so if you want to set up a big XLX cluster that supports transcoding, you need a transcoder for all nodes!

This limitation was the main driving force to develop this new XLX and XRF system: Transcoder hardware can be attached to an isolated XLX reflector and then linked to a larger network via XRF reflectors.

#### Create and edit the ambed `main.h` file (if you need transcoding capability for an XLX reflector)
```
cd ../ambed
cp main.example.h main.h
```
If you transcoding hardware is local, the default main.h works fine. If your transcoding hardware is not local, you need the specify the IP_ADDRESS of the hardware.

#### Compile, install and start the ambed program
```
make -j<N>
sudo make install
```
#### Compile the reflector
```
cd ../src
```
#### Compile, install and start the reflector
If you are building an XLX refector:
```
cp Makefile.xlx Makefile
```
Or, if you are building an XRF reflector:
```
cp Makefile.xrf Makefile
```
If you need/want gdb debugging support, you can modify your `Makefile`. Next, create a main.h file:
```
cp main.example.h main.h
```
User you your favorite editor to make configuration changes. At the very least, you will need to specify the reflector callsign and its IP address. If you are building an XLX reflector with transcoding, and the transcoder is not local, you will need to specify the IP address of the transcoder. You will also want to set the number of modules. The maximum supported is 26. Now your read to build and install:
```
make -j<N>
sudo make install
```
Replace the `<N>` with the number of processors on your system, which can be found with `cat /proc/cpuinfo`.

#### Stoping and starting the services
```
sudo systemctl stop xlxd # (or xrfd)
sudo systemctl stop ambed
```
You can start each component by replacing `stop` with `start`, or you can restart each by using `restart`.

#### Copy dashboard to /var/www
There are two supplied, one for XRF systems and one for XLX systems.
```
sudo cp -r ~/xlxd/dashboard.xlx /var/www/db     # or dashboard.xrf
```
Please note that your www root directory might be some place else. There are one file that needs configuration. Edit the copied files, not the ones from the repository:

* **pgs/config.inc.php** - At a minimum set your email address, country and comment. **Do not** enable the calling home feature if you built an XRF reflector. This feature is for **XLX systems only**.

#### Give the dashboard read access to the server log file
```
sudo chmod +r /var/log/messages
```

#### Reboot server to see if the auto-start is working
```
sudo shutdown -r now
```

# Updating xlxd and ambed
Go to the build directory and do a `git pull`. If you notice there are new versions of either of the `main.example.h` files or new `Makefile`s after you do the `git pull`, you will want to reconcile those new files with your copies **before** you make and install the executables.

# Firewall settings

XLX Server requires the following ports to be open and forwarded properly for in- and outgoing network traffic:
 - TCP port 80            (http) optional TCP port 443 (https)
 - TCP port 8080          (RepNet) optional
 - UDP port 10001         (json interface XLX Core)
 - UDP port 10002         (XLX interlink)
 - TCP port 22            (ssh) optional  TCP port 10022
 - UDP port 30001         (DExtra protocol)
 - UPD port 20001         (DPlus protocol)
 - UDP port 30051         (DCS protocol)
 - UDP port 8880          (DMR+ DMO mode)
 - UDP port 62030         (MMDVM protocol)
 - UDP port 10100         (AMBE controller port)
 - UDP port 10101 - 10199 (AMBE transcoding port)

# Copyright

- � 2016 Luc Engelmann LX1IQ
- � 2016 Thomas A. Early, N7TAE


The XLX Multiprotocol Gateway Reflector Server is part of the software system
for the D-Star Network.
The sources are published under GPL Licenses.
