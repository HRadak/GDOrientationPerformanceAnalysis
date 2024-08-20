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

## Environment settings

Set up your development environment using following settings: 

* C++17
* configure libs: armadillo, pthread, serial
