//
// @Project : SQLiteMiniReader
// @File    : SQLiteTools.cpp
// @Date    : 2016-02-20
// @Author  : wPgg1es
//

#include "SQLiteTools.h"

/************************************************************************
* ���ܣ����䳤����ת��ΪС��64λ���ȵĳ����͡�
* ������
*       bytes         [IN]  -> Ҫת�����ֽ����顣
*       useBytesCount [OUT] -> ���ر䳤������ʹ�õ��ֽ�����
* ���أ�ת����ĳ����͡�
************************************************************************/
ULONG SQLiteTools::VolatileLong(IN const BYTE * bytes, OUT int * useBytesCount)
{
    ByteNumber value;
    //��մ洢�Ļ���
    value.longValue = 0;

	int index = -1;

    //ͳ�Ʊ䳤����ռ���ֽ���
    while (bytes[++index] & 0x80 && index <= VOLATILE_NUMBER_MAX_LEN);

    if (useBytesCount != NULL)
    {
        *useBytesCount = index + 1;
    }
    
    int valueIndex = 0;

    //���ϵ�һ���ֽڵ�ֵ
    value.byteValue[valueIndex++] = bytes[index--];

    for (; index >= 0; --index, ++valueIndex)
    {
        //��Ϊ��ֹ����Խ�磬�䳤�����������9�ֽڣ�����ֻ����8�ֽڣ����Խ����һ�����벽�赥���ó���
        if (valueIndex < VOLATILE_NUMBER_MAX_LEN - 1)
        {
            //���θ�����һλ��ֵ
            value.byteValue[valueIndex] = (bytes[index] & 0x7F) >> valueIndex;
        }
        //����ǰһλ
        //���ڵ�9�ֽڣ�7λ��ȫ���ϲ���ǰһ�����֣��Լ���ռ�ֽڣ���������ֻ��Ҫ����ǰһ���ֽڼ���
        value.byteValue[valueIndex - 1] |= bytes[index] << (sizeof(ULONG) - valueIndex);
    }

    return value.longValue;
}

/************************************************************************
* ���ܣ������ģʽ������ת��ΪС�����֡�
* ������
*       bytes [IN] -> Ҫת�����ֽ����顣
*       len   [IN] -> Ŀ���������ʹ�С��
* ���أ�ת��������֡�
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
* ���ܣ������ģʽ�ĸ�����ת��ΪС�˸����͡�
* ������
*       bytes [IN] -> Ҫת�����ֽ����顣
* ���أ�ת����ĸ��������֡�
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
* ���ܣ������ģʽ�ĳ�����ת��ΪС�˳����͡�
* ������
*       bytes [IN] -> Ҫת�����ֽ����顣
* ���أ�ת����ĳ����͡�
************************************************************************/
LONG SQLiteTools::BigEndianLong(IN const BYTE * bytes)
{
    return (LONG)BigEndianNumber(bytes, sizeof(ULONG));
}

/************************************************************************
* ���ܣ������ģʽ����ת��ΪС�����͡�
* ������
*       bytes [IN] -> Ҫת�����ֽ����顣
* ���أ�ת��������͡�
************************************************************************/
int SQLiteTools::BigEndianInt(IN const BYTE * bytes)
{
    return (int)BigEndianNumber(bytes, sizeof(UINT));
}

/************************************************************************
* ���ܣ������ģʽ����ת��ΪС�˶����͡�
* ������
*       bytes [IN] -> Ҫת�����ֽ����顣
* ���أ�ת����Ķ����͡�
************************************************************************/
short SQLiteTools::BigEndianShort(IN const BYTE * bytes)
{
    return (short)BigEndianNumber(bytes, sizeof(USHORT));
}


