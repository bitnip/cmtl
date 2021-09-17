#include <string.h>
#include "wavefront_material_parser.h"
#include "cutil/src/assertion.h"

int wavefrontMaterialParseNewMaterial(struct WavefrontMTL *mtls, char* line);
int wavefrontMaterialParseAmbientRGB(struct WavefrontColor* color, char* line);
int wavefrontMaterialParseDiffuseRGB(struct WavefrontColor* color, char* line);
int wavefrontMaterialParseSpecularRGB(struct WavefrontColor* color, char* line);
int wavefrontMaterialParseTransmissionRGB(struct WavefrontColor* color, char* line);
int wavefrontMaterialParseComment(char* line);
int wavefrontMaterialParseIllumination(int* illuminationModel, const char* line);
int wavefrontMaterialParseSpecularExponent(float* specularExponent, const char* line);

void testParseNewMaterial() {
    char input[] = "newmtl new_material";
    struct WavefrontMTL mtl;
    mtl.materials = NULL;
    mtl.materialCount = 0;
    int result = wavefrontMaterialParseNewMaterial(&mtl, input);
    assertTrue(result);
    assertIntegersEqual(mtl.materialCount, 1);
    assertStringsEqual(mtl.materials[0].name, "new_material");
    wavefrontMTLFree(&mtl);
}

void testParseNewMaterialNoName() {
    char input[] = "newmtl";
    struct WavefrontMTL mtl;
    mtl.materials = NULL;
    mtl.materialCount = 0;
    int result = wavefrontMaterialParseNewMaterial(&mtl, input);
    assertFalse(result);
    assertIntegersEqual(mtl.materialCount, 0);
    wavefrontMTLFree(&mtl);
}

void testParseNewMaterialGarbage() {
    char input[] = "newmtl new_material asdf";
    struct WavefrontMTL mtl;
    mtl.materials = NULL;
    mtl.materialCount = 0;
    int result = wavefrontMaterialParseNewMaterial(&mtl, input);
    assertFalse(result);
    assertIntegersEqual(mtl.materialCount, 0);
    wavefrontMTLFree(&mtl);
}

void testParseAmbientRGB() {
    struct WavefrontColor color;
    char input[] = "Ka 0.1 0.5 0.7";
    int result;
    result = wavefrontMaterialParseAmbientRGB(&color, input);
    assertTrue(result);
    assertFloatsEqual(color.r, 0.1);
    assertFloatsEqual(color.g, 0.5);
    assertFloatsEqual(color.b, 0.7);
    assertFloatsEqual(color.a, 1.0);
}

void testParseAmbientRGBOnlyR() {
    struct WavefrontColor color;
    char input[] = "Ka 0.1";
    int result;
    result = wavefrontMaterialParseAmbientRGB(&color, input);
    assertTrue(result);
    assertFloatsEqual(color.r, 0.1);
    assertFloatsEqual(color.g, 0.1);
    assertFloatsEqual(color.b, 0.1);
    assertFloatsEqual(color.a, 1.0);
}

void testParseAmbientRGBOnlyREmptyG() {
    struct WavefrontColor color;
    char input[] = "Ka 0.1 ";
    int result;
    result = wavefrontMaterialParseAmbientRGB(&color, input);
    assertTrue(result);
    assertFloatsEqual(color.r, 0.1);
    assertFloatsEqual(color.g, 0.1);
    assertFloatsEqual(color.b, 0.1);
    assertFloatsEqual(color.a, 1.0);
}

void testParseAmbientRGBOnlyRG() {
    struct WavefrontColor color;
    char input[] = "Ka 0.1 0.5";
    int result;
    result = wavefrontMaterialParseAmbientRGB(&color, input);
    assertTrue(result);
    assertFloatsEqual(color.r, 0.1);
    assertFloatsEqual(color.g, 0.5);
    assertFloatsEqual(color.b, 0.1);
    assertFloatsEqual(color.a, 1.0);
}

