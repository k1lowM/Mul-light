/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/Common.h
	概要		：	共通ヘッダ
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"MLCommon.h"

//カーネル専用ライブラリ
#include	"KLib/KLib.h"


//スーパーグローバルクラス
#include	"MM/GMA.h"
#include	"MM/KMA.h"
#include	"DTM/GDT.h"
#include	"DTM/IDT.h"


#include	<stdio.h>
#include	"GUI/GUI.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数・マクロ定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#define	WORK_BASE		( 0x70000 )		//ワークスペースのベース(パラメータ受け渡しメモリ領域)



//デバッグ用マクロ
#define	DP( ... )		\
	{\
		char	_____char_Debug_String___[256];\
		sprintf( _____char_Debug_String___, __VA_ARGS__ );\
		G_GUI.DebugPrintString( _____char_Debug_String___, 64, Gui_DebugY++, 0xffffffff );\
		Gui_DebugY %= 48;\
		G_GUI.DebugPrintString( "                                                               ", 64, Gui_DebugY, 0xffffffff );\
	}\


/*******************************************************************************
	EFLAGS
*******************************************************************************
#define	EFLAGS_0RESERVED	( 0xffc0802a )		//0で予約されているもの
#define	EFLAGS_1RESERVED	( 0x00000002 )		//1で予約されているもの
#define	EFLAGS_RESERVED( EFLAGS )	( ( ( EFLAGS ) | EFLAGS_1RESERVED ) & ( ~EFLAGS_0RESERVED ) )		//予約ビットを適切に処理するマクロ

#define	EFLAGS_CF		( 0x00000001 )		//状態	キャリーフラグ			最上位ビットでキャリーかボローが生じたときに1がセットされる。
#define EFLAGS_RSV1		( 0x00000002 )		//予約	ビット1(常に1)
#define	EFLAGS_PF		( 0x00000004 )		//状態	パリティフラグ			演算結果の最下位バイトで、値が1のビットが偶数個なら1がセットされる。
#define	EFLAGS_RSV3		( 0x00000008 )		//予約	ビット3(常に0)

#define	EFLAGS_AF		( 0x00000010 )		//状態	補助キャリーフラグ
#define	EFLAGS_RSV5		( 0x00000020 )		//予約	ビット5(常に0)
#define	EFLAGS_ZF		( 0x00000040 )		//状態	ゼロフラグ				演算結果が0のとき1がセットされる。
#define	EFLAGS_SF		( 0x00000080 )		//状態	サインフラグ			演算結果の最上位ビットがそのままセットされる。

#define	EFLAGS_TF		( 0x00000100 )		//状態	トラップフラグ
#define	EFLAGS_IF		( 0x00000200 )		//Sys	割り込みフラグ			1のときは割り込みを許可し、0なら割り込みを禁止する。
#define	EFLAGS_DF		( 0x00000400 )		//制御	方向フラグ				ストリング命令を上位に向かって処理(0)するか、下位に向かって処理(1)するかを制御する。
#define	EFLAGS_OF		( 0x00000800 )		//状態	オーバーフローフラグ	最上位ビットが変化した場合に1がセットされる。

#define	EFLAGS_IOPL		( 0x00003000 )		//Sys	I/O特権レベル(2ビット)
#define	EFLAGS_NT		( 0x00004000 )		//Sys
#define	EFLAGS_RSV15	( 0x00008000 )		//予約	ビット15(常に0)

#define	EFLAGS_RF		( 0x00010000 )		//Sys
#define	EFLAGS_VM		( 0x00020000 )		//Sys	仮想8086モード
#define	EFLAGS_AC		( 0x00040000 )		//Sys
#define	EFLAGS_VIF		( 0x00080000 )		//Sys

#define	EFLAGS_VIP		( 0x00100000 )		//Sys
#define	EFLAGS_ID		( 0x00200000 )		//Sys
#define	EFLAGS_RSV22	( 0x00400000 )		//予約	ビット22(常に0)
#define	EFLAGS_RSV23	( 0x00800000 )		//予約	ビット23(常に0)

#define	EFLAGS_RSV24	( 0x01000000 )		//予約	ビット24(常に0)
#define	EFLAGS_RSV25	( 0x02000000 )		//予約	ビット25(常に0)
#define	EFLAGS_RSV26	( 0x04000000 )		//予約	ビット26(常に0)
#define	EFLAGS_RSV27	( 0x08000000 )		//予約	ビット27(常に0)

#define	EFLAGS_RSV28	( 0x10000000 )		//予約	ビット28(常に0)
#define	EFLAGS_RSV29	( 0x20000000 )		//予約	ビット29(常に0)
#define	EFLAGS_RSV30	( 0x40000000 )		//予約	ビット30(常に0)
#define	EFLAGS_RSV31	( 0x80000000 )		//予約	ビット31(常に0)
*/


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
extern GMA		G_GMA;		//全域メモリ割り当て(Global Memory Allocation)
extern KMA		G_KMA;		//カーネル用メモリ割り当て(Kernel Memory Allocation)

extern GDT		G_GDT;		//グローバル・ディスクリプタ・テーブル(Global Descriptor Table)
extern IDT		G_IDT;		//割り込みディスクリプタ・テーブル(Interrupt Descriptor Table)

extern GUI		G_GUI;		//グラフィカル・ユーザ・インターフェース

extern ui		Gui_DebugY;



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
