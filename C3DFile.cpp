///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2010 Aline Normoyle, SIG Lab for Computer Graphics
///////////////////////////////////////////////////////////////////////////////

#include "C3DFile.h"
#include <windows.h>
#include <iostream>

#include "C3DFile\c3dutil.c"
#include "C3DFile\Read C3D Header.c"
#include "C3DFile\Read C3D Parameters.c"
#include "C3DFile\Read C3D Data.c"

C3DFile::C3DFile() :
    mCurrentFrame(0),
    mFrameCount(0),
    mNumMarkers(0),
    mFrameRate(120.0)
{
}


C3DFile::C3DFile(const char* filename) :
    mCurrentFrame(0),
    mFrameCount(0),
    mNumMarkers(0),
    mFrameRate(120.0)
{
    Read(filename);
}

C3DFile::~C3DFile()
{
}

void C3DFile::Clear()
{
    mMarkers.clear();
    mNumMarkers = 0;
}

bool C3DFile::Read(const char* filename)
{
    // First the Header is read using Read C3D Header, 
    // then the parameters using Read C3D Parameters, 
    // then the actual data using the routine of choice depending on the type of data to read. 
    FILE* fp = fopen(filename, "rb");
    if (!fp)
    {
        printf("ERROR: Cannot open %s for reading\n", filename);
        return false;
    }

   	unsigned short num_markers;
	unsigned short num_channels;
	unsigned short first_field;
	unsigned short last_field;
	unsigned short start_record_num;
	unsigned short frames_per_field;
	float video_rate;
	float scale_factor;

    Read_C3D_Header(
	    &num_markers, 
	    &num_channels,
	    &first_field,
	    &last_field,
	    &scale_factor,
	    &start_record_num,
	    &frames_per_field,
	    &video_rate,
	    fp);

    printf("Reading in %s\n", filename);
    printf("\tnum_markers = %d\n", num_markers);
    printf("\tnum_channels = %d\n", num_channels);
    printf("\tvideo_rate = %f\n", video_rate);
    
    std::vector<std::string> labels;
	unsigned char** alabels = 0;
	float gscale = 1.0;
	float ascale = 1.0;
	int	zero_off = 0;
       
    Read_C3D_Parameters(
		labels,
		alabels,
		&gscale,
		&ascale,
		&zero_off,
		fp);

    // marker xyz positions [1..num_markers][first_field..last_field]
    int num_fields = last_field - first_field;
    float** x = new float*[num_markers];
    for (int i = 0; i < num_markers; i++) x[i] = new float[num_fields]; // ASN MOD

    float** y = new float*[num_markers];
    for (int i = 0; i < num_markers; i++) y[i] = new float[num_fields];

    float** z = new float*[num_markers];
    for (int i = 0; i < num_markers; i++) z[i] = new float[num_fields];

    char** residual = new char*[num_markers];
    for (int i = 0; i < num_markers; i++) residual[i] = new char[num_fields];

    char** num_cam = new char*[num_markers];
    for (int i = 0; i < num_markers; i++) num_cam[i] = new char[num_fields];

    // analog data [1..num_analog_channels][1..(last_field-first_field)*analog_frames_per_field
    short** analog_data = new short*[num_channels];
    for (int i = 0; i < num_channels; i++) analog_data[i] = new short[num_fields];

    Read_C3D_Data(
	    num_markers,		// number of marker trajectories
	    num_channels,		// number of analog channels
	    first_field,		// first frame to read
	    last_field,			// last frame to read
	    start_record_num,	// starting record number
	    frames_per_field,   // analog samples/ video frame
	    x, y, z, 
	    residual,			// marker residual
	    num_cam,			// cameras used in construction	
	    analog_data,
	    fp);


    gscale = 0.1; // HACK: Should get from file, here gscale = INCH 2 CM conversion
    mFrameCount = last_field - first_field;
    mNumMarkers = num_markers;
    mMarkers.resize(mNumMarkers);
    for (unsigned int i = 0; i < mNumMarkers; i++)
    {
        mMarkers[i].resize(mFrameCount);
        for (unsigned int j = 0; j < mFrameCount; j++)
        {
            mMarkers[i][j] = vec3(x[i][j]*gscale, y[i][j]*gscale, z[i][j]*gscale);
        }
    }

    mLabels.clear();
    
    for (unsigned int i = 0; i < labels.size(); i++)
    {
        std::string prefix, suffix;
        std::string label = labels[i];
        size_t found=label.find(":");
		if (found!=std::string::npos)
        {
            prefix = label.substr(0,found);
            suffix = label.substr(found+1, label.size()-1);
        }
        else
        {
            printf("ERROR: Only one character per file is currently supported!\n");
            prefix = "Unknown";
            suffix = label;
        }

        if (mLabels.count(prefix) == 0)
        {
            mLabels[prefix] = std::vector<std::string>();
        }
        mLabels[prefix].push_back(suffix);
     }
     

    for (int i = 0; i < num_channels; i++) delete[] num_cam[i];
    delete[] num_cam;

    for (int i = 0; i < num_markers; i++) delete[] residual[i];
    delete[] residual;

    for (int i = 0; i < num_channels; i++) delete[] analog_data[i];
    delete[] analog_data;

    for (int i = 0; i < num_markers; i++) delete[] z[i];
    delete[] z;

    for (int i = 0; i < num_markers; i++) delete[] y[i];
    delete[] y;

    for (int i = 0; i < num_markers; i++) delete[] x[i];
    delete[] x;

    fclose(fp);
    return true;
}

