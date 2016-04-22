//
// @Project : SQLiteMiniReader
// @File    : SQLiteCell.h
// @Date    : 2016-02-20
// @Author  : wPgg1es
//

#ifndef _SQLITE_CELL_H_
#define _SQLITE_CELL_H_

#include "../Tools/SQLiteTools.h"
#include <string.h>

//ͨ�������ж��Ƿ����ı��ֶ�
#define IS_TEXT_FIELD(x) ((x) >= 13 && ((x) % 2))
//ͨ�������ж��Ƿ��Ƕ����������ֶ�
#define IS_BLOB_FIELD(x) ((x) >= 12 && !((x) % 2))

//ͨ�������ж��ı�����
#define TEXT_LENGHT(x) (((UINT)(x) - 13) / 2)
//ͨ�������ж϶��������ݳ���
#define BLOB_LENGHT(x) (((UINT)(x) - 12) / 2)

//�ж��������Ƿ���Ч
#define IS_VALOD_INDEX(x) ((x) >= 0 && (x) < (int)m_ColumnCount)

//�ж�������ΪΪDBNULL
#define IS_DBNULL(x) (!m_Types[x])

//SQLite������
enum SQLiteTableType
{
    //δ֪
    STT_UNKNOWN,
    //���ݱ�
    STT_Table,
    //����
    STT_Index,
    //������
    STT_Trigger,
    //��ͼ
    STT_View
};

//SQLite��������
enum SQLiteDataType
{
    //δ֪
    SDT_UNKNOWN,
    //NULL
    SDT_DBNULL,
    //������
    SDT_BOOLEAN,
    //����
    SDT_INTEGER,
    //������
    SDT_REAL,
    //�ı�
    SDT_TEXT,
    //����������
    SDT_BLOB
};

//SQLite��Ԫ��
class SQLiteCell
{
protected:
    //ʹ���ֽ���
    ULONG   m_UseBytes;
    //�ڵ�ǰ���е�ID
    ULONG   m_RowID;
    //������
    ULONG   m_ColumnCount;
    //�����б�
    ULONG * m_Types;
    //�����б�
    BYTE **     m_Data;

public:
	/************************************************************************
	* ���ܣ�˽�й��췽������ʼ���ࡣ
	* ������
	*       data                  [IN] -> Ҫ������һ��Cell�����ݡ�
	*       rawData               [IN] -> ԭʼ���ݣ������ݿ��ļ���ʼ����ָ
	*                                     �룬�������ҳ����
	*       pageSize              [IN] -> ����ҳ��С���������ҳ����
	*       cellSizeInCurrentPage [IN] -> ��ǰ��Ԫ�ڵ�ǰҳ�Ѿ�ռ�õĳ��ȣ�
	*                                     �������ҳ����
	* ���أ��ޡ�
	* ��ע��
	*       1.���������ݱ�������������һ��Cell�����ݻ��Զ�����(���ҳ����)��
	*       2.����cellSizeInCurrentPage�������õ�ǰ��Ԫ��ǰһ����Ԫƫ�Ƽ�ȥ
	*         ��ǰ��Ԫ��ƫ�ƣ�������������ѵ�һ����Ԫ������ҳ��С������
	************************************************************************/
	SQLiteCell(IN const BYTE * bytes, IN const BYTE * rawData, IN const UINT pageSize, IN const UINT cellSizeInCurrentPage);

    /************************************************************************
    * ���ܣ������������ͷ���Դ��
    * �������ޡ�
    * ���أ��ޡ�
    ************************************************************************/
    virtual ~SQLiteCell();
};

#endif
