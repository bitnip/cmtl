#include "cutil/src/string.h"
#include "wavefront_material.h"

int wavefrontMTLAddMaterial(
        struct WavefrontMTL* mtl,
        char* name) {
    // Reuse existing material with same name.
    for (unsigned int i = 0; i < mtl->materialCount; i++) {
        if (strcmp(name, mtl->materials[i].name) == 0) {
            return i;
        }
    }
    // Create new material.
    mtl->materialCount++;
    struct WavefrontMaterial* temp = (struct WavefrontMaterial*) realloc(
        mtl->materials,
        mtl->materialCount * sizeof(struct WavefrontMaterial));
    // TODO: Handle realloc failure.
    mtl->materials = temp;
    // TODO: Move to init function.
    struct WavefrontMaterial* m = mtl->materials + (mtl->materialCount - 1);
    m->name = strCopy(name);
    m->ambientMap.file = NULL;
    m->diffuseMap.file = NULL;
    m->specularColorMap.file = NULL;
    m->specularHighlightMap.file = NULL;
    m->alphaMap.file = NULL;
    m->bumpMap.file = NULL;
    m->displacementMap.file = NULL;
    m->decalMap.file = NULL;
    return mtl->materialCount - 1;
}

void wavefrontMTLFree(struct WavefrontMTL* mtl) {
    for(unsigned int i = 0; i < mtl->materialCount; i++) {
        struct WavefrontMaterial *m = mtl->materials + i;
        free(m->name);
        free(m->ambientMap.file);
        free(m->diffuseMap.file);
        free(m->specularColorMap.file);
        free(m->specularHighlightMap.file);
        free(m->alphaMap.file);
        free(m->bumpMap.file);
        free(m->displacementMap.file);
        free(m->decalMap.file);
    }
    free(mtl->materials);
}

