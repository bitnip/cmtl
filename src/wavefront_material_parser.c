#include <stdlib.h>
#include "cutil/src/error.h"
#include "cutil/src/string.h"
#include "wavefront_material_parser.h"

static int parseNewMaterial(void* output, const char* line) {
    struct WavefrontMTL* mtl = output;
    if(!line) return STATUS_PARSE_ERR;

    const char* thisToken = line,* nextDelim = NULL,* nextToken = NULL;
    if (tokenize(&thisToken, &nextDelim, &nextToken, ASCII_H_DELIMITERS) &&
        nextToken == NULL
    ) {
        char* temp = strCopyN(thisToken, nextDelim-thisToken);
        if(temp == NULL) return STATUS_ALLOC_ERR;

        int result = wavefrontMTLAddMaterial(mtl, temp);
        if(result) {
            free(temp);
            return result;
        }
    }
    return STATUS_OK;
}

static int parseColor(void* output, const char* input) {
    struct WavefrontColor* color = output;

    const char *thisToken = input, *nextDelim = NULL, *nextToken = NULL;
    // Parse Red.
    tokenize(&thisToken, &nextDelim, &nextToken, ASCII_H_DELIMITERS);
    if(*thisToken==*nextDelim) return STATUS_PARSE_ERR; // Red must be specified.
    char* end = NULL;
    color->r = strtof(thisToken, &end);
    if(end != nextDelim)
        return STATUS_PARSE_ERR;

    color->a = 1.0;

    // Parse Green.
    if (!tokenize(&thisToken, &nextDelim, &nextToken, ASCII_H_DELIMITERS) ||
        thisToken == nextDelim) {
        // Green and blue default to red if omitted.
        color->g = color->b = color->r;
        return STATUS_OK;
    }
    color->g = strtof(thisToken, &end);
    if(end != nextDelim)
        return STATUS_PARSE_ERR;

    // Parse Blue.
    if (!tokenize(&thisToken, &nextDelim, &nextToken, ASCII_H_DELIMITERS) ||
        thisToken == nextDelim) {
        // Blue defaults to red if omitted.
        color->b = color->r;
        return STATUS_OK;
    }
    color->b = strtof(thisToken, &end);
    if(end != nextDelim)
        return STATUS_PARSE_ERR;

    return nextToken == NULL ? STATUS_OK : STATUS_PARSE_ERR;
}

static int parseInteger(void* output, const char* input) {
    *((int*)output) = atoi(input);
    return STATUS_OK;
}

static int parseFloat(void* output, const char* input) {
    *((float*)output) = atof(input);
    return STATUS_OK;
}

static int parseMap(void* output, const char* input) {
    struct WavefrontMap* map = output;

    const char *thisToken = input,* nextDelim = NULL, *nextToken = NULL;
    while (tokenize(&thisToken, &nextDelim, &nextToken, ASCII_H_DELIMITERS)) {
        if(strStartsWith(thisToken, "-")) {
            tokenize(&thisToken, &nextDelim, &nextToken, ASCII_H_DELIMITERS);
        } else {
            break;
        }
    }

    map->file = strCopy(thisToken);
    return STATUS_OK;
}

int parseWavefrontMTLFromString(struct WavefrontMTL* mtl, const char* input) {
    if(!input) return STATUS_INPUT_ERR;
    mtl->materials = NULL;
    mtl->materialCount = 0;

    const char *thisToken = input,* nextDelim = NULL, *nextToken = NULL;
    while (tokenize(&thisToken, &nextDelim, &nextToken, ASCII_V_DELIMITERS)) {
        char* line = strCopyN(thisToken, nextDelim-thisToken);
        if(line == NULL) return STATUS_ALLOC_ERR;

        const char* temp = strAfterWhitespace(line);

        struct WavefrontMaterial* m = NULL;
        if (mtl->materialCount) {
            m = mtl->materials + (mtl->materialCount-1);
        }

        const char* thisToken = temp,* nextDelim = NULL,* nextToken = NULL;
        tokenize(&thisToken, &nextDelim, &nextToken, ASCII_H_DELIMITERS);

        struct Parser {
            char name[22];
            int (*fn)(void* color, const char* input);
            void* result;
        };
        struct Parser parsers[] = {
            {"newmtl", parseNewMaterial, (void*)mtl},
            {"Ka", parseColor, (void*)&m->ambient},
            {"Kd", parseColor, (void*)&m->diffuse},
            {"Ks", parseColor, (void*)&m->specular},
            {"Tf", parseColor, (void*)&m->transmission},
            {"illum", parseInteger, (void*)&m->illuminationModel},
            {"Ns", parseFloat, (void*)&m->specularExponent},
            {"Ni", parseFloat, (void*)&m->opticalDensity},
            {"d", parseFloat, (void*)&m->dissolve},
            {"map_Kd", parseMap, (void*)&m->diffuseMap},
            {"map_Kn", parseMap, (void*)&m->normalMap},
            {"refl -type sphere", parseMap, (void*)&m->reflectionMapSphere},
            {"refl -type cube_top", parseMap, (void*)&m->reflectionMapCubeTop},
            {"refl -type cube_bottom", parseMap, (void*)&m->reflectionMapCubeBottom},
            {"refl -type cube_front", parseMap, (void*)&m->reflectionMapCubeFront},
            {"refl -type cube_back", parseMap, (void*)&m->reflectionMapCubeBack},
            {"refl -type cube_left", parseMap, (void*)&m->reflectionMapCubeLeft},
            {"refl -type cube_right", parseMap, (void*)&m->reflectionMapCubeRight}
        };
        if(!nextToken) continue;
        int result = STATUS_OK;
        for(int i = 0; i < sizeof(parsers)/sizeof(struct Parser); i++) {
            if((strStartsWith(thisToken, parsers[i].name) >= nextDelim)) {
                const char* temp = nextToken ? strAfterWhitespace(nextToken) : NULL;
                result = parsers[i].fn ? parsers[i].fn(parsers[i].result, temp) : STATUS_OK;
                break;
            }
        }
        free(line);
        if(result) {
            wavefrontMTLRelease(mtl);
            return result;
        }
    }

    return STATUS_OK;
}
