# Aster

A Toy Render I've been doing in a free time.
[YouTube](https://youtu.be/NRtGSt5dI-s?si=XTr9W9--Zumt7_Jv)

Originally the whole idea of Aster was to practice shaders programming and study graphics.\
But writing isolated shaders wasn’t fun enough so I decided to make it at least somehow related\
to the real engines and to collect the knowledge across the gamedev.

It's not a production quality code and never will be :P

For more info read the [Wiki](https://github.com/Rewlion/Aster/wiki)\
Main papers that used for the rendering located in the [docs folder](https://github.com/Rewlion/Aster/blob/main/docs/render.txt)

Main features:
- TFX PreShader compiler (Flex,Bison for parsing, DXC for HLSL)
- Custom config format (Flex, Bison for parsing)
- Data Driven ECS (LibClang for CPP parsing and CodeGen)
- Data Driven Input
- Reactive UI framework with QuickJS (javascript vm)
- Framegraph (+ DSL)
- Renderer (GI on Surfels, Raytracing(BLAS,TLAS,BVH, Compute), SSSR, Vulkan Api, Atmosphere, Spherical
Harmonics, IBL for specular, PBR, TAA, Decals, Virtual Texture, Clustered Deferred, Order
Independent Transparency, Tracy Profiler)

