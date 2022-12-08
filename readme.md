# CMTL

C library for interacting with Wavefront MTL files.

## Prerequisites
```
cutil: https://github.com/bitnip/cutil
```
### Windows
```
cygwin: https://www.cygwin.com/install.html
cygwin packages:
  gcc-core
  gcovr
  gdb
  make
  mingw64-x86_64-gcc-core
  mingw64-x86_64-gcc-g++
  python3
```
---

## Usage

### Build

`> make build`

### Test
`> make test`

### Coverage Report
`> make coverage`

### Documentation
`> make docs`

## Known Issues
- Alloc failures are not handled.
- Unit tests do not cover alloc failures.
- Only basic material properties are supported.
- Only importing is supported.
