/**************************************************************/
/**************************************************************/
/**************************************************************/
/***																			***/
/***				Write c3d Parameter Util.c							***/
/***																			***/
/**************************************************************/
/***																			***/
/***	Collection of utilities to write groups and paramters	***/
/***	to c3d files.														***/
/***																			***/
/***	Includes:															***/
/***		Write_Group														***/
/***		Copy_Parameter													***/
/***		Write_Parameter												***/
/***																			***/
/**************************************************************/
/**************************************************************/
/**************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**************************************************************/
/***								Write Group						 		***/
/**************************************************************/

void Write_Group(char group_num,
					  char group_name[],
					  char group_description[],
					  FILE *outfile)
{

unsigned char	m,n;
unsigned short	off;

/*** PARAMETER GROUPS ***/
// GROUP FORMAT
//
// Byte 1, Length = 1
// Number of characters in the Group name (1-127) - this value may be set to a 
// negative number to indicate that the group is "locked".
//
// Byte 2, Length = 1
// Group ID number (negative).
//
// Bytes 3 - 3+n-1, Length = n
// Group name (ASCII characters - normally upper case only).
//
// Bytes 3+n - 3+n+1, Length = 2
// INT*2 offset in bytes pointing to the start of the next group/parameter.
//
// Bytes 3+n+2, Length = 1
// Number of characters in the Group description.
//
// Bytes 3+n+3 - 3+n+3+m-1, Length = m
// Group description (ASCII characters - mixed case).
// Number of characters in the Group name

n = strlen(group_name);
// Number of characters in the Group description
m = strlen(group_description);
fwrite(&n,sizeof(char),1,outfile);							// Group byte 1
// Group ID number (negative)
group_num *= -1;
fwrite(&group_num,sizeof(char),1,outfile);							// Group byte 2
// Group name (ASCII characters - normally upper case only).
fwrite(group_name,sizeof(char),n,outfile);					// Group byte 3 - 3+n-1
// INT*2 offset in bytes pointing to the start of the next group/parameter.
// offset = length of paramter description m + size of integer storing offset = 2bytes 
// + size of character storing value m (1 byte)
off = 3+m;
fwrite(&off,sizeof(short),1,outfile);							// Group byte 3+n - 3+n+1
// Number of characters in the Group description.
fwrite(&m,sizeof(char),1,outfile);						// Group byte 3+n+2
// Group description (ASCII characters - mixed case).
fwrite(group_description,sizeof(char),m,outfile);	// Group byte 3+n+3 - 3+n+3+m-1

}

/**************************************************************/
/***							Copy Parameter						 		***/
/**************************************************************/

