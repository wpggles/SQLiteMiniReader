//
// @Project : SQLiteMiniReader
// @File    : SQLiteMiniReader.h
// @Date    : 2016-02-20
// @Author  : wPgg1es
//

#ifndef _SQLITE_MINI_READER_H_
#define _SQLITE_MINI_READER_H_

#include <fstream>
#include "FileHeader/SQLiteFileHeader.h"
#include "Traverse/SQLiteTable.h"
#include "Traverse/SQLiteRecord.h"
#include "Cell/SQLiteDataCell.h"
#include "Cell/SQLiteTableCell.h"
#include "Page/SQLitePage.h"
#include "Page/SQLiteLeafPage.h"
#include "Page/SQLiteInteriorPage.h"

/************************************************************************
* ���ܣ��������Ļص��������塣
* ������
*       table [IN]     -> ��ǰ���ݵ�Ҫ�����ı����ݡ�
*       pVoid [IN_OUT] -> �û��Զ���ֵ��
* ���أ�����������һ�����ݷ���TRUE�����򷵻�FALSE��
************************************************************************/
typedef BOOL(*HandleTableCallback)(IN SQLiteTable * table, IN_OUT void * pVoid);

/************************************************************************
* ���ܣ�������¼�Ļص��������塣
* ������
*       record [IN]     -> ��ǰ���ݵ�Ҫ�����ĵ�Ԫ��¼��
*       pVoid  [IN_OUT] -> �û��Զ���ֵ��
* ���أ�����������һ�����ݷ���TRUE�����򷵻�FALSE��
************************************************************************/
typedef BOOL(*HandleRecordCallback)(IN SQLiteRecord * record, IN_OUT void * pVoid);


//SQLite���ݿ��ȡ������
class SQLiteMiniReader
{
private:
    //�洢SQLite���ݿ��ļ��е�����
    BYTE * m_Data;
    //SQLite���ݿ��ļ��Ĵ�С
    UINT m_DataLenght;
    //SQLite���ݿ��ļ�ͷ��Ϣ
    SQLiteFileHeader * m_SQLiteFileHeaderInfo;

private:
    /************************************************************************
    * ���ܣ�����ָ������Ԫ�Ķ���
    * ������
    *       tableName [IN] -> Ҫ���ҵı�����
    *       pageIndex [IN] -> �ӵڼ�ҳ��ʼ���ҡ�
    * ���أ����ҵ��ı���Ԫ���󣬲��Ҳ�������NULL��
    * ��ע���˺�����ݹ���ң������״ε���pageIndexӦ��Ϊ1��
    ************************************************************************/
    SQLiteTableCell * FindTableCell(IN const char * tableName, IN UINT pageIndex);

    /************************************************************************
    * ���ܣ�����ָ�����µĵ�Ԫ��
    * ������
    *       pageIndex        [IN]     -> �ӵڼ�ҳ��ʼ������
    *       handleRecordFunc [IN]     -> ������¼���ݵĻص�������
    *       record           [IN]     -> Ҫ�������ļ�¼���ݡ�
    *       pVoid            [IN_OUT] -> �û��Զ���ֵ��
    * ���أ�������������TRUE�����򷵻�FALSE���ܻص��������ơ�
    ************************************************************************/
    BOOL TraverseCell(IN UINT pageIndex, IN HandleRecordCallback handleRecordFunc, IN SQLiteRecord * record, IN_OUT void * pVoid);

	/************************************************************************
	* ���ܣ��������еı���
	* ������
	*       pageIndex       [IN]     -> �ӵڼ�ҳ��ʼ������
	*       handleTableFunc [IN]     -> ���������ݵĻص�������
	*       table           [IN]     -> Ҫ�������ı����ݡ�
	*       pVoid           [IN_OUT] -> �û��Զ���ֵ��
	* ���أ�������������TRUE�����򷵻�FALSE���ܻص��������ơ�
	************************************************************************/
	BOOL TraverseTable(IN UINT pageIndex, IN HandleTableCallback handleTableFunc, IN_OUT void * pVoid);

public:
    /************************************************************************
    * ���ܣ���ʼ�����ݡ�
    * ������
    *       dbFileName [IN] -> Ҫ��ѯ��SQLite���ݿ�·����
    * ���أ��ޡ�
    ************************************************************************/
    SQLiteMiniReader(IN const char * dbFileName);

    /************************************************************************
    * ���ܣ����������ͷ���Դ��
    * �������ޡ�
    * ���أ��ޡ�
    ************************************************************************/
    ~SQLiteMiniReader();
    
    /************************************************************************
    * ���ܣ���ȡ���ݿ��ļ�ͷ��Ϣ��
    * �������ޡ�
    * ���أ����ݿ��ļ�ͷ��Ϣ��
    * ��ע����Ҫ�ֶ��ͷţ�SQLiteReader�ͷ�ʱ���Զ��ͷš�
    ************************************************************************/
    SQLiteFileHeader * GetFileInfo();

    /************************************************************************
    * ���ܣ�������ѯָ�������������ݡ�
    * ������
    *       tableName        [IN]     -> Ҫ��ѯ�ı�����
    *       handleRecordFunc [IN]     -> ������¼���ݵĻص�������
    *       pVoid            [IN_OUT] -> �û��Զ���ֵ��
    * ���أ���ѯ�ɹ�����TRUE�����򷵻�FALSE��
    * ��ע���˺�����Ƕ��ѭ�����ûص�����������ע�����̴�����
    ************************************************************************/
    BOOL QueryRecord(IN const char * tableName, IN HandleRecordCallback handleRecordFunc, IN_OUT void * pVoid);

	/************************************************************************
	* ���ܣ�������ѯ���еı���
	* ������
	*       handleTableFunc [IN]     -> ���������ݵĻص�������
	*       pVoid           [IN_OUT] -> �û��Զ���ֵ��
	* ���أ���ѯ�ɹ�����TRUE�����򷵻�FALSE��
	* ��ע���˺�����Ƕ��ѭ�����ûص�����������ע�����̴�����
	************************************************************************/
	void QueryTable(IN HandleTableCallback handleTableFunc, IN_OUT void * pVoid);
};

#endif