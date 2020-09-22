
// SerialPortDemoDlg.h: 头文件
//
#include"SerialPort.h"
#pragma once


// CSerialPortDemoDlg 对话框
class CSerialPortDemoDlg : public CDialogEx
{
// 构造
public:
	CSerialPortDemoDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERIALPORTDEMO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_PortNo;
	CComboBox m_Baud;
	CButton m_btn_Connect;
	CButton m_btn_Send;
	CEdit m_text_Send;
	CEdit m_text_Recieved;

	CSerialPort m_com;
	bool isConnected = false;
	bool port[10];
	void m_CallBack(byte* data, UINT length);
	int DetectPort();
	int SendCmd(CStringA cmd);


	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonSend();
};
