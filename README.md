This repo is forked from [base repo](https://github.com/chinuno-usami/Consulpp) to suit for our own project.

------

# Consulpp
Consul client for C++.
Based on Consul HTTP API.
Implemented
* Register service
* Deregister service
* Set checks for service
* Add/Modify KV store
* Detele KV store
* Get KV store

    **May not add new api interface if i am not using in my projects.**
    **PR welcome for new features.**

# Requirements
[C++ REST SDK](https://github.com/Microsoft/cpprestsdk)

# Build
Require compiler supports C++ 11.
```bash
mkdir build
cd build
cmake -DBUILD_TEST=OFF -DCPPREST_DIR=/path/to/cpprestsdk ../
make
```
# Usage
Refer to test.cpp
