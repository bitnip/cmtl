SOURCE= src/wavefront_material.c \
	src/wavefront_material_parser.c
TEST_SOURCE= \
	src/test.c \
	src/wavefront_material_parser_test.c
LIBRARIES=-L../cutil/bin -lcutil
INCLUDES=-I../
