# Proteus

CLI application for Protei IT-School

# Quick Start

## Build

```sh
mkdir build && cd build
cmake .. && make
```

## Run Client

```sh
./proteus -a 127.0.0.1 -p 8080
```

or 

```sh
./proteus -a 127.0.0.1:8080
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

# TODO:
- [x] Logging
- [x] CppCheck
- [x] Address Sanitizer
- [x] ResourceTest
- [x] ConnectionTest
- [ ] Class invariants
	- [x] Descriptions
	- [ ] Checkers
- [x] Templates?
- [ ] Better test coverage
- [x] Better Address and cli args parsing
- [ ] HEX Address support
- [x] Server-side data processing
- [x] Logger for Server
- [ ] Benchmarking