// Projection.cpp: implementation of the CProjection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BPR.h"
#include "Projection.h"

#include < math.h >

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProjection::CProjection()
{
	int row, col;

	m_LongAxis = LONGAXIS*MULTIPAL;
	m_ShortAxis = SHORTAXIS*MULTIPAL;
	m_RotateDegreeAngle = 3.0;
	m_Density = 11.0;

	m_PWidth = _SIZE;
	m_PHeight = (int)( 180.0/m_RotateDegreeAngle );
	m_DStep = 1;

	m_ProjectionData = new float* [m_PHeight];
	for( row = 0; row < m_PHeight; row++ )
	{
		m_ProjectionData[ row ] = new float[ m_PWidth ];
		// 
		for( col = 0; col < m_PWidth; col++ )
			m_ProjectionData[ row ][ col ] = 0.0;
	}
}

CProjection::CProjection( float RotateDegreeAngle )
{
	int row, col;

	m_LongAxis = LONGAXIS*MULTIPAL;
	m_ShortAxis = SHORTAXIS*MULTIPAL;
	m_RotateDegreeAngle = RotateDegreeAngle;
	m_Density = 11.0;

	m_PWidth = _SIZE;
	m_PHeight = (int)( 180.0/m_RotateDegreeAngle );
	m_DStep = 1;

	m_ProjectionData = new float* [m_PHeight];
	for( row = 0; row < m_PHeight; row++ )
	{
		m_ProjectionData[ row ] = new float[ m_PWidth ];
		// 
		for( col = 0; col < m_PWidth; col++ )
			m_ProjectionData[ row ][ col ] = 0.0;
	}
}

CProjection::~CProjection()
{
	int row;
	for( row = 0; row < m_PHeight; row++ )
	{
		if( m_ProjectionData[ row ] != NULL )
			delete [] m_ProjectionData[ row ];
	}

	if( m_ProjectionData != NULL )
		delete [] m_ProjectionData;
}

inline void CProjection::SetRotateDegreeAngle( float RotateDegreeAngle )
{
	m_RotateDegreeAngle = RotateDegreeAngle;
}

void CProjection::GetProjectionData( float **ProjectionData )
{
	int row, col;
	for( row = 0; row < m_PHeight; row++ )
		for( col = 0; col < m_PWidth; col++ )
		ProjectionData[ row ][ col ] = m_ProjectionData[ row ][ col ];
}

BOOL CProjection::ScanProjectionData()
{
	int col, row;
	float temp_rr, temp_d;
	float temp_Angle, temp_fCos, temp_fSin;

	for( row = 0; row < m_PHeight; row++ )
		for( col = 0; col < m_PWidth; col++ )
		{
			// 
			temp_Angle = m_RotateDegreeAngle*(float)row*PI/180.0;

			temp_fCos = cos( temp_Angle );
			temp_fSin = sin( temp_Angle );

			temp_rr = m_LongAxis*m_LongAxis*temp_fCos*temp_fCos 
						+ m_ShortAxis*m_ShortAxis*temp_fSin*temp_fSin;

			temp_d = -_SIZE/2 + col;	// [-128, 127]

			if( temp_rr - temp_d*temp_d > 0 )
				m_ProjectionData[ row ][ col ] = 
					2.0 * m_LongAxis * m_ShortAxis * m_Density * sqrt( temp_rr - temp_d*temp_d ) / temp_rr;
			else
				m_ProjectionData[ row ][ col ] = 0.0;
		}
	
	return TRUE;
}
