/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/IM/Int0.cpp
	概要		：	例外ルーチン
	詳細		：	例外処理ルーチン群です。
					例外が起こったから何なのよ…何をしろと。。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"IM.h"
#include	"TM.h"

#include	<stdio.h>
#include	<string.h>



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数定義・マクロ
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#define	INFO_REG\
	{\
		Task*	P_NowTask;\
		char	Ac_DebugStr[128];\
		sprintf( Ac_DebugStr, "EAX:%08x, ECX:%08x, EDX:%08x, EBX:%08x", u4_EAX, u4_ECX, u4_EDX, u4_EBX );\
		G_GUI.DebugPrintString( Ac_DebugStr, 0, 1, 0xff00ff00 );\
		sprintf( Ac_DebugStr, "ESP:%08x, EBP:%08x, ESI:%08x, EDI:%08x", u4_ESP, u4_EBP, u4_ESI, u4_EDI );\
		G_GUI.DebugPrintString( Ac_DebugStr, 0, 2, 0xff00ff00 );\
		sprintf( Ac_DebugStr, "DS:%04x, ES:%04x", u2_DS, u2_ES );\
		G_GUI.DebugPrintString( Ac_DebugStr, 0, 3, 0xff0000ff );\
		sprintf( Ac_DebugStr, "CS:%04x, EIP:%08x, EFlags:%08x", u2_CS, u4_EIP, u4_EFlags );\
		G_GUI.DebugPrintString( Ac_DebugStr, 0, 4, 0xffff00ff );\
		P_NowTask = G_TM.GetNowTask();\
		sprintf( Ac_DebugStr, "Task:" );\
		P_NowTask->GetCaption( Ac_DebugStr + strlen( Ac_DebugStr ) );\
		G_GUI.DebugPrintString( Ac_DebugStr, 0, 5, 0xffffffff );\
	}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
extern TM		G_TM;



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	関数
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	除算エラー例外
	説明	：	割り込み番号：00	例外タイプ：フォルト
				0除算や指定されたビットでは除算結果を表せない場合に起こる例外。
	Include	：	IM.h
	引数	：	レジスタの状態 IM.h参照。
	戻り値	：	void
*******************************************************************************/
void	Int00( ARG_REGI_STAT )
{
	G_GUI.DebugPrintString( "ExcInt00 #DE", 0, 0, 0xffff0000 );
	INFO_REG
	while(true)
	{}
}


/*******************************************************************************
	概要	：	デバッグ例外
	説明	：	割り込み番号：01	例外タイプ：トラップまたはフォルト。デバッグレジスタで判断する。
				複数のデバッグ例外条件の1つ以上が検出されたことを表す。
	Include	：	IM.h
	引数	：	レジスタの状態 IM.h参照。
	戻り値	：	void
*******************************************************************************/
void	Int01( ARG_REGI_STAT )
{
	G_GUI.DebugPrintString( "ExcInt01 #DB", 0, 0, 0xffff0000 );
	while(true)
	{}
}


/*******************************************************************************
	概要	：	NMI 割り込み
	説明	：	割り込み番号：02	例外タイプ：割り込み
	Include	：	IM.h
	引数	：	レジスタの状態 IM.h参照。
	戻り値	：	void
*******************************************************************************/
void	Int02( ARG_REGI_STAT )
{
	G_GUI.DebugPrintString( "ExcInt02 NMI", 0, 0, 0xffff0000 );
	while(true)
	{}
}


/*******************************************************************************
	概要	：	ブレークポイント例外
	説明	：	割り込み番号：03	例外タイプ：トラップ
				bbbbbbbbbbbbbbbbbbbbbbbbbb
	Include	：	IM.h
	引数	：	レジスタの状態 IM.h参照。
	戻り値	：	void
*******************************************************************************/
void	Int03( ARG_REGI_STAT )
{
	G_GUI.DebugPrintString( "ExcInt03 #BP", 0, 0, 0xffff0000 );
	while(true)
	{}
}


/*******************************************************************************
	概要	：	オーバーフロー例外
	説明	：	割り込み番号：04	例外タイプ：トラップ
				bbbbbbbbbbbbbbbbbbbbbbbbbb
	Include	：	IM.h
	引数	：	レジスタの状態 IM.h参照。
	戻り値	：	void
*******************************************************************************/
void	Int04( ARG_REGI_STAT )
{
	G_GUI.DebugPrintString( "ExcInt04 #OF", 0, 0, 0xffff0000 );
	while(true)
	{}
}


