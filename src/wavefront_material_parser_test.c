#include "wavefront_material_parser.h"
#include "cutil/src/error.h"
#include "cutil/src/assertion.h"

void testParseNewMaterial() {
    char input[] = "newmtl new_material";
    struct WavefrontMTL mtl;
    int result = parseWavefrontMTLFromString(&mtl, input);
    assertIntegersEqual(result, STATUS_OK);
    assertIntegersEqual(mtl.materialCount, 1);
    assertStringsEqual(mtl.materials[0].name, "new_material");
    wavefrontMTLRelease(&mtl);
}

void testParseNewMaterialNoName() {
    char input[] = "newmtl";
    struct WavefrontMTL mtl;
    int result = parseWavefrontMTLFromString(&mtl, input);
    assertIntegersEqual(result, STATUS_OK);
    assertIntegersEqual(mtl.materialCount, 0);
    wavefrontMTLRelease(&mtl);
}

void testParseNewMaterialGarbage() {
    char input[] = "newmtl new_material asdf";
    struct WavefrontMTL mtl;
    int result = parseWavefrontMTLFromString(&mtl, input);
    assertIntegersEqual(result, STATUS_OK);
    assertIntegersEqual(mtl.materialCount, 0);  
    wavefrontMTLRelease(&mtl);
}

void testParseAmbientRGB() {
    char input[] = "newmtl new_material\n"
                   "Ka 0.1 0.5 0.7";
    struct WavefrontMTL mtl;
    int result = parseWavefrontMTLFromString(&mtl, input);
    assertIntegersEqual(result, STATUS_OK);
    assertIntegersEqual(mtl.materialCount, 1);
    struct WavefrontColor color = mtl.materials[0].ambient;
    assertFloatsEqual(color.r, 0.1);
    assertFloatsEqual(color.g, 0.5);
    assertFloatsEqual(color.b, 0.7);
    assertFloatsEqual(color.a, 1.0);
}

void testParseAmbientRGBOnlyR() {
    char input[] = "newmtl new_material\n"
                   "Ka 0.1";
    struct WavefrontMTL mtl;
    int result = parseWavefrontMTLFromString(&mtl, input);
    assertIntegersEqual(result, STATUS_OK);
    assertIntegersEqual(mtl.materialCount, 1);
    struct WavefrontColor color = mtl.materials[0].ambient;
    assertFloatsEqual(color.r, 0.1);
    assertFloatsEqual(color.g, 0.1);
    assertFloatsEqual(color.b, 0.1);
    assertFloatsEqual(color.a, 1.0);
}

void testParseAmbientRGBOnlyREmptyG() {
    char input[] = "newmtl new_material\n"
                   "Ka 0.1 ";
    struct WavefrontMTL mtl;
    int result = parseWavefrontMTLFromString(&mtl, input);
    assertIntegersEqual(result, STATUS_OK);
    assertIntegersEqual(mtl.materialCount, 1);
    struct WavefrontColor color = mtl.materials[0].ambient;
    assertFloatsEqual(color.r, 0.1);
    assertFloatsEqual(color.g, 0.1);
    assertFloatsEqual(color.b, 0.1);
    assertFloatsEqual(color.a, 1.0);
}

void testParseAmbientRGBOnlyRG() {
    char input[] = "newmtl new_material\n"
                   "Ka 0.1 0.5";
    struct WavefrontMTL mtl;
    int result = parseWavefrontMTLFromString(&mtl, input);
    assertIntegersEqual(result, STATUS_OK);
    assertIntegersEqual(mtl.materialCount, 1);
    struct WavefrontColor color = mtl.materials[0].ambient;
    assertFloatsEqual(color.r, 0.1);
    assertFloatsEqual(color.g, 0.5);
    assertFloatsEqual(color.b, 0.1);
    assertFloatsEqual(color.a, 1.0);
}

void testParseAmbientRGBOnlyRGEmptyB() {
    char input[] = "newmtl new_material\n"
                   "Ka 0.1 0.5 ";
    struct WavefrontMTL mtl;
    int result = parseWavefrontMTLFromString(&mtl, input);
    assertIntegersEqual(result, STATUS_OK);
    assertIntegersEqual(mtl.materialCount, 1);
    struct WavefrontColor color = mtl.materials[0].ambient;
    assertFloatsEqual(color.r, 0.1);
    assertFloatsEqual(color.g, 0.5);
    assertFloatsEqual(color.b, 0.1);
    assertFloatsEqual(color.a, 1.0);
}

