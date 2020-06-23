# Readme

This is a path tracer that I developed in C++ for my Bachelor's thesis. It implements the following features:
- Unbiased Monte Carlo integration of the Rendering Equation via Path tracing
- Global illumination
- Anti-aliasing
- Defocus blur with customizable aperture and focus distance
- Importance sampling via BRDF sampling
- Next event estimation with shadow rays
- Smooth and flat shading
- Support for Diffuse, Crystal, Metal, Blinn-Phong and Emissive materials with parameterizable roughness, metallicity, reflectance and intensity
- Support for Fresnel reflections in diffuse materials
- PBR materials with support for texture maps, normal maps, roughness/glosiness maps, metallicity maps and ambient occlusion maps
- Point lights, area lights, and directional lights
- Image-based Lightning with sphere mapping
- Support for importing multiple asset formats such as OBJ, FBX and PLY via the [Assimp](http://assimp.org/) library.
- Support for saving completed renders in various formats (JPEG, PNG, TIFF, BMP, XBMP, WebP, PPM, PBM, XBM and XPM)
- Editable scene via selecting objects and changing their position and orientation
- Quaternions for representing orientations and rotations
- Blender-like orbital camera
- Unit-testing with the Google Test framework

Although it runs on the CPU, I am currently learning DirectX to add a DXR backend.

There are still some features I'm working on, such as a scene hierarchy view and the posibility to add spheres and lights to the scene (right now they are hardcoded).

## Images

These are some example renders I made with this software.

![cornell_box](/renders/cornell_box.png)

Typical Cornell Box with one area light

![pbr_materials](/renders/pbr_materials.png)

Some PBR materials. From left to right: leather, scratched metal and marble.

![panoramic_metal](/renders/panoramic_metal.png)

Environment map with a glass of crystal and a metal ball

![panoramic_blurry_metal](/renders/panoramic_blurry_metal.png)

Same as above but with camera aperture

![panoramic_blurry_glass](/renders/panoramic_blurry_glass.png)

Same as above but with a crystal ball

![metal_normal_map](/renders/metal_normal_map.png)

Metal ball with PBR material (texture map, normal map and metallicity map)

![metal_normal_map_rough](/renders/metal_normal_map_rough.png)

Same as above but with a roughness map as well

![gallery](/renders/gallery.PNG)

An art gallery

![beretta_specular_exagerado](/renders/beretta_specular_exagerado.PNG)

A pistol with Blinn-Phong shading, texture map and normal map

![blue_and_orange](/renders/blue_and_orange.PNG)

Monkey head with two colored point lights on the sides.

![emissive_material](/renders/emissive_material.png)

A table with various objects illuminated by a slightly colored ball with an emissive material.
The monkey is smooth shaded, unlike the previous image

## Libraries

All needed libraries are included in the repository. This project uses the following libraries:
- [Qt 5](https://www.qt.io/): Used for the GUI.
- [Assimp](http://assimp.org/): Used for importing mesh data.
- [Google Test](https://github.com/google/googletest): Used for unit-testing. Unit-tests are in a separate Qt Creator project in the /tests folder.
- [stb](https://github.com/nothings/stb): Specifically, stb_image and stb_image_write. Great little libraries for reading and writing in various image formats. 

## Build and Usage

This software was built with Qt 5 (any minor version should work) with MinGW, although it should also compile perfectly with GCC or Clang. It includes a Qt Creator project file that should build without anything else needed. The dependencies are included in the repository.
