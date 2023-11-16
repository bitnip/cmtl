#include <stdlib.h>
#include "cutil/src/error.h"
#include "cutil/src/string.h"
#include "wavefront_material.h"

int wavefrontMtlCompose(struct WavefrontMaterial* mtl) {
    memset(mtl, 0, sizeof(struct WavefrontMaterial));
    // NOTE: Does defaulting alpha to 1 seem more sane? 
    return STATUS_OK;
}

int wavefrontMTLAddMaterial(struct WavefrontMTL* mtl, char* name) {
    // Reuse existing material with same name.
    for (unsigned int i = 0; i < mtl->materialCount; i++) {
        if (strcmp(name, mtl->materials[i].name) == 0) {
            return i;
        }
    }
    // Create new material.
    struct WavefrontMaterial* temp = (struct WavefrontMaterial*)realloc(
        mtl->materials,
        (mtl->materialCount+1) * sizeof(struct WavefrontMaterial));
    int result = STATUS_ALLOC_ERR;
    if(temp) {
        mtl->materials = temp;
        struct WavefrontMaterial* m = mtl->materialCount++ + mtl->materials;
        result = wavefrontMtlCompose(m);
        m->name = name;
    }
    return result;
}

void wavefrontMTLRelease(struct WavefrontMTL* mtl) {
    for(unsigned int i = 0; i < mtl->materialCount; i++) {
        struct WavefrontMaterial* m = mtl->materials + i;
        free(m->name);
        free(m->ambientMap.file);
        free(m->diffuseMap.file);
        free(m->normalMap.file);
        free(m->specularColorMap.file);
        free(m->specularHighlightMap.file);
        free(m->alphaMap.file);
        free(m->bumpMap.file);
        free(m->displacementMap.file);
        free(m->decalMap.file);

        free(m->reflectionMapSphere.file);
        free(m->reflectionMapCubeTop.file);
        free(m->reflectionMapCubeBottom.file);
        free(m->reflectionMapCubeFront.file);
        free(m->reflectionMapCubeBack.file);
        free(m->reflectionMapCubeLeft.file);
        free(m->reflectionMapCubeRight.file);
    }
    free(mtl->materials);
}