void testParseAmbientRGBGarbage() {
    char input[] = "newmtl new_material\n"
                   "Ka 0.1 0.5 0.7 asdf";
    struct WavefrontMTL mtl;
    int result = parseWavefrontMTLFromString(&mtl, input);
    assertIntegersEqual(result, STATUS_PARSE_ERR);
}

void testParseDiffuseRGB() {
    char input[] = "newmtl new_material\n"
                   "Kd 0.1 0.5 0.7";
    struct WavefrontMTL mtl;
    int result = parseWavefrontMTLFromString(&mtl, input);
    assertIntegersEqual(result, STATUS_OK);
    assertIntegersEqual(mtl.materialCount, 1);
    struct WavefrontColor color = mtl.materials[0].diffuse;
    assertFloatsEqual(color.r, 0.1);
    assertFloatsEqual(color.g, 0.5);
    assertFloatsEqual(color.b, 0.7);
    assertFloatsEqual(color.a, 1.0);
}

void testParseDiffuseRGBOnlyR() {
    char input[] = "newmtl new_material\n"
                   "Kd 0.1";
    struct WavefrontMTL mtl;
    int result = parseWavefrontMTLFromString(&mtl, input);
    assertIntegersEqual(result, STATUS_OK);
    assertIntegersEqual(mtl.materialCount, 1);
    struct WavefrontColor color = mtl.materials[0].diffuse;
    assertFloatsEqual(color.r, 0.1);
    assertFloatsEqual(color.g, 0.1);
    assertFloatsEqual(color.b, 0.1);
    assertFloatsEqual(color.a, 1.0);
}

void testParseDiffuseRGBOnlyRG() {
    char input[] = "newmtl new_material\n"
                   "Kd 0.1 0.5";
    struct WavefrontMTL mtl;
    int result = parseWavefrontMTLFromString(&mtl, input);
    assertIntegersEqual(result, STATUS_OK);
    assertIntegersEqual(mtl.materialCount, 1);
    struct WavefrontColor color = mtl.materials[0].diffuse;
    assertFloatsEqual(color.r, 0.1);
    assertFloatsEqual(color.g, 0.5);
    assertFloatsEqual(color.b, 0.1);
    assertFloatsEqual(color.a, 1.0);
}

void testParseDiffuseRGBGarbage() {
    char input[] = "newmtl new_material\n"
                   "Kd 0.1 0.5 0.7 asdf";
    struct WavefrontMTL mtl;
    int result = parseWavefrontMTLFromString(&mtl, input);
    assertIntegersEqual(result, STATUS_PARSE_ERR);
}

void testParseSpecularRGB() {
    char input[] = "newmtl new_material\n"
                   "Ks 0.1 0.5 0.7";
    struct WavefrontMTL mtl;
    int result = parseWavefrontMTLFromString(&mtl, input);
    assertIntegersEqual(result, STATUS_OK);
    assertIntegersEqual(mtl.materialCount, 1);
    struct WavefrontColor color = mtl.materials[0].specular;
    assertFloatsEqual(color.r, 0.1);
    assertFloatsEqual(color.g, 0.5);
    assertFloatsEqual(color.b, 0.7);
    assertFloatsEqual(color.a, 1.0);
}

void testParseSpecularRGBOnlyR() {
    char input[] = "newmtl new_material\n"
                   "Ks 0.1";
    struct WavefrontMTL mtl;
    int result = parseWavefrontMTLFromString(&mtl, input);
    assertIntegersEqual(result, STATUS_OK);
    assertIntegersEqual(mtl.materialCount, 1);
    struct WavefrontColor color = mtl.materials[0].specular;
    assertFloatsEqual(color.r, 0.1);
    assertFloatsEqual(color.g, 0.1);
    assertFloatsEqual(color.b, 0.1);
    assertFloatsEqual(color.a, 1.0);
}

