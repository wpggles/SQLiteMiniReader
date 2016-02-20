//
// @Project : SQLiteMiniReader
// @File    : SQLiteCell.cpp
// @Date    : 2016-02-20
// @Author  : wPgg1es
//

#include "SQLiteCell.h"

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
SQLiteCell::SQLiteCell(IN const BYTE * bytes, IN const BYTE * rawData, IN const UINT pageSize, IN const UINT cellSizeInCurrentPage)
{
    const BYTE * dataBytes = bytes;
    int offset = 0;
    
    int useBytesCount;

    //��ȡ�����ʹ�õ��ֽ���
    m_UseBytes = SQLiteTools::VolatileLong(dataBytes + offset, &useBytesCount);
    offset += useBytesCount;
    //ȡ��RowID
    m_RowID = SQLiteTools::VolatileLong(dataBytes + offset, &useBytesCount);
    offset += useBytesCount;

    //�������ֽ���
    m_UseBytes += offset;
    
    //������ֽ��������˵�ǰ��Ԫ�ڴ�ҳ�еĴ�С����˵��ʹ�����ҳ
    if (m_UseBytes > cellSizeInCurrentPage)
    {
        //�ȼ�¼��ǰҳ�ĵ�Ԫ��ʵ�����ݴ�С
        UINT tmpOffset = cellSizeInCurrentPage - 4;
        //����洢ȫ�����ݵĿռ�
        dataBytes = new BYTE[(UINT)m_UseBytes];
        memcpy((void *)dataBytes, bytes, tmpOffset);

        //��һ�����ҳ��ҳ��
        UINT overflowPageIndex = SQLiteTools::BigEndianInt(bytes + tmpOffset);
        
        //ѭ����ȡ���ҳ����
        while (overflowPageIndex)
        {
            const BYTE * overflowData = rawData + pageSize * (overflowPageIndex - 1);
            overflowPageIndex = SQLiteTools::BigEndianInt(overflowData);
            overflowData += 4;
            UINT currentReadCount = ((UINT)m_UseBytes - tmpOffset) > (pageSize - 4) ? pageSize - 4 : (UINT)m_UseBytes - tmpOffset;
            memcpy((void *)(dataBytes + tmpOffset), overflowData, currentReadCount);
            tmpOffset += currentReadCount;
        }
    }

    int feildTypeUseBytes = (int)SQLiteTools::VolatileLong(dataBytes + offset, &useBytesCount);
    int tmpOffset = offset + feildTypeUseBytes;
    offset += useBytesCount;

    m_ColumnCount = 0;

    while (--tmpOffset >= offset)
    {
        //����1���ֽڵ����λ����1���ж�
        if (!(*(dataBytes + tmpOffset) & 0x80))
        {
            m_ColumnCount++;
        }
    }

    m_Types = new ULONGLONG[(int)m_ColumnCount];
    m_Data = new BYTE *[(int)m_ColumnCount];
    for (int i = 0; i < (int)m_ColumnCount; i++)
    {
        m_Types[i] = SQLiteTools::VolatileLong(dataBytes + offset, &useBytesCount);
        offset += useBytesCount;
    }
    for (int i = 0; i < (int)m_ColumnCount; i++)
    {
        switch (m_Types[i])
        {
            case 0:
            case 8:
            case 9:
                m_Data[i] = NULL;
                break;
            case 1:
            case 2:
            case 3:
            case 4:
                m_Data[i] = new BYTE[(int)m_Types[i]];
                memcpy(m_Data[i], dataBytes + offset, (int)m_Types[i]);
                offset += (int)m_Types[i];
                break;
            case 5:
                m_Data[i] = new BYTE[6];
                memcpy(m_Data[i], dataBytes + offset, 6);
                offset += 6;
                break;
            case 6:
            case 7:
                m_Data[i] = new BYTE[8];
                memcpy(m_Data[i], dataBytes + offset, 8);
                offset += 8;
                break;
            default:
                if (IS_BLOB_FIELD(m_Types[i]))
                {
                    //BLOB
                    int len = BLOB_LENGHT(m_Types[i]);
                    m_Data[i] = new BYTE[len];
                    memcpy(m_Data[i], dataBytes + offset, len);
                    offset += len;
                }
                else if (IS_TEXT_FIELD(m_Types[i]))
                {
                    //TEXT
                    int len = TEXT_LENGHT(m_Types[i]);
                    m_Data[i] = new BYTE[len];
                    memcpy(m_Data[i], dataBytes + offset, len);
                    offset += len;
                }
                break;
        }
    }
    if (m_UseBytes > cellSizeInCurrentPage)
    {
        delete[] dataBytes;
    }
}

/************************************************************************
* ���ܣ������������ͷ���Դ��
* �������ޡ�
* ���أ��ޡ�
************************************************************************/
SQLiteCell::~SQLiteCell()
{
    for (int i = 0; i < m_ColumnCount; ++i)
    {
        if (m_Data[i])
        {
            delete[] m_Data[i];
            m_Data[i] = NULL;
        }
    }
    delete[] m_Data;
    m_Data = NULL;
    delete[] m_Types;
    m_Types = NULL;
}
