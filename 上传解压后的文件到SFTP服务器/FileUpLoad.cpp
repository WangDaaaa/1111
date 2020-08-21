#include "FileUpLoad.h"

FileUpLoad::FileUpLoad()
{
	m_buffer_zip	= new  char[MAX_PATH];
	m_buffer_delete = new  char[MAX_PATH];
	m_buffer_xml	= new  char[MAX_PATH];
	m_buffer_in		= new  char[MAX_PATH];
/*-------------------------------------------��ȡ��ǰ·�� �洢��buffer��-------------------------------------------------*/
	GetCurrentPath();
/*-------------------------------------------��ȡ��ǰ·�� �洢��buffer��-------------------------------------------------*/

/*-------------------------------------------��XML�ļ��л�ȡ��������Ϣ----------------------------------------------------*/
	GetXMLInfo();
/*-------------------------------------------��XML�ļ��л�ȡ��������Ϣ----------------------------------------------------*/
	
}
FileUpLoad::~FileUpLoad()
{
	delete m_str_SourceAddress;
	delete m_buffer_zip;
	delete m_buffer_delete;
	delete m_buffer_xml;
	delete m_buffer_in;
	delete m_DeleteBuffer;
}

/*-------------------------------------------��ȡ��ǰ·��---------------------------------------------------------------*/
bool FileUpLoad::GetCurrentPath()
{
	if (nullptr == _getcwd(m_buffer_zip, MAX_PATH))
	{
		cout << "��ȡ��ǰ·��ʧ��..." << endl;
		return false;
	}

	if (nullptr == _getcwd(m_buffer_delete, MAX_PATH))
	{
		cout << "��ȡ��ǰ·��ʧ��..." << endl;
		return false;
	}

	if (nullptr == _getcwd(m_buffer_xml, MAX_PATH))
	{
		cout << "��ȡ��ǰ·��ʧ��..." << endl;
		return false;
	}
	return true;

}
/*-------------------------------------------��ȡ��ǰ·��---------------------------------------------------------------*/

/*-------------------------------------------��ȡ�����������zip�ļ���·��------------------------------------------------*/
void FileUpLoad::GetZipPath(char* strtmp)
{
	
	m_buffer_in = strtmp;
	int len = MultiByteToWideChar(CP_ACP, 0, m_buffer_in, strlen(m_buffer_in), NULL, 0);
	m_str_SourceAddress = new wchar_t[len + 2];
	MultiByteToWideChar(CP_ACP, 0, m_buffer_in, strlen(m_buffer_in), m_str_SourceAddress, len);
	m_str_SourceAddress[len] = '\0';

	// �����в���
	m_DeleteBuffer = new char[MAX_PATH];
	strcpy_s(m_DeleteBuffer, MAX_PATH, "rd /s/q ");
}
/*-------------------------------------------��ȡ�����������zip�ļ���·��------------------------------------------------*/

/*-------------------------------------------��ȡ�����ļ�XML�еķ�������Ϣ------------------------------------------------*/
void FileUpLoad::GetXMLInfo()
{
	//����XML�ļ�
	strcat_s(m_buffer_xml, MAX_PATH, "\\info.xml");

	rapidxml::file<> ffile(m_buffer_xml);

	xml_document<> doc;
	doc.parse<0>(ffile.data());
	// ��ȡ one  �ڵ�
	xml_node<>* one = doc.first_node();
	//��ȡ two  �ڵ�
	xml_node<>* two = one->first_node();
	// ��ȡ�����ڵ�
	xml_node<>* three = two->first_node();
	m_str_url = three->first_attribute()->value();
	m_str_url = m_str_url + "/";
	three = three->next_sibling();
	m_str_user = three->first_attribute()->value();
	three = three->next_sibling();
	m_str_password = three->first_attribute()->value();

	// ƴ���˺ź�����
	m_str_info = m_str_user + ":" + m_str_password;
}
/*-------------------------------------------��ȡ�����ļ�XML�еķ�������Ϣ------------------------------------------------*/

/*-------------------------------------------��ѹstr_SourceAddress�е�zip�ļ�-------------------------------------------*/
bool FileUpLoad::UncompressZipFile(const wchar_t* str_SourceAddress)
{
	// ��ǰ·�����½��������ڴ洢��ѹ�ļ����ļ���
	strcat_s(m_buffer_zip, MAX_PATH,"\\Temp\\");
	strcat_s(m_buffer_delete, MAX_PATH,"\\Temp");
	
	strcat_s(m_DeleteBuffer, MAX_PATH, m_buffer_delete);
	
	CString str_TargetAddress = m_buffer_zip;
	
	// �жϵ�ַ�Ƿ����
	if (0 == PathFileExistsW(str_SourceAddress))
	{
		// ������
		cout <<"·�������ڻ��ߴ��ڴ��� ����!"<< endl;
		return false;
	}

	// ��zip�ļ� 
	HZIP hzip = OpenZip(str_SourceAddress, NULL);
	if (nullptr == hzip)
	{
		cout << "open zip file failed " << str_SourceAddress << endl;
		return false;
	}

	// To zip����������ļ�����������������

	// ����zip�ļ���Ϣ��struct
	ZIPENTRY zipinfo;
	GetZipItem(hzip, -1, &zipinfo);
	int numitems = zipinfo.index;
	for (int i = 0; i < numitems; i++)
	{
		GetZipItem(hzip, i, &zipinfo);
		CString  strFile = str_TargetAddress + zipinfo.name;
		DWORD retCode = UnzipItem(hzip, i, strFile);
		if (ZR_OK != retCode)
		{
			cout << "unzip file:" << strFile.GetBuffer() << "failed, error code:" << retCode << endl;
			return 0;
		}
	}
	CloseZip(hzip);

	return true;
}
/*-------------------------------------------��ѹstr_SourceAddress�е�zip�ļ�-------------------------------------------*/

