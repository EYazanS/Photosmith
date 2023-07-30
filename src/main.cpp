#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <jpeglib.h>

#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#endif

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

int read_jpeg_file(char *filename, image_data *image)
{
	// Declare the JPEG decompression struct and error handling struct
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	// Open the JPEG file
	FILE *infile;

	if (fopen_s(&infile, filename, "rb") != 0)
	{
		fprintf(stderr, "Error opening JPEG file: %s\n", filename);
		return 0;
	}

	// Initialize the error handler and decompression struct
	cinfo.err = jpeg_std_error(&jerr);

	jpeg_create_decompress(&cinfo);

	// Set the source file
	jpeg_stdio_src(&cinfo, infile);

	// Read JPEG header information
	jpeg_read_header(&cinfo, TRUE);

	// Start the decompression process
	jpeg_start_decompress(&cinfo);

	// Retrieve image information
	image->width = cinfo.output_width;
	image->height = cinfo.output_height;
	int num_components = cinfo.output_components; // Usually 3 (RGB)

	// Allocate memory to store the image data
	image->data = (u8 *)malloc(image->width * image->height * num_components);

	// Read scanlines one by one and store them in image_data
	while (cinfo.output_scanline < image->height)
	{
		u8 *row_pointer = &image->data[cinfo.output_scanline * image->width * num_components];
		jpeg_read_scanlines(&cinfo, &row_pointer, 1);
	}

	// Finish the decompression process
	jpeg_finish_decompress(&cinfo);

	// Release the JPEG decompression struct
	jpeg_destroy_decompress(&cinfo);

	// Close the file
	fclose(infile);

	return 1;
}

// Function to free memory used by the iamge_data struct
void free_image_data(image_data *image)
{
	if (image->data)
	{
		free(image->data);
		image->data = NULL;
	}
}

int main(int argc, char **argv)
{
	params args = parse_params(argc, argv);

	if (!args.path || access(args.path, F_OK) != 0)
	{
		fprintf(stderr, "Please enter a valid path\n");

		return -1;
	}

	if (args.height < 1)
	{
		fprintf(stderr, "Please enter valid hegiht\n");

		return -1;
	}

	if (args.width < 1)
	{
		fprintf(stderr, "Please enter valid width\n");

		return -1;
	}

	char *accepted_extentions[] = {"png", "jpeg", "jpg"};

	if (!is_file_valid(accepted_extentions, ARRAY_SIZE(accepted_extentions), args.path))
	{
		fprintf(stderr, "Please enter supported file ('png', 'jpeg', 'jpg')\n");

		return -1;
	}

	args.type = get_file_type(accepted_extentions, ARRAY_SIZE(accepted_extentions), args.path);

	printf_s("W: %i, H: %i, path: %s, type: %i\n", args.width, args.height, args.path, args.type);

	image_data image;

	int success = read_jpeg_file(args.path, &image);

	if (success)
	{
		printf("JPEG file read successfully.\n");
		printf("Image width: %d\n", image.width);
		printf("Image height: %d\n", image.height);
		// You can access the image data in `image.data` for further processing.
	}
	else
	{
		printf("Failed to read JPEG file.\n");
	}

	// Don't forget to free the allocated memory when you're done with it.
	free_image_data(&image);

	return 0;
}