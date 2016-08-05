/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/DTM/IDT.cpp
	概要		：	IDT管理
	詳細		：	IDTの管理を行います。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"IDT.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#define	IDT_BASE		( 0x60000 )			//IDTのベースアドレス
#define	IDT_SIZE		( 0x800 )				//IDTのサイズ
#define	IDT_LIMIT		( IDT_SIZE - 1 )		//IDTのリミット
#define	NUM_IDT			( IDT_SIZE >> 3 )		//IDTの要素数(0x100)

#define	USABLE_NUMBER	( 0x20 )		//予約されていない番号の先頭番号



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	IDTクラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	IDTクラスの初期化
	説明	：	IDTを0クリアし、例外用ゲートを作成します。
	Include	：	IDT.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	IDT::Init( void )
{
	DP( "IDT::Init()\t\t\t\t\t\t\t\t\t[  OK  ]" );
}


//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■


/*******************************************************************************
	概要	：	ゲート・ディスクリプタの登録
	説明	：	IDTにゲート・ディスクリプタを登録します。
				番号は、自動で設定します。
	Include	：	IDT.h
	引数	：	GateInfo GateInfo		ゲート情報構造体
				u1		IDTに登録したゲートの番号
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		IDT::SetGate( GateInfo GateInfo, u1* Pu1_Number )
{
	u8*		Pu8_IDT = (u8*)IDT_BASE;
	u4		u4_IDTIndex;

	//空きを検索
	for( u4_IDTIndex = USABLE_NUMBER; u4_IDTIndex < NUM_IDT; u4_IDTIndex++ )
		if( Pu8_IDT[u4_IDTIndex] == 0 )
			return SetGate( u4_IDTIndex, GateInfo, Pu1_Number );		//空いていたら、ゲートを登録して終了。

	//空いていなければエラー
	return ERROR_FULL;
}


/*******************************************************************************
	概要	：	ゲート・ディスクリプタの登録
	説明	：	IDTの指定された番号にゲート・ディスクリプタを登録します。
	Include	：	IDT.h
	引数	：	u1 u1_Number			IDTに登録する番号(0x00～0xff)
				GateInfo GateInfo		ゲート情報構造体
				u1		IDTに登録したゲートの番号
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		IDT::SetGate( u1 u1_Number, GateInfo GateInfo, u1* Pu1_Number )
{
	u4		u4_Number = u1_Number;
	GateDesc*	P_GateDesc = (GateDesc*)( IDT_BASE + ( u4_Number << 3 ) );	//ディスクリプタのアドレスを設定

	//指定のディスクリプタが空いていなければエラー
	if( *(u8*)P_GateDesc )
		return ERROR_ALREADYUSED;

	switch( GateInfo.u5b_Type )
	{
	case TYPE_TASK_GATE:			//タスク・ゲート・ディスクリプタ
		GateInfo.u4_Offset = 0;
	case TYPE_TRAP_GATE_32:			//トラップ・ゲート・ディスクリプタ(32ビット)
	case TYPE_INT_GATE_32:			//割り込みゲート・ディスクリプタ(32ビット)
		GateInfo.u1_ParamCount = 0;
		break;

	default:	//その他はエラー
		return ERROR_INVALIDTYPE;
	}

	SetGateSub( &GateInfo, P_GateDesc );	//ゲート情報設定
	*Pu1_Number = u1_Number;				//番号を返す
	return SUCCESS;
}


//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■


/*******************************************************************************
	概要	：	ゲート情報取得
	説明	：	IDTの指定の番号の情報を取得します。
	Include	：	IDT.h
	引数	：	u1 u1_Number		情報を取得するゲート・ディスクリプタの番号
	戻り値	：	GateInfo		ゲート情報
*******************************************************************************/
GateInfo	IDT::GetGateInfo( u1 u1_Number )
{
	u4		u4_Number = u1_Number;
	GateDesc*	P_GateDesc = (GateDesc*)( IDT_BASE + ( u4_Number << 3 ) );	//ディスクリプタのアドレスを設定
	GateInfo	GateInfo;

	//情報取得
	GetGateSub( &GateInfo, P_GateDesc );
	return GateInfo;
}


//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■


/*******************************************************************************
	概要	：	ディスクリプタの削除
	説明	：	IDTからディスクリプタを削除します。
	Include	：	IDT.h
	引数	：	u1 u1_Number			IDTから削除する番号(0x00～0xff)
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		IDT::ClearDescriptor( u1 u1_Number )
{
	u4		u4_Number = u1_Number;
	u8*		Pu8_Descriptor = (u8*)( IDT_BASE + ( u4_Number << 3 ) );	//ディスクリプタのアドレスを設定

	//指定のディスクリプタが既に空いている。
	if( !( *Pu8_Descriptor ) )
		return SUCCESS_ALREADYFREE;

	*Pu8_Descriptor = 0;
	return SUCCESS;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

