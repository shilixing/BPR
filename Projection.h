// Projection.h: interface for the CProjection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROJECTION_H__77C6A1CF_5BC6_47F4_8F35_0BD9430C7CC0__INCLUDED_)
#define AFX_PROJECTION_H__77C6A1CF_5BC6_47F4_8F35_0BD9430C7CC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////
const float LONGAXIS = 10.0;
const float SHORTAXIS = 5.0;
const float MULTIPAL = 10.0;
const double PI = 3.1415926535897;
const int _SIZE = 256;
const int _FILTERSIZE = 511;
///////////////////////////////////////////////////////////////////////

class CProjection  
{
	//
public:
	CProjection();
	CProjection( float RotateDegreeAngle );
	virtual ~CProjection();

public:
	// 
	void SetRotateDegreeAngle( float RotateDegreeAngle );

	// 

	// 
	void GetProjectionData( float **ProjectionData );

	// 
	float GetShortAxis() { return m_ShortAxis; }

	// 
	float GetLongAxis() { return m_LongAxis; }

	// 
	float GetRotateDegreeAngle() { return m_RotateDegreeAngle; }

	// 
	float GetDStep() { return m_DStep; }

public:
	// 
	BOOL ScanProjectionData( void );

private:
	float	m_LongAxis;				//
	float	m_ShortAxis;			//
	float	m_RotateDegreeAngle;	// 
	float	m_Density;				// 
	float	m_DStep;				// 
protected:
	int		m_PWidth;			// 
	int		m_PHeight;			// 
	float	**m_ProjectionData;
};

#endif // !defined(AFX_PROJECTION_H__77C6A1CF_5BC6_47F4_8F35_0BD9430C7CC0__INCLUDED_)
