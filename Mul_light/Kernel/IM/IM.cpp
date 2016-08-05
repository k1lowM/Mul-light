/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/IM/IM.cpp
	概要		：	割り込み管理
	詳細		：	割り込みハンドラの登録を管理します。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"IM.h"
#include	"IntEntry.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#define	NUM_INT			( 0x100 )		//割り込み数



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	割り込み管理クラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	割り込み管理クラスの初期化
	説明	：	例外割り込みルーチンをIDTに登録します。
	Include	：	IM.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	IM::Init( void )
{
	u4		u4_Count;
	GateInfo	IntGateInfo;
	u4		u4_IntEntry[NUM_INT] = {
		(u4)IntEntry00, (u4)IntEntry01, (u4)IntEntry02, (u4)IntEntry03, (u4)IntEntry04, (u4)IntEntry05, (u4)IntEntry06, (u4)IntEntry07, (u4)IntEntry08, (u4)IntEntry09, (u4)IntEntry0A, (u4)IntEntry0B, (u4)IntEntry0C, (u4)IntEntry0D, (u4)IntEntry0E, (u4)IntEntry0F,
		(u4)IntEntry10, (u4)IntEntry11, (u4)IntEntry12, (u4)IntEntry13, (u4)IntEntry14, (u4)IntEntry15, (u4)IntEntry16, (u4)IntEntry17, (u4)IntEntry18, (u4)IntEntry19, (u4)IntEntry1A, (u4)IntEntry1B, (u4)IntEntry1C, (u4)IntEntry1D, (u4)IntEntry1E, (u4)IntEntry1F,
		(u4)IntEntry20, (u4)IntEntry21, (u4)IntEntry22, (u4)IntEntry23, (u4)IntEntry24, (u4)IntEntry25, (u4)IntEntry26, (u4)IntEntry27, (u4)IntEntry28, (u4)IntEntry29, (u4)IntEntry2A, (u4)IntEntry2B, (u4)IntEntry2C, (u4)IntEntry2D, (u4)IntEntry2E, (u4)IntEntry2F,
		(u4)IntEntry30, (u4)IntEntry31, (u4)IntEntry32, (u4)IntEntry33, (u4)IntEntry34, (u4)IntEntry35, (u4)IntEntry36, (u4)IntEntry37, (u4)IntEntry38, (u4)IntEntry39, (u4)IntEntry3A, (u4)IntEntry3B, (u4)IntEntry3C, (u4)IntEntry3D, (u4)IntEntry3E, (u4)IntEntry3F,
		(u4)IntEntry40, (u4)IntEntry41, (u4)IntEntry42, (u4)IntEntry43, (u4)IntEntry44, (u4)IntEntry45, (u4)IntEntry46, (u4)IntEntry47, (u4)IntEntry48, (u4)IntEntry49, (u4)IntEntry4A, (u4)IntEntry4B, (u4)IntEntry4C, (u4)IntEntry4D, (u4)IntEntry4E, (u4)IntEntry4F,
		(u4)IntEntry50, (u4)IntEntry51, (u4)IntEntry52, (u4)IntEntry53, (u4)IntEntry54, (u4)IntEntry55, (u4)IntEntry56, (u4)IntEntry57, (u4)IntEntry58, (u4)IntEntry59, (u4)IntEntry5A, (u4)IntEntry5B, (u4)IntEntry5C, (u4)IntEntry5D, (u4)IntEntry5E, (u4)IntEntry5F,
		(u4)IntEntry60, (u4)IntEntry61, (u4)IntEntry62, (u4)IntEntry63, (u4)IntEntry64, (u4)IntEntry65, (u4)IntEntry66, (u4)IntEntry67, (u4)IntEntry68, (u4)IntEntry69, (u4)IntEntry6A, (u4)IntEntry6B, (u4)IntEntry6C, (u4)IntEntry6D, (u4)IntEntry6E, (u4)IntEntry6F,
		(u4)IntEntry70, (u4)IntEntry71, (u4)IntEntry72, (u4)IntEntry73, (u4)IntEntry74, (u4)IntEntry75, (u4)IntEntry76, (u4)IntEntry77, (u4)IntEntry78, (u4)IntEntry79, (u4)IntEntry7A, (u4)IntEntry7B, (u4)IntEntry7C, (u4)IntEntry7D, (u4)IntEntry7E, (u4)IntEntry7F,
		(u4)IntEntry80, (u4)IntEntry81, (u4)IntEntry82, (u4)IntEntry83, (u4)IntEntry84, (u4)IntEntry85, (u4)IntEntry86, (u4)IntEntry87, (u4)IntEntry88, (u4)IntEntry89, (u4)IntEntry8A, (u4)IntEntry8B, (u4)IntEntry8C, (u4)IntEntry8D, (u4)IntEntry8E, (u4)IntEntry8F,
		(u4)IntEntry90, (u4)IntEntry91, (u4)IntEntry92, (u4)IntEntry93, (u4)IntEntry94, (u4)IntEntry95, (u4)IntEntry96, (u4)IntEntry97, (u4)IntEntry98, (u4)IntEntry99, (u4)IntEntry9A, (u4)IntEntry9B, (u4)IntEntry9C, (u4)IntEntry9D, (u4)IntEntry9E, (u4)IntEntry9F,
		(u4)IntEntryA0, (u4)IntEntryA1, (u4)IntEntryA2, (u4)IntEntryA3, (u4)IntEntryA4, (u4)IntEntryA5, (u4)IntEntryA6, (u4)IntEntryA7, (u4)IntEntryA8, (u4)IntEntryA9, (u4)IntEntryAA, (u4)IntEntryAB, (u4)IntEntryAC, (u4)IntEntryAD, (u4)IntEntryAE, (u4)IntEntryAF,
		(u4)IntEntryB0, (u4)IntEntryB1, (u4)IntEntryB2, (u4)IntEntryB3, (u4)IntEntryB4, (u4)IntEntryB5, (u4)IntEntryB6, (u4)IntEntryB7, (u4)IntEntryB8, (u4)IntEntryB9, (u4)IntEntryBA, (u4)IntEntryBB, (u4)IntEntryBC, (u4)IntEntryBD, (u4)IntEntryBE, (u4)IntEntryBF,
		(u4)IntEntryC0, (u4)IntEntryC1, (u4)IntEntryC2, (u4)IntEntryC3, (u4)IntEntryC4, (u4)IntEntryC5, (u4)IntEntryC6, (u4)IntEntryC7, (u4)IntEntryC8, (u4)IntEntryC9, (u4)IntEntryCA, (u4)IntEntryCB, (u4)IntEntryCC, (u4)IntEntryCD, (u4)IntEntryCE, (u4)IntEntryCF,
		(u4)IntEntryD0, (u4)IntEntryD1, (u4)IntEntryD2, (u4)IntEntryD3, (u4)IntEntryD4, (u4)IntEntryD5, (u4)IntEntryD6, (u4)IntEntryD7, (u4)IntEntryD8, (u4)IntEntryD9, (u4)IntEntryDA, (u4)IntEntryDB, (u4)IntEntryDC, (u4)IntEntryDD, (u4)IntEntryDE, (u4)IntEntryDF,
		(u4)IntEntryE0, (u4)IntEntryE1, (u4)IntEntryE2, (u4)IntEntryE3, (u4)IntEntryE4, (u4)IntEntryE5, (u4)IntEntryE6, (u4)IntEntryE7, (u4)IntEntryE8, (u4)IntEntryE9, (u4)IntEntryEA, (u4)IntEntryEB, (u4)IntEntryEC, (u4)IntEntryED, (u4)IntEntryEE, (u4)IntEntryEF,
		(u4)IntEntryF0, (u4)IntEntryF1, (u4)IntEntryF2, (u4)IntEntryF3, (u4)IntEntryF4, (u4)IntEntryF5, (u4)IntEntryF6, (u4)IntEntryF7, (u4)IntEntryF8, (u4)IntEntryF9, (u4)IntEntryFA, (u4)IntEntryFB, (u4)IntEntryFC, (u4)IntEntryFD, (u4)IntEntryFE, (u4)IntEntryFF
	};

	//初期化
	IntGateInfo.SegmSelector	= 0x0008;					//コードセグメントのセレクタ
	IntGateInfo.u5b_Type		= G_IDT.TYPE_INT_GATE_32;	//ディスクリプタタイプ：32bit割り込みゲート
	IntGateInfo.u2b_DPL			= G_IDT.DPL0;				//特権レベル0
	IntGateInfo.u1b_Present		= TRUE1;					//存在フラグ

	//IDTに登録
	for( u4_Count = 0; u4_Count < NUM_INT; u4_Count++ )
	{
		IntGateInfo.u4_Offset = u4_IntEntry[u4_Count];
		G_IDT.SetGate( u4_Count, IntGateInfo );
	}

	DP( "IM::Init()\t\t\t\t\t\t\t\t\t[  OK  ]" );
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

