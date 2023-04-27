Parser for carl data structures
--------------------------------------

This is an [ANTLR](http://www.antlr.org)-based parser which is meant as an extension to [CArL](https://github.com/moves-rwth/carl-storm).

### Requirements

The parser requires the following tools:
- [CArL](https://github.com/moves-rwth/carl-storm)
- [Maven](https://maven.apache.org/)

### Building

Build carl-parser with

```bash
mkdir build
cd build
cmake ..
make
```

You can run tests with

```bash
make test
```

### Frequent errors

On macOS, there is a known problem with the rpath, which might lead to an error when executing the tests.
The problem can be alleviated by re-running both

```bash
cmake ..
make
```

### Authors:

- Harold Bruintjes
- Sebastian Junges
- Gereon Kremer
- Matthias Volk
