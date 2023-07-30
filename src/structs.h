#ifndef structs_h
#define structs_h

enum file_type
{
	jpg,
	jpeg,
	png
};

struct params
{
	file_type type;
	int width;
	int height;
	char *path;
};

#endif