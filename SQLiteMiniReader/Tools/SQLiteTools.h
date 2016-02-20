//
// @Project : SQLiteMiniReader
// @File    : SQLiteTools.h
// @Date    : 2016-02-20
// @Author  : wPgg1es
//

#ifndef _SQLITE_TOOLS_H_
#define _SQLITE_TOOLS_H_

#ifndef IN
//����
#define IN
#endif

#ifndef OUT
//���
#define OUT
#endif

#ifndef IN_OUT
//�������
#define IN_OUT
#endif

/* Define NULL pointer value */
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else  /* __cplusplus */
#define NULL    ((void *)0)
#endif  /* __cplusplus */
#endif  /* NULL */

#ifndef BYTE
#define BYTE unsigned char
#endif

#ifndef USHORT
#define USHORT unsigned short
#endif

#ifndef UINT
#define UINT unsigned int
#endif

#ifndef ULONGLONG
#define ULONGLONG unsigned long long
#endif

#ifndef BOOL
#define BOOL int
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

//�䳤������󳤶�(�ֽ�)
#define VOLATILE_NUMBER_MAX_LEN 9

class SQLiteTools
{
private:
	//����ת�����ݵ�������
	union ByteNumber
	{
		//�洢�ֽ�
		BYTE byteValue[sizeof(long long)];
		//8�ֽڶ�Ӧ�ĸ�����
		double doubleValue;
		//8�ֽڶ�Ӧ�ĳ�����
		ULONGLONG longValue;
		//4�ֽڶ�Ӧ������
		UINT intValue;
		//2�ֽڶ�Ӧ�Ķ�����
		USHORT shortValue;
	};
public:
	/************************************************************************
	* ���ܣ����䳤����ת��ΪС��64λ���ȵĳ����͡�
	* ������
	*       bytes         [IN]  -> Ҫת�����ֽ����顣
	*       useBytesCount [OUT] -> ���ر䳤������ʹ�õ��ֽ�����
	* ���أ�ת����ĳ����͡�
	************************************************************************/
	static ULONGLONG VolatileLong(IN const BYTE * bytes, OUT int * useBytesCount);

	/************************************************************************
	* ���ܣ������ģʽ������ת��ΪС�����֡�
	* ������
	*       bytes [IN] -> Ҫת�����ֽ����顣
	*       len   [IN] -> Ŀ���������ʹ�С��
	* ���أ�ת��������֡�
	************************************************************************/
	static ULONGLONG BigEndianNumber(IN const BYTE * bytes, IN UINT size);

	/************************************************************************
	* ���ܣ������ģʽ�ĸ�����ת��ΪС�˸����͡�
	* ������
	*       bytes [IN] -> Ҫת�����ֽ����顣
	* ���أ�ת����ĸ��������֡�
	************************************************************************/
	static double BigEndianDouble(IN const BYTE * bytes);

	/************************************************************************
	* ���ܣ������ģʽ�ĳ�����ת��ΪС�˳����͡�
	* ������
	*       bytes [IN] -> Ҫת�����ֽ����顣
	* ���أ�ת����ĳ����͡�
	************************************************************************/
	static long long BigEndianLong(IN const BYTE * bytes);

	/************************************************************************
	* ���ܣ������ģʽ����ת��ΪС�����͡�
	* ������
	*       bytes [IN] -> Ҫת�����ֽ����顣
	* ���أ�ת��������͡�
	************************************************************************/
	static int BigEndianInt(IN const BYTE * bytes);

	/************************************************************************
	* ���ܣ������ģʽ����ת��ΪС�˶����͡�
	* ������
	*       bytes [IN] -> Ҫת�����ֽ����顣
	* ���أ�ת����Ķ����͡�
	************************************************************************/
	static short BigEndianShort(IN const BYTE * bytes);
};

#endif


