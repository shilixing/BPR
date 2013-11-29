// BackProjection.cpp: implementation of the CBackProjection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BPR.h"
#include "BackProjection.h"

#include < math.h >
#include < vector >

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CBackProjection::CBackProjection()
{
	int row, col;

	m_BPWidth = _SIZE;
	m_BPHeight = _SIZE;

	m_BackProjectionData = new BYTE* [ m_BPHeight ];
	for( row = 0; row < m_BPHeight; row++ )
	{
		m_BackProjectionData[ row ] = new BYTE[ m_BPWidth ];

		// 
		for( col = 0; col < m_BPWidth; col++ )
			m_BackProjectionData[ row ][ col ] = 0;
	}

	m_FilteredProjectionData = new float* [ m_PHeight ];
	for( row = 0; row < m_PHeight; row++ )
	{
		m_FilteredProjectionData[ row ] = new float[ m_PWidth ];

		// 
		for( col = 0; col < m_PWidth; col++ )
			m_FilteredProjectionData[ row ][ col ] = 0.0;
	}

	// 
	this ->ScanProjectionData();
}

CBackProjection::CBackProjection( float RotateDegreeAngle )
				 : CProjection( RotateDegreeAngle )
{
	int row, col;

	m_BPWidth = _SIZE;
	m_BPHeight = _SIZE;

	m_BackProjectionData = new BYTE* [ m_BPHeight ];
	for( row = 0; row < m_BPHeight; row++ )
	{
		m_BackProjectionData[ row ] = new BYTE[ m_BPWidth ];

		// 
		for( col = 0; col < m_BPWidth; col++ )
			m_BackProjectionData[ row ][ col ] = 0;
	}

	m_FilteredProjectionData = new float* [ m_PHeight ];
	for( row = 0; row < m_PHeight; row++ )
	{
		m_FilteredProjectionData[ row ] = new float[ m_PWidth ];

		// 
		for( col = 0; col < m_PWidth; col++ )
			m_FilteredProjectionData[ row ][ col ] = 0.0;
	}

	// 
	this ->ScanProjectionData();
}

CBackProjection::~CBackProjection()
{
	int row;
	for( row = 0; row < m_PHeight; row++ )
	{
		if( m_FilteredProjectionData[ row ] != NULL )
			delete m_FilteredProjectionData[ row ];
	}
	if( m_FilteredProjectionData != NULL )
		delete [] m_FilteredProjectionData;

	for( row = 0; row < m_BPHeight; row++ )
	{
		if( m_BackProjectionData[ row ] != NULL )
			delete [] m_BackProjectionData[ row ];
	}
	if( m_BackProjectionData != NULL )
		delete [] m_BackProjectionData;
}

void CBackProjection::GetBackProjectData( BYTE **BackProjectionData )
{
	int row, col;

	for( row = 0; row < m_BPHeight; row++ )
		for( col = 0; col < m_BPWidth ; col++ )
			BackProjectionData[ row ][ col ] = m_BackProjectionData[ row ][ col ];
}

void CBackProjection::BackProjection()
{
	int row, col, i, j, n;
	float **BPData, BPMax, BPMin;		// 
	float RotateAngle, fCos, fSin, C, x_r, delta, d, temp;

	// BackProjectionData
	for( i = 0; i < m_BPHeight; i++ )
		memset( (BYTE*)m_BackProjectionData[ i ], 0, m_BPWidth );

	// BPData
	BPData = new float *[ m_BPHeight ];
	for( row = 0; row < m_BPHeight; row++ )
	{
		BPData[ row ] = new float[ m_BPWidth ];
		
		for( col = 0; col < m_BPWidth; col++ )
			BPData[ row ][ col ] = 0.0;
	}

	// 
	d = GetDStep();		//
	for( row = 0; row < m_PHeight; row++ )
	{
		RotateAngle = GetRotateDegreeAngle()*(float)row*PI/180.0;

		fCos = cos( RotateAngle );
		fSin = sin( RotateAngle );
    
		//
		C = 0.5 * (float)( _SIZE - 1 ) * ( 1.0 - fCos - fSin );
    
		// 
		for( i = 0; i < m_BPHeight; i++ )
			for( j = 0; j < m_BPWidth; j++ )
			{
				x_r = C + (float)( i - 1 )*fCos + (float)( j - 1 )*fSin;
				n = ( int )x_r;
				delta = ( x_r - (float)n )/d;
            
				if ( ( n >= 0 ) && ( n < _SIZE - 1 ) )
				{
					 temp = (1.0 - delta) * ( float )m_FilteredProjectionData[ row ][ n ] + 
								delta * m_FilteredProjectionData[ row ][ n + 1 ];
				}
				else
				{
					if ( n == _SIZE -1 )
						temp = (1.0 - delta) * ( float )m_FilteredProjectionData[ row ][ n ];
					else
						temp = 0.0;
				}

				// 
				BPData[ i ][ j ] += temp;
			}
	}

	// 
	BPMax = BPMin = BPData[ 0 ][ 0 ];
	for( row = 0; row < m_BPHeight; row++ )
		for( col = 1; col < m_BPWidth; col++ )
		{
			if( BPMax < BPData[ row ][ col ] )
				BPMax = BPData[ row ][ col ];
			if( BPMin > BPData[ row ][ col ] )
				BPMin = BPData[ row ][ col ];
		}

	// 
	temp = 250.0/( BPMax - BPMin );
	for( row = 0; row < m_BPHeight; row++ )
		for( col = 0; col < m_BPWidth; col++ )
			m_BackProjectionData[ row ][ col ] = ( BPData[ row ][ col ] - BPMin ) * temp;

	// 
	for( row = 0; row < m_BPHeight; row++ )
	{
		if( BPData[ row ] != NULL )
			delete BPData[ row ];
	}
	if( BPData != NULL )
		delete BPData;
}