void Copy_Parameter(char group_num,
						  char group_name[63],
						  char param_name[63],
						  FILE *infile,
						  FILE *outfile)
{

// data length is max value of type short = 0xFFFF = 65535
char				data[65535],t;
unsigned char	cdum,glen,plen,gname[63],pname[63],d,n;
short				dlen,sdum;

// Start at Byte 512 of infile
fseek(infile,512,0);

// First scan for Group name
glen = strlen(group_name);
while (strncmp(gname,group_name,glen) != 0)
{
	fread(&gname,sizeof(char),glen,infile);
	fseek(infile,-(glen-1)*(long)sizeof(char),SEEK_CUR);
}
//Set cursor to just after group name
fseek(infile,(glen-1)*sizeof(char),SEEK_CUR);

// Next scan for Parameter name
plen = strlen(param_name);
while (strncmp(pname,param_name,plen) != 0)
{
	fread(&pname,sizeof(char),plen,infile);
	fseek(infile,-(plen-1)*(long)sizeof(char),SEEK_CUR);
}
//Set cursor to 2 bytes before parameter name
fseek(infile,-3*sizeof(char),SEEK_CUR);

//First byte is length of parameter name
fread(&plen,sizeof(char),1,infile);
fwrite(&plen,sizeof(char),1,outfile);

//Second byte is parameter group number
fread(&cdum,sizeof(char),1,infile);
fwrite(&group_num,sizeof(char),1,outfile);

//Next plen bytes are parameter name
fread(pname,sizeof(char),plen,infile);
fwrite(pname,sizeof(char),plen,outfile);

//Next two bytes are integer value of offset
fread(&sdum,sizeof(short),1,infile);
fwrite(&sdum,sizeof(short),1,outfile);

//Next byte is parameter type t(-1,1,2,4)
fread(&t,sizeof(char),1,infile);
fwrite(&t,sizeof(char),1,outfile);

//Next byte is #dimensions (0=scalar)
fread(&d,sizeof(char),1,infile);
fwrite(&d,sizeof(char),1,outfile);

//Next d bytes are array dimensions
dlen=abs(t);
for(n=1;n<=d;n++)
{
	fread(&cdum,sizeof(char),1,infile);
	fwrite(&cdum,sizeof(char),1,outfile);
	dlen *=cdum;
}

//Next dlen bytes are parameter data
//for(n=1;n<=dlen;n++)
//{
//	fread(&cdum,sizeof(char),1,infile);
//	fwrite(&cdum,sizeof(char),1,outfile);
//}

fread(data,sizeof(char),dlen,infile);
fwrite(data,sizeof(char),dlen,outfile);


//Next byte is length of parameter description
fread(&cdum,sizeof(char),1,infile);
fwrite(&cdum,sizeof(char),1,outfile);

//Next cdum bytes are parameter name
fread(pname,sizeof(char),cdum,infile);
fwrite(pname,sizeof(char),cdum,outfile);

}

/**************************************************************/
/***							Write Parameter					 		***/
/**************************************************************/

void Write_Parameter(char group_num,			// Group number
							char param_name[31],		// Parameter name
							char param_type,			// Variable type: -1=char, 2=short, 4=float
							char num_dim,				// Numer of dimensions: 0=scalar
							char dim[7],				// Array[num_dim] containing dimensions
							char *data,					// Character array[dlen] of data
							char param_desc[63],		// Parameter description
							FILE *outfile)

