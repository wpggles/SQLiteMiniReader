//
// @Project : SQLiteMiniReader
// @File    : SQLiteTableCell.cpp
// @Date    : 2016-02-20
// @Author  : wPgg1es
//

#include "SQLiteTableCell.h"

/************************************************************************
* ���ܣ���ȡָ���ֶε��ı������ݡ�
* ������
*       index     [IN]  -> ָ�����ֶ�������
*       bufferPtr [OUT] -> �洢�������ݵ�ָ�롣
* ���أ��ı����ȡ�
* ע�⣺bufferPtr ֻ��Ҫһ���ַ���ָ�룬����������ռ䣬ʹ����Ҫ�ͷŵ���
************************************************************************/
UINT SQLiteTableCell::GetText(IN int index, OUT char * & bufferPtr)
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
* ���ܣ���ȡָ���ֶε����������ݡ�
* ������
*       index [IN] -> ָ�����ֶ�������
* ���أ�ת�����������
************************************************************************/
LONG SQLiteTableCell::GetInteger(IN int index)
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
* ���ܣ���ȡ��ǰTable��ID��
* �������ޡ�
* ���أ���ǰTable��ID��
************************************************************************/
ULONG SQLiteTableCell::GetTableID()
{
    return m_RowID;
}

/************************************************************************
* ���ܣ���ȡ��ǰTable���͡�
* �������ޡ�
* ���أ���ǰTable���͡�
************************************************************************/
SQLiteTableType SQLiteTableCell::GetTableType()
{
    switch (m_Data[0][1])
    {
        case 'a':
            return STT_Table;
        case 'n':
            return STT_Index;
        case 'r':
            return STT_Trigger;
        case 'i':
            return STT_View;
    }
    return STT_UNKNOWN;
}

/************************************************************************
* ���ܣ���ȡ��ǰTable�Ķ������ơ�
* ������
*       bufferPtr [OUT] -> �洢�������ݵ�ָ�롣
* ���أ���ǰTable�Ķ������Ƶĳ��ȡ�
************************************************************************/
UINT SQLiteTableCell::GetObjectName(OUT char * & bufferPtr)
{
    return GetText(1, bufferPtr);
}

/************************************************************************
* ���ܣ���ȡ��ǰTable������Ķ������ơ�
* ������
*       bufferPtr [OUT] -> �洢�������ݵ�ָ�롣
* ���أ���ǰTable������Ķ������Ƶĳ��ȡ�
* ��ע������Ǳ����ͼ�����뵱ǰ����������ͬ��
*       ����������򴥷������󣬴��ֶ�ֵΪ������صı�����
************************************************************************/
UINT SQLiteTableCell::GetRelatedObjectName(OUT char * & bufferPtr)
{
    return GetText(2, bufferPtr);
}

/************************************************************************
* ���ܣ���ȡ��ǰTable��Ӧ�����ݵĸ�ҳ������
* �������ޡ�
* ���أ���ǰTable��Ӧ�����ݵĸ�ҳ������
************************************************************************/
ULONG SQLiteTableCell::GetRootPageIndex()
{
    return GetInteger(3);
}

/************************************************************************
* ���ܣ���ȡ��ǰTable����ʱʹ�õ�SQL��䡣
* ������
*       bufferPtr [OUT] -> �洢�������ݵ�ָ�롣
* ���أ���ǰTable����ʱʹ�õ�SQL���ĳ��ȡ�
************************************************************************/
UINT SQLiteTableCell::GetCreateSQL(OUT char * & bufferPtr)
{
    return GetText(4, bufferPtr);
}
