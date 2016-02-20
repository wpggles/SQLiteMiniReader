//
// @Project : SQLiteMiniReader
// @File    : SQLitePage.h
// @Date    : 2016-02-20
// @Author  : wPgg1es
//

#ifndef _SQLITE_PAGE_H_
#define _SQLITE_PAGE_H_

#include "../Tools/SQLiteTools.h"
#include <string.h>

//��һҳ������ʼƫ����
#define FIRST_PAGE_OFFSET            0x64
//ҳ����ƫ����
#define PAGE_TYPE_OFFSET             0x00
//��һ�����ɿ�ƫ����
#define FIRST_FREE_BLOCK_OFFSET      0x01
//��ҳ��Ԫ����ƫ����
#define CELLS_COUNT_OFFSET           0x03
//���һ����Ԫƫ����
#define LAST_CELL_OFFSET             0x05
//��Ƭ�ֽ���ƫ����
#define FRAGMENTEDED_BYTES_OFFSET    0x07
//��Ԫ�б�ƫ����
#define CELL_LIST_OFFSET             0x08

#define LAST_RIGHT_PAGE_INDEX_OFFSET 0x08

#define PAGE_CELL_LIST_OFFSET        0x0C

//SQLiteҳ����
enum SQLitePageType
{
    //����(��Ч)ҳ
    SPT_ErrorPage = 0x00,
    //�ڲ�����ҳ
    SPT_InteriorIndexPage = 0x02,
    //�ڲ�ҳ
    SPT_InteriorPage = 0x05,
    //Ҷ������ҳ
    SPT_LeafIndexPage = 0x0A,
    //Ҷ��ҳ
    SPT_LeafPage = 0x0D
};

//SQLiteҳ�������
class SQLitePage
{
protected:
    //����ƫ��
    USHORT m_BaseOffset;
    //ҳ����
    USHORT m_PageIndex;
    //ҳ��С
    USHORT m_PageSize;
    //ҳ����
    BYTE   m_PageType;
    //ҳ����
    BYTE * m_PageData;
    //��һ�����ɿ�
    USHORT m_FirstFreeBlock;
    //��Ԫ����
    USHORT m_CellsCount;
    //(�߼�˳����)���һ����Ԫƫ��
    USHORT m_LastCellOffset;
    //��Ƭ�ֽ���
    USHORT m_FragmentedBytes;

    //ԭʼ���ݵ�ָ��
    const BYTE * m_RawData;

public:
    /************************************************************************
    * ���ܣ����췽������ʼ���ࡣ
    * ������
    *       bytes     [IN] -> SQLite���ݿ��ļ�(��ͷ��ʼ��)���ݡ�
    *       pageSize  [IN] -> һ��Page���ݵ��ֽ�����
    *       pageIndex [IN] -> ��ǰPage������(��1��ʼ)��
    * ���أ��ޡ�
    * ��ע�����������ݱ�������������һ��Page�����ݻ��Զ����ԡ�
    ************************************************************************/
    SQLitePage(IN const BYTE * bytes, IN USHORT pageSize, IN USHORT pageIndex);

    /************************************************************************
    * ���ܣ��������������ͷ����ݡ�
    * �������ޡ�
    * ���أ��ޡ�
    ************************************************************************/
    virtual ~SQLitePage();

    /************************************************************************
    * ���ܣ���ǰҳ�Ƿ��Ǵ���ҳ��
    * �������ޡ�
    * ���أ��Ǵ���ҳ����TRUE�����򷵻�FALSE��
    ************************************************************************/
    BOOL IsErrorPage();

    /************************************************************************
    * ���ܣ���ȡ��ǰҳ�����͡�
    * �������ޡ�
    * ���أ���ǰҳ�����͡�
    ************************************************************************/
    SQLitePageType GetPageType();
};

#endif