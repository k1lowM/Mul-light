/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	vsprintf2.c
	概要		：	vsprintf改良版
	詳細		：	golibのvsprintfが、%dと%sしか対応していないようなので、
					%xとか、各種フラグにまで対応した関数を作っちゃいます。
					浮動小数点数は面倒なのでやらない。
					■対応
					・フラグ：-, +, 半角SP, 0, #
					・文字数：数値, *
					・型指定：h, l
					・変換形式：d, i, o, x, X, u, c, s, %
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*メモ
書式文字列 ： % [フラグ] [文字数] [精度] [ h | l | L ] 変換形式

■フラグ■
-		左詰め（省略時は右詰め）
+		符号付き数値の時、'+' '-' 符号を表示（省略時は-のみ表示）
空白	符号付き数値の時、正の数値の先頭に空白を表示
0		0で桁を埋める（'-'指定時は無効）
#		出力形式の変換
		・変換形式が o の時 : 先頭に 0 を付加
		・変換形式が x(X) の時 : 先頭に 0x(0X) を付加
		・変換形式が f, e(E) の時 : 小数点をつける
		・変換形式が g(G) の時 : 小数点を付け、続く0も表示

■文字数■
数値	出力文字数が設定文字数に足りない時、空白か 0（0フラグ指定時）で埋める
*		引数で指定された文字数	int a=3; int b=4; printf("%*d",a,b);とすると3桁のフィールド幅で4が出力される．(int型)

■精度■浮動小数点数に対応する予定なし。
.数値	小数点以下の桁数
		・変換形式が d,i,o,u,x(X) の時 :
			最低でも数値分の数字を置く（不足時は0詰め）
		・変換形式が f, e(E) の時 :
			小数点以下に数値分の数字を置く
		・変換形式が g(G) の時 :　最大桁数
		・変換形式が s の時 :　　 最大表示文字数
		. ( .0)	小数を非表示
.*	引数で指定された精度

■型指定■
h		変換形式が d,i,o,u,x(X)の時、(unsigned) short型
			変換形式が pの時、short型へのポインタ
l		変換形式が d,i,,o,u,x(X)の時、(unsigned) long型
			変換形式が pの時、long型へのポインタ
L		変換形式が f,e(E),g(G)の時、double型
			変換形式が pの時、double型へのポインタ

■変換形式■
d, i	十進値
o		八進値
x(X)	十六進値
u		符号なし十進値
f		浮動小数点（float, double）
g(G)	fと同じ。ただし以下の時は e(E)と同じ。
		・指数部が-5以下または有効精度を超過
e(E)	指数形式
p		ポインタ（アドレス）
c		文字（char）
s		文字列
n		%nまでの出力文字数を引数（整数型ポインタ）に格納
%		'%' 自身
*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include <stdio.h>
#include <stdarg.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

unsigned long strtoul0(const char **ps, int base, char *errflag);



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	型定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
typedef	signed char				s1;
typedef	signed short			s2;
typedef	signed long int			s4;
typedef	signed long long int	s8;

typedef	unsigned char			u1;
typedef	unsigned short			u2;
typedef	unsigned long int		u4;
typedef	unsigned long long int	u8;



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数・マクロ定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#define	true		( 1 )
#define	false		( 0 )

//型指定
enum	TYPE
{
	TYPE_NON,
	TYPE_ushort,	//h		変換形式が d,i,o,u,x(X)の時、(unsigned) short型		変換形式が pの時、short型へのポインタ
	TYPE_ulong,		//l		変換形式が d,i,,o,u,x(X)の時、(unsigned) long型		変換形式が pの時、long型へのポインタ
	TYPE_double		//L		変換形式が f,e(E),g(G)の時、double型				変換形式が pの時、double型へのポインタ
};

