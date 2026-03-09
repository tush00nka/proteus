# Proteus

CLI application for Protei IT-School

# Quick Start

```sh
mkdir build && cd build
cmake .. && make
./proteus
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
- [x] Templates?
- [ ] Better test coverage
- [x] Better Address and cli args parsing