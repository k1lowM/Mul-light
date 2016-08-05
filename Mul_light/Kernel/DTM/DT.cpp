/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/DTM/DT.cpp
	概要		：	ディスクリプタ・テーブル　ベースクラス
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"DT.h"


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	DTベースクラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	セレクタ作成
	説明	：	番号、TIフラグ、DPLからセレクタを作る。
	Include	：	DT.h
	引数	：	u2 u2_Number			GDT or LDTの番号
				u1 u1_TableIndicator	GDTかLDTかを判断するフラグ(0:GDT, 1:LDT)
				u1 u1_RPL				要求特権レベル
	戻り値	：	SELECTOR		GDT or LDTのセレクタ
*******************************************************************************/
SELECTOR	DTBase::MakeSelector( u2 u2_Number, u1 u1_TableIndicator, u1 u1_RPL )
{
	return ( u2_Number << 3 )
		| ( u1_TableIndicator & SELECTOR_TI_MASK )
		| ( u1_RPL & SELECTOR_RPL_MASK );
}


//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■


/*******************************************************************************
	概要	：	セグメント情報をセグメントディスクリプタに設定
	説明	：	P_SegmInfoの内容を変換してP_SegmDescに格納します。
	Include	：	DT.h
	引数	：	SegmInfo* P_SegmInfo		設定するセグメント情報の構造体ポインタ
				SegmDesc* P_SegmDesc		設定するセグメントディスクリプタのポインタ
	戻り値	：	void
*******************************************************************************/
void	DTBase::SetSegmSub( SegmInfo* P_SegmInfo, SegmDesc* P_SegmDesc )
{
	//ベース
	P_SegmDesc->u2_BaseLow		= (u2)P_SegmInfo->u4_Base;
	P_SegmDesc->u1_BaseMid		= (u1)( P_SegmInfo->u4_Base >> 16 );
	P_SegmDesc->u1_BaseHigh		= (u1)( P_SegmInfo->u4_Base >> 24 );
	//リミット
	P_SegmDesc->u2_LimitLow		= (u2)( P_SegmInfo->u20b_Limit );
	P_SegmDesc->u4b_LimitHigh	= (u1)( ( P_SegmInfo->u20b_Limit >> 16 ) );
	//属性
	P_SegmDesc->u5b_Type		= P_SegmInfo->u5b_Type;
	P_SegmDesc->u2b_DPL			= P_SegmInfo->u2b_DPL;
	P_SegmDesc->u1b_Present		= PRESENT;		//常に存在する。
	P_SegmDesc->u1b_Available	= 0;			//不使用
	P_SegmDesc->u1b_DefaultSize	= SIZE_32;		//32bitモードでしか動きません。
//	P_SegmDesc->u1b_DefaultSize	= P_SegmInfo->u1b_DefaultSize;
	P_SegmDesc->u1b_Granularity	= P_SegmInfo->u1b_Granularity;
}


/*******************************************************************************
	概要	：	ゲート情報をゲートディスクリプタに設定
	説明	：	P_GateInfoの内容を変換してP_GateDescに格納します。
	Include	：	DT.h
	引数	：	GateInfo* P_GateInfo		設定するゲート情報の構造体ポインタ
				GateDesc* P_GateDesc		設定するゲートディスクリプタのポインタ
	戻り値	：	void
*******************************************************************************/
void	DTBase::SetGateSub( GateInfo* P_GateInfo, GateDesc* P_GateDesc )
{
	//オフセット
	P_GateDesc->u2_OffsetLow	= (u2)( P_GateInfo->u4_Offset );
	P_GateDesc->u2_OffsetHigh	= (u2)( P_GateInfo->u4_Offset >> 16 );
	//セグメント・セレクタ
	P_GateDesc->u2_Selector		= P_GateInfo->SegmSelector;
	//属性
	P_GateDesc->u1_ParamCount	= P_GateInfo->u1_ParamCount;
	P_GateDesc->u5b_Type		= P_GateInfo->u5b_Type;
	P_GateDesc->u2b_DPL			= P_GateInfo->u2b_DPL;
	P_GateDesc->u1b_Present		= PRESENT;		//常に存在する。
}


//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■


/*******************************************************************************
	概要	：	セグメント情報をセグメントディスクリプタから取得
	説明	：	P_SegmDescから読み取ったデータの内容を変換してP_SegmInfoに格納します。
	Include	：	DT.h
	引数	：	SegmInfo* P_SegmInfo		セグメント情報を受け取る構造体ポインタ
				SegmDesc* P_SegmDesc		取得するゲートディスクリプタのポインタ
	戻り値	：	void
*******************************************************************************/
void	DTBase::GetSegmSub( SegmInfo* P_SegmInfo, SegmDesc* P_SegmDesc )
{
	P_SegmInfo->u4_Base			= P_SegmDesc->u2_BaseLow | ( P_SegmDesc->u1_BaseMid << 16 ) | ( P_SegmDesc->u1_BaseHigh << 24 );
	P_SegmInfo->u20b_Limit		= P_SegmDesc->u2_LimitLow | ( P_SegmDesc->u4b_LimitHigh << 16 );
	P_SegmInfo->u5b_Type		= P_SegmDesc->u5b_Type;
	P_SegmInfo->u2b_DPL			= P_SegmDesc->u2b_DPL;
	P_SegmInfo->u1b_Present		= P_SegmDesc->u1b_Present;
	P_SegmInfo->u1b_Available	= P_SegmDesc->u1b_Available;
	P_SegmInfo->u1b_DefaultSize	= P_SegmDesc->u1b_DefaultSize;
	P_SegmInfo->u1b_Granularity	= P_SegmDesc->u1b_Granularity;
}


/*******************************************************************************
	概要	：	ゲート情報をゲートディスクリプタから取得
	説明	：	P_GateDescから読み取ったデータの内容を変換してP_GateInfoに格納します。
	Include	：	DT.h
	引数	：	GateInfo* P_GateInfo		ゲート情報を受け取る構造体ポインタ
				GateDesc* P_GateDesc		取得するゲートディスクリプタのポインタ
	戻り値	：	void
*******************************************************************************/
void	DTBase::GetGateSub( GateInfo* P_GateInfo, GateDesc* P_GateDesc )
{
	P_GateInfo->SegmSelector	= P_GateDesc->u2_Selector;
	P_GateInfo->u4_Offset		= P_GateDesc->u2_OffsetLow | ( P_GateDesc->u2_OffsetHigh << 16 );
	P_GateInfo->u1_ParamCount	= P_GateDesc->u1_ParamCount;
	P_GateInfo->u5b_Type		= P_GateDesc->u5b_Type;
	P_GateInfo->u2b_DPL			= P_GateDesc->u2b_DPL;
	P_GateInfo->u1b_Present		= P_GateDesc->u1b_Present;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

