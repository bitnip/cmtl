#include "cutil/src/string.h"
#include "wavefront_material_parser.h"

int wavefrontMaterialParseNewMaterial(struct WavefrontMTL* mtl, const char* line) {
    line = strAfterWhitespace(line);
    const char *thisToken = line, *nextDelim = NULL, *nextToken = NULL;
    tokenize(&thisToken, &nextDelim, &nextToken, ASCII_H_DELIMITERS);
    if (strStartsWith(thisToken, "newmtl") != nextDelim) return 0;

    if (tokenize(&thisToken, &nextDelim, &nextToken, ASCII_H_DELIMITERS)) {
        line = thisToken;
        if(tokenize(&line, &nextDelim, &nextToken, ASCII_H_DELIMITERS)) return 0;


        char* line = strCopyN(thisToken, nextDelim-thisToken);
        if(line == NULL) return 0;
        wavefrontMTLAddMaterial(mtl, line);
        free(line);
        return 1;
    }
    return 0;
}

static int parseColor(struct WavefrontColor* color, const char* line, const char* name) {
    line = strAfterWhitespace(line);
    const char *thisToken = line, *nextDelim = NULL, *nextToken = NULL;
    tokenize(&thisToken, &nextDelim, &nextToken, ASCII_H_DELIMITERS);
    if (strStartsWith(thisToken, name) != nextDelim) return 0; // Garbage after map_kd.

    // Parse Red.
    tokenize(&thisToken, &nextDelim, &nextToken, ASCII_H_DELIMITERS);
    if(*thisToken==*nextDelim) return 0; // Red must be specified.
    char* end = NULL;
    color->r = strtof(thisToken, &end);
    if(end != nextDelim)
        return 0;

    color->a = 1.0;

    // Parse Green.
    if (!tokenize(&thisToken, &nextDelim, &nextToken, ASCII_H_DELIMITERS) ||
        thisToken == nextDelim)
    {
        // Green and blue default to red if omitted.
        color->g = color->b = color->r;
        return 1;
    }
    color->g = strtof(thisToken, &end);
    if(end != nextDelim)
        return 0;

    // Parse Blue.
    if (!tokenize(&thisToken, &nextDelim, &nextToken, ASCII_H_DELIMITERS) ||
        thisToken == nextDelim)
    {
        // Blue defaults to red if omitted.
        color->b = color->r;
        return 1;
    }
    color->b = strtof(thisToken, &end);
    if(end != nextDelim)
        return 0;

    return !tokenize(&thisToken, &nextDelim, &nextToken, ASCII_H_DELIMITERS);
}

int wavefrontMaterialParseAmbientRGB(struct WavefrontColor* color, const char* line) {
    return parseColor(color, line, "Ka");
}

int wavefrontMaterialParseDiffuseRGB(struct WavefrontColor* color, const char* line) {
    return parseColor(color, line, "Kd");
}

int wavefrontMaterialParseSpecularRGB(struct WavefrontColor* color, const char* line) {
    return parseColor(color, line, "Ks");
}

int wavefrontMaterialParseTransmissionRGB(struct WavefrontColor* color, const char* line) {
    return parseColor(color, line, "Tf");
}

int wavefrontMaterialParseComment(const char* line) {
    return *line == '#';
}

int wavefrontMaterialParseIllumination(int* illuminationModel, const char* input) {
    return sscanf(input, "illum %d", illuminationModel) == 1;
}

int wavefrontMaterialParseSpecularExponent(float* specularExponent, const char* input) {
    return sscanf(input, "Ns %f", specularExponent) == 1;
}

int wavefrontMaterialParseDissolve(float* dissolve, const char* input) {
    return sscanf(input, "d %f", dissolve) == 1;
}

int wavefrontMaterialParseOpticalDensity(float* opticalDensity, const char* input) {
    return sscanf(input, "Ni %f", opticalDensity) == 1;
}

int wavefrontMaterialParseDiffuseMap(char** file, const char* line) {
    line = strAfterWhitespace(line);
    const char* thisToken = line,* nextDelim = NULL,* nextToken = NULL;
    tokenize(&thisToken, &nextDelim, &nextToken, ASCII_H_DELIMITERS);
    if (strStartsWith(thisToken, "map_Kd") != nextDelim) return 0; // Garbage after map_kd.
    *file = strCopy(nextToken);
    return 1;
}

int wavefrontMaterialParseNormalMap(char** file, const char* line) {
    line = strAfterWhitespace(line);
    const char* thisToken = line,* nextDelim = NULL,* nextToken = NULL;
    tokenize(&thisToken, &nextDelim, &nextToken, ASCII_H_DELIMITERS);
    if (strStartsWith(thisToken, "map_Kn") != nextDelim) return 0; // Garbage after map_kd.
    *file = strCopy(nextToken);
    return 1;
}

int wavefrontMTLParseLine(struct WavefrontMTL* mtl, const char* line) {
    line = strAfterWhitespace(line);

    struct WavefrontMaterial* m = NULL;
    if (mtl->materialCount) {
        m = mtl->materials + (mtl->materialCount-1);
    }
    return
        !line ||
        wavefrontMaterialParseComment(line) ||
        wavefrontMaterialParseNewMaterial(mtl, line) ||
        (m && (
            wavefrontMaterialParseAmbientRGB(&m->ambient, line) ||
            wavefrontMaterialParseDiffuseRGB(&m->diffuse, line) ||
            wavefrontMaterialParseSpecularRGB(&m->specular, line) ||
            wavefrontMaterialParseTransmissionRGB(&m->transmission, line) ||
            wavefrontMaterialParseIllumination(&m->illuminationModel, line) ||
            wavefrontMaterialParseSpecularExponent(&m->specularExponent, line) ||
            wavefrontMaterialParseDissolve(&m->dissolve, line) ||
            wavefrontMaterialParseOpticalDensity(&m->opticalDensity, line) ||
            wavefrontMaterialParseDiffuseMap(&m->diffuseMap.file, line) ||
            wavefrontMaterialParseNormalMap(&m->normalMap.file, line)
        ));
}

int parseWavefrontMTLFromString(struct WavefrontMTL* mtl, const char* input) {
    if(!input) return 0;
    const char *thisToken = input, *nextDelim = NULL, *nextToken = NULL;
    while (tokenize(&thisToken, &nextDelim, &nextToken, ASCII_V_DELIMITERS)) {
        char* line = strCopyN(thisToken, nextDelim-thisToken);
        if(line == NULL) return 0;

        wavefrontMTLParseLine(mtl, line);

        free(line);
    }
    // TODO: return a meaningful result.
    return 1;
}