{
// NOTE: Parameter data must be converted into array of type char before passing
//			floats must be segmented into 4 bytes using "Float2DEC", shorts into 2 bytes

unsigned char	m,n;
unsigned short	d,offset;
unsigned int	dlen;
// PARAMETER FORMAT
//
// Byte 1, Length = 1
// Number of characters in the Parameter name (1-127) - this value may be set 
// to a negative number to indicate that the group is "locked".
//
// Byte 2, Length = 1
// Group ID number (positive) to which the Parameter belongs.
// 
// Bytes 3 - 3+n-1, Length = n
// The parameter name (ASCII characters - normally upper case only).
//
// Bytes 3+n - 3+n+1, Length = 2
// INT*2 offset in bytes pointing to the start of the next group/parameter.
//
// Byte 3+n+2, Length = 1
// Length in bytes of each data element
//           -1 for character data
//            1 for byte data
//            2 for Integer data (INT*2)
//            4 for real data (REAL*4)
//
// Byte 3+n+3, Length = 1
// Number of dimensions (0-7) of the parameter - 0 if the parameter is scalar.
//
// Byte 3+n+4 - 3+n+4+d-1, Length = d
// Parameter dimensions.
//
// Byte 3+n+4+d - 3+n+4+d+t-1, Length = t
// The Parameter data
//
// Byte 3+n+4+d+t, Length = 1
// Number of characters in the parameter description
//
// Bytes 3+n+4+d+t+1 - 3+n+4+d+t+m, Length = m
// Parameter description (ASCII characters - mixed case).

// Determine length of data
dlen=abs(param_type);
for(d=1;d<=num_dim;d++)	
{
	dlen *= dim[d];
}

// Number of characters in the Parameter name = n
n = strlen(param_name);
// Number of characters in the Parameter descritpion = m
m = strlen(param_desc);

fwrite(&n,sizeof(char),1,outfile);								// Param byte 1
// Group ID number (positive)
fwrite(&group_num,sizeof(char),1,outfile);					// Param byte 2
// Parameter name (ASCII characters - normally upper case only).
fwrite(param_name,sizeof(char),n,outfile);					// Param byte 3 - 3+n-1
// INT*2 offset in bytes pointing to the start of the next group/parameter.
// Offset = 2 bytes containing short val of offset + 1 byte containing char value 
// of data type + 1 byte containing char val of # dimensions d + d bytes 
// containing dimension values + dlen bytes + 1 byte containing char value of
// # bytes m in description + m bytes for number of characters in parameter description
offset = 5+num_dim+dlen+m;
fwrite(&offset,sizeof(short),1,outfile);						// Param byte 3+n - 3+n+1
// Length in bytes of each data element (-1,1,2,4)
fwrite(&param_type,sizeof(char),1,outfile);					// Param byte 3+n+2
// Number of dimensions (0-7) of the parameter - 0 if the parameter is scalar.
fwrite(&num_dim,sizeof(char),1,outfile);						// Param byte 3+n+3
// Parameter dimensions
for(d=1;d<=num_dim;d++)
{
	fwrite(&dim[d],sizeof(char),1,outfile);					// Param byte 3+n+4 - 3+n+4+d-1
}
// Write Parameter data 
fwrite(&data[1],sizeof(char),dlen,outfile);						// Param byte 3+n+4+d - 3+n+4+d+dlen
// # bytes in Parameter description = m
fwrite(&m,sizeof(char),1,outfile);								// 3+n+4+d+dlen+1
// Parameter description
fwrite(param_desc,sizeof(char),m,outfile);					// 3+n+4+d+dlen+2 - 3+n+4+d+dlen+2+m


//*******************************************************************
// Notes:
// Always use the absolute value when reading the first byte to determine 
// the length of the parameter name. This value may be set to a negative 
// value to indicate that the data item has been marked as locked and 
// should not be edited. 
//
// For all non-array parameters, the usual method of having 'd = 0' is 
// directly equivalent to having 'd = 1' and 't = 1', the only difference 
// is that the second approach requires one extra byte of storage.
// 
// The storage order of multi-dimensioned array parameters follows the 
// FORTRAN convention (for historical reasons). In this format, the dimension 
// that changes more rapidly appears first. For example, the reconstruction 
// volume (parameter "DATA_LIMITS" in group "SEG") is made up from two 3D 
// vectors stored in the order: MinX, MinY, MinZ, MaxX, MaxY, MaxZ
//
// Using the convention, this is defined as a 3 by 2 array. Therefore, the 
// correct definition for the parameter is:
//
//		Number of parameter dimensions = 2
//		Value of first dimension = 3
//		Value of second dimension = 2
//
// Notes for programmers
// There is no count stored for the number of parameters in each group and 
// all group and parameter records can appear in any order. This means that 
// it is permissible for a parameter to appear in the parameters section before 
// the group information and software accessing a C3D file should be prepared 
// to deal with this situation. 
//
// Parameters are connected to groups by use of the group ID number.  Group 
// records have unique ID numbers within the file which are stored as a negative 
// value in byte 2. All parameters belonging to a group will store the same ID 
// as a positive value, also in byte 2. 
//
// The parameter data section ends when a the length of the group/parameter name 
// (byte 1) is zero. 
//
// Do not assume that a group ID number will be the same for any parameter in 
// any given set of files or even that for the same group in different files.  
// Group ID numbers are required to be internally consistent in a single file 
// but may vary even within successive saves of the same file, although in 
// practice, most software tends to preserve Group ID numbers. 
//
// It is important to note that many older "C3D" compatible programs may have 
// a fixed amount of storage allocated within the application for Parameter Storage.  
// This can cause problems for users who add large numbers of parameters (or reserve 
// storage space by dimensioning large arrays within the parameter area).  This 
// limitation can cause these applications to fail in unexpected ways when they attempt 
// to access C3D files with large parameters. 
//*******************************************************************
}