#define	FLAG_ARG_MIN_DIGIT	( 0x00000001 )		//引数で桁数を判断するフラグ(*フラグ)
#define	FLAG_LEFT			( 0x00000002 )		//左詰め（省略時は右詰め）
#define	FLAG_SIGN			( 0x00000004 )		//符号付き数値の時、'+' '-' 符号を表示（省略時は-のみ表示）
#define	FLAG_ALIGN			( 0x00000008 )		//符号付き数値の時、正の数値の先頭に空白を表示
#define	FLAG_ZERO			( 0x00000010 )		//0で桁を埋める（'-'指定時は無効）
#define	FLAG_SHARP			( 0x00000020 )		//出力形式の変換	%oの時先頭に0を付加、%x(X)の時先頭に0x(0X)を付加、%f, %e(E)の時小数点をつける、%g(G)の時小数点を付け、続く0も表示
#define	FLAG_FORMAT_LOOP	( 0x00000040 )		//フォーマット処理ループフラグ


#define	FLAG_ON( _VALUE_, _FLAG_ )		( ( _VALUE_ ) |= ( _FLAG_ ) )
#define	FLAG_OFF( _VALUE_, _FLAG_ )		( ( _VALUE_ ) &= ~( _FLAG_ ) )
#define	FLAG_CHECK( _VALUE_, _FLAG_ )	( ( _VALUE_ ) & ( _FLAG_ ) )



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	プロトタイプ
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
static u4	Value2sDecStr( char* Pc_Dest, s4 s4_Value, u4 u4_MinDigit, u4 u4_Flag );
static u4	Value2uDecStr( char* Pc_Dest, u4 u4_Value, u4 u4_MinDigit, u4 u4_Flag );
static u4	Value2hexstr( char* Pc_Dest, u4 u4_Value, u4 u4_MinDigit, u4 u4_Flag );
static u4	Value2HEXSTR( char* Pc_Dest, u4 u4_Value, u4 u4_MinDigit, u4 u4_Flag );



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	関数
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	整数を8進数文字列に変換
*******************************************************************************/
static u4	Value2OctStr( char* Pc_Dest, u4 u4_Value, u4 u4_MinDigit, u4 u4_Flag )
{
	s4		s4_DigitCount;
	s4		s4_MinDigit = u4_MinDigit;
	u4		u4_DestCount = 0;
	char	c_Work[16] = "0";		//Max:10桁
	char	c_ZeroSpace = ' ';

	//シグネチャ(0)の付加
	if( FLAG_CHECK( u4_Flag, FLAG_SHARP ) )
	{
		Pc_Dest[u4_DestCount++] = '0';
		Pc_Dest[u4_DestCount++] = 'o';
	}

	//桁埋めする値を'0'か' 'か決める
	if( FLAG_CHECK( u4_Flag, FLAG_ZERO ) )
		c_ZeroSpace = '0';

	if( u4_Value == 0 )
	{
		c_Work[0] = '0';
		s4_DigitCount = 1;
	}
	else
	{
		//値を8進数文字列に変換(逆順)
		for( s4_DigitCount = 0; u4_Value; s4_DigitCount++ )
		{
			c_Work[s4_DigitCount] = ( u4_Value & 0x7 ) + '0';
			u4_Value >>= 3;
		}
	}

	//文字列出力
	//桁埋め
	if( !FLAG_CHECK( u4_Flag, FLAG_LEFT ) )
		for( ; s4_MinDigit > s4_DigitCount; s4_MinDigit-- )
			Pc_Dest[u4_DestCount++] = c_ZeroSpace;

	//文字列出力
	for( s4_DigitCount--; s4_DigitCount >= 0; s4_DigitCount--, s4_MinDigit-- )
		Pc_Dest[u4_DestCount++] = c_Work[s4_DigitCount];

	//桁埋め
	if( FLAG_CHECK( u4_Flag, FLAG_LEFT ) )
		for( ; s4_MinDigit > 0; s4_MinDigit-- )
			Pc_Dest[u4_DestCount++] = ' ';

	return u4_DestCount;
}


