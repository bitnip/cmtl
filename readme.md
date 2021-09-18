# CMTL
C library for interacting with Wavefront MTL files.

## Prerequisites
Build
```
    gcc : https://gcc.gnu.org/install/
  cutil : https://github.com/bitnip/cutil
```
Coverage Report
```
  gcovr : https://github.com/gcovr/gcovr
```
Documentation
```
 pandoc : https://pandoc.org/installing.html
```
## Usage
Build
: `make build`

Test
: `make test`

Coverage Report
: `make coverage`

Documentation
: `make docs`

## Known Issues
- Alloc failures are not handled.
- Unit tests do not cover alloc failures.
- Only basic material properties are supported.
- Only importing is supported.