# Building QBeat
## Ensure your system has cmake, gcc, minizip, and Qt5 development libraries
`sudo apt install gcc g++ cmake libQt5*-dev libminizip-dev git`

## Grab the source

```
git clone https://github.com/geefr/beatsaber-linux-goodies
cd beatsaber-linux-goodies/QBeat
```

## Configure and build QBeat. 
By default the install prefix will be /usr/local, so you might want to change it
If any dependencies are missing you should get a fairly obvious error


`cmake -DCMAKE_INSTALL_PREFIX=<installation prefix> .`

`make install`