void testParseAmbientRGBOnlyRGEmptyB() {
    struct WavefrontColor color;
    char input[] = "Ka 0.1 0.5 ";
    int result;
    result = wavefrontMaterialParseAmbientRGB(&color, input);
    assertTrue(result);
    assertFloatsEqual(color.r, 0.1);
    assertFloatsEqual(color.g, 0.5);
    assertFloatsEqual(color.b, 0.1);
    assertFloatsEqual(color.a, 1.0);
}

void testParseAmbientRGBGarbage() {
    struct WavefrontColor color;
    char input[] = "Ka 0.1 0.5 0.7 asdf";
    int result;
    result = wavefrontMaterialParseAmbientRGB(&color, input);
    assertFalse(result);
}

void testParseDiffuseRGB() {
    struct WavefrontColor color;
    char input[] = "Kd 0.1 0.5 0.7";
    int result;
    result = wavefrontMaterialParseDiffuseRGB(&color, input);
    assertTrue(result);
    assertFloatsEqual(color.r, 0.1);
    assertFloatsEqual(color.g, 0.5);
    assertFloatsEqual(color.b, 0.7);
    assertFloatsEqual(color.a, 1.0);
}

void testParseDiffuseRGBOnlyR() {
    struct WavefrontColor color;
    char input[] = "Kd 0.1";
    int result;
    result = wavefrontMaterialParseDiffuseRGB(&color, input);
    assertTrue(result);
    assertFloatsEqual(color.r, 0.1);
    assertFloatsEqual(color.g, 0.1);
    assertFloatsEqual(color.b, 0.1);
    assertFloatsEqual(color.a, 1.0);
}

void testParseDiffuseRGBOnlyRG() {
    struct WavefrontColor color;
    char input[] = "Kd 0.1 0.5";
    int result;
    result = wavefrontMaterialParseDiffuseRGB(&color, input);
    assertTrue(result);
    assertFloatsEqual(color.r, 0.1);
    assertFloatsEqual(color.g, 0.5);
    assertFloatsEqual(color.b, 0.1);
    assertFloatsEqual(color.a, 1.0);
}

void testParseDiffuseRGBGarbage() {
    struct WavefrontColor color;
    char input[] = "Kd 0.1 0.5 0.7 asdf";
    int result;
    result = wavefrontMaterialParseDiffuseRGB(&color, input);
    assertFalse(result);
}

void testParseSpecularRGB() {
    struct WavefrontColor color;
    char input[] = "Ks 0.1 0.5 0.7";
    int result;
    result = wavefrontMaterialParseSpecularRGB(&color, input);
    assertTrue(result);
    assertFloatsEqual(color.r, 0.1);
    assertFloatsEqual(color.g, 0.5);
    assertFloatsEqual(color.b, 0.7);
    assertFloatsEqual(color.a, 1.0);
}

void testParseSpecularRGBOnlyR() {
    struct WavefrontColor color;
    char input[] = "Ks 0.1";
    int result;
    result = wavefrontMaterialParseSpecularRGB(&color, input);
    assertTrue(result);
    assertFloatsEqual(color.r, 0.1);
    assertFloatsEqual(color.g, 0.1);
    assertFloatsEqual(color.b, 0.1);
    assertFloatsEqual(color.a, 1.0);
}

void testParseSpecularRGBOnlyRG() {
    struct WavefrontColor color;
    char input[] = "Ks 0.1 0.5";
    int result;
    result = wavefrontMaterialParseSpecularRGB(&color, input);
    assertTrue(result);
    assertFloatsEqual(color.r, 0.1);
    assertFloatsEqual(color.g, 0.5);
    assertFloatsEqual(color.b, 0.1);
    assertFloatsEqual(color.a, 1.0);
}

void testParseSpecularRGBGarbage() {
    struct WavefrontColor color;
    char input[] = "Ks 0.1 0.5 0.7 asdf";
    int result;
    result = wavefrontMaterialParseSpecularRGB(&color, input);
    assertFalse(result);
}


