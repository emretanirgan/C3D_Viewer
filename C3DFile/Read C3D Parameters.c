/**************************************************************/
/***						Read C3D Parameters.c					 	***/
/**************************************************************/

/**************************************************************/
/***	Reads in parameter records from C3D file passed			***/
/***	in "infile" (files should be opened before calling.	***/
/**************************************************************/


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <conio.h>
//#include "c3dutil.c"

// All data in header is specified as "16 bit words", or the equivalent of
// one unsigned char

void Read_C3D_Parameters(
        std::vector<std::string>& labels,
		unsigned char	**alabels,
		float			*gscale,
		float			*ascale,
		int				*zero_off,
		FILE			*infile)

{ /* Begin Read_C3D_Parameters */

float			ConvertDecToFloat(char bytes[4]);
char			cdum,dim,type,gname[25],pname[25],DEC_float[4];
unsigned char	nrow,ncol,i,j;
int				offset;
fpos_t			gbyte;
void			RPF(int *offset, char *type, char *dim, FILE *infile);
void            Search_For_Name(const char* name, FILE *infile);
void            Read_String_List(std::vector<std::string>& list, FILE *infile);

/* Read in Header */

// Start at Byte 512 of infile

fseek(infile,512,0);

// First four bytes specified

// bytes 1 and 2 are part of ID key (1 and 80)
fread(&cdum,sizeof(char),1,infile);		// byte 1
fread(&cdum,sizeof(char),1,infile);		// byte 2
// byte 3 holds # of parameter records to follow
// but is not set correctly by 370. Ignore.
fread(&cdum,sizeof(char),1,infile);		// byte 3
// byte 4 is processor type, Vicon uses DEC (type 2)
fread(&cdum,sizeof(char),1,infile);		// byte 3
if (cdum-83 != 2) 
{
	printf("Non DEC processor type specified\n");
	//_getch(); // ASN MOD TODO: Support float format automatically
}

// Because it is unknown how many groups and how many
// paremeters/group are stored, must scan for each variable 
// of interest.
// Note: Only data of interest passed back from this routine
// is read. Program can be modified to read other data values.

// 1st scan for group POINT
// Parameters stored in POINT are:
//		1. LABELS
//		2. DESCRIPTIONS
//		3. USED
//		4. UNITS
//		5. SCALE
//		6. RATE
//		7. DATA_START
//		8. FRAMES
//		9. INITIAL_COMMAND
//		10.X_SCREEN
//		11.Y_SCREEN

Search_For_Name("POINT", infile); // infile should now point to group POINT
// Record this file position to go back to for each parameter in group
fgetpos(infile, &gbyte);

 //ASN FIX ME
// Scan for each parameter of interest in group POINT
Search_For_Name("LABELS", infile);
RPF(&offset, &type, &dim, infile);	
assert(dim == 2); // dim should be 2 for a 2D array of labels[np][4]
Read_String_List(labels, infile);

/*
Search_For_Name("LABELS2", infile);
RPF(&offset, &type, &dim, infile);	// dim should be 2 for a 2D array of labels[np][4]
assert(dim == 2);
Read_String_List(labels, infile);
*/

// Scan for group SUBJECTS
// Parameters stored in SUBJECTS are:
//     1. IS_STATIC
//     2. USES_PREFIXES
//     3. USED
//     4. NAMES
//     5. LABEL_PREFIXES
//     6. MARKER_SETS
//     7. DISPLAY_SETS
//     8. MODELS
//     9. MODEL_PARAMS

// Scan for group ANALOG
// Parameters stored in group ANALOG are 
//		1. LABELS
//		2. DESCRIPTIONS
//		3. GEN_SCALE
//		4. SCALE
//		5. OFFSET
//		6. UNITS
//		7. USED
//		8. RATE
return;

printf("searching for group ANALOG ... ");
while (strncmp(gname,"ANALOG",6) != 0 && !feof(infile))
{
	fread(&gname,sizeof(char),6,infile);
	fseek(infile,-5*(long)sizeof(char),SEEK_CUR);
}
fseek(infile,5*sizeof(char),SEEK_CUR);

// Record this file position to go back to for each parameter in group
fgetpos(infile, &gbyte);
printf("found group ANALOG\n");


// Scan for each parameter of interest in group ANALOG

// 1. GEN_SCALE
printf("searching for group GEN_SCALE ... ");
while (strncmp(pname,"GEN_SCALE",9) != 0 && !feof(infile))
{
	fread(&pname,sizeof(char),9,infile);
	fseek(infile,-8*(long)sizeof(char),SEEK_CUR);
}
// reposition to end of string GEN_SCALE
fseek(infile,8*sizeof(char),SEEK_CUR);
printf("\tfound parameter GEN_SCALE\n");

RPF(&offset, &type, &dim, infile);	// dim should be 0 for scalar, type=3 (real)
fread(&DEC_float[0], sizeof DEC_float[0], 1, infile); // 1st byte
fread(&DEC_float[1], sizeof DEC_float[0], 1, infile); // 2nd byte
fread(&DEC_float[2], sizeof DEC_float[0], 1, infile); // 3rd byte
fread(&DEC_float[3], sizeof DEC_float[0], 1, infile); // 4th byte
*gscale = ConvertDecToFloat(DEC_float); 

// 2. SCALE - scale factor for each analog channel
// Don't reposition to start of group! Else will read SCALE in GEN_SCALE
//fseek(infile,gbyte,0);
while (strncmp(pname,"SCALE",5) != 0 && !feof(infile))
{
	fread(&pname,sizeof(char),5,infile);
	fseek(infile,-4*(long)sizeof(char),SEEK_CUR);
}
// reposition to end of string SCALE
fseek(infile,4*sizeof(char),SEEK_CUR);
printf("\tfound parameter SCALE\n");

RPF(&offset, &type, &dim, infile);	// dim should be 1 for 1D array, type=3 (real)
// Read # of components in array
fread(&ncol, sizeof(char),1,infile);
for(i=1;i<=ncol;i++)
{
	fread(&DEC_float[0], sizeof DEC_float[0], 1, infile); // 1st byte
	fread(&DEC_float[1], sizeof DEC_float[0], 1, infile); // 2nd byte
	fread(&DEC_float[2], sizeof DEC_float[0], 1, infile); // 3rd byte
	fread(&DEC_float[3], sizeof DEC_float[0], 1, infile); // 4th byte
	ascale[i] = ConvertDecToFloat(DEC_float); 
}

// 3. OFFSET - zero offset for each analog channel
// Reposition to start of group
fseek(infile,(long)gbyte,0);
while (strncmp(pname,"OFFSET",6) != 0)
{
	fread(&pname,sizeof(char),6,infile);
	fseek(infile,-5*(long)sizeof(char),SEEK_CUR);
}
// reposition to end of string OFFSET
fseek(infile,5*sizeof(char),SEEK_CUR);
printf("\tfound parameter OFFSET\n");

RPF(&offset, &type, &dim, infile);	// dim should be 1 for 1D array, type=2 (integer)
// Read # of components in array
fread(&ncol, sizeof(char),1,infile);
for(i=1;i<=ncol;i++)
{
	fread(&DEC_float[0], sizeof DEC_float[0], 1, infile); // 1st byte
	fread(&DEC_float[1], sizeof DEC_float[0], 1, infile); // 2nd byte
	zero_off[i] = 256*DEC_float[1] + DEC_float[0]; 
}

// 4. LABELS
// Reposition to start of group
fseek(infile,(long)gbyte,0);
while (strncmp(pname,"LABELS",6) != 0)
{
	fread(&pname,sizeof(char),6,infile);
	fseek(infile,-5*(long)sizeof(char),SEEK_CUR);
}
// reposition to end of LABELS
fseek(infile,5*sizeof(char),SEEK_CUR);
printf("\tfound parameter LABELS\n");

RPF(&offset, &type, &dim, infile);	// dim should be 2 for a 2D array of labels[np][4]
// read in array dimensions: should be 4 x np
fread(&ncol, sizeof(char),1,infile);
fread(&nrow, sizeof(char),1,infile);
// ASN WARNING: This needs to be updated before it can be used!
//alabels = cmatrix(1,nrow,1,ncol);
/*
fread(&string, sizeof(char),(nrow*ncol),infile);
for (i=1;i<=nrow;i++) 
{
	//printf("%d ",i);
	for (j=1;j<=ncol;j++) 
	{
		alabels[i][j] = string[(i-1)*ncol+j-1];
		//printf("%c",alabels[i][j]);
	}
	//printf("\n");
}
*/
for (i=1;i<=nrow;i++) 
{
	for (j=1;j<=ncol;j++) 
	{
		fread(&alabels[i][j],sizeof(char),1,infile); 
	}
}
} /* End Read_C3D_Parameters */

