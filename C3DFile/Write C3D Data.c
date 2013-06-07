/**************************************************************/
/***					Write C3D Data.c				 					***/
/**************************************************************/

/**************************************************************/
/***	Write both 3D position and analog data to C3D 			***/
/***	file passed in "outfile" (files should be opened		***/
/***	before calling. Output file pointer should be			***/
/***  properly located at the data start record)				***/
/**************************************************************/

#include <stdio.h>
#include <stdlib.h>

// All data in header is specified as "16 bit words", or the equivalent of
// one unsigned char

void Write_C3D_Data(
	unsigned short	num_markers,				// number of marker trajectories
	unsigned short	num_analog_channels,		// number of analog channels
	unsigned short	first_field,				// first frame to read
	unsigned short	last_field,					// last frame to read
	unsigned short	analog_frames_per_field,// analog samples/ video frame
	// marker xyz positions[1..num_markers][first_field..last_field]
	short			**x,							
	short			**y,
	short			**z,
	char			**residual,						// marker residual
	char			**num_cam,						// cameras used in construction
	// analog data[1..num_analog_channels][1..(last_field-first_field)*analog_frames_per_field
	short			**analog,
	// input file to read: make sure it is opened as binary
	FILE			*outfile)

{

unsigned short	frame, marker, sample, channel;

// For each frame
for (frame = first_field; frame <= last_field; frame++)
{
	// Write Video data

	// For each marker
	for (marker = 1; marker <= num_markers; marker ++)
	{
		// Read X,Y,Z positions
		fwrite(&x[marker][frame], sizeof(short), 1, outfile);
		fwrite(&y[marker][frame], sizeof(short), 1, outfile);
		fwrite(&z[marker][frame], sizeof(short), 1, outfile);

		// Read residual value and # cameras (1 byte each)
		fwrite(&residual[marker][frame], sizeof(char), 1, outfile);
		fwrite(&num_cam[marker][frame], sizeof(char), 1, outfile);
	} // Next marker

	// Read Analog Data

	// For each analog sample / frame
	for (sample = 1; sample <= analog_frames_per_field; sample ++)
	{
		// For each channel of analog data
		for (channel = 1; channel<= num_analog_channels; channel ++)
			fwrite(&analog[channel][(frame-first_field)*analog_frames_per_field+sample], sizeof(short), 1, outfile);
	} // Next sample
}

}