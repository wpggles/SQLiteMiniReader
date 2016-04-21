//
// @Project : SQLiteMiniReader
// @File    : SQLiteMiniReader.cpp
// @Date    : 2016-02-20
// @Author  : wPgg1es
//

#include "SQLiteMiniReader.h"

/************************************************************************
* ���ܣ���ʼ�����ݡ�
* ������
*       dbFileName [IN] -> Ҫ��ѯ��SQLite���ݿ�·����
* ���أ��ޡ�
************************************************************************/
SQLiteMiniReader::SQLiteMiniReader(const char * dbFileName)
{
    m_Data = NULL;
    m_SQLiteFileHeaderInfo = NULL;
    std::ifstream in;
    in.open(dbFileName, std::ios::in | std::ios::binary | std::ios::ate);
    m_DataLenght = (UINT)in.tellg();
    if (m_DataLenght <= 100)
    {
        in.close();
        return;
    }
    m_Data = new BYTE[m_DataLenght];
    in.seekg(std::ios::beg);
    in.read((char *)m_Data, m_DataLenght);
    in.close();
    m_SQLiteFileHeaderInfo = new SQLiteFileHeader(m_Data);
}

/************************************************************************
* ���ܣ����������ͷ���Դ��
* �������ޡ�
* ���أ��ޡ�
************************************************************************/
SQLiteMiniReader::~SQLiteMiniReader()
{
    if (m_Data != NULL)
    {
        delete[] m_Data;
        m_Data = NULL;
    }
    if (m_SQLiteFileHeaderInfo != NULL)
    {
        delete m_SQLiteFileHeaderInfo;
        m_SQLiteFileHeaderInfo = NULL;
    }
}

/************************************************************************
* ���ܣ���ȡ���ݿ��ļ�ͷ��Ϣ��
* �������ޡ�
* ���أ����ݿ��ļ�ͷ��Ϣ��
* ��ע����Ҫ�ֶ��ͷţ�SQLiteReader�ͷ�ʱ���Զ��ͷš�
************************************************************************/
SQLiteFileHeader * SQLiteMiniReader::GetFileInfo()
{
    return m_SQLiteFileHeaderInfo;
}

/************************************************************************
* ���ܣ�����ָ����Ԫ�Ķ���
* ������
*       tableName [IN] -> Ҫ���ҵı�����
*       pageIndex [IN] -> �ӵڼ�ҳ��ʼ���ҡ�
* ���أ����ҵ��ı�Ԫ���󣬲��Ҳ�������NULL��
* ��ע���˺�����ݹ���ң������״ε���pageIndexӦ��Ϊ1��
************************************************************************/
SQLiteTableCell * SQLiteMiniReader::FindTableCell(const char * tableName, UINT pageIndex)
{
    //ָ����ҵ��ı�Ԫ
    SQLiteTableCell * queryTableCell = NULL;
    //����ҳ����
    SQLitePage * page = new SQLitePage(m_Data, m_SQLiteFileHeaderInfo->GetPageSize(), pageIndex);
    //��ȡҳ���ͣ������Ҷ��ҳ��ʼ�ǲ�������
    if (page->GetPageType() == SPT_LeafPage)
    {
        //ת��ΪҶ��ҳ����
        SQLiteLeafPage * leafPage = (SQLiteLeafPage *)page;
        //��ȡ��Ԫ����
        int count = leafPage->GetCellsCount();
        //ѭ���������б����
        for (int i = 0; i < count && queryTableCell == NULL; ++i)
        {
            //��ȡ�����
            SQLiteTableCell * tableCell = (SQLiteTableCell *)leafPage->GetCell(i);
            //�洢����
            char * tableNameBuffer;
            tableCell->GetObjectName(tableNameBuffer);
            //����Ǳ��ұ�����Ҫ���ҵģ��򷵻ض�Ӧ�ı����
            if (tableCell->GetTableType() == STT_Table && !_stricmp(tableNameBuffer, tableName))
            {
                queryTableCell = tableCell;
            }
            else
            {
                delete tableCell;
            }
            //�ͷŵ������ռ�
            delete[] tableNameBuffer;
        }
    }
    //������ڲ�ҳ�������ǰ�ڲ�ҳ��ҳָ��
    else if (page->GetPageType() == SPT_InteriorPage)
    {
        //ת��Ϊ�ڲ�ҳ����
        SQLiteInteriorPage * interiorPage = (SQLiteInteriorPage *)page;
        //��ȡ�ڲ�ҳָ���Ҷ��ҳ(�ڲ�ҳ)����
        int count = interiorPage->GetPageCount();
        //ѭ���ݹ��������
        for (int i = 0; i < count && queryTableCell == NULL; ++i)
        {
            queryTableCell = FindTableCell(tableName, interiorPage->GetPageIndex(i));
        }
    }
    //ɾ��������ҳ����
    delete page;

    return queryTableCell;
}

