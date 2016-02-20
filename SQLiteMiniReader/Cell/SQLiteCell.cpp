//
// @Project : SQLiteMiniReader
// @File    : SQLiteCell.cpp
// @Date    : 2016-02-20
// @Author  : wPgg1es
//

#include "SQLiteCell.h"

/************************************************************************
* 功能：私有构造方法，初始化类。
* 参数：
*       data                  [IN] -> 要分析的一个Cell的数据。
*       rawData               [IN] -> 原始数据，即数据库文件起始数据指
*                                     针，用于溢出页处理。
*       pageSize              [IN] -> 数据页大小，用于溢出页处理。
*       cellSizeInCurrentPage [IN] -> 当前单元在当前页已经占用的长度，
*                                     用于溢出页处理。
* 返回：无。
* 备注：
*       1.给定的数据必须完整，超过一个Cell的数据会自动忽略(溢出页除外)。
*       2.对于cellSizeInCurrentPage，可以用当前单元的前一个单元偏移减去
*         当前单元的偏移，计算而出，对已第一个单元可以用页大小来减。
************************************************************************/
SQLiteCell::SQLiteCell(IN const BYTE * bytes, IN const BYTE * rawData, IN const UINT pageSize, IN const UINT cellSizeInCurrentPage)
{
    const BYTE * dataBytes = bytes;
    int offset = 0;
    
    int useBytesCount;

    //先取出标记使用的字节数
    m_UseBytes = SQLiteTools::VolatileLong(dataBytes + offset, &useBytesCount);
    offset += useBytesCount;
    //取出RowID
    m_RowID = SQLiteTools::VolatileLong(dataBytes + offset, &useBytesCount);
    offset += useBytesCount;

    //计算总字节数
    m_UseBytes += offset;
    
    //如果总字节数超过了当前单元在此页中的大小，则说明使用溢出页
    if (m_UseBytes > cellSizeInCurrentPage)
    {
        //先记录当前页的单元的实际数据大小
        UINT tmpOffset = cellSizeInCurrentPage - 4;
        //申请存储全部数据的空间
        dataBytes = new BYTE[(UINT)m_UseBytes];
        memcpy((void *)dataBytes, bytes, tmpOffset);

        //下一个溢出页的页号
        UINT overflowPageIndex = SQLiteTools::BigEndianInt(bytes + tmpOffset);
        
        //循环读取溢出页数据
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
        //利用1个字节的最高位不是1来判断
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
* 功能：析构方法，释放资源。
* 参数：无。
* 返回：无。
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
