/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/KLib/IO.cpp
	概要		：	I/O関数群
	詳細		：	周辺装置のインプット・アウトプットを行う関数群です。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"IO.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	I/Oクラス：静的パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	周辺装置入力
	説明	：	指定のポートからデータを取得します。
	Include	：	IO.h
	引数	：	u2 u2_Port		受信するためのポート番号
	戻り値	：	u1/u2/u4		受信したデータ
*******************************************************************************/
u1		IO::In1( u2 u2_Port )
{
	u1		u1_Dest;
	__asm__
	(
			"inb	%1,		%0;"
			: "=a"( u1_Dest )
			: "d"( u2_Port )
	);
	return u1_Dest;
}
u2		IO::In2( u2 u2_Port )
{
	u2		u2_Dest;
	__asm__
	(
			"inw	%1,		%0;"
			: "=a"( u2_Dest )
			: "d"( u2_Port )
	);
	return u2_Dest;
}
u4		IO::In4( u2 u2_Port )
{
	u4		u4_Dest;
	__asm__
	(
			"inl	%1,		%0;"
			: "=a"( u4_Dest )
			: "d"( u2_Port )
	);
	return u4_Dest;
}


/*******************************************************************************
	概要	：	周辺装置連続入力
	説明	：	指定のポートから指定回数分データを受け取り、バッファに格納します。
	Include	：	IO.h
	引数	：	u2 u2_Port		受信するためのポート番号
				void* Pv_Dest	格納先バッファ
				ui ui_Count		入力回数
	戻り値	：	void
*******************************************************************************/
void	IO::Ins1( u2 u2_Port, void* Pv_Dest, ui ui_Count )
{
	__asm__
	(
			"rep insb;"
			:
			: "c"( ui_Count ), "d"( u2_Port ), "D"( Pv_Dest )
	);
}
void	IO::Ins2( u2 u2_Port, void* Pv_Dest, ui ui_Count )
{
	__asm__
	(
			"rep insw;"
			:
			: "c"( ui_Count ), "d"( u2_Port ), "D"( Pv_Dest )
	);
}
void	IO::Ins4( u2 u2_Port, void* Pv_Dest, ui ui_Count )
{
	__asm__
	(
			"rep insl;"
			:
			: "c"( ui_Count ), "d"( u2_Port ), "D"( Pv_Dest )
	);
}


/*******************************************************************************
	概要	：	周辺装置出力
	説明	：	指定のポートへデータを出力します。
	Include	：	IO.h
	引数	：	u2 u2_Port			送信するためのポート番号
				u1/2/4 u1/2/4_Src	送信するデータ
	戻り値	：	void
*******************************************************************************/
void	IO::Out1( u2 u2_Port, u1 u1_Src )
{
	__asm__
	(
			"outb	%1,		%0;"
			:
			: "d"( u2_Port ), "a"( u1_Src )
	);
}
void	IO::Out2( u2 u2_Port, u2 u2_Src )
{
	__asm__
	(
			"outw	%1,		%0;"
			:
			: "d"( u2_Port ), "a"( u2_Src )
	);
}
void	IO::Out4( u2 u2_Port, u4 u4_Src )
{
	__asm__
	(
			"outl	%1,		%0;"
			:
			: "d"( u2_Port ), "a"( u4_Src )
	);
}


/*******************************************************************************
	概要	：	周辺装置連続出力
	説明	：	バッファから指定のポートへ指定回数分データを出力します。
	Include	：	IO.h
	引数	：	u2 u2_Port			出力するためのポート番号
				void* Pv_Src		出力データのバッファ
				ui ui_Count			出力回数
	戻り値	：	void
*******************************************************************************/
void	IO::Outs1( u2 u2_Port, void* Pv_Src, ui ui_Count )
{
	__asm__
	(
			"rep outsb;"
			:
			: "c"( ui_Count ), "d"( u2_Port ), "S"( Pv_Src )
	);
}
void	IO::Outs2( u2 u2_Port, void* Pv_Src, ui ui_Count )
{
	__asm__
	(
			"rep outsw;"
			:
			: "c"( ui_Count ), "d"( u2_Port ), "S"( Pv_Src )
	);
}
void	IO::Outs4( u2 u2_Port, void* Pv_Src, ui ui_Count )
{
	__asm__
	(
			"rep outsl;"
			:
			: "c"( ui_Count ), "d"( u2_Port ), "S"( Pv_Src )
	);
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