void testParseSpecularRGBOnlyRG() {
    char input[] = "newmtl new_material\n"
                   "Ks 0.1 0.5";
    struct WavefrontMTL mtl;
    int result = parseWavefrontMTLFromString(&mtl, input);
    assertIntegersEqual(result, STATUS_OK);
    assertIntegersEqual(mtl.materialCount, 1);
    struct WavefrontColor color = mtl.materials[0].specular;
    assertFloatsEqual(color.r, 0.1);
    assertFloatsEqual(color.g, 0.5);
    assertFloatsEqual(color.b, 0.1);
    assertFloatsEqual(color.a, 1.0);
}

void testParseSpecularRGBGarbage() {
    char input[] = "newmtl new_material\n"
                   "Ks 0.1 0.5 0.7 asdf";
    struct WavefrontMTL mtl;
    int result = parseWavefrontMTLFromString(&mtl, input);
    assertIntegersEqual(result, STATUS_PARSE_ERR);
}


void testParseTransmissionRGB() {
    char input[] = "newmtl new_material\n"
                   "Tf 0.1 0.5 0.7";
    struct WavefrontMTL mtl;
    int result = parseWavefrontMTLFromString(&mtl, input);
    assertIntegersEqual(result, STATUS_OK);
    assertIntegersEqual(mtl.materialCount, 1);
    struct WavefrontColor color = mtl.materials[0].transmission;
    assertFloatsEqual(color.r, 0.1);
    assertFloatsEqual(color.g, 0.5);
    assertFloatsEqual(color.b, 0.7);
    assertFloatsEqual(color.a, 1.0);
}

void testParseTransmissionRGBOnlyR() {
    char input[] = "newmtl new_material\n"
                   "Tf 0.1";
    struct WavefrontMTL mtl;
    int result = parseWavefrontMTLFromString(&mtl, input);
    assertIntegersEqual(result, STATUS_OK);
    assertIntegersEqual(mtl.materialCount, 1);
    struct WavefrontColor color = mtl.materials[0].transmission;
    assertFloatsEqual(color.r, 0.1);
    assertFloatsEqual(color.g, 0.1);
    assertFloatsEqual(color.b, 0.1);
    assertFloatsEqual(color.a, 1.0);
}

void testParseTransmissionRGBOnlyRG() {
    char input[] = "newmtl new_material\n"
                   "Tf 0.1 0.5";
    struct WavefrontMTL mtl;
    int result = parseWavefrontMTLFromString(&mtl, input);
    assertIntegersEqual(result, STATUS_OK);
    assertIntegersEqual(mtl.materialCount, 1);
    struct WavefrontColor color = mtl.materials[0].transmission;
    assertFloatsEqual(color.r, 0.1);
    assertFloatsEqual(color.g, 0.5);
    assertFloatsEqual(color.b, 0.1);
    assertFloatsEqual(color.a, 1.0);
}

void testParseTransmissionRGBGarbage() {
    char input[] = "newmtl new_material\n"
                   "Tf 0.1 0.5 0.7 asdf";
    struct WavefrontMTL mtl;
    int result = parseWavefrontMTLFromString(&mtl, input);
    assertIntegersEqual(result, STATUS_PARSE_ERR);
}

void testParseComment() {
    char input[] = "# A comment.";
    struct WavefrontMTL mtl;
    int result = parseWavefrontMTLFromString(&mtl, input);
    assertIntegersEqual(result, STATUS_OK);
}

void testParseIllumination() {
    char input[] = "newmtl new_material\n"
                   "illum 5";
    struct WavefrontMTL mtl;
    int result = parseWavefrontMTLFromString(&mtl, input);
    assertIntegersEqual(result, STATUS_OK);
    assertIntegersEqual(mtl.materialCount, 1);
    int illuminationModel = mtl.materials[0].illuminationModel;
    assertIntegersEqual(illuminationModel, 5);
}

void testParseIlluminationMissingNumber() {
    char input[] = "newmtl new_material\n"
                   "illum";
    struct WavefrontMTL mtl;
    int result = parseWavefrontMTLFromString(&mtl, input);
    assertIntegersEqual(result, STATUS_OK);
    assertIntegersEqual(mtl.materialCount, 1);
    int illuminationModel = mtl.materials[0].illuminationModel;
    assertIntegersEqual(illuminationModel, 0);
}