bool C3DFile::Write(const char* filename)
{
   FILE* fp = fopen(filename, "wb");
   if (!fp)
   {
       printf("ERROR: Cannot open %s for reading\n", filename);
       return false;
   }
   return true;
}

bool C3DFile::Tick()
{
    mCurrentFrame++;
    if (mCurrentFrame >= mFrameCount)
    {
        return false; // Done playing file
    }
    return true;
}

bool C3DFile::isEmpty() const
{
    return mNumMarkers == 0;
}

int C3DFile::GetUnlabeledMarkerCount() const
{
    return mNumMarkers;

    // TODO
    /*
    if (mNumMarkers == 0) return 0;
    if (mLabels.size() == 0) return mNumMarkers;
    MarkerSet::iterator start = mLabels.begin();
    return mNumMarkers - start->second.size();
    */
}

void C3DFile::GetUnlabeledMarkerData(int markerIndex, vec3& position) const
{
    if (GetUnlabeledMarkerCount() > 0)
    {
        //MarkerSet::const_iterator start = mLabels.begin();
        int startIndex = 0; // ASN TODO mNumMarkers - start->second.size();
        if (mCurrentFrame < mMarkers[startIndex+markerIndex].size())
            position = mMarkers[startIndex+markerIndex][mCurrentFrame];
    }
}

int C3DFile::GetSubjectCount() const
{
    return mLabels.size();
}

std::string C3DFile::GetSubjectName(int subjectIndex) const
{
    MarkerSet::const_iterator start = mLabels.begin();
    return start->first;
}

int C3DFile::GetMarkerCount(const std::string& subjectName) const
{
    return mNumMarkers;
}

void C3DFile::GetMarkerData(const std::string& subjectName, int markerIndex,
	std::string& markerName, std::string& markerParentName, vec3& position) const
{
    markerName = "TODO"; // ASN TODO mLabels[subjectName][markerIndex];
    if (markerIndex < mMarkers.size() && mCurrentFrame < mMarkers[markerIndex].size())
        position = mMarkers[markerIndex][mCurrentFrame];
}

void C3DFile::SetCurrentFrame(unsigned int frame) const
{
    mCurrentFrame = frame;
}

unsigned int C3DFile::GetCurrentFrame() const
{
    return mCurrentFrame;
}

unsigned int C3DFile::GetFrameCount() const
{
    return mFrameCount;
}

float C3DFile::GetFrameRate() const
{
    return 0.0;
}
