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
				MessageBox(m_hWnd, _T("按钮单击"), _T("test"), IDOK);
			}
			
			//MessageBox(m_hWnd, _T("按钮单击"), _T("test"), IDOK);
		}
		else if (msg.sType == _T("itemselect"))
		{
			//MessageBox(NULL, _T("Combo"), _T("test"), IDOK);
			CComboBoxUI* pComboSelect = (CComboBoxUI*)m_PaintManager.FindControl(_T("combo_select"));
			int iSelect = pComboSelect->GetCurSel();
			if (0 == iSelect)
			{
				MessageBox(NULL, _T("选择图片"), _T("test"), IDOK);
			}
			else
			{
				MessageBox(NULL, _T("选择视频"), _T("test"), IDOK);
			}
		}
	}
};

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int
	nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	// 设置资源的默认路径（此处设置为和exe在同一目录）
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());

	DuiwinWnd Wnd;
	// Cashier即在窗口右上角显式的名字
	// UI_WNDSTYLE_FRAME: duilib封装的宏，代表窗口可视，具有标题栏，最大化最小化，关闭功能等
	// WS_EX_WINDOWEDGE: Win32的窗口风格，带有边框
	Wnd.Create(NULL, _T("DuilibTest"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);

	Wnd.CenterWindow();

	//显示窗口，激活消息循环
	Wnd.ShowModal();
	return 0;
}