/*******************************************************************************
	整数を符号付き10進数文字列に変換
*******************************************************************************/
static u4	Value2sDecStr( char* Pc_Dest, s4 s4_Value, u4 u4_MinDigit, u4 u4_Flag )
{
	s4		s4_DigitCount;
	s4		s4_MinDigit = u4_MinDigit;
	u4		u4_DestCount = 0;
	char	c_Work[16] = "0";		//42億 = 10桁
	char	c_ZeroSpace = ' ';

	//符号の処理
	if( s4_Value < 0 )
		Pc_Dest[u4_DestCount++] = '-';
	else if( FLAG_CHECK( u4_Flag, FLAG_SIGN ) )
		Pc_Dest[u4_DestCount++] = '+';
	else if( FLAG_CHECK( u4_Flag, FLAG_SIGN ) )
		Pc_Dest[u4_DestCount++] = ' ';

	//桁埋めする値を'0'か' 'か決める
	if( FLAG_CHECK( u4_Flag, FLAG_ZERO ) )
		c_ZeroSpace = '0';

	if( s4_Value == 0 )
	{
		c_Work[0] = '0';
		s4_DigitCount = 1;
	}
	else
	{
		//値を10進数文字列に変換(逆順)
		for( s4_DigitCount = 0; s4_Value; s4_DigitCount++ )
		{
			c_Work[s4_DigitCount] = abs( s4_Value % 10 ) + '0';
			s4_Value /= 10;
		}
	}


	//文字列出力
	//桁埋め
	if( !FLAG_CHECK( u4_Flag, FLAG_LEFT ) )
		for( ; s4_MinDigit > s4_DigitCount; s4_MinDigit-- )
			Pc_Dest[u4_DestCount++] = c_ZeroSpace;

	//文字列出力
	for( s4_DigitCount--; s4_DigitCount >= 0; s4_DigitCount--, s4_MinDigit-- )
		Pc_Dest[u4_DestCount++] = c_Work[s4_DigitCount];

	//桁埋め
	if( FLAG_CHECK( u4_Flag, FLAG_LEFT ) )
		for( ; s4_MinDigit > 0; s4_MinDigit-- )
			Pc_Dest[u4_DestCount++] = ' ';

	return u4_DestCount;
}

/*******************************************************************************
	整数を符号なし10進数文字列に変換
*******************************************************************************/
static u4	Value2uDecStr( char* Pc_Dest, u4 u4_Value, u4 u4_MinDigit, u4 u4_Flag )
{
	s4		s4_DigitCount;
	s4		s4_MinDigit = u4_MinDigit;
	u4		u4_DestCount = 0;
	char	c_Work[16] = "0";		//42億 = 10桁
	char	c_ZeroSpace = ' ';

	//桁埋めする値を'0'か' 'か決める
	if( FLAG_CHECK( u4_Flag, FLAG_ZERO ) )
		c_ZeroSpace = '0';

	if( u4_Value == 0 )
	{
		c_Work[0] = '0';
		s4_DigitCount = 1;
	}
	else
	{
		//値を10進数文字列に変換(逆順)
		for( s4_DigitCount = 0; u4_Value; s4_DigitCount++ )
		{
			c_Work[s4_DigitCount] = ( u4_Value % 10 ) + '0';
			u4_Value /= 10;
		}
	}

	//文字列出力
	//桁埋め
	if( !FLAG_CHECK( u4_Flag, FLAG_LEFT ) )
		for( ; s4_MinDigit > s4_DigitCount; s4_MinDigit-- )
			Pc_Dest[u4_DestCount++] = c_ZeroSpace;

	//文字列出力
	for( s4_DigitCount--; s4_DigitCount >= 0; s4_DigitCount--, s4_MinDigit-- )
		Pc_Dest[u4_DestCount++] = c_Work[s4_DigitCount];

	//桁埋め
	if( FLAG_CHECK( u4_Flag, FLAG_LEFT ) )
		for( ; s4_MinDigit > 0; s4_MinDigit-- )
			Pc_Dest[u4_DestCount++] = ' ';

	return u4_DestCount;
}


