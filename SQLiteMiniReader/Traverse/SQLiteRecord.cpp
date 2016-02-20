//
// @Project : SQLiteMiniReader
// @File    : SQLiteRecord.cpp
// @Date    : 2016-02-20
// @Author  : wPgg1es
//

#include "SQLiteRecord.h"

/************************************************************************
* ���ܣ�˽�й��졣
* ������
*       tableCell [IN] -> ��Ԫ����
* ���أ��ޡ�
************************************************************************/
SQLiteRecord::SQLiteRecord(IN SQLiteTableCell * tableCell)
{
	m_TmpDataCell = NULL;
	m_SQLiteTable = new SQLiteTable(tableCell);
}

/************************************************************************
* ���ܣ�������ʱ���ڴ��ݵ�һ�����ݼ�¼��
* ������
*       cell [IN] -> ���ݵ�Ԫ��
* ���أ��ޡ�
* ��ע����������ά�����ݵ����ݵ�Ԫ�������ɵ����ߴ���
************************************************************************/
void SQLiteRecord::SetTmpDataCell(IN SQLiteDataCell * cell)
{
    m_TmpDataCell = cell;
}

/************************************************************************
* ���ܣ�˽��������
* �������ޡ�
* ���أ��ޡ�
************************************************************************/
SQLiteRecord::~SQLiteRecord()
{
    if (m_SQLiteTable)
    {
        delete m_SQLiteTable;
    }
}

/************************************************************************
* ���ܣ���ȡ�ֶ�������
* �������ޡ�
* ���أ��ֶ�������
************************************************************************/
UINT SQLiteRecord::GetFieldCount()
{
    return m_SQLiteTable->GetFieldCount();
}

/************************************************************************
* ���ܣ���ȡָ���ֶε��ֶ����ơ�
* ������
*       index [IN] -> ָ���ֶε�������
* ���أ��ֶ����Ƶ��ַ�����
* ��ע������ֵ�����޸ģ�Ҳ���õ����ͷš�
************************************************************************/
const char * SQLiteRecord::GetFieldName(IN UINT index)
{
    return m_SQLiteTable->GetFieldName(index);
}

/************************************************************************
* ���ܣ���ȡָ���ֶε��ֶ����͡�
* ������
*       index [IN] -> ָ���ֶε�������
* ���أ��ֶ����͵��ַ�����
* ��ע������ֵ��Ҫ�޸ģ�Ҳ��Ҫ�����ͷš�
************************************************************************/
const char * SQLiteRecord::GetFeildTypeName(IN UINT index)
{
    return m_SQLiteTable->GetTypeName(index);
}

/************************************************************************
* ���ܣ���ȡָ���ֶ���SQLite�еĽ������͡�
* ������
*       index [IN] -> ָ���ֶε�������
* ���أ�ָ���ֶ���SQLite�еĽ������͡�
************************************************************************/
SQLiteDataType SQLiteRecord::GetFeildSimilarType(IN UINT index)
{
    return m_SQLiteTable->GetSimilarType(index);
}

/************************************************************************
* ���ܣ���ȡָ���ֶε����ԡ�
* ������
*       index      [IN] -> ָ���ֶε�������
*       properties [IN] -> Ҫ��������(��SFP_NOP�����ʹ��'|'����)��
* ���أ�ָ�������Դ��ڷ���TRUE��ֻҪ��һ�������ھͷ���FALSE��
* ע�⣺����properties�����Դ���SQLiteFeildProperty���ͼ��ɡ�
************************************************************************/
BOOL SQLiteRecord::GetFeildProperty(IN UINT index, IN int properties)
{
    return m_SQLiteTable->GetFeildProperty(index, properties);
}

/************************************************************************
* ���ܣ���ȡ��ǰ��Ԫ�����ڱ��е�RowID��
* �������ޡ�
* ���أ�RowID��
************************************************************************/
ULONG SQLiteRecord::GetRowID()
{
    return m_TmpDataCell ? m_TmpDataCell->GetRowID() : 0;
}

/************************************************************************
* ���ܣ���ȡָ���ֶεĲ��������ݡ�
* ������
*       index [IN] -> ָ�����ֶ�������
* ���أ�����ֵ��
* ��ע��С��256���ҵķ�0ΪTRUE������ΪFALSE��
************************************************************************/
BOOL SQLiteRecord::GetBoolean(IN int index)
{
    return m_TmpDataCell ? m_TmpDataCell->GetBoolean(index) : 0;
}

/************************************************************************
* ���ܣ���ȡָ���ֶε����������ݡ�
* ������
*       index [IN] -> ָ�����ֶ�������
* ���أ�ת�����������
************************************************************************/
LONG SQLiteRecord::GetInteger(IN int index)
{

    if (m_TmpDataCell)
    {
        LONG result = m_TmpDataCell->GetInteger(index);
        //��ΪSQLiteΪ�˽�ʡ�ռ䣬�����������������ֶ�Ĭ�ϲ��õ���rowid
        if (!result)
        {
            if (m_SQLiteTable->GetFeildProperty(index, SFP_PRIMARY_KEY) ||
                m_SQLiteTable->GetFeildProperty(index, SFP_AUTOINCREMENT))
            {
                return m_TmpDataCell->GetRowID();
            }
        }
        else
        {
            return result;
        }
    }
    return 0;
}

/************************************************************************
* ���ܣ���ȡָ���ֶεĸ��������ݡ�
* ������
*       index [IN] -> ָ�����ֶ�������
* ���أ�ת����ĸ�������
************************************************************************/
double SQLiteRecord::GetReal(IN int index)
{
    return m_TmpDataCell ? m_TmpDataCell->GetReal(index) : 0;
}

/************************************************************************
* ���ܣ���ȡָ���ֶε��ı������ݡ�
* ������
*       index     [IN]  -> ָ�����ֶ�������
*       bufferPtr [OUT] -> �洢�������ݵ�ָ�롣
* ���أ��ı����ȡ�
* ע�⣺bufferPtr ֻ��Ҫһ���ַ���ָ�룬����������ռ䣬ʹ����Ҫ�ͷŵ���
************************************************************************/
UINT SQLiteRecord::GetText(IN int index, OUT char * & bufferPtr)
{
    return m_TmpDataCell ? m_TmpDataCell->GetText(index, bufferPtr) : 0;
}

/************************************************************************
* ���ܣ���ȡָ���ֶεĶ����������ݡ�
* ������
*       index     [IN]  -> ָ�����ֶ�������
*       bufferPtr [OUT] -> �洢�������ݵ�ָ�롣
* ���أ������������ݳ��ȡ�
* ע�⣺bufferPtr ֻ��Ҫһ��BYTEָ�룬����������ռ䣬ʹ����Ҫ�ͷŵ���
************************************************************************/
UINT SQLiteRecord::GetBLOB(IN int index, OUT BYTE * & bufferPtr)
{
    return m_TmpDataCell ? m_TmpDataCell->GetBLOB(index, bufferPtr) : 0;
}
