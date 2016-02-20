//
// @Project : SQLiteMiniReader
// @File    : SQLiteLeafPage.cpp
// @Date    : 2016-02-20
// @Author  : wPgg1es
//

#include "SQLiteLeafPage.h"

/************************************************************************
* ���ܣ���ȡ��ǰҳ��Ԫ������
* �������ޡ�
* ���أ���ǰҳ�ĵ�Ԫ������
************************************************************************/
USHORT SQLiteLeafPage::GetCellsCount()
{
    return m_CellsCount;
}

/************************************************************************
* ���ܣ���ȡ��ǰҳָ���ĵ�Ԫ��
* ������
*       index [IN] -> Ҫ��ȡ�ĵ�Ԫ������
* ���أ���Ԫ��������������Ի���󴴽�ʧ�ܷ���NULL��
* ��ע�����صĵ�ԪҪ�ֶ��ͷš�
************************************************************************/
SQLiteCell * SQLiteLeafPage::GetCell(IN int index)
{
    if (index >= m_CellsCount || index < 0)
    {
        return NULL;
    }
    //��¼��ǰҪ���صĵ�Ԫƫ��
    int offset = SQLiteTools::BigEndianShort(m_PageData + m_BaseOffset + CELL_LIST_OFFSET + index * 2);
    //���㵱ǰ��Ԫǰһ����Ԫ��ƫ��
    int prevOffset = index == 0 ? m_PageSize : SQLiteTools::BigEndianShort(m_PageData + m_BaseOffset + CELL_LIST_OFFSET + (index - 1) * 2);
    //���ش����ĵ�Ԫ����
    return new SQLiteCell(m_PageData + offset, m_RawData, m_PageSize, prevOffset - offset);
}
