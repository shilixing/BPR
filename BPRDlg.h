// BPRDlg.h : header file
//

#if !defined(AFX_BPRDLG_H__A162638A_6B5D_43C0_AFBE_18E3FFA31846__INCLUDED_)
#define AFX_BPRDLG_H__A162638A_6B5D_43C0_AFBE_18E3FFA31846__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "BtnST.h"

#include "BackProjection.h"
/////////////////////////////////////////////////////////////////////////////
// CBPRDlg dialog

class CBPRDlg : public CDialog
{
// Construction
public:
	CBPRDlg(CWnd* pParent = NULL);	// standard constructor
	~CBPRDlg();

// Dialog Data
	//{{AFX_DATA(CBPRDlg)
	enum { IDD = IDD_BPR_DIALOG };
	CComboBox	m_comboAngle;
	//}}AFX_DATA

private:
	void InitImage( int IDC_IMAGE, CRect &rcImage );
	void ShowImage( int IDC_IMAGE, CRect &rcImage );

private:
	CButtonST m_ctlBtn1;
	CButtonST m_ctlBtn2;
	CButtonST m_ctlBtn3;
	CButtonST m_ctlBtn4;

	// 
	CRect m_rcImage1;
	CRect m_rcImage2;
	CRect m_rcImage3;
	CRect m_rcImage4;

private:
	void CreateEllipse();
	CBackProjection *m_BackProjection;
	BYTE **m_ImageData;
	int m_ImageHeight;
	int m_ImageWidth;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBPRDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CBPRDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnSelchangeCombo1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BPRDLG_H__A162638A_6B5D_43C0_AFBE_18E3FFA31846__INCLUDED_)
