#pragma once

#include <curl/curl.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include "unzip.h"
#include <shlwapi.h>	
#include <atlstr.h>	  
#include <vector>
#include <io.h>		 
#include <direct.h>   
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"

using namespace std;
using namespace rapidxml;

class FileUpLoad
{
public:
	FileUpLoad();
	~FileUpLoad();
public:
	string m_str_url;						// sftp ��ַ
	string m_str_user;						// sftp �û���
	string m_str_password;					// sftp ����
	string m_str_info;						// ƴ��user��password

	wchar_t* m_str_SourceAddress;			// ��������zip�ļ���·��
	char   * m_buffer_zip;					// �洢��ǰ·����Ϣ ���ڲ��ҽ�ѹ��������ļ�
	char   * m_buffer_delete;				// �洢��ǰ·����Ϣ ����ɾ����ʱ�ļ�
	char   * m_buffer_xml;					// �洢��ǰ·����Ϣ ���ڲ���xml�����ļ�
	char   * m_buffer_in;					// ��ʱ��������·��
	char   * m_DeleteBuffer;				// ���ɾ������

	vector<string> m_files;					// �洢�ļ�·������  files�洢������·�� 
	vector<string> m_temp;					// �洢�ļ�·������  temp�洢�����ļ�·��

public:

	bool GetCurrentPath();
	void GetZipPath(char* strtmp);
	void GetXMLInfo();
	bool UncompressZipFile(const wchar_t* str_SourceAddress);
	void GetFiles(string path, vector<string>& files);
	void UpLoadFuntion();
	void DeleteFiles();
};

