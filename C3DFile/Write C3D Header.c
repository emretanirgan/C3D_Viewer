/**************************************************************/
/***						Write C3D Header.c					 		***/
/**************************************************************/

/**************************************************************/
/***	Writes header information to C3D file passed				***/
/***	in "outfile" (files should be opened before calling.)	***/
/***  Writes the values passed in call as passed, copies		***/
/***	all other values from file passed in infile.				***/
/**************************************************************/
/***	4/21/00																***/
/***	Added commments from www.c3d.org								***/
/**************************************************************/


#include<stdio.h>
#include<stdlib.h>
//#include "c3dutil.c"
//#include "F:\BMacWilliams\Source\gait\binconvert.c"

void Write_C3D_Header(
	unsigned short	num_markers, 
	unsigned short	num_channels,
	unsigned short	first_field,
	unsigned short	last_field,
	float				scale_factor,
	unsigned short	start_record_num,
	unsigned short	frames_per_field,
	float				video_rate,
	FILE				*infile,
	FILE				*outfile)

{ /* Begin Read_C3D_Header */


/* Function prototypes */
float					ConvertDecToFloat(char bytes[4]);
unsigned short		max_gap,i,idum;
short					key1;
char					DEC_float[3];

/* Read in Header */

// Word = 1 (bytes = 0,1 = hex 00, 01)
// Byte 1: The number of the first parameter record (normally 02) 
// Byte 2: Key value of 50 hex indicating a C3D file.
// Typical value = 0x5002
fread(&key1, sizeof(short), 1, infile); 
fwrite(&key1, sizeof(short), 1, outfile); 
//printf("key1 = %d\n",key1);
 
// Word = 2 (bytes = 2,3 = hex 02, 03)
// Number of 3D points stored in the file (i.e. the number of stored trajectories).
fread(&idum, sizeof(short), 1, infile); 
fwrite(&num_markers, sizeof(short), 1, outfile); 
//printf("num_markers = %d\n",*num_markers);

// Word = 3 (bytes = 4,5 = hex 04, 05) 
// Number of analog channels per 3D sample. 
fread(&idum, sizeof(short), 1, infile); 
fwrite(&num_channels, sizeof(short), 1, outfile); 
//printf("num_channels = %d\n",*num_channels);

// Word = 4 (bytes = 6,7 = hex 06, 07)
// Number of the first sample of 3D data (normally 1). 
fread(&idum, sizeof(short), 1, infile); 
fwrite(&first_field, sizeof(short), 1, outfile); 
//printf("first_field = %d\n",*first_field);

// Word = 5 (bytes = 8,9 = hex 08, 09)
// Number of the last sample of 3D data.
fread(&idum, sizeof(short), 1, infile); 
fwrite(&last_field, sizeof(short), 1, outfile); 
//printf("last_field = %d\n",*last_field);

// Word = 6 (bytes = 10,11 = hex 0A, 0B)
// Maximum interpolation gap in 3D data samples. 
fread(&max_gap, sizeof(short), 1, infile); 
fwrite(&max_gap, sizeof(short), 1, outfile); 
//printf("max_gap = %d\n",max_gap);

// Words = 7,8 (bytes = 12,13,14,15 = hex 0C 0D 0E 0F)
// The 3D scale factor (REAL*4) that converts integer 3D data 
// to reference system measurement units. If this is negative 
// then the 3D and analog data contained in the file is already 
fread(DEC_float, sizeof(char), 4, infile);

//*scale_factor = ConvertDecToFloat(DEC_float);
//printf("scale_factor = %f\n",*scale_factor);
fwrite(DEC_float, sizeof(char), 4, outfile);

// Word = 9 (bytes = 16,17 = hex 10, 11)
// DATA_START - the record number for the start of data. 
fread(&idum, sizeof(short), 1, infile);
fwrite(&start_record_num, sizeof(short), 1, outfile);
//printf("start_record_num = %d\n",*start_record_num);

// Word = 10 (bytes = 18,19 = hex 12, 13)
// The number of analog samples per 3D sample interval.
fread(&idum, sizeof(short), 1, infile);
fwrite(&frames_per_field,sizeof(short), 1, outfile);
//printf("frames_per_field = %d\n",*frames_per_field);

// Analog channels sampled
// Convert channels from total number of samples in each 
// video frame to the number of analog channels collected
//if (*frames_per_field != 0) 
//	*num_channels /= *frames_per_field;
//printf("num_channels reset to = %d\n",*num_channels);

// Words = 11,12 (bytes = 20,21,22,23 = hex 14, 15, 16, 17)
// The 3D sample rate in Hz (REAL*4).
fread(&DEC_float[0], sizeof(char), 1, infile); // 1st byte
fread(&DEC_float[1], sizeof(char), 1, infile); // 2nd byte
fread(&DEC_float[2], sizeof(char), 1, infile); // 3rd byte
fread(&DEC_float[3], sizeof(char), 1, infile); // 4th byte

//*video_rate = ConvertDecToFloat(DEC_float); 
//printf("video_rate = %6.2f\n",*video_rate);

fwrite(&DEC_float[0], sizeof(char), 1, outfile); // 1st byte
fwrite(&DEC_float[1], sizeof(char), 1, outfile); // 2nd byte
fwrite(&DEC_float[2], sizeof(char), 1, outfile); // 3rd byte
fwrite(&DEC_float[3], sizeof(char), 1, outfile); // 4th byte

// 370 does not use the rest of the header

for (i=13;i<=256;i++) 
{
	fread(&idum, sizeof(short), 1, infile);
	fwrite(&idum, sizeof(short), 1, outfile);
}

// Words = 13-147 (bytes = 24-293 = hex 18-125)
// Reserved for future use.
//for (i=13;i<=147;i++) 
//{
//	fread(&idum, sizeof(short), 1, infile);
//	fwrite(&idum, sizeof(short), 1, outfile);
//}

// Word = 148 (bytes = 294,295 = hex 126, 127) 
// Normally 0x00 this key value (12345 base10, 0x3039 hex) is set  
// to indicate that Label and Range data is present in the file.
//fread(&idum, sizeof(short), 1, infile);
//fwrite(&idum, sizeof(short), 1, outfile);
//printf("label key = %d\n",idum);

// Word = 149 (bytes = 296,297 = hex 128, 129) 
// Record number of the start of Label and Range Data (if present).
//fread(&idum, sizeof(short), 1, infile);
//fwrite(&idum, sizeof(short), 1, outfile);
//printf("label record = %d\n",idum);

// Word = 150 (bytes = 298,299 = hex 12A, 12B) 
// A key value (12345 base 10) present if this file supports 4 char 
// event labels. An older format supported only 2 character labels.
//fread(&idum, sizeof(short), 1, infile);
//fwrite(&idum, sizeof(short), 1, outfile);

// Word = 151 (bytes = 300,301 = hex 12C, 12D) 
// Number of defined time events (0 to 18)
//fread(num_time_events, sizeof(short), 1, infile);
//printf("num_time_events = %d\n",*num_time_events);

// Word = 152 (bytes = 302,303 = hex 12E, 12F) 
// Reserved for future use.
//fread(&idum, sizeof(short), 1, infile);
//fwrite(&idum, sizeof(short), 1, outfile);

// Words = 153-188 (bytes = 304-375 = hex 130-177) 
// REAL*4 event times in seconds ( maximum of 18 events).
//for (i=1;i<=18;i++) 
//{
//		fread(&DEC_float[0], sizeof(char), 1, infile); // 1st byte
//		fread(&DEC_float[1], sizeof(char), 1, infile); // 2nd byte
//		fread(&DEC_float[2], sizeof(char), 1, infile); // 3rd byte
//		fread(&DEC_float[3], sizeof(char), 1, infile); // 4th byte
//		event_time[i] = ConvertDecToFloat(DEC_float); 
//		printf("event_time[%d] = %f\n",i,event_time[i]);
//		fwrite(&DEC_float[0], sizeof(char), 1, outfile); // 1st byte
//		fwrite(&DEC_float[1], sizeof(char), 1, outfile); // 2nd byte
//		fwrite(&DEC_float[2], sizeof(char), 1, outfile); // 3rd byte
//		fwrite(&DEC_float[3], sizeof(char), 1, outfile); // 4th byte
//}

// Words = 189-198 (bytes = 376-395 = hex 178-18B) 
// Event display flags 0x00 = ON, 0x01 = OFF: 9 events.
//for (i=1;i<=9;i++) 
//{
//	fread(&event_switch[i], sizeof event_switch[i], 1, infile);
//	printf("event_switch[%d] = %d\n",i,event_switch[i]);
//}

// Words = 199-234 (bytes = 396-467 = hex 18C-1D3) 
// Event labels. Each label is 4 characters long: 9 labels of 4 unsigned chars
//for (i=1;i<=9;i++) 
//{
//	fread(&event_label[i], sizeof(char), 4, infile);
// fwrite(&event_label[i], sizeof(char), 4, outfile);
//	printf("event_label[%d] = %s\n",i,event_label[i]);
//}

// Words = 235-256 (bytes = 468-511 = hex 1D4-1FF) 
// Reserved for future use. 
//for (i=234;i<=255;i++) 	
//{
//	fread(&idum, sizeof(short), 1, infile);
//	fwrite(&idum, sizeof(short), 1, outfile);
//}


} // End Copy_C3D_Header

