///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2010 Aline Normoyle, SIG Lab for Computer Graphics
//
// WARNINGS: This file is based on Bruce A. MacWilliams's partial implementation
//   KNOWN ISSUES: Analog data will crash on load
//                 Multiple subject data probably doesn't work
//                 Loader only reads in data which is uses!
//                 Floating point positions not supported
///////////////////////////////////////////////////////////////////////////////

#ifndef C3DFile_H_
#define C3DFile_H_

#include <string>
#include <vector>
#include <map>
#include "Transformation.h"

class C3DFile
{
public:
    C3DFile();
	C3DFile(const char* filename);
	virtual ~C3DFile();

    virtual bool Read(const char* filename);
    virtual bool Write(const char* filename);
	virtual bool Tick();
    virtual bool isEmpty() const;
    virtual void Clear();

	virtual int GetUnlabeledMarkerCount() const;
	virtual void GetUnlabeledMarkerData(int markerIndex, vec3& position) const;

	virtual int GetSubjectCount() const;
	virtual std::string GetSubjectName(int subjectIndex) const;

	virtual int GetMarkerCount(const std::string& subjectName) const;
	virtual void GetMarkerData(const std::string& subjectName, int markerIndex,
		std::string& markerName, std::string& markerParentName, vec3& position) const;

	virtual void SetCurrentFrame(unsigned int frame) const;
	virtual unsigned int GetCurrentFrame() const;
	virtual unsigned int GetFrameCount() const;
    virtual float GetFrameRate() const;

protected:
    typedef std::map<std::string, std::vector<std::string>> MarkerSet;
    typedef std::vector<std::vector<vec3>> MarkerList;

    MarkerSet mLabels; // Subject x Marker label list
    MarkerList mMarkers;  // Markers x Frame
    mutable unsigned int mCurrentFrame;
    unsigned int mFrameCount;
    unsigned int mNumMarkers;
    float mFrameRate;    
};

#endif