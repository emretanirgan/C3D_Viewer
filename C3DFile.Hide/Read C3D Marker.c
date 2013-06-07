/**************************************************************/
/***					Read C3D Marker.c				 					***/
/**************************************************************/

/**************************************************************/
/***	Reads in 3D position data from C3D file passed			***/
/***	in "infile" (files should be opened before calling.	***/
/***	DEPENDENCIES: Binconvert.c										***/
/**************************************************************/

#include<stdio.h>
#include<stdlib.h>
//#include "c3dutil.c"
//#include "F:\BMacWilliams\Source\gait\binconvert.c"

// All data in header is specified as "16 bit words", or the equivalent of
// one unsigned char

void Read_C3D_Marker(
	unsigned short marker_num,					// Read this marker #
	unsigned short	num_markers,				// This is the total # markers 
	unsigned short	num_analog_channels,		// Number of analog channels
	unsigned short	first_field,				// Start field #
	unsigned short	last_field,					// End field #
	unsigned short	start_byte,					// Where data starts
	unsigned short	analog_frames_per_field,// analog samples/field
	short				**val,						// x,y,z
	unsigned char	*res,							// residual
	FILE				*infile)

{

unsigned short frame_length, frame_num, i, offset;
char				cam;

// Data is stored in the following format
// Each frame (or field) from first_field to last_field
//		Each marker from 1 to num_markers 
//			Data: X,Y,Z,R,C (Total of 8 bytes)
//		Next marker
//		Each analog sample per frame from 1 to analog_frames_per_field
//			Each analog channel from 1 to num_analog_channels
//				Data: A (total of 2 bytes)
//			Next channel
//		Next sample
// Next frame

// Startbyte is the starting location of tvd/adc data
start_byte = 512 * (start_byte - 1);

// Determine data offset based upon starting channel number
offset = 8*(marker_num-1);

// Determine interval to next frame of this markers data
frame_length = (8*(num_markers-1))+(2*num_analog_channels*analog_frames_per_field);

// Position cursor to first data point

fseek(infile,start_byte+offset,SEEK_SET);

for (frame_num = first_field; frame_num <= last_field; frame_num++)
{
	// Read XYZ data for this frame
	for (i=1;i<=3;i++)							// for x,y,z
	{
		fread(&val[frame_num][i], sizeof(short), 1, infile);
	}
	// Read Residual
	fread(&res[frame_num], sizeof(char), 1, infile);
	// Read # cameras
	fread(&cam, sizeof(char), 1, infile);
	// Skip to the next frame
	fseek(infile,frame_length,SEEK_CUR);
}

}