/**************************************************************/
/***							C3D Util.c				 					***/
/**************************************************************/

/**************************************************************/
/***	Utilities to convert floating points to and from 		***/
/***	the DEC file format used by Vicon.							***/
/**************************************************************/
// Records are 256 (16 bit) words long.

// All floating point numbers are stored in DEC format. 
// The following C code may be used to convert to PC format and back again:

float ConvertDecToFloat(char bytes[4])
{
    char p[4];
    p[0] = bytes[2];
    p[1] = bytes[3];
    p[2] = bytes[0];
    p[3] = bytes[1];
    if (p[0] || p[1] || p[2] || p[3])
        --p[3];          // adjust exponent

    return *(float*)p;
}


void ConvertFloatToDec(float f, char* bytes)
{
    char* p = (char*)&f;
    bytes[0] = p[2];
    bytes[1] = p[3];
    bytes[2] = p[0];
    bytes[3] = p[1];
    if (bytes[0] || bytes[1] || bytes[2] || bytes[3])
        ++bytes[1];      // adjust exponent
}

