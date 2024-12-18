```sh
mkdir build
mkdir gen
# CMAKE_PREFIX_PATH if needed for custom gRPC installation
cmake -B build -S .
cmake --build build --parallel
```

1. Run server: `./build/bin/server`
2. Run client: `./build/bin/client`
3. Ctrl+C server to stop the server
4. `use-after-poison` should occur on client

If running client before server, another `use-after-poison` occurs immediately.