/*******************************************************************************
	整数を16進数文字列に変換(小文字)
*******************************************************************************/
static u4	Value2hexstr( char* Pc_Dest, u4 u4_Value, u4 u4_MinDigit, u4 u4_Flag )
{
	s4		s4_DigitCount;
	s4		s4_MinDigit = u4_MinDigit;
	u4		u4_DestCount = 0;
	char	c_Work[16] = "0";		//Max:8桁
	char	c_ZeroSpace = ' ';

	//シグネチャ(0x)の付加
	if( FLAG_CHECK( u4_Flag, FLAG_SHARP ) )
	{
		Pc_Dest[u4_DestCount++] = '0';
		Pc_Dest[u4_DestCount++] = 'x';
	}

	//桁埋めする値を'0'か' 'か決める
	if( FLAG_CHECK( u4_Flag, FLAG_ZERO ) )
		c_ZeroSpace = '0';

	if( u4_Value == 0 )
	{
		c_Work[0] = '0';
		s4_DigitCount = 1;
	}
	else
	{
		//値を16進数文字列に変換(逆順)
		for( s4_DigitCount = 0; u4_Value; s4_DigitCount++ )
		{
			c_Work[s4_DigitCount] = ( u4_Value & 0xf );
			u4_Value >>= 4;
			if( c_Work[s4_DigitCount] < 10 )
				c_Work[s4_DigitCount] += '0';
			else
				c_Work[s4_DigitCount] += 'a' - 10;
		}
	}

	//文字列出力
	//桁埋め
	if( !FLAG_CHECK( u4_Flag, FLAG_LEFT ) )
		for( ; s4_MinDigit > s4_DigitCount; s4_MinDigit-- )
			Pc_Dest[u4_DestCount++] = c_ZeroSpace;

	//文字列出力
	for( s4_DigitCount--; s4_DigitCount >= 0; s4_DigitCount--, s4_MinDigit-- )
		Pc_Dest[u4_DestCount++] = c_Work[s4_DigitCount];

	//桁埋め
	if( FLAG_CHECK( u4_Flag, FLAG_LEFT ) )
		for( ; s4_MinDigit > 0; s4_MinDigit-- )
			Pc_Dest[u4_DestCount++] = ' ';

	return u4_DestCount;
}


/*******************************************************************************
	整数を16進数文字列に変換(大文字)
*******************************************************************************/
static u4	Value2HEXSTR( char* Pc_Dest, u4 u4_Value, u4 u4_MinDigit, u4 u4_Flag )
{
	s4		s4_DigitCount;
	s4		s4_MinDigit = u4_MinDigit;
	u4		u4_DestCount = 0;
	char	c_Work[16] = "0";		//Max:8桁
	char	c_ZeroSpace = ' ';

	//シグネチャ(0X)の付加
	if( FLAG_CHECK( u4_Flag, FLAG_SHARP ) )
	{
		Pc_Dest[u4_DestCount++] = '0';
		Pc_Dest[u4_DestCount++] = 'X';
	}

	//桁埋めする値を'0'か' 'か決める
	if( FLAG_CHECK( u4_Flag, FLAG_ZERO ) )
		c_ZeroSpace = '0';

	if( u4_Value == 0 )
	{
		c_Work[0] = '0';
		s4_DigitCount = 1;
	}
	else
	{
		//値を16進数文字列に変換(逆順)
		for( s4_DigitCount = 0; u4_Value; s4_DigitCount++ )
		{
			c_Work[s4_DigitCount] = ( u4_Value & 0xf );
			u4_Value >>= 4;
			if( c_Work[s4_DigitCount] < 10 )
				c_Work[s4_DigitCount] += '0';
			else
				c_Work[s4_DigitCount] += 'A' - 10;
		}
	}

	//文字列出力
	//桁埋め
	if( !FLAG_CHECK( u4_Flag, FLAG_LEFT ) )
		for( ; s4_MinDigit > s4_DigitCount; s4_MinDigit-- )
			Pc_Dest[u4_DestCount++] = c_ZeroSpace;

	//文字列出力
	for( s4_DigitCount--; s4_DigitCount >= 0; s4_DigitCount-- )
		Pc_Dest[u4_DestCount++] = c_Work[s4_DigitCount];

	//桁埋め
	if( FLAG_CHECK( u4_Flag, FLAG_LEFT ) )
		for( ; s4_MinDigit > s4_DigitCount; s4_MinDigit-- )
			Pc_Dest[u4_DestCount++] = ' ';

	return u4_DestCount;
}




