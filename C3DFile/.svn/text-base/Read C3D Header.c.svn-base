/**************************************************************/
/***						Read C3D Header 3.c					 		***/
/**************************************************************/

/**************************************************************/
/***	Reads in header information from C3D file passed		***/
/***	in "infile" (files should be opened before calling.	***/
/***	DEPENDENCIES: Binconvert.c										***/
/**************************************************************/


#include<stdio.h>
#include<stdlib.h>
//#include "c3dutil.c"
//#include "binconvert.c"

void Read_C3D_Header(
	unsigned short	*num_markers, 
	unsigned short	*num_channels,
	unsigned short	*first_field,
	unsigned short	*last_field,
	float			*scale_factor,
	unsigned short	*start_record_num,
	unsigned short	*frames_per_field,
	float			*video_rate,
	FILE			*infile)

{ /* Begin Read_C3D_Header */


/* Function prototypes */
float					ConvertDecToFloat(char bytes[4]);

unsigned short		key1,max_gap;
char					DEC_float[4];

/* Read in Header */

// Key1, byte = 1,2; word = 1
fread(&key1, sizeof key1, 1, infile); 
//printf("key1 = %d\n",key1);
 
// Number of 3D points per field, byte = 3,4; word = 2
fread(num_markers, sizeof *num_markers, 1, infile); 
//printf("num_markers = %d\n",*num_markers);

// Number of analog channels per field byte = 5,6; word = 3
fread(num_channels, sizeof *num_channels, 1, infile); 
//printf("num_channels = %d\n",*num_channels);

// Field number of first field of video data, byte = 7,8; word = 4
fread(first_field, sizeof *first_field, 1, infile); 
//printf("first_field = %d\n",*first_field);

// Field number of last field of video data, byte = 9,10; word = 5	
fread(last_field, sizeof *last_field, 1, infile); 
//printf("last_field = %d\n",*last_field);

// Maximum interpolation gap in fields, byte = 11,12; word = 6
fread(&max_gap, sizeof max_gap, 1, infile); 
//printf("max_gap = %d\n",max_gap);

// Scaling Factor, bytes = 13,14,15,16; word = 7,8
fread(&DEC_float[0], sizeof(char), 1, infile); // 1st byte
fread(&DEC_float[1], sizeof(char), 1, infile); // 2nd byte
fread(&DEC_float[2], sizeof(char), 1, infile); // 3rd byte
fread(&DEC_float[3], sizeof(char), 1, infile); // 4th byte
//DEC_float[2] = 0;				// 3rd byte
//DEC_float[3] = 0;				// 4th byte

//*scale_factor = ConvertDecToFloat(DEC_float); 
*scale_factor = ConvertDecToFloat(DEC_float);
//printf("scale_factor = %f\n",*scale_factor);

// Starting record number, byte = 17,18; word = 9
fread(start_record_num, sizeof *start_record_num, 1, infile);
//printf("start_record_num = %d\n",*start_record_num);

// Number of analog frames per video field, byte = 19,20; word = 10
fread(frames_per_field, sizeof *frames_per_field, 1, infile);
//printf("frames_per_field = %d\n",*frames_per_field);

// Analog channels sampled
if (*frames_per_field != 0) 
	*num_channels /= *frames_per_field;
//printf("num_channels reset to = %d\n",*num_channels);

// Video rate in Hz, bytes = 21,22; word = 11
fread(&DEC_float[0], sizeof DEC_float[0], 1, infile); // 1st byte
fread(&DEC_float[1], sizeof DEC_float[0], 1, infile); // 2nd byte
DEC_float[2] = 0;
DEC_float[3] = 0;

*video_rate = ConvertDecToFloat(DEC_float); 
//printf("video_rate = %6.2f\n",*video_rate);

// 370 does not use the rest of the header

// Words 12 - 148 unused; position file pointer at byte 149*2=298

//for (i=12;i<=148;i++) 
//	fread(&cdum, sizeof cdum, 1, infile);
//fseek(infile,298,0);

// Key2, byte = 297,298; word = 149
//fread(&key2, sizeof key2, 1, infile);
//printf("key2 = %d\n",key2);

// Number of defined time events, byte = 299,300; word = 150
//fread(num_time_events, sizeof *num_time_events, 1, infile);
//printf("num_time_events = %d\n",*num_time_events);

// Skip byte 301,302; word = 151
//fread(&cdum, sizeof cdum, 1, infile);
//fseek(infile,302,0);

// Event times, bytes 303-374;words = 152-187: 9 events (0-8) of 4 bytes
//for (i=0;i<9;i++) 
//{
//fread(&DEC_float[0], sizeof DEC_float[0], 1, infile); // 1st byte
//fread(&DEC_float[1], sizeof DEC_float[0], 1, infile); // 2nd byte
//fread(&DEC_float[2], sizeof DEC_float[0], 1, infile); // 3rd byte
//fread(&DEC_float[3], sizeof DEC_float[0], 1, infile); // 4th byte
//	event_time[i] = ConvertDecToFloat(DEC_float); 
//	printf("event_time[%d] = %f\n",i,event_time[i]);
//}

// Event switches, bytes = 375-394; words 188-197
//for (i=0;i<9;i++) 
//{
//	fread(&event_switch[i], sizeof event_switch[i], 1, infile);
//	printf("event_switch[%d] = %d\n",i,event_switch[i]);
//}

// Event Labels, bytes = 395-466; words 198-233: 9 labels (0-8) of 4 unsigned chars
//for (i=0;i<9;i++) 
//{
//	fread(&event_label[i], sizeof event_label[i], 1, infile);
//	printf("event_label[%d] = %s\n",i,event_label[i]);
//}

// Bytes 234 - 255 unused
//for (i=234;i<=255;i++) 	fread(&cdum, sizeof cdum, 1, infile);


} /* End Read_C3D_Header */

