# Ray-Tracing
Offline ray tracing render
## Sample

![render](./output_images/render.bmp)
Updated cube:
![cube9](./output_images/cube9.png)
## Run

Build:

```shell
g++ -o tracer main.cpp
```

Run:

For Windows:

```shell
./tracer.exe > tracer.ppm
```

Linux / Mac:

```shell
tracer.exe > tracer.ppm
```

For more shapes, add new class imitating `sphere.h`
