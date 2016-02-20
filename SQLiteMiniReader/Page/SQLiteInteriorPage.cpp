//
// @Project : SQLiteMiniReader
// @File    : SQLiteInteriorPage.cpp
// @Date    : 2016-02-20
// @Author  : wPgg1es
//

#include "SQLiteInteriorPage.h"

/************************************************************************
* ���ܣ���ȡ��ǰ�ڲ�ҳָ���Ҷ��ҳ��������
* �������ޡ�
* ���أ���ǰ�ڲ�ҳָ���Ҷ��ҳ��������
************************************************************************/
USHORT SQLiteInteriorPage::GetPageCount()
{
    return m_CellsCount;
}

/************************************************************************
* ���ܣ���ȡ��ǰ�ڲ�ҳָ���Ҷ��ҳ���ڵ�ҳ������
* ������
*       index [IN] -> ��ǰ�ڲ�ҳ�е�Ҷ��ҳ������
* ���أ���ǰ�ڲ�ҳָ���Ҷ��ҳ���ڵ�ҳ������
************************************************************************/
UINT SQLiteInteriorPage::GetPageIndex(IN int index)
{
    //���������Ч��Χ��ֱ�ӷ���0
    if (index < 0 || index >= m_CellsCount)
    {
        return 0;
    }
    //�����������һ������Ϊλ�ò�ͬ
    if (index == m_CellsCount - 1)
    {
        return SQLiteTools::BigEndianInt(m_PageData + m_BaseOffset + LAST_RIGHT_PAGE_INDEX_OFFSET);
    }
    return SQLiteTools::BigEndianInt(m_PageData + SQLiteTools::BigEndianShort(m_PageData + m_BaseOffset + PAGE_CELL_LIST_OFFSET + index * 2));
}

/************************************************************************
* ���ܣ���ȡ��ǰ�ڲ�ҳָ���Ҷ��ҳ����ʼ����ֵ��
* ������
*       index [IN] -> ��ǰ�ڲ�ҳ�е�Ҷ�ӵ���ʼ����ֵ��
* ���أ���ǰ�ڲ�ҳָ���Ҷ��ҳ����ʼ����ֵ��
************************************************************************/
ULONG SQLiteInteriorPage::GetCompareIndex(IN int index)
{
    //���������Ч��Χ��ֱ�ӷ���0
    if (index < 0 || index >= m_CellsCount)
    {
        return 0;
    }
    //����ǵ�һ����ֱ�ӷ���1
    if (index == 0)
    {
        return 1;
    }
    int useByteCount;
    return SQLiteTools::VolatileLong(m_PageData + SQLiteTools::BigEndianShort(m_PageData + m_BaseOffset + PAGE_CELL_LIST_OFFSET + (index - 1) * 2) + 4, &useByteCount);
}
