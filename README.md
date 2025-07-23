# GFXBlit

GFXBlit is a lightweight, GPU-accelerated user-space library designed for efficient image and video frame processing using OpenGL ES shaders. It provides a unified and modular interface for high-performance operations like rotation, scaling, and color space conversion — all in real-time.

Ideal for video pipelines, compositors, embedded graphics systems, and any performance-critical rendering task.

## Key Features

- Unified `blitter()` API for chaining operations like rotation, scaling, and CSC
- Combinable Operations: e.g., Rotation + Scaling + CSC
- Built-in benchmarking and timing tools
- Predefined modular GLSL shader sets
- Lightweight C/C++ interface
- Supports OpenGL ES 2.0+ (optimized for embedded/mobile GPUs)

## Supported Operations

- Rotation: 0°, 90°, 180°, 270°
- Scaling: Upscale/Downscale with aspect-ratio preservation
- Color Space Conversion (CSC): e.g., RGB ↔ NV12
- Channel Packing/Unpacking: e.g., RGB ↔ RGBA, RGB ↔ ARGB

All operations are executed using programmable shaders for maximum performance and minimal CPU overhead.

## Format Support Matrix

| Input Format | Output Format | Supported Features                      |
|--------------|----------------|------------------------------------------|
| RGB          | RGBA           | Rotation, Scaling, CSC                  |
| ARGB         | RGB            | Rotation, Scaling                       |
| RGB          | NV12 (Semi)    | Scaling, CSC                            |
| NV12         | RGB            | Rotation, Scaling, CSC                  |
| ...          | ...            | Extensible combinations supported