/*******************************************************************************
	vsprintf
*******************************************************************************/
int		vsprintf( char *s, const char *format, va_list arg )
{
	char*		Pc_Dest		= s;		//格納先
	const char*	CPc_Source	= format;	//書式付き文字列

	s4		s4_ValueBuf;		//格納する値のバッファ
	char*	Pc_String;			//%s用の文字列ポインタ

	u4		u4_MinDigit = 0;	//最小の桁数。%8dとかの8。約42億桁まで対応ww
	u4		u4_Flag = 0;		//各種フラグ
	u1		u1_Type = 0;		//型指定

	char	c_CharBuf = true;	//1文字格納バッファ



	//文字列の終端が見つかるまで実行し続ける。
	while( c_CharBuf != 0 )
	{
		//文字取得
		c_CharBuf = *CPc_Source++;

		//フラグ類初期化
		s4_ValueBuf = u4_MinDigit = u4_Flag = u1_Type = 0;

		//%でなければそのまま格納して次へ。
		if( c_CharBuf != '%' )
		{
			*Pc_Dest++ = c_CharBuf;
			continue;
		}

		//以下、%だった場合の処理
		FLAG_ON( u4_Flag, FLAG_FORMAT_LOOP );
		while( c_CharBuf != 0 && FLAG_CHECK( u4_Flag, FLAG_FORMAT_LOOP ) )
		{
			//文字読み込み
			c_CharBuf = *CPc_Source++;

			//フラグ指定の前に、文字数や型が指定してあれば、ループを抜ける
			if( ( u4_MinDigit || u1_Type ) && ( c_CharBuf == '-' || c_CharBuf == '+' || c_CharBuf == 0x20 || c_CharBuf == '#' ) )
				break;

			switch( c_CharBuf )
			{
			/*■フラグ■*/
			//左詰め（省略時は右詰め）
			case '-':
				FLAG_ON( u4_Flag, FLAG_LEFT);		//左詰めフラグ
				break;
			//符号付き数値の時、'+' '-' 符号を表示（省略時は-のみ表示）
			case '+':
				FLAG_ON( u4_Flag, FLAG_SIGN );		//符号フラグ
				break;
			//符号付き数値の時、正の数値の先頭に空白を表示(負の数のときの-符号と位置を合わせる)
			case 0x20:
				FLAG_ON( u4_Flag, FLAG_ALIGN );	//空白フラグ
				break;
			//出力形式の変換	%oの時先頭に0を付加、%x(X)の時先頭に0x(0X)を付加、%f, %e(E)の時小数点をつける、%g(G)の時小数点を付け、続く0も表示
			case '#':
				FLAG_ON( u4_Flag, FLAG_SHARP );	//出力形式フラグ
				break;


			/*■文字数■*/
			//出力文字数が設定文字数に足りない時、空白か 0（0フラグ指定時）で埋める
			case '0':	//文字数兼フラグ
				//0フラグ
				if( !u4_MinDigit )
				{
					if( FLAG_CHECK( u4_Flag, FLAG_ZERO ) )
						FLAG_OFF( u4_Flag, FLAG_FORMAT_LOOP );
					FLAG_ON( u4_Flag, FLAG_ZERO );		//0フラグ
					break;
				}
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				//文字数指定の前に、型指定や引数で桁数を指定するフラグがあれば、ループを抜ける
				if( u1_Type || FLAG_CHECK( u4_Flag, FLAG_ARG_MIN_DIGIT ) )
				{
					FLAG_OFF( u4_Flag, FLAG_FORMAT_LOOP );
					break;
				}

				u4_MinDigit *= 10;
				u4_MinDigit += c_CharBuf - '0';
				break;
			//引数で指定された文字数	int a=3; int b=4; printf("%*d",a,b);とすると3桁のフィールド幅で4が出力される．
			case '*':
				//引数で桁数を指定するフラグの前に、文字数指定があれば、ループを抜ける
				if( u4_MinDigit )
				{
					FLAG_OFF( u4_Flag, FLAG_FORMAT_LOOP );
					break;
				}

				FLAG_ON( u4_Flag, FLAG_ARG_MIN_DIGIT );
				u4_MinDigit = va_arg( arg, int );		//桁数の取得
				break;


			/*■型指定■*/
			//変換形式が d,i,o,u,x(X)の時、(unsigned) short型		変換形式が pの時、short型へのポインタ
			case 'h':
				u1_Type = TYPE_ushort;
				break;
			//変換形式が d,i,o,u,x(X)の時、(unsigned) long型		変換形式が pの時、long型へのポインタ
			case 'l':
				u1_Type = TYPE_ulong;
				break;


			/*■変換形式■*/
			//8進数
			case 'o':
				//値の取得
				if( u1_Type == TYPE_ushort )
				{
					s4_ValueBuf = va_arg( arg, int );
					s4_ValueBuf &= 0xffff;
				}
				else
					s4_ValueBuf = va_arg( arg, int );

				Pc_Dest += Value2OctStr( Pc_Dest, s4_ValueBuf, u4_MinDigit, u4_Flag );
				FLAG_OFF( u4_Flag, FLAG_FORMAT_LOOP );
				break;
			//10進数
			case 'd':
			case 'i':
				//値の取得
				if( u1_Type == TYPE_ushort )
				{
					s4_ValueBuf = va_arg( arg, int );
					s4_ValueBuf &= 0xffff;
				}
				else
					s4_ValueBuf = va_arg( arg, int );

				Pc_Dest += Value2sDecStr( Pc_Dest, s4_ValueBuf, u4_MinDigit, u4_Flag );
				FLAG_OFF( u4_Flag, FLAG_FORMAT_LOOP );
				break;
			//符号なし10進数
			case 'u':
				//値の取得
				if( u1_Type == TYPE_ushort )
				{
					s4_ValueBuf = va_arg( arg, int );
					s4_ValueBuf &= 0xffff;
				}
				else
					s4_ValueBuf = va_arg( arg, int );

				Pc_Dest += Value2uDecStr( Pc_Dest, s4_ValueBuf, u4_MinDigit, u4_Flag );
				FLAG_OFF( u4_Flag, FLAG_FORMAT_LOOP );
				break;
			//16進数小文字
			case 'x':
				//値の取得
				if( u1_Type == TYPE_ushort )
				{
					s4_ValueBuf = va_arg( arg, int );
					s4_ValueBuf &= 0xffff;
				}
				else
					s4_ValueBuf = va_arg( arg, int );

				Pc_Dest += Value2hexstr( Pc_Dest, s4_ValueBuf, u4_MinDigit, u4_Flag );
				FLAG_OFF( u4_Flag, FLAG_FORMAT_LOOP );
				break;
			//16進数大文字
			case 'X':
				//値の取得
				if( u1_Type == TYPE_ushort )
				{
					s4_ValueBuf = va_arg( arg, int );
					s4_ValueBuf &= 0xffff;
				}
				else
					s4_ValueBuf = va_arg( arg, int );

				Pc_Dest += Value2HEXSTR( Pc_Dest, s4_ValueBuf, u4_MinDigit, u4_Flag );
				FLAG_OFF( u4_Flag, FLAG_FORMAT_LOOP );
				break;
			//一文字
			case 'c':
				s4_ValueBuf = va_arg( arg, int );
				*Pc_Dest++ = (char)s4_ValueBuf;
				FLAG_OFF( u4_Flag, FLAG_FORMAT_LOOP );
				break;
			//文字列
			case 's':
				Pc_String = va_arg( arg, char* );
				while( *Pc_String )
					*Pc_Dest++ = *Pc_String++;
				FLAG_OFF( u4_Flag, FLAG_FORMAT_LOOP );
				break;
			//ポインタ
//			case 'p':
//				FLAG_OFF( u4_Flag, FLAG_FORMAT_LOOP );
//				break;
			//%nまでの出力文字数を引数（整数型ポインタ）に格納
//			case 'n':
//				FLAG_OFF( u4_Flag, FLAG_FORMAT_LOOP );
//				break;
			//%出力
			case '%':
				//%%なら%を格納する
				if( *( CPc_Source - 1 ) == '%' )
					*Pc_Dest++ = c_CharBuf;
				FLAG_OFF( u4_Flag, FLAG_FORMAT_LOOP );
				break;

			//その他
			default:
				FLAG_OFF( u4_Flag, FLAG_FORMAT_LOOP );
				break;
			}
		}
	}

	*Pc_Dest++ = 0;
	return (int)( Pc_Dest - s - 1 );
}
