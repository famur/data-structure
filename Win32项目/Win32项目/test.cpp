#define _CRT_SECURE_NO_WARNINGS 1

#include<Windows.h>
#include<fstream>
#include"UIlib.h"
 
using namespace DuiLib; 

#pragma comment(lib,"DuiLib_ud.lib")

class CDuiFramWnd : public WindowImplBase//简单的通过xml文件描述窗口---窗口创建
	//类名WindowImplBase
	//重写三个虚函数
	//消息处理：
	//1.window系统消息---HandleMessage---cwindowwnd
	//2.duilib消息---duilib所维护的空间所产生的消息：Nofity（）：iNotifyUI
	//3.熟悉Designer工具
{
protected:
	virtual CDuiString GetSkinFolder()//获取皮肤
	{
		return _T("");
	}
	virtual CDuiString GetSkinFile()//获取xml文件名字
	{
		return _T("duilib.xml");
	}
	virtual LPCTSTR GetWindowClassName(void)const//窗口类的名字
	{
		return _T("CDuiFramWnd");
	}

	virtual void Notify(TNotifyUI& msg)
	{
		CDuiString strName = msg.pSender->GetName();
		if (msg.sType == _T("click"))//鼠标单击的消息
		{
			if (strName == _T("btn_close"))
			{
				Close();
			}

			else if (strName == _T("btn_min"))
			{
				MessageBox(NULL, _T("最小化"), _T("测试"), IDOK);

			}

			else if (strName == _T("btn_cut"))
			{
				//截取视频
				CutView();
			}

			else if (strName == _T("btn_getsrt"))
			{
				//获取字幕
				GetSRTFile();
				//界面显示
				LoadSRT();
			}

			else if (strName == _T("but_comnt"))
			{
				CEditUI* pEdit = (CEditUI*)m_PaintManager.FindControl(_T("edit_word"));
				CDuiString strWord = pEdit->GetText();
				//将该文本写回到List中
				//获取list控件
				CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("list_srt"));
				//获取当前list哪一项被选中
				CListTextElementUI* pListItemp = (CListTextElementUI*)pList->GetItemAt(pList->GetCurSel());
				pListItemp->SetText(1, strWord);
			}
			else if (strName == _T("btn_view"))
			{
				//获取视频流
				GenerateView();
			}

			else if (strName == _T("btn_wirtsrt"))
			{
				//写入srt
				WriteSRT();
			}

			else if (strName == _T("btn_bron"))
			{
				//写入视频流
				BornSRT2View();
			}

			else if (strName == _T("bin_load"))
			{
				//载入文件
				LoadFile();
			}
			else if (strName == _T("btn_genera"))
			{
				//根据CComboBox获取控件的name拿到控件
				//基类向子类进行转换
				CComboBoxUI* pCombo = (CComboBoxUI*)m_PaintManager.FindControl(_T("combo_select"));
				if (0 == pCombo->GetCurSel()) //选择是图片
				{
					GenerateGifWithPic();
				}
				//该函数执行后无问题但图片并未生成此时将strSEInfo.nShow = SW_HIDE;设置为显示
				//并将断点打在ShellExecuteEx(&strSEInfo);进行调试观察 cmd进入的路径是否为正确路径
				//将构造的命令输入cmd中查看是否生成GIF,若果错误则要观察命令
				else
				{
					GenerateGifWithView();
				}
			}
		}

		else if (msg.sType == _T("itemselect"))
		{
			//list和combo都可以响应
			if (strName == _T("list_srt"))
			{
				//获取list控件
				CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("list_srt"));
				//获取当前list哪一项被选中
				CListTextElementUI* pListItemp = ((CListTextElementUI*)pList->GetItemAt(pList->GetCurSel()));
				//将list选中的对应文本加载到右侧文本框中
				CEditUI* pEdit = ((CEditUI*)m_PaintManager.FindControl(_T("edit_word")));
				pEdit->SetText(pListItemp->GetText(1));
			}
			
		}
		//windowinit:在窗口初始化期间需要做的事情
		else if (msg.sType == _T("windowinit"))
		{
			SetControlEnable(false);
		}
		else if (strName == _T("combo_select"))
			{
				CComboBoxUI* pComboUI = (CComboBoxUI*)m_PaintManager.FindControl(_T("combo_select"));
				int select = pComboUI->GetCurSel();
				if (0 == select)
				{
					SetControlEnable(false);
				}
				else
				{
					SetControlEnable(true);
				}
		}
	}

	void SetControlEnable(bool IsValid)
	{
		((CEditUI*)m_PaintManager.FindControl(_T("edit_start")))->SetEnabled(IsValid);
		((CEditUI*)m_PaintManager.FindControl(_T("edit_end")))->SetEnabled(IsValid);
		((CButtonUI*)m_PaintManager.FindControl(_T("btn_cut")))->SetEnabled(IsValid);
		((CButtonUI*)m_PaintManager.FindControl(_T("btn_getsrt")))->SetEnabled(IsValid);
		((CButtonUI*)m_PaintManager.FindControl(_T("btn_wirtsrt")))->SetEnabled(IsValid);
		((CButtonUI*)m_PaintManager.FindControl(_T("btn_view")))->SetEnabled(IsValid);
		((CButtonUI*)m_PaintManager.FindControl(_T("btn_bron")))->SetEnabled(IsValid);
		((CButtonUI*)m_PaintManager.FindControl(_T("btn_genera")))->SetEnabled(true);
		((CEditUI*)m_PaintManager.FindControl(_T("edit_word")))->SetEnabled(IsValid);
		((CButtonUI*)m_PaintManager.FindControl(_T("but_comnt")))->SetEnabled(IsValid);
		((CEditUI*)m_PaintManager.FindControl(_T("edit_path")))->SetEnabled(IsValid);
		((CButtonUI*)m_PaintManager.FindControl(_T("bin_load")))->SetEnabled(IsValid);
	}

	void SendMessage(CDuiString& strCMD)//调用命令行窗口，执行用户命令（在该函数中会新建一个进程，来负责调用命令行窗口执行命令）
	{
		//1.初始化结构体
		SHELLEXECUTEINFO strSEInfo;
		memset(&strSEInfo, 0, sizeof(SHELLEXECUTEINFO)); //将结构体中的所有字段设置为0；

		strSEInfo.cbSize = sizeof(SHELLEXECUTEINFO); //（存储该结构的长度，以字节为单位）获取结构体大小

		strSEInfo.fMask = SEE_MASK_NOCLOSEPROCESS;  //（SEE_MASK_NOCLOSEPROCESS 若没有新的进程启动，该项的值仍为null） （一个标志数组，用来设置其他成员的有效性。）
		strSEInfo.lpFile = _T("C:\\Windows\\System32\\cmd.exe");//命令行（cmd）工具的路径
		strSEInfo.lpParameters = strCMD;//第一个参数必须是 /c 接ffmpeg的命令（运行/打开程序的参数，如果打开的是一个文档，则该项无效）
		strSEInfo.nShow = SW_HIDE; //（说明ShellExecuteEx打开的程序将以什么形式出现）隐藏

		//2.给命令行（cmd）发消息(问题：首先要先进入 E:...目录下面，不然命令发不出去)
		ShellExecuteEx(&strSEInfo); //创建进程给控制台发送消息
		//等待命令响应完成

		WaitForSingleObject(strSEInfo.hProcess, INFINITE);
		MessageBox(NULL, _T("命令操作完成"), _T("MakeGif"), IDOK);
	}



	//在这里遇到了一个问题：打不上断点（当前不会命中断点，还没有为该文档加载任何符号）其实主要是没有生成调试信息：
	//解决方法：（1）项目-属性-配置属性-c/c++ - 常规-调试信息格式  这里不能设置为“禁用”
	//（2）项目-属性-配置属性-连接器-调试-生成调试信息 这里设置为“是”


	void GenerateGifWithPic()//通过图片生成GIF图
	{
		//生成GIF的命令 ffmpeg - r 3 - i Pictrue\\%d.jpg output.gif -y
		//-r控制帧数 （快慢的速度）-i后面的输入 Pictrue\\%d.jpg图片的路径  output.gif生成图片的路径  -y如果存在图片覆盖
		//1.构造命令
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//获取路径
		strPath += _T("ffmpeg\\");//进入ffmpeg所在的路径
		CDuiString strCMD;
		strCMD += _T("/c cd ");
		strCMD += CPaintManagerUI::GetInstancePath() + _T("ffmpeg");
		strCMD += _T(" & ");
		strCMD += _T("ffmpeg -r 3 -i Pictrue\\%d.jpg output.gif -y");
		//2.给cmd发送消息
		SendMessage(strCMD);
		//1.构造命令
		//CDuiString strCMD;
		//strCMD += _T("/c ");
		//strCMD += strPath;
		//strCMD += _T("ffmpeg -r 3 -i ");
		//strCMD += strPath;  //图片所在的路径
		//strCMD += _T(".\\Pictrue\\%d.jpg ");
		//strCMD += strPath;  //生成动态图的路径
		//strCMD += _T("output.gif -y");  //-y：如果该目录下存在该图片，就覆盖掉该图片
		//2.给cmd发命令
		//在这里又遇到了些问题：命令发布出去
		//在网上查看了关于ShellExecuteEx的资料，发现ShellExecuteEx在执行cmd命令的时候
		//命令行要加入 /c  来让命令完成后关闭，否则命令就一直存在。
		//SendMessage(strCMD);
	}

	void CutView()
	{
		//1.构造命令
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//获取路径
		strPath += _T("ffmpeg\\");//进入ffmpeg所在的路径
		CDuiString strViewPath = ((CEditUI*)m_PaintManager.FindControl(_T("edit_path")))->GetText();
		CDuiString strCMD;
		strCMD += _T("/c cd ");
		strCMD += CPaintManagerUI::GetInstancePath() + _T("ffmpeg");
		strCMD += _T(" & ");
		//视频的路径
		//优先：通过界面方式加载
		//如果界面没有配置，再到ffmpeg目录下获取默认文件
		if (!strViewPath.IsEmpty())
		{
			strCMD += _T("ffmpeg  -i ");
			strCMD += strViewPath;
			strCMD += _T(" -vcodec copy -acodec copy -ss ");
		}
		else
		{
			strCMD += _T("ffmpeg  -i ./00.mp4 -vcodec copy -acodec copy -ss ");
		}
		//ffmpeg - i input.mkv - vcodec copy - acodec copy - ss 00:40 : 07 - to 00 : 40 : 28 11.mkv - y - i -vcodec copy -acodec copy -ss 00:00:10 -to 00:00:15 ./cutout1.mp4 -y
		//strCMD += _T("ffmpeg  -i ./input.mp4");
		//strCMD += _T("ffmpeg  -i ./00.mp4 -vcodec copy -acodec copy -ss ");
		
		////获取起始时间和结尾时间
		CDuiString strStartTime=((CEditUI*)m_PaintManager.FindControl(_T("edit_start")))->GetText();
		if (!IsValidTime(strStartTime))
		{
			MessageBox(NULL, _T("起始时间有误"), _T("MakeGif"), IDOK);
			return;
		}
		CDuiString strEndTime = ((CEditUI*)m_PaintManager.FindControl(_T("edit_end")))->GetText();
		if (!IsValidTime(strEndTime))
		{
			MessageBox(NULL, _T("终止时间有误"), _T("MakeGif"), IDOK);
			return; 
		}
		if (strEndTime <= strStartTime)
		{
			MessageBox(NULL, _T("时间输入有误!"), _T("wrong"), IDOK);
			return;
		}
		strCMD += strStartTime;
		strCMD += _T(" -to ");
		strCMD += strEndTime;
		strCMD += _T(" ");
		////输出文件的路径
		//strCMD += strPath;
		strCMD += _T("./11.mp4 -y");
		//2.给cmd发送消息
		//2.执行命令
		SendMessage(strCMD);	
	}

	void LoadFile()
	{
		OPENFILENAME ofn;
		memset(&ofn, 0, sizeof(OPENFILENAME));

		TCHAR strPath[MAX_PATH] = { 0 };
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.lpstrFile = strPath;
		ofn.nMaxFile = sizeof(strPath);
		ofn.lpstrCustomFilter = _T("All(*.*)\0 *.*\0mp4(*.mp4)\0 * .mp4\0;");
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileName(&ofn))
		{
			//将文件路径设置到edit
			((CEditUI*)m_PaintManager.FindControl(_T("edit_path")))->SetText(strPath);
			MessageBox(NULL, _T("test"), _T("gif"), IDOK);
		}
	}
	void GetSRTFile()
	{
		//ffmpeg - i 11.mkv input.srt - y
		//1.构造命令
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//获取路径
		strPath += _T("ffmpeg\\");//进入ffmpeg所在的路径
		CDuiString strCMD;
		strCMD += _T("/c cd ");
		strCMD += CPaintManagerUI::GetInstancePath() + _T("ffmpeg");
		strCMD += _T(" & ");
		strCMD += _T("ffmpeg -i ./11.mp4 input.srt -y");
		//2.给cmd发送消息
		SendMessage(strCMD);
	}

	void LoadSRT()
	{
		//将srt格式的字幕文件，加载到界面中的list控件中
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//获取路径
		strPath += _T("ffmpeg\\input.srt");//进入ffmpeg所在的路径
		std::ifstream fIn(strPath.GetData());

		char strSRTCon[521] = { 0 };
		CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("list_srt"));

		pList->RemoveAll();
		while (!fIn.eof())
		{
			//读取字幕序号
			fIn.getline(strSRTCon, 512);
			CListTextElementUI* pListItem = new CListTextElementUI;
			pList->Add(pListItem);

			//读取时间
			fIn.getline(strSRTCon, 512);
			pListItem->SetText(0, UTF8ToUnicode(strSRTCon));

			//读取字幕
			fIn.getline(strSRTCon, 512);
			pListItem->SetText(1, UTF8ToUnicode(strSRTCon));

			//读取空行
			fIn.getline(strSRTCon, 512);
		}
		pList = NULL;
		fIn.close();

	}

	void WriteSRT()
	{
		//获取STR文件的路径
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//获取路径
		strPath += _T("ffmpeg\\input.srt");//进入ffmpeg所在的路径

		std::ofstream fOut(strPath.GetData());
		//1.从List控件中获取文本内容
		CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("list_srt"));
		int szCount = pList->GetCount();

		for (int i = 0; i < szCount; ++i)
		{
			CListTextElementUI* pListItem = (CListTextElementUI*)pList->GetItemAt(i);
			//序号
			CDuiString strNo;
			strNo.Format(_T("%d"), i + 1);
			//时间轴
			CDuiString strTime = pListItem->GetText(0);
			//文本内容
			CDuiString strWord = pListItem->GetText(1);
			//2.将获取到的内容写回到srt文件中

			string strNewLine = Unicode2ANSI(_T("\n"));

			//写行号
			string itemNo = Unicode2ANSI(strNo);
			fOut.write(itemNo.c_str(), itemNo.size());//需要转换格式
			fOut.write(strNewLine.c_str(), strNewLine.size());

			//时间轴
			string itemTime = Unicode2ANSI(strTime);
			fOut.write(itemTime.c_str(), itemTime.size());
			fOut.write(strNewLine.c_str(), strNewLine.size());

			//文本内容
			string itemWord = Unicode2ANSI(strWord);
			fOut.write(itemWord.c_str(), itemWord.size());
			fOut.write(strNewLine.c_str(), strNewLine.size());

			//字幕和字幕之间有换行
			fOut.write(strNewLine.c_str(), strNewLine.size());
		}
		//2.将获取到的内容写回到str文件中
		fOut.close();
	}
	void GenerateView()
	{
		//ffmpeg - i 11.mkv - vcodec copy - an - sn 22.mkv - y
		//1.构造命令
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//获取路径
		strPath += _T("ffmpeg\\");//进入ffmpeg所在的路径
		CDuiString strCMD;
		strCMD += _T("/c cd ");
		strCMD += CPaintManagerUI::GetInstancePath() + _T("ffmpeg");
		strCMD += _T(" & ");
		strCMD += _T("ffmpeg -i 11.mp4 -vcodec copy -an -sn 22.mp4 -y");
		//2.给cmd发送消息
		SendMessage(strCMD);
	}

	void BornSRT2View()
	{
		//ffmpeg - i 22.mkv - vf subtitles = 11.srt 33.mkv - y
		//1.构造命令
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//获取路径
		strPath += _T("ffmpeg\\");//进入ffmpeg所在的路径
		CDuiString strCMD;
		strCMD += _T("/c cd ");
		strCMD += CPaintManagerUI::GetInstancePath() + _T("ffmpeg");
		strCMD += _T(" & ");
		strCMD += _T("ffmpeg -i 22.mp4 -vf subtitles=11.srt 33.mp4 -y");
		//2.给cmd发送消息
		SendMessage(strCMD);
	}

	void GenerateGifWithView()
	{
		//ffmpeg - i 33.mkv - vf scale = iw / 2:ih / 2 - f gif output.gif - y
		//1.构造命令
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//获取路径
		strPath += _T("ffmpeg\\");//进入ffmpeg所在的路径
		CDuiString strCMD;
		strCMD += _T("/c cd ");
		strCMD += CPaintManagerUI::GetInstancePath() + _T("ffmpeg");
		strCMD += _T(" & ");
		strCMD += _T("ffmpeg -i 33.mkv -vf scale=iw/2:ih/2 -f gif output.gif -y");
		//2.给cmd发送消息
		SendMessage(strCMD);
	}
	CDuiString UTF8ToUnicode(const char* str)
	{
		//获取转换以后目标的长度
		int szLen = ::MultiByteToWideChar(CP_UTF8, 0, str,strlen(str), NULL, 0);
		wchar_t* pContent = new wchar_t[szLen + 1];
		
		//真正的转换
		::MultiByteToWideChar(CP_UTF8, 0, str, strlen(str), pContent, szLen);
		pContent[szLen] = '\0';

		CDuiString s(pContent);
		delete[] pContent;
		return s;
	}

	string Unicode2ANSI(CDuiString str) {
		int len;
		len = WideCharToMultiByte(CP_ACP, 0, str.GetData(), -1, NULL, 0, NULL, NULL);
		char *szUtf8 = (char*)malloc(len + 1);
		memset(szUtf8, 0, len + 1);
		WideCharToMultiByte(CP_ACP, 0, str.GetData(), -1, szUtf8, len, NULL, NULL);
		string s(szUtf8);
		free(szUtf8);
		return s;
	}

	bool IsValidTime(CDuiString strTime)
	{
		//8个字符
		if (strTime.GetLength() != 8)
		{
			return false;
		}
		// :  数字类型的字符
		for (int i = 0; i < strTime.GetLength(); ++i)
		{
			if (strTime[i] == ':')
				continue;
			if (!(strTime[i] >= '0' && strTime[i] <= '9'))
			{
				return false;
			}
		}
		return true;
	}
	/*string Unicode2ANSI(CDuiString str)
	{
		int szLen = ::WideCharToMultiByte(CP_ACP  , 0, str.GetData(), -1, NULL, 0, NULL, FALSE);
		char* pBuff = new char[szLen + 1];
		::WideCharToMultiByte(CP_ACP, 0, str.GetData(), str.GetLength(), pBuff, szLen, NULL, FALSE);
		pBuff[szLen] = '\0';
		string s(pBuff);
		delete[] pBuff;
		return s;
	}*/
};

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int
	nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	// 设置资源的默认路径（此处设置为和exe在同一目录）
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());

	CDuiFramWnd framWnd;

	// Cashier即在窗口右上角显式的名字
	// UI_WNDSTYLE_FRAME: duilib封装的宏，代表窗口可视，具有标题栏，最大化最小化，关闭功能等
	// WS_EX_WINDOWEDGE: Win32的窗口风格，带有边框

	framWnd.Create(NULL, _T("DuilibTest"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	framWnd.CenterWindow();
	//             句柄      窗口名字        窗口样子            
	//            显示窗口，激活消息循环
	framWnd.CenterWindow();
	framWnd.ShowModal();//消息循环
	//system("pause");
	return 0;
}