void testParseTransmissionRGB() {
    struct WavefrontColor color;
    char input[] = "Tf 0.1 0.5 0.7";
    int result;
    result = wavefrontMaterialParseTransmissionRGB(&color, input);
    assertTrue(result);
    assertFloatsEqual(color.r, 0.1);
    assertFloatsEqual(color.g, 0.5);
    assertFloatsEqual(color.b, 0.7);
    assertFloatsEqual(color.a, 1.0);
}

void testParseTransmissionRGBOnlyR() {
    struct WavefrontColor color;
    char input[] = "Tf 0.1";
    int result;
    result = wavefrontMaterialParseTransmissionRGB(&color, input);
    assertTrue(result);
    assertFloatsEqual(color.r, 0.1);
    assertFloatsEqual(color.g, 0.1);
    assertFloatsEqual(color.b, 0.1);
    assertFloatsEqual(color.a, 1.0);
}

void testParseTransmissionRGBOnlyRG() {
    struct WavefrontColor color;
    char input[] = "Tf 0.1 0.5";
    int result;
    result = wavefrontMaterialParseTransmissionRGB(&color, input);
    assertTrue(result);
    assertFloatsEqual(color.r, 0.1);
    assertFloatsEqual(color.g, 0.5);
    assertFloatsEqual(color.b, 0.1);
    assertFloatsEqual(color.a, 1.0);
}

void testParseTransmissionRGBGarbage() {
    struct WavefrontColor color;
    char input[] = "Tf 0.1 0.5 0.7 asdf";
    int result;
    result = wavefrontMaterialParseTransmissionRGB(&color, input);
    assertFalse(result);
}

void testParseComment() {
    char input[] = "# A comment.";
    int result = wavefrontMaterialParseComment(input);
    assertTrue(result);
}

void testParseIllumination() {
    char input[] = "illum 5";
    int result, illuminationModel;
    result = wavefrontMaterialParseIllumination(&illuminationModel, input);
    assertTrue(result);
    assertIntegersEqual(illuminationModel, 5);
}

void testParseIlluminationMissingNumber() {
    char input[] = "illum";
    int result, illuminationModel;
    result = wavefrontMaterialParseIllumination(&illuminationModel, input);
    assertFalse(result);
}

void testParseIlluminationEmptyNumber() {
    char input[] = "illum ";
    int result, illuminationModel;
    result = wavefrontMaterialParseIllumination(&illuminationModel, input);
    assertFalse(result);
}

void testParseIlluminationInvalidNumber() {
    char input[] = "illum asdf";
    int result, illuminationModel;
    result = wavefrontMaterialParseIllumination(&illuminationModel, input);
    assertFalse(result);
}

void testParseIlluminationIgnoresGarbage() {
    char input[] = "illum 10 asdf";
    int result, illuminationModel;
    result = wavefrontMaterialParseIllumination(&illuminationModel, input);
    assertTrue(result);
    assertIntegersEqual(illuminationModel, 10);
}

void testParseSpecularExponent() {
    char input[] = "Ns 0.5";
    int result;
    float specularExponent;
    result = wavefrontMaterialParseSpecularExponent(&specularExponent, input);
    assertTrue(result);
    assertFloatsEqual(specularExponent, 0.5);
}

