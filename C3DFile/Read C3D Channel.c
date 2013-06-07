/**************************************************************/
/***					Read C3D Channel.c						 	***/
/**************************************************************/

/**************************************************************/
/***	Reads in analog data from C3D file passed					***/
/***	in "infile" (files should be opened before calling).	***/
/***  Channels are numbered starting with 1						***/
/***	Values returned indexed from [1,#frames*samples/frame]***/
/**************************************************************/

#include<stdio.h>
#include<stdlib.h>

void Read_C3D_Channel(
	unsigned short channel_num,				// Read this analog channel #
	unsigned short	num_markers,				// This is the total # markers 
	unsigned short	num_analog_channels,		// Number of analog channels
	unsigned short	first_field,				// Start field #
	unsigned short	last_field,					// End field #
	unsigned short	start_byte,					// Where data starts
	unsigned short	analog_frames_per_field,// analog samples/field
	short				*val,							// Data for specified channel
	FILE				*infile)

{

unsigned short frame_num, sample_num, i, offset, ch_offset, fr_offset;
unsigned long	fpnt;

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

// Determine initial data offset based upon number of markers and starting channel
offset = 8*num_markers+2*(channel_num-1);

// Determine offset to next sample reading within a frame
ch_offset = 2*(num_analog_channels-1);

// Determine offset to next frame after all samples read within frame
fr_offset = 8*num_markers;


// Position cursor to first data point

fseek(infile,start_byte+offset,SEEK_SET);

i = 1;

// For each frame number
for (frame_num = first_field; frame_num <= last_field; frame_num++)
{
	// Read #analog samples per frame
	for (sample_num = 1; sample_num <= analog_frames_per_field; sample_num++)
	{
		fread(&val[i], sizeof(short), 1, infile);
		//fpnt = ftell(infile);
		//printf("%d %d %d\n",i,fpnt,val[i]);
		// Skip over other channels to next reading of this channel
		fseek(infile,ch_offset,SEEK_CUR);
		i++;
	}
	// Skip to the next frame
	fseek(infile,fr_offset,SEEK_CUR);
}

}