/************************************************************************
* ���ܣ�����ָ�����µĵ�Ԫ��
* ������
*       pageIndex        [IN]     -> �ӵڼ�ҳ��ʼ������
*       handleRecordFunc [IN]     -> �����¼���ݵĻص�������
*       record           [IN]     -> Ҫ������ļ�¼���ݡ�
*       pVoid            [IN_OUT] -> �û��Զ���ֵ��
* ���أ�������������TRUE�����򷵻�FALSE���ܻص��������ơ�
************************************************************************/
BOOL SQLiteMiniReader::TraverseCell(IN UINT pageIndex, IN HandleRecordCallback handleRecordFunc, IN SQLiteRecord * record, IN_OUT void * pVoid)
{
    //����Ƿ��������
    BOOL continueTravers = TRUE;
    //����ҳ����
    SQLitePage * page = new SQLitePage(m_Data, m_SQLiteFileHeaderInfo->GetPageSize(), pageIndex);
    //��ȡҳ���ͣ������Ҷ��ҳ��ʼ�ǲ�������
    if (page->GetPageType() == SPT_LeafPage)
    {
        //ת����Ҷ��ҳ������
        SQLiteLeafPage * leafPage = (SQLiteLeafPage *)page;
        int count = leafPage->GetCellsCount();
        
        for (int i = 0; i < count && continueTravers; ++i)
        {
            SQLiteDataCell * dataCell = (SQLiteDataCell *)leafPage->GetCell(i);
            record->SetTmpDataCell(dataCell);
            continueTravers = handleRecordFunc(record, pVoid);

            delete dataCell;
        }
    }
    //������ڲ�ҳ�������ǰ�ڲ�ҳ��ҳָ��
    else if (page->GetPageType() == SPT_InteriorPage)
    {
        SQLiteInteriorPage * interiorPage = (SQLiteInteriorPage *)page;
        int count = interiorPage->GetPageCount();
        for (int i = 0; i < count && continueTravers; ++i)
        {
            continueTravers = TraverseCell(interiorPage->GetPageIndex(i), handleRecordFunc, record, pVoid);
        }
    }
    delete page;
    return continueTravers;
}

/************************************************************************
* ���ܣ���ѯָ�������������ݡ�
* ������
*       tableName        [IN]     -> Ҫ��ѯ�ı�����
*       handleRecordFunc [IN]     -> �����¼���ݵĻص�������
*       pVoid            [IN_OUT] -> �û��Զ���ֵ��
* ���أ���ѯ�ɹ�����TRUE�����򷵻�FALSE��
* ��ע���˺�����Ƕ��ѭ�����ûص�����������ע�����̴���
************************************************************************/
BOOL SQLiteMiniReader::QueryRecord(IN const char * tableName, IN HandleRecordCallback handleRecordFunc, IN_OUT void * pVoid)
{
    //�ӵ�1ҳ��ʼ��ѯָ���ı�
    SQLiteTableCell * queryTableCell = FindTableCell(tableName, 1);
    //�鲻��ֱ�ӷ���
    if (queryTableCell == NULL)
    {
        return FALSE;
    }
    //�������ڴ��εĶ���
    SQLiteRecord sqliteHandleCell(queryTableCell);
	if (!sqliteHandleCell.m_SQLiteTable->HasNoTypeFeild())
	{
		//���������µļ�¼
		TraverseCell((UINT)queryTableCell->GetRootPageIndex(), handleRecordFunc, &sqliteHandleCell, pVoid);
	}
    //ɾ�������
    delete queryTableCell;
    return TRUE;
}

/************************************************************************
* ���ܣ��������еı�
* ������
*       pageIndex       [IN]     -> �ӵڼ�ҳ��ʼ������
*       handleTableFunc [IN]     -> ��������ݵĻص�������
*       pVoid           [IN_OUT] -> �û��Զ���ֵ��
* ���أ�������������TRUE�����򷵻�FALSE���ܻص��������ơ�
************************************************************************/
BOOL SQLiteMiniReader::TraverseTable(IN UINT pageIndex, IN HandleTableCallback handleTableFunc, IN_OUT void * pVoid)
{
	//����Ƿ��������
	BOOL continueTravers = TRUE;
	//����ҳ����
	SQLitePage * page = new SQLitePage(m_Data, m_SQLiteFileHeaderInfo->GetPageSize(), pageIndex);
	//��ȡҳ���ͣ������Ҷ��ҳ��ʼ�ǲ�������
	if (page->GetPageType() == SPT_LeafPage)
	{
		//ת����Ҷ��ҳ������
		SQLiteLeafPage * leafPage = (SQLiteLeafPage *)page;
		int count = leafPage->GetCellsCount();

		for (int i = 0; i < count && continueTravers; ++i)
		{
			SQLiteTableCell * tableCell = (SQLiteTableCell *)leafPage->GetCell(i);

			if (tableCell->GetTableType() == STT_Table)
			{
				SQLiteTable * table = new SQLiteTable(tableCell);
				if (!table->HasNoTypeFeild())
				{
					continueTravers = handleTableFunc(table, pVoid);
				}
				delete table;
			}
			delete tableCell;
		}
	}
	//������ڲ�ҳ�������ǰ�ڲ�ҳ��ҳָ��
	else if (page->GetPageType() == SPT_InteriorPage)
	{
		SQLiteInteriorPage * interiorPage = (SQLiteInteriorPage *)page;
		int count = interiorPage->GetPageCount();
		for (int i = 0; i < count && continueTravers; ++i)
		{
			continueTravers = TraverseTable(interiorPage->GetPageIndex(i), handleTableFunc, pVoid);
		}
	}
	delete page;
	return continueTravers;
}

/************************************************************************
* ���ܣ�������ѯ���еı�
* ������
*       handleTableFunc [IN]     -> ��������ݵĻص�������
*       pVoid           [IN_OUT] -> �û��Զ���ֵ��
* ���أ���ѯ�ɹ�����TRUE�����򷵻�FALSE��
* ��ע���˺�����Ƕ��ѭ�����ûص�����������ע�����̴���
************************************************************************/
void SQLiteMiniReader::QueryTable(IN HandleTableCallback handleTableFunc, IN_OUT void * pVoid)
{
	TraverseTable(1, handleTableFunc, pVoid);
}