void testParseIlluminationEmptyNumber() {
    char input[] = "newmtl new_material\n"
                   "illum ";
    struct WavefrontMTL mtl;
    int result = parseWavefrontMTLFromString(&mtl, input);
    assertIntegersEqual(result, STATUS_OK);
    assertIntegersEqual(mtl.materialCount, 1);
    int illuminationModel = mtl.materials[0].illuminationModel;
    assertIntegersEqual(illuminationModel, 0);
}

void testParseIlluminationInvalidNumber() {
    char input[] = "newmtl new_material\n"
                   "illum asdf";
    struct WavefrontMTL mtl;
    int result = parseWavefrontMTLFromString(&mtl, input);
    assertIntegersEqual(result, STATUS_OK);
    assertIntegersEqual(mtl.materialCount, 1);
    int illuminationModel = mtl.materials[0].illuminationModel;
    assertIntegersEqual(illuminationModel, 0);
}

void testParseIlluminationIgnoresGarbage() {
    char input[] = "newmtl new_material\n"
                   "illum 10 asdf";
    struct WavefrontMTL mtl;
    int result = parseWavefrontMTLFromString(&mtl, input);
    assertIntegersEqual(result, STATUS_OK);
    assertIntegersEqual(mtl.materialCount, 1);
    int illuminationModel = mtl.materials[0].illuminationModel;
    assertIntegersEqual(illuminationModel, 10);
}

void testParseSpecularExponent() {
    char input[] = "newmtl new_material\n"
                   "Ns 0.5";
    struct WavefrontMTL mtl;
    int result = parseWavefrontMTLFromString(&mtl, input);
    assertIntegersEqual(result, STATUS_OK);
    assertIntegersEqual(mtl.materialCount, 1);
    float specularExponent = mtl.materials[0].specularExponent;
    assertFloatsEqual(specularExponent, 0.5);
}

void testParseReflectionMapSphere() {
    char input[] = "newmtl new_material\n"
                   "refl -type sphere sphere.png";
    struct WavefrontMTL mtl;
    int result = parseWavefrontMTLFromString(&mtl, input);
    assertIntegersEqual(result, STATUS_OK);
    assertIntegersEqual(mtl.materialCount, 1);
    char *reflectionMapSphere = mtl.materials[0].reflectionMapSphere.file;
    assertStringsEqual(reflectionMapSphere, "sphere.png");
}

void testParseReflectionMapCube() {
    char input[] = "newmtl new_material\n"
                   "refl -type cube_top ../skybox/up.png\n"
                   "refl -type cube_bottom ../skybox/dn.png\n"
                   "refl -type cube_front ../skybox/ft.png\n"
                   "refl -type cube_back ../skybox/bk.png\n"
                   "refl -type cube_left ../skybox/lf.png\n"
                   "refl -type cube_right ../skybox/rt.png\n";
    struct WavefrontMTL mtl;
    int result = parseWavefrontMTLFromString(&mtl, input);
    assertIntegersEqual(result, STATUS_OK);
    assertIntegersEqual(mtl.materialCount, 1);
    assertStringsEqual(mtl.materials[0].reflectionMapCubeTop.file, "../skybox/up.png");
    assertStringsEqual(mtl.materials[0].reflectionMapCubeBottom.file, "../skybox/dn.png");
    assertStringsEqual(mtl.materials[0].reflectionMapCubeLeft.file, "../skybox/lf.png");
    assertStringsEqual(mtl.materials[0].reflectionMapCubeRight.file, "../skybox/rt.png");
    assertStringsEqual(mtl.materials[0].reflectionMapCubeFront.file, "../skybox/ft.png");
    assertStringsEqual(mtl.materials[0].reflectionMapCubeBack.file, "../skybox/bk.png");

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
    assertIntegersEqual(result, STATUS_OK);
    struct WavefrontMaterial *m = mtl.materials;
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
    assertFloatsEqual(m->dissolve, 0.2);
    assertFloatsEqual(m->opticalDensity, 0.5);
    assertIntegersEqual(m->illuminationModel, 2);
    assertStringsEqual(m->diffuseMap.file, "test.png");
    wavefrontMTLRelease(&mtl);
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
    assertIntegersEqual(result, STATUS_OK);
    struct WavefrontMaterial *m = mtl.materials;
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
    wavefrontMTLRelease(&mtl);
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

    testParseReflectionMapSphere();
    testParseReflectionMapCube();

    testParseBlenderWavefrontMaterial();
    testParseGuruWavefrontMaterial();
}
