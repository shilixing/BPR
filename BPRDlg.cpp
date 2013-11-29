// BPRDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BPR.h"
#include "BPRDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBPRDlg dialog

CBPRDlg::CBPRDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBPRDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBPRDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// m_BackProjection
	m_BackProjection = new CBackProjection( 3.0 );

	m_ImageHeight = m_BackProjection->GetBPHeight();
	m_ImageWidth = m_BackProjection->GetBPWidth();

	m_ImageData = new BYTE* [m_ImageHeight];
	for( int row = 0; row < m_ImageHeight; row++ )
	{
		m_ImageData[ row ] = new BYTE[ m_ImageWidth ];

		memset( (BYTE *)m_ImageData[ row ], 0, m_ImageWidth );
	}
}

CBPRDlg::~CBPRDlg()
{
	int row;
	for( row = 0; row < m_ImageHeight; row++ )
	{
		if( m_ImageData[ row ] != NULL )
		delete [] m_ImageData[ row ];
	}

	if( m_ImageData != NULL )
		delete [] m_ImageData;

	if( m_BackProjection != NULL )
		delete m_BackProjection;
}

void CBPRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBPRDlg)
	DDX_Control(pDX, IDC_COMBO1, m_comboAngle);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON1, m_ctlBtn1);
	DDX_Control(pDX, IDC_BUTTON2, m_ctlBtn2);
	DDX_Control(pDX, IDC_BUTTON3, m_ctlBtn3);
	DDX_Control(pDX, IDC_BUTTON4, m_ctlBtn4);
}

BEGIN_MESSAGE_MAP(CBPRDlg, CDialog)
	//{{AFX_MSG_MAP(CBPRDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBPRDlg message handlers

BOOL CBPRDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	// 
	int nIndex;
	nIndex = m_comboAngle.AddString( "1 degree" );
	m_comboAngle.SetItemData( nIndex, 1 );
	nIndex = m_comboAngle.AddString( "3 degree" );
	m_comboAngle.SetItemData( nIndex, 3 );
	nIndex = m_comboAngle.AddString( "5 degree" );
	m_comboAngle.SetItemData( nIndex, 5 );
	m_comboAngle.SetCurSel( 1 );

	// 
	this->InitImage( IDC_IMAGE1, m_rcImage1 );
	this->InitImage( IDC_IMAGE2, m_rcImage2 );
	this->InitImage( IDC_IMAGE3, m_rcImage3 );
	this->InitImage( IDC_IMAGE4, m_rcImage4 );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBPRDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBPRDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBPRDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
////////////////////////////////////////////////////////////////////////////////////////////
void CBPRDlg::InitImage( int IDC_IMAGE, CRect &rcImage )
{
	CWnd *pWnd;
	CDC *pDC;

	pWnd = GetDlgItem( IDC_IMAGE );
	pDC = pWnd->GetDC();
	pWnd->GetClientRect( &rcImage );
	rcImage.top +=15;
}

void CBPRDlg::ShowImage( int IDC_IMAGE, CRect &rcImage )
{
	CWnd *pWnd;
	CDC *pDC;
	CPoint point;
	COLORREF crColor;
	int row, col, colorValue;

	pWnd = GetDlgItem( IDC_IMAGE );
	pDC = pWnd->GetDC();

	// 
	for( row = 0; row < m_ImageHeight; row++ )
		for( col = 0; col < m_ImageWidth; col++ )
	{
		point.x = rcImage.left + col;
		point.y = rcImage.top + row;
		colorValue = m_ImageData[ row ][ col ];
		crColor = RGB( colorValue, colorValue, colorValue );
		pDC->SetPixel( point, crColor );
	}
}

void CBPRDlg::CreateEllipse()
{
	int col, row;
	float a, b,distance;
	float origin_x = m_ImageWidth/2;
	float origin_y = m_ImageHeight/2;


	a = m_BackProjection->GetLongAxis();
	b = m_BackProjection->GetShortAxis();

	for( col = 0; col < m_ImageHeight; col++ )
		for( row = 0; row < m_ImageWidth; row++ )
		{
			distance = (float)((row-origin_x)*(row-origin_x))/(b*b)
				+(float)((col-origin_y)*(col-origin_y))/(a*a);

			if( distance < 1 )
				m_ImageData[ col ][ row ] = 255;
			else
				m_ImageData[ col ][ row ] = 0;
		}
		
}

////////////////////////////////////////////////////////////////////////////////////////////
void CBPRDlg::OnButton1() 
{
	this->CreateEllipse();

	// 
	ShowImage( IDC_IMAGE1, m_rcImage1 );
}

void CBPRDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	// 
	m_BackProjection->RLFilteredBackProjection();

	// 
	m_BackProjection->GetBackProjectData( m_ImageData );

	// 
	ShowImage( IDC_IMAGE2, m_rcImage2 );
}

void CBPRDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here
	// 
	m_BackProjection->SLFilteredBackProjection();

	// 
	m_BackProjection->GetBackProjectData( m_ImageData );

	// 
	ShowImage( IDC_IMAGE3, m_rcImage3 );
}

void CBPRDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
	//
	m_BackProjection->DirectBackProjection();

	// 
	m_BackProjection->GetBackProjectData( m_ImageData );

	// 
	ShowImage( IDC_IMAGE4, m_rcImage4 );
}

void CBPRDlg::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	float angle;

	int nIndex = m_comboAngle.GetCurSel();
	angle = m_comboAngle.GetItemData( nIndex );

	if( m_BackProjection != NULL )
		delete m_BackProjection;

	m_BackProjection = new CBackProjection( angle );
}


void CAboutDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}
