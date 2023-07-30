#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#endif

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

int is_file_valid(char *file_path)
{
	int result = 0;

	char *accepted_extentions[] = {"png", "jpeg", "jpg"};

	int extention_length = 0;

	int path_length = 0;

	int ext_dot_index = 0;

	while (file_path[path_length])
	{
		if (file_path[path_length] == '.')
		{
			ext_dot_index = path_length;
		}

		path_length++;
	}

	for (size_t i = 0; i < ARRAY_SIZE(accepted_extentions); i++)
	{
		if (strcmp(accepted_extentions[i], file_path + ext_dot_index) == 0)
		{
			result = 1;
			break;
		}
	}

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

	printf_s("W: %i, H: %i, path: %s", args.width, args.height, args.path);

	if (!is_file_valid(args.path))
	{
		fprintf(stderr, "Please enter supported file ('png', 'jpeg', 'jpg')");

		return -1;
	}

	return 0;
}