void CBackProjection::DirectBackProjection()
{
	int i, j;

	//
	for( i = 0; i < m_PHeight; i++ )
		for ( j = 0; j < m_PWidth; j++ )
		m_FilteredProjectionData[ i ][ j ] = m_ProjectionData[ i ][ j ];

	//
	BackProjection();
}

void CBackProjection::Convolution(float *Filter)
{
	int row, col, L, point1, point2, point3;
	float FPData, FPMax, FPMin;
	float *EProjectionData;

	// 
	for( row = 0; row < m_PHeight; row++ )
	{
		// 
		for( col = 0; col < m_PWidth; col++ )
			m_FilteredProjectionData[ row ][ col ] = 0.0;
	}

	EProjectionData = new float [ _SIZE + _FILTERSIZE - 1 ];

	for( row = 0; row < _SIZE + _FILTERSIZE - 1; row++ )
		EProjectionData[ row ] = 0.0;

	// 
	point1 = ( _FILTERSIZE - 1 ) / 2;	//255
	point2 = ( _FILTERSIZE - 1 ) / 2 + _SIZE;	//511
	point3 = _SIZE + _FILTERSIZE - 1;	//	765

	for( row = 0; row < m_PHeight; row++ )
	{
		// 
		for( col = 0; col < point1 ; col++ ) // 0---255
			EProjectionData[ col ] = ( m_ProjectionData[ row][ 0 ] + 
									   m_ProjectionData[ row][ 1 ] ) / 2.0;

		for( col = point1 + 1; col < point2; col++ ) // 256--511
			EProjectionData[ col ] = m_ProjectionData[ row ][ col - point1 - 1 ];

		for( col = point2 + 1; col < point3; col++ )	// 512--765
			EProjectionData[ col ] = ( m_ProjectionData[ row ][ _SIZE - 2 ] + 
									   m_ProjectionData[ row ][ _SIZE - 1 ] ) / 2.0;

		// 
		for( col = 0; col < m_PWidth; col++ )
		{
			FPData = 0.0;
			for( L = 0; L < _FILTERSIZE ; L++ )
				FPData += Filter[ L ] * EProjectionData[ col + L ];

			m_FilteredProjectionData[ row ][ col ] = FPData;
		}
	}

	// m_FilteredProjectionData的最大值和最小值
	/*
	FPMax = FPMin = m_FilteredProjectionData[ 0 ][ 0 ];
	for( row = 0; row < m_PHeight; row++ )
		for( col = 1; col < m_PWidth; col++ )
		{
			if( FPMax < m_FilteredProjectionData[ row ][ col ] )
				FPMax = m_FilteredProjectionData[ row ][ col ];
			if( FPMin > m_FilteredProjectionData[ row ][ col ] )
				FPMin = m_FilteredProjectionData[ row ][ col ];
		}
	*/
}

void CBackProjection::SLFilteredBackProjection()
{
	int n, i;
	float d, SLFilter[ _FILTERSIZE ];
	
	// 
	for( i = 0; i < _FILTERSIZE; i++ )
		SLFilter[ i ] = 0.0;

	//  
	d = GetDStep();
	for( i = 0; i < _FILTERSIZE; i++ )
	{
		n = i - _FILTERSIZE / 2;

		SLFilter[ i ] = - 2.0 / ( PI * PI * d * d *( 4.0 * float( n * n ) - 1.0 ) );
	}

	// 
	this->Convolution( SLFilter );

	// 
	this->BackProjection();
}

void CBackProjection::RLFilteredBackProjection()
{
	int n, i;
	float d, RLFilter[ _FILTERSIZE ];
	
	// 
	for( i = 0; i < _FILTERSIZE; i++ )
		RLFilter[ i ] = 0.0;

	//  
	d = GetDStep();
	for( i = 0; i < _FILTERSIZE; i++ )
	{
		n = i - _FILTERSIZE / 2;

		if( n == 0 )
			RLFilter[ i ] = 1.0 / ( 4.0 * d * d );
		else
		{
			if( n % 2 == 0 )
				RLFilter[ i ] = 0;
			else
				RLFilter[ i ] = - 1.0 / ( float( n * n ) * PI * PI * d * d );
		}

	}

	// 
	this->Convolution( RLFilter );

	// 
	this->BackProjection();
}