/*******************************************************************************
	概要	：	BOUND 範囲超過例外
	説明	：	割り込み番号：05	例外タイプ：フォルト
				bbbbbbbbbbbbbbbbbbbbbbbbbb
	Include	：	IM.h
	引数	：	レジスタの状態 IM.h参照。
	戻り値	：	void
*******************************************************************************/
void	Int05( ARG_REGI_STAT )
{
	G_GUI.DebugPrintString( "ExcInt05 #BR", 0, 0, 0xffff0000 );
	while(true)
	{}
}


/*******************************************************************************
	概要	：	無効オペコード例外
	説明	：	割り込み番号：06	例外タイプ：フォルト
				bbbbbbbbbbbbbbbbbbbbbbbbbb
	Include	：	IM.h
	引数	：	レジスタの状態 IM.h参照。
	戻り値	：	void
*******************************************************************************/
void	Int06( ARG_REGI_STAT )
{
	G_GUI.DebugPrintString( "ExcInt06 #UD", 0, 0, 0xffff0000 );
	INFO_REG
	while(true)
	{}
}


/*******************************************************************************
	概要	：	デバイス使用不可例外
	説明	：	割り込み番号：07	例外タイプ：フォルト
				bbbbbbbbbbbbbbbbbbbbbbbbbb
	Include	：	IM.h
	引数	：	レジスタの状態 IM.h参照。
	戻り値	：	void
*******************************************************************************/
void	Int07( ARG_REGI_STAT )
{
	G_GUI.DebugPrintString( "ExcInt07 #NM", 0, 0, 0xffff0000 );
	while(true)
	{}
}


/*******************************************************************************
	概要	：	ダブルフォルト例外
	説明	：	割り込み番号：08	例外タイプ：アボート
				bbbbbbbbbbbbbbbbbbbbbbbbbb
	Include	：	IM.h
	引数	：	レジスタの状態 IM.h参照。
	戻り値	：	void
*******************************************************************************/
void	Int08( ARG_REGI_STAT )
{
	G_GUI.DebugPrintString( "ExcInt08 #DF", 0, 0, 0xffff0000 );
	INFO_REG
	while(true)
	{}
}


/*******************************************************************************
	概要	：	コプロセッサ・セグメント・オーバーラン
	説明	：	割り込み番号：09	例外タイプ：フォルト
				bbbbbbbbbbbbbbbbbbbbbbbbbb
	Include	：	IM.h
	引数	：	レジスタの状態 IM.h参照。
	戻り値	：	void
*******************************************************************************/
void	Int09( ARG_REGI_STAT )
{
	G_GUI.DebugPrintString( "ExcInt09 Coprocessor segment over run", 0, 0, 0xffff0000 );
	while(true)
	{}
}


/*******************************************************************************
	概要	：	無効TSS 例外
	説明	：	割り込み番号：0A	例外タイプ：フォルト
				bbbbbbbbbbbbbbbbbbbbbbbbbb
	Include	：	IM.h
	引数	：	レジスタの状態 IM.h参照。
	戻り値	：	void
*******************************************************************************/
void	Int0A( ARG_REGI_STAT )
{
	G_GUI.DebugPrintString( "ExcInt0A #TS", 0, 0, 0xffff0000 );
	while(true)
	{}
}


/*******************************************************************************
	概要	：	セグメント不在例外
	説明	：	割り込み番号：0B	例外タイプ：フォルト
				仮想記憶を実装しないので、セグメントが不在になることはない。
	Include	：	IM.h
	引数	：	レジスタの状態 IM.h参照。
	戻り値	：	void
*******************************************************************************/
void	Int0B( ARG_REGI_STAT )
{
	G_GUI.DebugPrintString( "ExcInt0B #NP", 0, 0, 0xffff0000 );
	while(true)
	{}
}


/*******************************************************************************
	概要	：	スタック・セグメント・フォルト例外
	説明	：	割り込み番号：0C	例外タイプ：フォルト
				bbbbbbbbbbbbbbbbbbbbbbbbbb
	Include	：	IM.h
	引数	：	レジスタの状態 IM.h参照。
	戻り値	：	void
*******************************************************************************/
void	Int0C( ARG_REGI_STAT )
{
	G_GUI.DebugPrintString( "ExcInt0C #SS", 0, 0, 0xffff0000 );
	while(true)
	{}
}


