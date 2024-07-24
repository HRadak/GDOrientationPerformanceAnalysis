# QGD-OE

## Install requirements

### Ubuntu 20.04

* Boost library for modern C++ addons 

```bash
    sudo apt-get install libboost-all-dev
```


* Armadillo mathematics library

```bash
    sudo apt-get install libopenblas-openmp-dev libarmadillo9 libarmadillo-dev
```

* Serial interface library (read serial data from a MCU)

```bash
    sudo apt-get install libserial-dev
```

* FTDI USB-SPI driver (copy the deb-Package *libftd2xx-1.4.22-amd64.deb* and the file *89-libftd2xx.rules*´ from the resources folder)

```bash
    sudo cp 89-libftd2xx.rules /etc/udev/rules.d/
    sudo apt-get install libftd2xx-1.4.22-amd64.deb
```

## Environment settings

Set up your development environment using following settings: 

* C++17
* configure libs: armadillo, pthread, serial
