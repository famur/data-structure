#define _CRT_SECURE_NO_WARNINGS 1

#include<Windows.h>
#include<fstream>
#include"UIlib.h"
 
using namespace DuiLib; 

#pragma comment(lib,"DuiLib_ud.lib")

class CDuiFramWnd : public WindowImplBase//�򵥵�ͨ��xml�ļ���������---���ڴ���
	//����WindowImplBase
	//��д�����麯��
	//��Ϣ����
	//1.windowϵͳ��Ϣ---HandleMessage---cwindowwnd
	//2.duilib��Ϣ---duilib��ά���Ŀռ�����������Ϣ��Nofity������iNotifyUI
	//3.��ϤDesigner����
{
protected:
	virtual CDuiString GetSkinFolder()//��ȡƤ��
	{
		return _T("");
	}
	virtual CDuiString GetSkinFile()//��ȡxml�ļ�����
	{
		return _T("duilib.xml");
	}
	virtual LPCTSTR GetWindowClassName(void)const//�����������
	{
		return _T("CDuiFramWnd");
	}

	virtual void Notify(TNotifyUI& msg)
	{
		CDuiString strName = msg.pSender->GetName();
		if (msg.sType == _T("click"))//��굥������Ϣ
		{
			if (strName == _T("btn_close"))
			{
				Close();
			}

			else if (strName == _T("btn_min"))
			{
				MessageBox(NULL, _T("��С��"), _T("����"), IDOK);

			}

			else if (strName == _T("btn_cut"))
			{
				//��ȡ��Ƶ
				CutView();
			}

			else if (strName == _T("btn_getsrt"))
			{
				//��ȡ��Ļ
				GetSRTFile();
				//������ʾ
				LoadSRT();
			}

			else if (strName == _T("but_comnt"))
			{
				CEditUI* pEdit = (CEditUI*)m_PaintManager.FindControl(_T("edit_word"));
				CDuiString strWord = pEdit->GetText();
				//�����ı�д�ص�List��
				//��ȡlist�ؼ�
				CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("list_srt"));
				//��ȡ��ǰlist��һ�ѡ��
				CListTextElementUI* pListItemp = (CListTextElementUI*)pList->GetItemAt(pList->GetCurSel());
				pListItemp->SetText(1, strWord);
			}
			else if (strName == _T("btn_view"))
			{
				//��ȡ��Ƶ��
				GenerateView();
			}

			else if (strName == _T("btn_wirtsrt"))
			{
				//д��srt
				WriteSRT();
			}

			else if (strName == _T("btn_bron"))
			{
				//д����Ƶ��
				BornSRT2View();
			}

			else if (strName == _T("bin_load"))
			{
				//�����ļ�
				LoadFile();
			}
			else if (strName == _T("btn_genera"))
			{
				//����CComboBox��ȡ�ؼ���name�õ��ؼ�
				//�������������ת��
				CComboBoxUI* pCombo = (CComboBoxUI*)m_PaintManager.FindControl(_T("combo_select"));
				if (0 == pCombo->GetCurSel()) //ѡ����ͼƬ
				{
					GenerateGifWithPic();
				}
				//�ú���ִ�к������⵫ͼƬ��δ���ɴ�ʱ��strSEInfo.nShow = SW_HIDE;����Ϊ��ʾ
				//�����ϵ����ShellExecuteEx(&strSEInfo);���е��Թ۲� cmd�����·���Ƿ�Ϊ��ȷ·��
				//���������������cmd�в鿴�Ƿ�����GIF,����������Ҫ�۲�����
				else
				{
					GenerateGifWithView();
				}
			}
		}

		else if (msg.sType == _T("itemselect"))
		{
			//list��combo��������Ӧ
			if (strName == _T("list_srt"))
			{
				//��ȡlist�ؼ�
				CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("list_srt"));
				//��ȡ��ǰlist��һ�ѡ��
				CListTextElementUI* pListItemp = ((CListTextElementUI*)pList->GetItemAt(pList->GetCurSel()));
				//��listѡ�еĶ�Ӧ�ı����ص��Ҳ��ı�����
				CEditUI* pEdit = ((CEditUI*)m_PaintManager.FindControl(_T("edit_word")));
				pEdit->SetText(pListItemp->GetText(1));
			}
			
		}
		//windowinit:�ڴ��ڳ�ʼ���ڼ���Ҫ��������
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

	void SendMessage(CDuiString& strCMD)//���������д��ڣ�ִ���û�����ڸú����л��½�һ�����̣���������������д���ִ�����
	{
		//1.��ʼ���ṹ��
		SHELLEXECUTEINFO strSEInfo;
		memset(&strSEInfo, 0, sizeof(SHELLEXECUTEINFO)); //���ṹ���е������ֶ�����Ϊ0��

		strSEInfo.cbSize = sizeof(SHELLEXECUTEINFO); //���洢�ýṹ�ĳ��ȣ����ֽ�Ϊ��λ����ȡ�ṹ���С

		strSEInfo.fMask = SEE_MASK_NOCLOSEPROCESS;  //��SEE_MASK_NOCLOSEPROCESS ��û���µĽ��������������ֵ��Ϊnull�� ��һ����־���飬��������������Ա����Ч�ԡ���
		strSEInfo.lpFile = _T("C:\\Windows\\System32\\cmd.exe");//�����У�cmd�����ߵ�·��
		strSEInfo.lpParameters = strCMD;//��һ������������ /c ��ffmpeg���������/�򿪳���Ĳ���������򿪵���һ���ĵ����������Ч��
		strSEInfo.nShow = SW_HIDE; //��˵��ShellExecuteEx�򿪵ĳ�����ʲô��ʽ���֣�����

		//2.�������У�cmd������Ϣ(���⣺����Ҫ�Ƚ��� E:...Ŀ¼���棬��Ȼ�������ȥ)
		ShellExecuteEx(&strSEInfo); //�������̸�����̨������Ϣ
		//�ȴ�������Ӧ���

		WaitForSingleObject(strSEInfo.hProcess, INFINITE);
		MessageBox(NULL, _T("����������"), _T("MakeGif"), IDOK);
	}



	//������������һ�����⣺���϶ϵ㣨��ǰ�������жϵ㣬��û��Ϊ���ĵ������κη��ţ���ʵ��Ҫ��û�����ɵ�����Ϣ��
	//�����������1����Ŀ-����-��������-c/c++ - ����-������Ϣ��ʽ  ���ﲻ������Ϊ�����á�
	//��2����Ŀ-����-��������-������-����-���ɵ�����Ϣ ��������Ϊ���ǡ�


	void GenerateGifWithPic()//ͨ��ͼƬ����GIFͼ
	{
		//����GIF������ ffmpeg - r 3 - i Pictrue\\%d.jpg output.gif -y
		//-r����֡�� ���������ٶȣ�-i��������� Pictrue\\%d.jpgͼƬ��·��  output.gif����ͼƬ��·��  -y�������ͼƬ����
		//1.��������
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//��ȡ·��
		strPath += _T("ffmpeg\\");//����ffmpeg���ڵ�·��
		CDuiString strCMD;
		strCMD += _T("/c cd ");
		strCMD += CPaintManagerUI::GetInstancePath() + _T("ffmpeg");
		strCMD += _T(" & ");
		strCMD += _T("ffmpeg -r 3 -i Pictrue\\%d.jpg output.gif -y");
		//2.��cmd������Ϣ
		SendMessage(strCMD);
		//1.��������
		//CDuiString strCMD;
		//strCMD += _T("/c ");
		//strCMD += strPath;
		//strCMD += _T("ffmpeg -r 3 -i ");
		//strCMD += strPath;  //ͼƬ���ڵ�·��
		//strCMD += _T(".\\Pictrue\\%d.jpg ");
		//strCMD += strPath;  //���ɶ�̬ͼ��·��
		//strCMD += _T("output.gif -y");  //-y�������Ŀ¼�´��ڸ�ͼƬ���͸��ǵ���ͼƬ
		//2.��cmd������
		//��������������Щ���⣺�������ȥ
		//�����ϲ鿴�˹���ShellExecuteEx�����ϣ�����ShellExecuteEx��ִ��cmd�����ʱ��
		//������Ҫ���� /c  ����������ɺ�رգ����������һֱ���ڡ�
		//SendMessage(strCMD);
	}

	void CutView()
	{
		//1.��������
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//��ȡ·��
		strPath += _T("ffmpeg\\");//����ffmpeg���ڵ�·��
		CDuiString strViewPath = ((CEditUI*)m_PaintManager.FindControl(_T("edit_path")))->GetText();
		CDuiString strCMD;
		strCMD += _T("/c cd ");
		strCMD += CPaintManagerUI::GetInstancePath() + _T("ffmpeg");
		strCMD += _T(" & ");
		//��Ƶ��·��
		//���ȣ�ͨ�����淽ʽ����
		//�������û�����ã��ٵ�ffmpegĿ¼�»�ȡĬ���ļ�
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
		
		////��ȡ��ʼʱ��ͽ�βʱ��
		CDuiString strStartTime=((CEditUI*)m_PaintManager.FindControl(_T("edit_start")))->GetText();
		if (!IsValidTime(strStartTime))
		{
			MessageBox(NULL, _T("��ʼʱ������"), _T("MakeGif"), IDOK);
			return;
		}
		CDuiString strEndTime = ((CEditUI*)m_PaintManager.FindControl(_T("edit_end")))->GetText();
		if (!IsValidTime(strEndTime))
		{
			MessageBox(NULL, _T("��ֹʱ������"), _T("MakeGif"), IDOK);
			return; 
		}
		if (strEndTime <= strStartTime)
		{
			MessageBox(NULL, _T("ʱ����������!"), _T("wrong"), IDOK);
			return;
		}
		strCMD += strStartTime;
		strCMD += _T(" -to ");
		strCMD += strEndTime;
		strCMD += _T(" ");
		////����ļ���·��
		//strCMD += strPath;
		strCMD += _T("./11.mp4 -y");
		//2.��cmd������Ϣ
		//2.ִ������
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
			//���ļ�·�����õ�edit
			((CEditUI*)m_PaintManager.FindControl(_T("edit_path")))->SetText(strPath);
			MessageBox(NULL, _T("test"), _T("gif"), IDOK);
		}
	}
	void GetSRTFile()
	{
		//ffmpeg - i 11.mkv input.srt - y
		//1.��������
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//��ȡ·��
		strPath += _T("ffmpeg\\");//����ffmpeg���ڵ�·��
		CDuiString strCMD;
		strCMD += _T("/c cd ");
		strCMD += CPaintManagerUI::GetInstancePath() + _T("ffmpeg");
		strCMD += _T(" & ");
		strCMD += _T("ffmpeg -i ./11.mp4 input.srt -y");
		//2.��cmd������Ϣ
		SendMessage(strCMD);
	}

	void LoadSRT()
	{
		//��srt��ʽ����Ļ�ļ������ص������е�list�ؼ���
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//��ȡ·��
		strPath += _T("ffmpeg\\input.srt");//����ffmpeg���ڵ�·��
		std::ifstream fIn(strPath.GetData());

		char strSRTCon[521] = { 0 };
		CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("list_srt"));

		pList->RemoveAll();
		while (!fIn.eof())
		{
			//��ȡ��Ļ���
			fIn.getline(strSRTCon, 512);
			CListTextElementUI* pListItem = new CListTextElementUI;
			pList->Add(pListItem);

			//��ȡʱ��
			fIn.getline(strSRTCon, 512);
			pListItem->SetText(0, UTF8ToUnicode(strSRTCon));

			//��ȡ��Ļ
			fIn.getline(strSRTCon, 512);
			pListItem->SetText(1, UTF8ToUnicode(strSRTCon));

			//��ȡ����
			fIn.getline(strSRTCon, 512);
		}
		pList = NULL;
		fIn.close();

	}

	void WriteSRT()
	{
		//��ȡSTR�ļ���·��
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//��ȡ·��
		strPath += _T("ffmpeg\\input.srt");//����ffmpeg���ڵ�·��

		std::ofstream fOut(strPath.GetData());
		//1.��List�ؼ��л�ȡ�ı�����
		CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("list_srt"));
		int szCount = pList->GetCount();

		for (int i = 0; i < szCount; ++i)
		{
			CListTextElementUI* pListItem = (CListTextElementUI*)pList->GetItemAt(i);
			//���
			CDuiString strNo;
			strNo.Format(_T("%d"), i + 1);
			//ʱ����
			CDuiString strTime = pListItem->GetText(0);
			//�ı�����
			CDuiString strWord = pListItem->GetText(1);
			//2.����ȡ��������д�ص�srt�ļ���

			string strNewLine = Unicode2ANSI(_T("\n"));

			//д�к�
			string itemNo = Unicode2ANSI(strNo);
			fOut.write(itemNo.c_str(), itemNo.size());//��Ҫת����ʽ
			fOut.write(strNewLine.c_str(), strNewLine.size());

			//ʱ����
			string itemTime = Unicode2ANSI(strTime);
			fOut.write(itemTime.c_str(), itemTime.size());
			fOut.write(strNewLine.c_str(), strNewLine.size());

			//�ı�����
			string itemWord = Unicode2ANSI(strWord);
			fOut.write(itemWord.c_str(), itemWord.size());
			fOut.write(strNewLine.c_str(), strNewLine.size());

			//��Ļ����Ļ֮���л���
			fOut.write(strNewLine.c_str(), strNewLine.size());
		}
		//2.����ȡ��������д�ص�str�ļ���
		fOut.close();
	}
	void GenerateView()
	{
		//ffmpeg - i 11.mkv - vcodec copy - an - sn 22.mkv - y
		//1.��������
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//��ȡ·��
		strPath += _T("ffmpeg\\");//����ffmpeg���ڵ�·��
		CDuiString strCMD;
		strCMD += _T("/c cd ");
		strCMD += CPaintManagerUI::GetInstancePath() + _T("ffmpeg");
		strCMD += _T(" & ");
		strCMD += _T("ffmpeg -i 11.mp4 -vcodec copy -an -sn 22.mp4 -y");
		//2.��cmd������Ϣ
		SendMessage(strCMD);
	}

	void BornSRT2View()
	{
		//ffmpeg - i 22.mkv - vf subtitles = 11.srt 33.mkv - y
		//1.��������
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//��ȡ·��
		strPath += _T("ffmpeg\\");//����ffmpeg���ڵ�·��
		CDuiString strCMD;
		strCMD += _T("/c cd ");
		strCMD += CPaintManagerUI::GetInstancePath() + _T("ffmpeg");
		strCMD += _T(" & ");
		strCMD += _T("ffmpeg -i 22.mp4 -vf subtitles=11.srt 33.mp4 -y");
		//2.��cmd������Ϣ
		SendMessage(strCMD);
	}

	void GenerateGifWithView()
	{
		//ffmpeg - i 33.mkv - vf scale = iw / 2:ih / 2 - f gif output.gif - y
		//1.��������
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//��ȡ·��
		strPath += _T("ffmpeg\\");//����ffmpeg���ڵ�·��
		CDuiString strCMD;
		strCMD += _T("/c cd ");
		strCMD += CPaintManagerUI::GetInstancePath() + _T("ffmpeg");
		strCMD += _T(" & ");
		strCMD += _T("ffmpeg -i 33.mkv -vf scale=iw/2:ih/2 -f gif output.gif -y");
		//2.��cmd������Ϣ
		SendMessage(strCMD);
	}
	CDuiString UTF8ToUnicode(const char* str)
	{
		//��ȡת���Ժ�Ŀ��ĳ���
		int szLen = ::MultiByteToWideChar(CP_UTF8, 0, str,strlen(str), NULL, 0);
		wchar_t* pContent = new wchar_t[szLen + 1];
		
		//������ת��
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
		//8���ַ�
		if (strTime.GetLength() != 8)
		{
			return false;
		}
		// :  �������͵��ַ�
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
	// ������Դ��Ĭ��·�����˴�����Ϊ��exe��ͬһĿ¼��
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());

	CDuiFramWnd framWnd;

	// Cashier���ڴ������Ͻ���ʽ������
	// UI_WNDSTYLE_FRAME: duilib��װ�ĺ꣬�����ڿ��ӣ����б������������С�����رչ��ܵ�
	// WS_EX_WINDOWEDGE: Win32�Ĵ��ڷ�񣬴��б߿�

	framWnd.Create(NULL, _T("DuilibTest"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	framWnd.CenterWindow();
	//             ���      ��������        ��������            
	//            ��ʾ���ڣ�������Ϣѭ��
	framWnd.CenterWindow();
	framWnd.ShowModal();//��Ϣѭ��
	//system("pause");
	return 0;
}