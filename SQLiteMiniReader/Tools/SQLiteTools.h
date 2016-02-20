//
// @Project : SQLiteMiniReader
// @File    : SQLiteTools.h
// @Date    : 2016-02-20
// @Author  : wPgg1es
//

#ifndef _SQLITE_TOOLS_H_
#define _SQLITE_TOOLS_H_

#ifndef IN
//输入
#define IN
#endif

#ifndef OUT
//输出
#define OUT
#endif

#ifndef IN_OUT
//输入输出
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

//变长整数最大长度(字节)
#define VOLATILE_NUMBER_MAX_LEN 9

class SQLiteTools
{
private:
	//用于转换数据的联合体
	union ByteNumber
	{
		//存储字节
		BYTE byteValue[sizeof(long long)];
		//8字节对应的浮点型
		double doubleValue;
		//8字节对应的长整型
		ULONGLONG longValue;
		//4字节对应的整型
		UINT intValue;
		//2字节对应的短整型
		USHORT shortValue;
	};
public:
	/************************************************************************
	* 功能：将变长数字转换为小端64位长度的长整型。
	* 参数：
	*       bytes         [IN]  -> 要转换的字节数组。
	*       useBytesCount [OUT] -> 返回变长数字所使用的字节数。
	* 返回：转换后的长整型。
	************************************************************************/
	static ULONGLONG VolatileLong(IN const BYTE * bytes, OUT int * useBytesCount);

	/************************************************************************
	* 功能：将大端模式的数字转换为小端数字。
	* 参数：
	*       bytes [IN] -> 要转换的字节数组。
	*       len   [IN] -> 目标数字类型大小。
	* 返回：转换后的数字。
	************************************************************************/
	static ULONGLONG BigEndianNumber(IN const BYTE * bytes, IN UINT size);

	/************************************************************************
	* 功能：将大端模式的浮点型转换为小端浮点型。
	* 参数：
	*       bytes [IN] -> 要转换的字节数组。
	* 返回：转换后的浮点型数字。
	************************************************************************/
	static double BigEndianDouble(IN const BYTE * bytes);

	/************************************************************************
	* 功能：将大端模式的长整型转换为小端长整型。
	* 参数：
	*       bytes [IN] -> 要转换的字节数组。
	* 返回：转换后的长整型。
	************************************************************************/
	static long long BigEndianLong(IN const BYTE * bytes);

	/************************************************************************
	* 功能：将大端模式整型转换为小端整型。
	* 参数：
	*       bytes [IN] -> 要转换的字节数组。
	* 返回：转换后的整型。
	************************************************************************/
	static int BigEndianInt(IN const BYTE * bytes);

	/************************************************************************
	* 功能：将大端模式整型转换为小端短整型。
	* 参数：
	*       bytes [IN] -> 要转换的字节数组。
	* 返回：转换后的短整型。
	************************************************************************/
	static short BigEndianShort(IN const BYTE * bytes);
};

#endif


