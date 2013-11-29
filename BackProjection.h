// BackProjection.h: interface for the CBackProjection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BACKPROJECTION_H__4E780C03_5839_41FE_B47C_0C37DC8F258E__INCLUDED_)
#define AFX_BACKPROJECTION_H__4E780C03_5839_41FE_B47C_0C37DC8F258E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Projection.h"
///////////////////////////////////////////////////////////////////////
const int FILTERSIZE = 511;
///////////////////////////////////////////////////////////////////////
class CBackProjection : public CProjection  
{
public:
	CBackProjection();
	CBackProjection( float RotateDegreeAngle );
	virtual ~CBackProjection();

private:
	// 
	void Convolution( float *Filter );
	// 
	void BackProjection();

public:
	int GetBPWidth() { return m_BPWidth; }
	int GetBPHeight() { return m_BPHeight; }
	void GetBackProjectData( BYTE **BackProjectionData );

public:
	// 
	void DirectBackProjection();
	// 
	void RLFilteredBackProjection();
	// 
	void SLFilteredBackProjection();

protected:
	int m_BPWidth;
	int m_BPHeight;
	float **m_FilteredProjectionData;
	BYTE **m_BackProjectionData;
};

#endif // !defined(AFX_BACKPROJECTION_H__4E780C03_5839_41FE_B47C_0C37DC8F258E__INCLUDED_)
