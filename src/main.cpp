#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#endif

#include <jpeglib.h>

#include "typesdef.h"
#include "structs.h"
#include "functions.h"

params parse_params(int argc, char **argv)
{
	params args = {};

	char *current_command;

	for (int count = 1; count < argc; count++)
	{
		current_command = argv[count];

		if (strcmp(current_command, "-w") == 0)
		{
			if (count + 1 < argc)
			{
				args.width = atoi(argv[++count]);
			}
		}
		else if (strcmp(current_command, "-h") == 0)
		{
			if (count + 1 < argc)
			{
				args.height = atoi(argv[++count]);
			}
		}
		else
		{
			args.path = current_command;
		}
	}

	return args;
}

int is_file_valid(char *accepted_extentions[], int accepted_extentions_count, char *file_path)
{
	int result = 0;

	int extention_length = 0;

	int path_length = 0;

	int ext_dot_index = 0;

	while (file_path[path_length])
	{
		if (file_path[path_length] == '.')
		{
			ext_dot_index = path_length + 1;
		}

		path_length++;
	}

	for (size_t i = 0; i < accepted_extentions_count; i++)
	{
		if (strcmp(accepted_extentions[i], file_path + ext_dot_index) == 0)
		{
			result = 1;
			break;
		}
	}

	return result;
}

file_type get_file_type(char *accepted_extentions[], int accepted_extentions_count, char *file_path)
{
	file_type result;

	int extention_length = 0;

	int path_length = 0;

	int ext_dot_index = 0;

	while (file_path[path_length])
	{
		if (file_path[path_length] == '.')
		{
			ext_dot_index = path_length + 1;
		}

		path_length++;
	}

	for (size_t i = 0; i < accepted_extentions_count; i++)
	{
		if (strcmp(accepted_extentions[i], file_path + ext_dot_index) == 0)
		{
			result = (file_type)i;
			break;
		}
	}

	return result;
}

image read_file_data(params args, char *file_path)
{
	image result;



	return result;
}

int main(int argc, char **argv)
{
	params args = parse_params(argc, argv);

	if (!args.path || access(args.path, F_OK) != 0)
	{
		fprintf(stderr, "Please enter path");

		return -1;
	}

	if (args.height < 1)
	{
		fprintf(stderr, "Please enter valid hegiht");

		return -1;
	}

	if (args.width < 1)
	{
		fprintf(stderr, "Please enter width hegiht");

		return -1;
	}

	char *accepted_extentions[] = {"png", "jpeg", "jpg"};

	if (!is_file_valid(accepted_extentions, ARRAY_SIZE(accepted_extentions), args.path))
	{
		fprintf(stderr, "Please enter supported file ('png', 'jpeg', 'jpg')");

		return -1;
	}

	args.type = get_file_type(accepted_extentions, ARRAY_SIZE(accepted_extentions), args.path);

	printf_s("W: %i, H: %i, path: %s, type: %i", args.width, args.height, args.path, args.type);

	return 0;
}