void testParseBlenderWavefrontMaterial() {
    char input[] = "# Blender MTL File: 'test.xyz'\n"
                   "# Material Count: 1 \n"
                   "\n"
                   "newmtl Material_001.001\n"
                   "Ns 3.920000\n"
                   "Ka 0.100000 0.200000 0.300000\n"
                   "Kd 0.400000 0.500000 0.600000\n"
                   "Ks 0.700000 0.800000 0.900000\n"
                   "Ni 0.500000\n"
                   "d 0.200000\n"
                   "illum 2\n"
           "map_Kd test.png\n";
    struct WavefrontMTL mtl;
    mtl.materials = NULL;
    mtl.materialCount = 0;
    int result = parseWavefrontMTLFromString(&mtl, input);
    assertTrue(result);
    struct WavefrontMaterial* m = mtl.materials;
    assertStringsEqual(m->name, "Material_001.001");
    assertFloatsEqual(m->specularExponent, 3.92);
    assertFloatsEqual(m->ambient.r, 0.1);
    assertFloatsEqual(m->ambient.g, 0.2);
    assertFloatsEqual(m->ambient.b, 0.3);
    assertFloatsEqual(m->diffuse.r, 0.4);
    assertFloatsEqual(m->diffuse.g, 0.5);
    assertFloatsEqual(m->diffuse.b, 0.6);
    assertFloatsEqual(m->specular.r, 0.7);
    assertFloatsEqual(m->specular.g, 0.8);
    assertFloatsEqual(m->specular.b, 0.9);
    assertFloatsEqual(m->opticalDensity, 0.5);
    assertIntegersEqual(m->illuminationModel, 2);
    assertStringsEqual(m->diffuseMap.file, "test.png");
    wavefrontMTLFree(&mtl);
}

void testParseGuruWavefrontMaterial() {
    char input[] = "# Guru\n"
                   "# File Created: 09.01.2016 12:00:00\n\n"
                   "newmtl Material_Test\n"
                   "\tNs 32\n"
                   "\td 1\n"
                   "\tTr 0\n"
                   "\tTf 1 1 1\n"
                   "\tillum 2\n"
                   "\tKa 0.1000 0.2000 0.3000\n"
                   "\tKd 0.4000 0.5000 0.6000\n"
                   "\tKs 0.7000 0.8000 0.9000\n";
    struct WavefrontMTL mtl;
    mtl.materials = NULL;
    mtl.materialCount = 0;
    int result = parseWavefrontMTLFromString(&mtl, input);
    assertTrue(result);
    struct WavefrontMaterial* m = mtl.materials;
    assertStringsEqual(m->name, "Material_Test");
    assertFloatsEqual(m->specularExponent, 32.0);
    assertFloatsEqual(m->ambient.r, 0.1);
    assertFloatsEqual(m->ambient.g, 0.2);
    assertFloatsEqual(m->ambient.b, 0.3);
    assertFloatsEqual(m->diffuse.r, 0.4);
    assertFloatsEqual(m->diffuse.g, 0.5);
    assertFloatsEqual(m->diffuse.b, 0.6);
    assertFloatsEqual(m->specular.r, 0.7);
    assertFloatsEqual(m->specular.g, 0.8);
    assertFloatsEqual(m->specular.b, 0.9);
    assertIntegersEqual(m->illuminationModel, 2);
    wavefrontMTLFree(&mtl);
}

void wavefrontMaterialParserTest() {
    testParseNewMaterial();
    testParseNewMaterialNoName();
    testParseNewMaterialGarbage();

    testParseAmbientRGB();
    testParseAmbientRGBOnlyR();
    testParseAmbientRGBOnlyREmptyG();
    testParseAmbientRGBOnlyRG();
    testParseAmbientRGBOnlyRGEmptyB();
    testParseAmbientRGBGarbage();

    testParseDiffuseRGB();
    testParseDiffuseRGBOnlyR();
    testParseDiffuseRGBOnlyRG();
    testParseDiffuseRGBGarbage();

    testParseSpecularRGB();
    testParseSpecularRGBOnlyR();
    testParseSpecularRGBOnlyRG();
    testParseSpecularRGBGarbage();

    testParseTransmissionRGB();
    testParseTransmissionRGBOnlyR();
    testParseTransmissionRGBOnlyRG();
    testParseTransmissionRGBGarbage();

    testParseComment();

    testParseIllumination();
    testParseIlluminationMissingNumber();
    testParseIlluminationEmptyNumber();
    testParseIlluminationInvalidNumber();
    testParseIlluminationIgnoresGarbage();

    testParseSpecularExponent();

    testParseBlenderWavefrontMaterial();
    testParseGuruWavefrontMaterial();
}
