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
cmake ..
make
make install
```
# Usage
Refer to test.cpp
