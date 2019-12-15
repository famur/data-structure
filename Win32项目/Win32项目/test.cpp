#include<Windows.h>
#include"UIlib.h"

using namespace DuiLib;

#pragma comment (lib, "DuiLib_ud.lib")

class DuiwinWnd : public WindowImplBase
{
protected:
	virtual CDuiString GetSkinFolder()
	{
		return _T("");
	}

	virtual CDuiString GetSkinFile()
	{
		return _T("duilib.xml");
	}

	virtual LPCTSTR GetWindowClassName(void)const
	{
		return _T("DuiWnd");
	}

	virtual void Notify(TNotifyUI& msg)
	{
		if (msg.sType == _T("click"))
		{
			CDuiString strName = msg.pSender->GetName();
			if (strName == _T("btn_close"))
			{
				Close();
			}
			else if (strName == _T("btn_min"))
			{
				MessageBox(m_hWnd, _T("��ť����"), _T("test"), IDOK);
			}
			
			//MessageBox(m_hWnd, _T("��ť����"), _T("test"), IDOK);
		}
		else if (msg.sType == _T("itemselect"))
		{
			//MessageBox(NULL, _T("Combo"), _T("test"), IDOK);
			CComboBoxUI* pComboSelect = (CComboBoxUI*)m_PaintManager.FindControl(_T("combo_select"));
			int iSelect = pComboSelect->GetCurSel();
			if (0 == iSelect)
			{
				MessageBox(NULL, _T("ѡ��ͼƬ"), _T("test"), IDOK);
			}
			else
			{
				MessageBox(NULL, _T("ѡ����Ƶ"), _T("test"), IDOK);
			}
		}
	}
};

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int
	nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	// ������Դ��Ĭ��·�����˴�����Ϊ��exe��ͬһĿ¼��
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());

	DuiwinWnd Wnd;
	// Cashier���ڴ������Ͻ���ʽ������
	// UI_WNDSTYLE_FRAME: duilib��װ�ĺ꣬�����ڿ��ӣ����б������������С�����رչ��ܵ�
	// WS_EX_WINDOWEDGE: Win32�Ĵ��ڷ�񣬴��б߿�
	Wnd.Create(NULL, _T("DuilibTest"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);

	Wnd.CenterWindow();

	//��ʾ���ڣ�������Ϣѭ��
	Wnd.ShowModal();
	return 0;
}