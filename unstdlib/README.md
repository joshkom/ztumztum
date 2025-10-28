# unstdlib

> An unstandard C library.

Just another libc wrapper, reimplementing the wheel? Yes.

**unstdlib** is a basic C (un)standard libc wrapper designed to enhance productivity in C
programming. By providing a robust set of abstractions and utilities, unstdlib aims to simplify common tasks and reduce
boilerplate code, allowing developers to focus on writing efficient and maintainable code.

## Table of Contents

- [Getting started](#getting-started)
- [Updates](#updates)
- [Build](#build)
- [Installation](#install)
- [Tests](#tests)
- [Documentation](#documentation)

<br>

## Getting started

Currently in semi-active development, Stay tuned for upcoming releases :)

## Updates

#### Check [`devel`](https://github.com/X/unstdlib/tree/devel) for recent commits.

## Build

To build unstdlib, follow these steps:

1. **Ensure you have [CMake](https://cmake.org/) and [GCC](https://gcc.gnu.org/) installed.**
2. **Open your terminal and navigate to the project directory.**
3. **Run the following command:**
   ```shell
   chmod +x ./unstdlib.sh && ./unstdlib.sh -build
   ```

This only builds the project as Release and moves libraries **(&ast;.so&ast; | &ast;.a)** and headers **(&ast;.h)** to
the `build` directory per default.

## Install

```shell
./unstdlib.sh -install
```

Builds the project as Release and moves libraries **(&ast;.so&ast; | &ast;.a)** and headers **(&ast;.h)** to
`/usr/local/lib` and `/usr/local/include/unstdlib` directories respectively, per default.

## Documentation

Documentation is provided for each function specifically in the header files.
<br>
Each function is documented with comments explaining its purpose, parameters, and return value.

For detailed information on each function, please refer to the corresponding header file.


---

## Tests

**unstdlib** has been tested on Arch Linux kernel `6.7.6-zen1-1-zen`. You can run the unit tests
manually by executing the following command:

```shell
./unstdlib.sh -test
```

## More

For more information on available options, execute:

```shell
./unstdlib.sh -h
```

## Uninstall

Changed your mind? Simply run:

```shell
./unstdlib.sh -uninstall
```

## Compatibility

**unstdlib** is based on GCC version 11. Ensure that you have [GCC GNU 11](https://gcc.gnu.org/) installed on your
system to avoid any compatibility
issues.

## Contributing

I would even appreciate a cat's help.

---
