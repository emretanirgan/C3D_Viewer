/**************************************************************/
/***						Read C3D Data.c			 					***/
/**************************************************************/

/**************************************************************/
/***	Reads in both 3D position and analog data from C3D 	***/
/***	file passed in "infile" (files should be opened			***/
/***	before calling.)													***/
/**************************************************************/
/***	4/20/00 - Altered from previous routine Read C3D File ***/
/***	to include reading of residual and camera bits			***/
/**************************************************************/

#include<stdio.h>
#include<stdlib.h>

// All data in header is specified as "16 bit words", or the equivalent of
// one unsigned char

void Read_C3D_Data(
	unsigned short	num_markers,				// number of marker trajectories
	unsigned short	num_analog_channels,		// number of analog channels
	unsigned short	first_field,				// first frame to read
	unsigned short	last_field,					// last frame to read
	unsigned short	start_byte,					// starting record number
	unsigned short	analog_frames_per_field,// analog samples/ video frame
	// marker xyz positions[1..num_markers][first_field..last_field]
	float			**x,							
	float			**y,
	float			**z,
	char			**residual,						// marker residual
	char			**num_cam,						// cameras used in construction
	// analog data[1..num_analog_channels][1..(last_field-first_field)*analog_frames_per_field
	short			**analog,
	// input file to read: make sure it is opened as binary
	FILE			*infile)

{

//unsigned char	residual, num_cam;
unsigned short	frame, marker, sample, channel;

// Startbyte is the starting location of tvd/adc data
start_byte = 512 * (start_byte - 1);

// Position file pointer to start of data record
fseek(infile,start_byte,0);

// For each frame
int num_frames = last_field - first_field;
for (frame = 0; frame < num_frames; frame++)
{
	// Read Video data
	//printf("Frame #%d\n",frame);
	// For each marker
	///printf("Marker #:\n\t\t");
	for (marker = 0; marker < num_markers; marker ++)
	{
		//printf("%d ",marker);
		// Read X,Y,Z positions
		fread(&x[marker][frame], sizeof(float), 1, infile);
		fread(&y[marker][frame], sizeof(float), 1, infile);
		fread(&z[marker][frame], sizeof(float), 1, infile);

		// Read residual value and # cameras (1 byte each)
        float dummy;
        fread(&dummy, sizeof(float), 1, infile);

		//fread(&residual[marker][frame], sizeof(char), 1, infile);
		//printf("%d ",residual[marker][frame]);
		//fread(&num_cam[marker][frame], sizeof(char), 1, infile);
	} // Next marker

	// Read Analog Data
/* ANS MOD
	// For each analog sample / frame
	for (sample = 0; sample < analog_frames_per_field; sample ++)
	{
		// For each channel of analog data
		for (channel = 0; channel< num_analog_channels; channel ++)
			fread(&analog[channel][(frame-first_field)*analog_frames_per_field+sample], sizeof(short), 1, infile);
	} // Next sample
    */
}

}