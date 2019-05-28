# consulpp

> This repo is forked from [base repo](https://github.com/chinuno-usami/Consulpp) to suit for our own project.
> I will commit PR to the base repo.

Head-only C++ consul client, based on Consul HTTP API.  
Just have single head file.

Implemented

- Register service
- Deregister service
- Set checks for service
- Add/Modify KV store
- Detele KV store
- Get KV store
- Health check

## Requirements

[C++ REST SDK](https://github.com/Microsoft/cpprestsdk)

## Build

Because we just have a single header file, so just need to build the test project.

Require compiler supports C++ 11.

```bash
mkdir build
cd build
cmake -DBUILD_TEST=ON -DCPPREST_DIR=/path/to/cpprestsdk -DCPPREST_LIB=/path/tp/cpprestsdk_lib ../
make
```

## Usage

Refer to test.cpp

## TODO

- Different check type (_Now just have `Script+Interval` type, will add `Script+Interval` `Script+Interval` `Script+Interval`_)
- Bug fix and more test

## Note

Some newer APIs may not support for now, if you are intreasting about this project, welcome to commit PR.
