//
// @Project : SQLiteMiniReader
// @File    : SQLitePage.cpp
// @Date    : 2016-02-20
// @Author  : wPgg1es
//

#include "SQLitePage.h"

/************************************************************************
* ���ܣ����췽������ʼ���ࡣ
* ������
*       bytes     [IN] -> SQLite���ݿ��ļ�(��ͷ��ʼ��)���ݡ�
*       pageSize  [IN] -> һ��Page���ݵ��ֽ�����
*       pageIndex [IN] -> ��ǰPage������(��1��ʼ)��
* ���أ��ޡ�
* ��ע�����������ݱ�������������һ��Page�����ݻ��Զ����ԡ�
************************************************************************/
SQLitePage::SQLitePage(IN const BYTE * bytes, IN USHORT pageSize, IN USHORT pageIndex) : m_RawData(bytes)
{
    //����ǵ�һ��page����Ϊ��ͷ����100�ֽڵ��ļ�ͷ���ݣ�����Ҫ����һ��������ƫ��
    m_BaseOffset = pageIndex == 1 ? FIRST_PAGE_OFFSET : 0;
    bytes += pageSize * (pageIndex - 1);
    m_PageIndex = pageIndex;
    m_PageSize = pageSize;
   
    m_PageType = *(bytes + m_BaseOffset);

    m_PageData = new BYTE[m_PageSize];
    memcpy(m_PageData, bytes, m_PageSize);

    //�ж�����Ǵ���ҳ�Ͳ��������ж���
    if (IsErrorPage())
    {
        return;
    }

    m_FirstFreeBlock = SQLiteTools::BigEndianShort(bytes + m_BaseOffset + FIRST_FREE_BLOCK_OFFSET);
    m_CellsCount = SQLiteTools::BigEndianShort(bytes + m_BaseOffset + CELLS_COUNT_OFFSET);
    m_LastCellOffset = SQLiteTools::BigEndianShort(bytes + m_BaseOffset + LAST_CELL_OFFSET);
    m_FragmentedBytes = (USHORT)*(bytes + m_BaseOffset + FRAGMENTEDED_BYTES_OFFSET);
    
    //������ڲ�ҳ
    if (GetPageType() == SPT_InteriorPage)
    {
        //��Ϊ���Ҳ���Ҷ�����ڵ�ԭҳ����֮��
        m_CellsCount += 1;
    }
}

/************************************************************************
* ���ܣ��������������ͷ����ݡ�
* �������ޡ�
* ���أ��ޡ�
************************************************************************/
SQLitePage::~SQLitePage()
{
    delete [] m_PageData;
}

/************************************************************************
* ���ܣ���ǰҳ�Ƿ��Ǵ���ҳ��
* �������ޡ�
* ���أ��Ǵ���ҳ����TRUE�����򷵻�FALSE��
************************************************************************/
BOOL SQLitePage::IsErrorPage()
{
    return GetPageType() == SPT_ErrorPage;
}

/************************************************************************
* ���ܣ���ȡ��ǰҳ�����͡�
* �������ޡ�
* ���أ���ǰҳ�����͡�
************************************************************************/
SQLitePageType SQLitePage::GetPageType()
{
    switch (m_PageType)
    {
        case 0x02:
            return SPT_InteriorIndexPage;
        case 0x05:
            return SPT_InteriorPage;
        case 0x10:
            return SPT_LeafIndexPage;
        case 0x0D:
            return SPT_LeafPage;
        default:
            return SPT_ErrorPage;
    }
}
