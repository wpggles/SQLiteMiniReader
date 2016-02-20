//
// @Project : SQLiteMiniReader
// @File    : SQLiteTools.cpp
// @Date    : 2016-02-20
// @Author  : wPgg1es
//

#include "SQLiteTools.h"

/************************************************************************
* 功能：将变长数字转换为小端64位长度的长整型。
* 参数：
*       bytes         [IN]  -> 要转换的字节数组。
*       useBytesCount [OUT] -> 返回变长数字所使用的字节数。
* 返回：转换后的长整型。
************************************************************************/
ULONG SQLiteTools::VolatileLong(IN const BYTE * bytes, OUT int * useBytesCount)
{
    ByteNumber value;
    //清空存储的缓存
    value.longValue = 0;

	int index = -1;

    //统计变长数字占用字节数
    while (bytes[++index] & 0x80 && index <= VOLATILE_NUMBER_MAX_LEN);

    if (useBytesCount != NULL)
    {
        *useBytesCount = index + 1;
    }
    
    int valueIndex = 0;

    //赋上第一个字节的值
    value.byteValue[valueIndex++] = bytes[index--];

    for (; index >= 0; --index, ++valueIndex)
    {
        //因为防止数组越界，变长整型最长可以是9字节，这里只定义8字节，所以将最后一步补齐步骤单独拿出来
        if (valueIndex < VOLATILE_NUMBER_MAX_LEN - 1)
        {
            //依次赋上下一位的值
            value.byteValue[valueIndex] = (bytes[index] & 0x7F) >> valueIndex;
        }
        //补齐前一位
        //对于第9字节，7位将全部合并到前一个数字，自己不占字节，所以这里只需要补齐前一个字节即可
        value.byteValue[valueIndex - 1] |= bytes[index] << (sizeof(ULONG) - valueIndex);
    }

    return value.longValue;
}

/************************************************************************
* 功能：将大端模式的数字转换为小端数字。
* 参数：
*       bytes [IN] -> 要转换的字节数组。
*       len   [IN] -> 目标数字类型大小。
* 返回：转换后的数字。
************************************************************************/
ULONG SQLiteTools::BigEndianNumber(IN const BYTE * bytes, IN UINT size)
{
    ByteNumber value;
    value.longValue = 0;
    int index = size - 1;
    for (int i = 0; i <= index; ++i)
    {
        value.byteValue[i] = bytes[index - i];
    }
    return value.longValue;
}

/************************************************************************
* 功能：将大端模式的浮点型转换为小端浮点型。
* 参数：
*       bytes [IN] -> 要转换的字节数组。
* 返回：转换后的浮点型数字。
************************************************************************/
double SQLiteTools::BigEndianDouble(IN const BYTE * bytes)
{
    ByteNumber value;
    value.longValue = 0;
    int index = sizeof(double) - 1;
    for (int i = 0; i <= index; ++i)
    {
        value.byteValue[i] = bytes[index - i];
    }
    return value.doubleValue;
}

/************************************************************************
* 功能：将大端模式的长整型转换为小端长整型。
* 参数：
*       bytes [IN] -> 要转换的字节数组。
* 返回：转换后的长整型。
************************************************************************/
LONG SQLiteTools::BigEndianLong(IN const BYTE * bytes)
{
    return (LONG)BigEndianNumber(bytes, sizeof(ULONG));
}

/************************************************************************
* 功能：将大端模式整型转换为小端整型。
* 参数：
*       bytes [IN] -> 要转换的字节数组。
* 返回：转换后的整型。
************************************************************************/
int SQLiteTools::BigEndianInt(IN const BYTE * bytes)
{
    return (int)BigEndianNumber(bytes, sizeof(UINT));
}

/************************************************************************
* 功能：将大端模式整型转换为小端短整型。
* 参数：
*       bytes [IN] -> 要转换的字节数组。
* 返回：转换后的短整型。
************************************************************************/
short SQLiteTools::BigEndianShort(IN const BYTE * bytes)
{
    return (short)BigEndianNumber(bytes, sizeof(USHORT));
}


