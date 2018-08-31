# Introduction

This is a (hopefully) improved version of the multi-protocol XLX-Reflector. Nearly all std::vector containers have been replaced with std::list containers. I beleive this is a far better choice for any collection where it is common to delete elements that are not at the end of the collection. In most cases, I beleive it makes the code handling the containers simpler as well. In this version, no classes are derrived from any standard containers. I consider this highly ill-advised and while the origin XLX server worked using such derivations, it represents a possible problem when considering future development.

The Makefile has been improved to provide automatically generated dependances. This significantly speeds up updating or improving the code

The xlxd no longer has a daemon-mode. It is unnecessary for systemd. I only support systemd-based operating systems. Debian jessie or Ubuntu 18 is recommended. If you want to install this on a non-systemd based OS, you are on your own. Also, by default, ambed and xlxd are built without gdb support. If you want to add it, copy the `Makefile` in each build directory to `makefile` and modify that file. Finally, I have designed this repository so that you don't have to modify any file in the repository. Follow the instructions below to build your transcoding XLX reflector.

- 73 de n7tae

# Usage

The packages which are described in this document are designed to install server software which is used for the D-Star network infrastructure. It requires a 24/7 internet connection which can support 20 voice streams or more to connect repeaters and hotspot dongles!

- The server requires a static IP-address!
- The public IP address should have a DNS record which must be published in the common host files.

If you want to run this software please make sure that you can provide this service free of charge, like the developer team provides the software and they network infrastructure free of charge! If you don't need transcoding support, you don't need to build or install `ambed`.

# Installation

#### After a clean installation of debian make sure to run update and upgrade
```
sudo apt update
sudo apt upgrade
```
#### Required packages (some of these will probabaly already be installed)
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
Use your favorite editory to modify `main.h`. **You will need to change the parameters at the begining of this file!** REFLECTOR_CALLSIGN must be set as well as the MY_IP_ADDRESS. By default, the DMR ID file is downloaded from XLX950 every three hours. If you want to provide your own source, build a cron-based script that will download a suitable file. You also need to modify the `DMRIDDB_USE_RLX_SERVER` and `DMRIDDB_PATH` variables.

You will also want to set NB_OF_MODULES to the number of modules you need. The max is 26. If you want to use module Z you need to set `NB_OF_MODULES` to 26.

#### Create and edit your blacklist, whitelist and linking files
```
cp ../config/xlx.blacklist .
cp ../config/xlx.whitelist .
cp ../config/xlx.interlink .
```
Use your favorite editor to modify each of these files. If you want a totally open network, the blacklist and whitelist files are ready to go. The blacklist determine which callsigns can't use the reflector. The whitelist determines which callsigns can use the reflector and the interlink files set up the XLX<--->XLX linking. When building your network, remember that XLX only supports a single hop, so each XLX reflector needs to be interlinked with all the reflectors for that module's network. Along with multi-protocol support, this is the outstanding feature of the XLX design!

#### Create and edit the ambed `main.h` file
```
cd ../ambed
cp main.example.h main.h
```
If you transcoding hardware is local, the default main.h works fine. If your transcoding hardware is not local, you need the specify the IP_ADDRESS of the hardware.

#### Compile, install and start the ambed and xlxd programs
```
make -j<N>
sudo make install
cd ../xlxd
make -j<N>
sudo make install
```
Replace the `<N>` with the number of processors on your system, which can be found with `cat /proc/cpuinfo`.

#### Stoping and starting the services
```
sudo systemctl stop xlxd
sudo systemctl stop ambed
```
You can start each component by replacing `stop` with `start`, or you can restart each by using `restart`.

#### Copy dashboard to /var/www
```
sudo cp -r ~/xlxd/dashboard /var/www/db
```

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

- © 2016 Luc Engelmann LX1IQ
- © 2016 Thomas A. Early, N7TAE


The XLX Multiprotocol Gateway Reflector Server is part of the software system
for the D-Star Network.
The sources are published under GPL Licenses.
