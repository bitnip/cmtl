#ifndef __WAVEFRONT_MATERIAL_PARSER_H
#define __WAVEFRONT_MATERIAL_PARSER_H
#ifdef __cplusplus
extern "C"{
#endif

#include "wavefront_material.h"

int parseWavefrontMTLFromString(struct WavefrontMTL *mtl, const char *input);

#ifdef __cplusplus
}
#endif
#endif
