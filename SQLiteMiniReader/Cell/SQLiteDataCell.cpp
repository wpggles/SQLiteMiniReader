//
// @Project : SQLiteMiniReader
// @File    : SQLiteDataCell.cpp
// @Date    : 2016-02-20
// @Author  : wPgg1es
//

#include "SQLiteDataCell.h"

/************************************************************************
* ���ܣ���ȡ��ǰ��Ԫ�����ڱ��е�RowID��
* �������ޡ�
* ���أ�RowID��
************************************************************************/
ULONG SQLiteDataCell::GetRowID()
{
    return m_RowID;
}

/************************************************************************
* ���ܣ���ȡ��ǰ��Ԫ���ֶ�������
* �������ޡ�
* ���أ��ֶ�������
************************************************************************/
ULONG SQLiteDataCell::GetColumnCount()
{
    return m_ColumnCount;
}

/************************************************************************
* ���ܣ���ȡָ���е��ֶ����͡�
* ������
*       index [IN] -> ָ�����ֶ�������
* ���أ�SQLite�������͡�
* ��ע�������������жϵģ���׼ȷ�������ο���
*       ���ﷵ�ز�����Ҳ�п��������֣�ֻ�������ֿ�����0��1��
************************************************************************/
SQLiteDataType SQLiteDataCell::GetTpye(IN int index)
{
    if (!IS_VALOD_INDEX(index))
    {
        return SDT_UNKNOWN;
    }
    switch (m_Types[index])
    {
        case 0:
            return SDT_DBNULL;
        case 8:
        case 9:
            return SDT_BOOLEAN;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            return SDT_INTEGER;
        case 7:
            return SDT_REAL;
        default:
            if (IS_BLOB_FIELD(m_Types[index]))
            {
                return SDT_BLOB;
            }
            else if (IS_TEXT_FIELD(m_Types[index]))
            {
                return SDT_TEXT;
            }
    }
    return SDT_UNKNOWN;
}

/************************************************************************
* ���ܣ���ȡָ���ֶεĲ��������ݡ�
* ������
*       index [IN] -> ָ�����ֶ�������
* ���أ�����ֵ��
* ��ע��С��256���ҵķ�0ΪTRUE������ΪFALSE��
************************************************************************/
BOOL SQLiteDataCell::GetBoolean(IN int index)
{
    switch (m_Types[index])
    {
    case 1:
        return *m_Data[index] != 0;
    case 8:
        return TRUE;
    }
    return FALSE;
}

/************************************************************************
* ���ܣ���ȡָ���ֶε����������ݡ�
* ������
*       index [IN] -> ָ�����ֶ�������
* ���أ�ת�����������
************************************************************************/
LONG SQLiteDataCell::GetInteger(IN int index)
{
    if (!IS_VALOD_INDEX(index) || IS_DBNULL(index))
    {
        return 0;
    }
    int len = (int)m_Types[index];
    if (len >= 8 || len <= 0)
    {
        return len == 9 ? 1 : 0;
    }
    switch (len)
    {
        case 6:
            len += 1;
        case 5:
            len += 1;
    }
    return SQLiteTools::BigEndianNumber(m_Data[index], len);
}

/************************************************************************
* ���ܣ���ȡָ���ֶεĸ��������ݡ�
* ������
*       index [IN] -> ָ�����ֶ�������
* ���أ�ת����ĸ�������
************************************************************************/
double SQLiteDataCell::GetReal(int index)
{
    if (!IS_VALOD_INDEX(index) || IS_DBNULL(index))
    {
        return 0;
    }
    return SQLiteTools::BigEndianDouble(m_Data[index]);
}

/************************************************************************
* ���ܣ���ȡָ���ֶε��ı������ݡ�
* ������
*       index     [IN]  -> ָ�����ֶ�������
*       bufferPtr [OUT] -> �洢�������ݵ�ָ�롣
* ���أ��ı����ȡ�
* ע�⣺bufferPtr ֻ��Ҫһ���ַ���ָ�룬����������ռ䣬ʹ����Ҫ�ͷŵ���
************************************************************************/
UINT SQLiteDataCell::GetText(IN int index, OUT char * & bufferPtr)
{
    if (!IS_VALOD_INDEX(index) || IS_DBNULL(index))
    {
        bufferPtr = NULL;
        return 0;
    }
    UINT len = TEXT_LENGHT(m_Types[index]);
    if (len == 0)
    {
        bufferPtr = NULL;
        return 0;
    }
    bufferPtr = new char[len + 1];
    memcpy(bufferPtr, m_Data[index], len);
    bufferPtr[len] = 0;
    return len;
}

/************************************************************************
* ���ܣ���ȡָ���ֶεĶ����������ݡ�
* ������
*       index     [IN]  -> ָ�����ֶ�������
*       bufferPtr [OUT] -> �洢�������ݵ�ָ�롣
* ���أ������������ݳ��ȡ�
* ע�⣺bufferPtr ֻ��Ҫһ��BYTEָ�룬����������ռ䣬ʹ����Ҫ�ͷŵ���
************************************************************************/
UINT SQLiteDataCell::GetBLOB(IN int index, OUT BYTE * & bufferPtr)
{
    if (!IS_VALOD_INDEX(index) || IS_DBNULL(index))
    {
        bufferPtr = NULL;
        return 0;
    }
    UINT len = BLOB_LENGHT(m_Types[index]);
    if (len == 0)
    {
        bufferPtr = NULL;
        return 0;
    }
    bufferPtr = new BYTE[len];
    memcpy(bufferPtr, m_Data[index], len);
    return len;
}