/*-------------------------------------------��ȡ��ѹ���������ļ���·��----------------------------------------------------*/
void FileUpLoad::GetFiles(string path, vector<string>& filepaths)
{
	intptr_t   hFile = 0;//�ļ�������������������
	struct _finddata_t fileinfo;//�ļ���Ϣ
	string p;
	if ((hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileinfo)) != -1)
		//������ҵ���һ���ļ�
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))//������ļ���
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					GetFiles(p.assign(path).append("/").append(fileinfo.name), filepaths);
			}
			else//������ļ�
			{
				filepaths.push_back(p.assign(path).append("/").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);	//��Ѱ�ҵ������ļ�

		_findclose(hFile);	//�������ң��رվ��
	}
}
/*-------------------------------------------��ȡ��ѹ���������ļ���·��----------------------------------------------------*/

/*-------------------------------------------�ص�����------------------------------------------------------------------*/
size_t read_callback(void* ptr, size_t size, size_t nmemb, void* stream)
{
	return fread(ptr, size, nmemb, (FILE*)stream);
}
/*-------------------------------------------�ص�����------------------------------------------------------------------*/

/*-------------------------------------------�ϴ������ļ���sftp������----------------------------------------------------*/
void FileUpLoad::UpLoadFuntion()
{
	

	// �õ���Ҫ�ϴ����ļ�·��
	GetFiles(m_buffer_delete, m_files);
	GetFiles(m_buffer_delete, m_temp);
	size_t nSize = strlen(m_buffer_delete) ;

	// �ض��ַ���
	for (auto ite = m_files.begin(); ite != m_files.end(); ite++)
	{
		(*ite).erase(0, nSize);
	}
	
	// ����Libcur(curl_global_init)
	if (0 != curl_global_init(CURL_GLOBAL_ALL))
	{
		cout << "curl_global_init failed..." << endl;
	}

	// ʵ�پ��(curl_easy_init) CURL = void 
	CURL* curlHandle = curl_easy_init();
	if (nullptr == curlHandle)
	{
		cout << "curl_easy_init failed..." << endl;
	}

	// �򿪱����ļ�
	cout << "please waiting... it is uploading..." << endl;

	// �ϴ������ļ�
	FILE* SendFile = nullptr;
	
	for (size_t i = 0; i < m_temp.size(); i++)
	{
		if (0 != fopen_s(&SendFile, m_temp[i].c_str(), "rb"))
		{
			cout << "open file failed..." << endl;
			return;
		}
		string str_temp = m_str_url + m_files[i];
		if (nullptr == SendFile)
		{
			return;
		}
		curl_easy_setopt(curlHandle, CURLOPT_READFUNCTION, read_callback);
		curl_easy_setopt(curlHandle, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curlHandle, CURLOPT_URL, str_temp.c_str());
		curl_easy_setopt(curlHandle, CURLOPT_READDATA, SendFile);
		curl_easy_setopt(curlHandle, CURLOPT_USERPWD, m_str_info.c_str());
		curl_easy_setopt(curlHandle, CURLOPT_FTP_CREATE_MISSING_DIRS, 1); 

		if (CURLE_OK != curl_easy_perform(curlHandle))
		{
			// ��ӡ������Ϣ
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(curl_easy_perform(curlHandle)));
			// ж��
			curl_easy_cleanup(curlHandle);
			curl_global_cleanup();
			fprintf(stderr, "finished update.");
			fclose(SendFile);
			return;
		}
		str_temp.clear();
		size_t pos = m_temp[i].rfind("/");
		if (pos == string::npos)
		{
			cout << "Ѱ��/ʧ��..." << endl;
		}
		str_temp = m_temp[i].substr(pos + 1);
		cout << to_string(i + 1) + "/" + to_string(m_temp.size()) <<"�ļ�: "<<str_temp << "  �����..." << endl;

		if (nullptr != SendFile)
		{
			fclose(SendFile);
		}
		str_temp.clear();
	}
	// ж��
	curl_easy_cleanup(curlHandle);
	curl_global_cleanup();
	
}
/*-------------------------------------------�ϴ������ļ���sftp������----------------------------------------------------*/

/*-------------------------------------------ɾ�����ɵ���ʱ�ļ�-----------------------------------------------------------*/
void FileUpLoad::DeleteFiles()
{
	system(m_DeleteBuffer);
}
/*-------------------------------------------ɾ�����ɵ���ʱ�ļ�-----------------------------------------------------------*/
