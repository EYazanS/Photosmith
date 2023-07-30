#ifndef structs_h
#define structs_h

enum file_type
{
	png = 0,
	jpeg = 1,
	jpg = 2,
};

struct params
{
	file_type type;
	int width;
	int height;
	char *path;
};

#endif