/*******************************************************************************
	概要	：	一般保護例外
	説明	：	割り込み番号：0D	例外タイプ：フォルト
				bbbbbbbbbbbbbbbbbbbbbbbbbb
	Include	：	IM.h
	引数	：	レジスタの状態 IM.h参照。
	戻り値	：	void
*******************************************************************************/
void	Int0D( ARG_REGI_STAT )
{
	G_GUI.DebugPrintString( "ExcInt0D #GP", 0, 0, 0xffff0000 );
	INFO_REG
	while(true)
	{}
}


/*******************************************************************************
	概要	：	ページフォルト例外
	説明	：	割り込み番号：0E	例外タイプ：フォルト
				ページングを実装しないので、ページフォルトは発生しない。
	Include	：	IM.h
	引数	：	レジスタの状態 IM.h参照。
	戻り値	：	void
*******************************************************************************/
void	Int0E( ARG_REGI_STAT )
{
	G_GUI.DebugPrintString( "ExcInt0D #PF", 0, 0, 0xffff0000 );
	while(true)
	{}
}


/*******************************************************************************
	概要	：	インテル予約
	説明	：	割り込み番号：0F	例外タイプ：
				インテルで予約済の割り込み。
	Include	：	IM.h
	引数	：	レジスタの状態 IM.h参照。
	戻り値	：	void
*******************************************************************************/
void	Int0F( ARG_REGI_STAT )
{
	G_GUI.DebugPrintString( "ExcInt0F", 0, 0, 0xffff0000 );
	while(true)
	{}
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*******************************************************************************
	概要	：	x87 FPU 浮動小数点エラー
	説明	：	割り込み番号：10	例外タイプ：フォルト
				bbbbbbbbbbbbbbbbbbbbbbbbbb
	Include	：	IM.h
	引数	：	レジスタの状態 IM.h参照。
	戻り値	：	void
*******************************************************************************/
void	Int10( ARG_REGI_STAT )
{
	G_GUI.DebugPrintString( "ExcInt10", 0, 0, 0xffff0000 );
	while(true)
	{}
}


/*******************************************************************************
	概要	：	アライメント・チェック例外
	説明	：	割り込み番号：11	例外タイプ：フォルト
				bbbbbbbbbbbbbbbbbbbbbbbbbb
	Include	：	IM.h
	引数	：	レジスタの状態 IM.h参照。
	戻り値	：	void
*******************************************************************************/
void	Int11( ARG_REGI_STAT )
{
	G_GUI.DebugPrintString( "ExcInt11", 0, 0, 0xffff0000 );
	while(true)
	{}
}


/*******************************************************************************
	概要	：	マシンチェック例外
	説明	：	割り込み番号：12	例外タイプ：アボート
				bbbbbbbbbbbbbbbbbbbbbbbbbb
	Include	：	IM.h
	引数	：	レジスタの状態 IM.h参照。
	戻り値	：	void
*******************************************************************************/
void	Int12( ARG_REGI_STAT )
{
	G_GUI.DebugPrintString( "ExcInt12", 0, 0, 0xffff0000 );
	while(true)
	{}
}


/*******************************************************************************
	概要	：	SIMD 浮動小数点例外
	説明	：	割り込み番号：13	例外タイプ：フォルト
				bbbbbbbbbbbbbbbbbbbbbbbbbb
	Include	：	IM.h
	引数	：	レジスタの状態 IM.h参照。
	戻り値	：	void
*******************************************************************************/
void	Int13( ARG_REGI_STAT )
{
	G_GUI.DebugPrintString( "ExcInt13", 0, 0, 0xffff0000 );
	while(true)
	{}
}


/*******************************************************************************
	概要	：	予約
	説明	：	割り込み番号：14	例外タイプ：
	Include	：	IM.h
	引数	：	レジスタの状態 IM.h参照。
	戻り値	：	void
*******************************************************************************/
void	Int14( ARG_REGI_STAT )
{
	G_GUI.DebugPrintString( "ExcInt14", 0, 0, 0xffff0000 );
	while(true)
	{}
}


/*******************************************************************************
	概要	：	予約
	説明	：	割り込み番号：15	例外タイプ：
	Include	：	IM.h
	引数	：	レジスタの状態 IM.h参照。
	戻り値	：	void
*******************************************************************************/
void	Int15( ARG_REGI_STAT )
{
	G_GUI.DebugPrintString( "ExcInt15", 0, 0, 0xffff0000 );
	while(true)
	{}
}


/*******************************************************************************
	概要	：	予約
	説明	：	割り込み番号：16	例外タイプ：
	Include	：	IM.h
	引数	：	レジスタの状態 IM.h参照。
	戻り値	：	void
*******************************************************************************/
void	Int16( ARG_REGI_STAT )
{
	G_GUI.DebugPrintString( "ExcInt16", 0, 0, 0xffff0000 );
	while(true)
	{}
}


/*******************************************************************************
	概要	：	予約
	説明	：	割り込み番号：17	例外タイプ：
	Include	：	IM.h
	引数	：	レジスタの状態 IM.h参照。
	戻り値	：	void
*******************************************************************************/
void	Int17( ARG_REGI_STAT )
{
	G_GUI.DebugPrintString( "ExcInt17", 0, 0, 0xffff0000 );
	while(true)
	{}
}


/*******************************************************************************
	概要	：	予約
	説明	：	割り込み番号：18	例外タイプ：
	Include	：	IM.h
	引数	：	レジスタの状態 IM.h参照。
	戻り値	：	void
*******************************************************************************/
void	Int18( ARG_REGI_STAT )
{
	G_GUI.DebugPrintString( "ExcInt18", 0, 0, 0xffff0000 );
	while(true)
	{}
}


/*******************************************************************************
	概要	：	予約
	説明	：	割り込み番号：19	例外タイプ：
	Include	：	IM.h
	引数	：	レジスタの状態 IM.h参照。
	戻り値	：	void
*******************************************************************************/
void	Int19( ARG_REGI_STAT )
{
	G_GUI.DebugPrintString( "ExcInt19", 0, 0, 0xffff0000 );
	while(true)
	{}
}


/*******************************************************************************
	概要	：	予約
	説明	：	割り込み番号：1A	例外タイプ：
	Include	：	IM.h
	引数	：	レジスタの状態 IM.h参照。
	戻り値	：	void
*******************************************************************************/
void	Int1A( ARG_REGI_STAT )
{
	G_GUI.DebugPrintString( "ExcInt1A", 0, 0, 0xffff0000 );
	while(true)
	{}
}


/*******************************************************************************
	概要	：	予約
	説明	：	割り込み番号：1B	例外タイプ：フォルト
	Include	：	IM.h
	引数	：	レジスタの状態 IM.h参照。
	戻り値	：	void
*******************************************************************************/
void	Int1B( ARG_REGI_STAT )
{
	G_GUI.DebugPrintString( "ExcInt1B", 0, 0, 0xffff0000 );
	while(true)
	{}
}


/*******************************************************************************
	概要	：	予約
	説明	：	割り込み番号：1C	例外タイプ：
	Include	：	IM.h
	引数	：	レジスタの状態 IM.h参照。
	戻り値	：	void
*******************************************************************************/
void	Int1C( ARG_REGI_STAT )
{
	G_GUI.DebugPrintString( "ExcInt1C", 0, 0, 0xffff0000 );
	while(true)
	{}
}


/*******************************************************************************
	概要	：	予約
	説明	：	割り込み番号：1D	例外タイプ：
	Include	：	IM.h
	引数	：	レジスタの状態 IM.h参照。
	戻り値	：	void
*******************************************************************************/
void	Int1D( ARG_REGI_STAT )
{
	G_GUI.DebugPrintString( "ExcInt1D", 0, 0, 0xffff0000 );
	while(true)
	{}
}


/*******************************************************************************
	概要	：	予約
	説明	：	割り込み番号：1E	例外タイプ：
	Include	：	IM.h
	引数	：	レジスタの状態 IM.h参照。
	戻り値	：	void
*******************************************************************************/
void	Int1E( ARG_REGI_STAT )
{
	G_GUI.DebugPrintString( "ExcInt1E", 0, 0, 0xffff0000 );
	while(true)
	{}
}


/*******************************************************************************
	概要	：	予約
	説明	：	割り込み番号：1F	例外タイプ：
	Include	：	IM.h
	引数	：	レジスタの状態 IM.h参照。
	戻り値	：	void
*******************************************************************************/
void	Int1F( ARG_REGI_STAT )
{
	G_GUI.DebugPrintString( "ExcInt1F", 0, 0, 0xffff0000 );
	while(true)
	{}
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

