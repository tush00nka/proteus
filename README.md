# Proteus

SMS-Center model for Protei Ordinate Task

# Quick Start

## Build

```sh
mkdir build && cd build
cmake .. && make
```

## Run Client

```sh
./proteus -a 127.0.0.1 -p 8080 --msisdn 89991112233 --imsi 12345601112233 --imei 158863118273320 -x 100
```


## Run Server
```sh
./proteus_server
```

# Run Tests
```sh
./proteus_tests
```
or
```sh
ctest ./proteus_tests
```

# Crucial Parameters
- -a ADDRESS
- -p PORT
- --msisdn MSISDN
- --imsi IMSI
- --imei IMEI

# TODO:
- [x] New CLI arguments parsing for UE
- [x] Move command