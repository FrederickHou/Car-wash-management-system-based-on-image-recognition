#pragma once
#import "E:\C\2\Server\Server\msado15.dll" no_namespace rename("EOF","adoEOF")rename("BOF","adoBOF")
class AdoSql
{
public:
	AdoSql();
	~AdoSql();
	_ConnectionPtr   m_pConnection; // ���ݿ�
	_RecordsetPtr    m_pRecordset; // ���� 
	_CommandPtr      m_pCommand; // ��¼

	bool OnInitADOConn();
	void ExitConnect();
};

