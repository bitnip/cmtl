#ifndef __WAVEFRONT_MATERIAL_H
#define __WAVEFRONT_MATERIAL_H
#ifdef __cplusplus
extern "C"{
#endif

struct WavefrontColor {
    float r, g, b, a;
};

struct WavefrontMap {
    char *file;
    char *options;
};

struct WavefrontMaterial {
    char *name;
    struct WavefrontColor ambient;
    struct WavefrontColor diffuse;
    struct WavefrontColor specular;
    struct WavefrontColor transmission;
    float specularExponent;
    float dissolve;
    float opticalDensity;
    int illuminationModel;
    struct WavefrontMap ambientMap;
    struct WavefrontMap diffuseMap;
    struct WavefrontMap normalMap;
    struct WavefrontMap specularColorMap;
    struct WavefrontMap specularHighlightMap;
    struct WavefrontMap alphaMap;
    struct WavefrontMap bumpMap;
    struct WavefrontMap displacementMap;
    struct WavefrontMap decalMap;
    struct WavefrontMap reflectionMapSphere;
    struct WavefrontMap reflectionMapCubeTop;
    struct WavefrontMap reflectionMapCubeBottom;
    struct WavefrontMap reflectionMapCubeFront;
    struct WavefrontMap reflectionMapCubeBack;
    struct WavefrontMap reflectionMapCubeLeft;
    struct WavefrontMap reflectionMapCubeRight;
};

struct WavefrontMTL {
    struct WavefrontMaterial *materials;
    unsigned int materialCount;
};

void wavefrontMTLRelease(struct WavefrontMTL *mtl);
int wavefrontMTLAddMaterial(struct WavefrontMTL *mtl, char *name);

#ifdef __cplusplus
}
#endif
#endif