void RPF(int *offset, char *type, char *dim, FILE *infile)
{
char	offset_low,offset_high;
// Read Parameter Format for the variable following the 
// parameter name
//		offset = number of bytes to start of next parameter (2 Bytes, reversed order: low, high)
//		T = parameter type: -1 = char, 1 = boolean, 2 = int, 3 = float
//		D = total size of array storage (incorrect, ignore)
//		d = number of dimensions for an array parameter (d=0 for single value)
//		dlen = length of data
fread(&offset_low,sizeof(char),1,infile);		// byte 1
fread(&offset_high,sizeof(char),1,infile);	// byte 2
*offset = 256*offset_high + offset_low;
fread(type, sizeof(char),1,infile);				// byte 3
fread(dim, sizeof(char),1,infile);				// byte 4
}

void Read_String_List(std::vector<std::string>& list, FILE* infile)
{
// read in array dimensions: should be 4 x np
unsigned char ncol, nrow;
fread(&ncol, sizeof(char),1,infile);
fread(&nrow, sizeof(char),1,infile);

char* string = new char[nrow*ncol];
fread(string, sizeof(char),(nrow*ncol),infile);
for (int i=0;i<nrow;i++) 
{
	//ASN DEBUG printf("%d ",i);
    char label[64];

	for (int j=0;j<ncol;j++) 
	{
		label[j] = string[i*ncol+j];
        label[31] = '\0';
		//ASN DEBUG printf("%c",label[j]);
	}
    list.push_back(label);
	//ASN DEBUG printf("\n");
}
delete[] string;
}

void Search_For_Name(const char* name, FILE* infile)
{
printf("searching for name %s ... ", name);
char gname[25];
gname[0] = '\0';
int len = strlen(name);
while (strncmp(gname,name,len) != 0 && !feof(infile))
{
	fread(&gname,sizeof(char),len,infile);
	fseek(infile,-(len-1)*(long)sizeof(char),SEEK_CUR);
}
fseek(infile,(len-1)*sizeof(char),SEEK_CUR);
printf("found name %s\n", name);
}
