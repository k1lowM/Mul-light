/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/DTM/GDT.cpp
	概要		：	GDT管理
	詳細		：	GDTの管理を行います。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"GDT.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#define	GDT_BASE		( 0x50000 )				//GDTのベースアドレス
#define	GDT_SIZE		( 0x10000 )				//GDTのサイズ
#define	GDT_LIMIT		( GDT_SIZE - 1 )		//GDTのリミット
#define	NUM_GDT			( GDT_SIZE >> 3 )		//GDTの要素数(0x2000)

#define	USABLE_NUMBER	( 0x4 )		//自動登録可能な番号の先頭番号



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	GDTクラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	GDTクラスの初期化
	説明	：	GDTを0クリアし、カーネル用のコード・セグメントとデータ・セグメントを再作成します。
	Include	：	GDT.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	GDT::Init( void )
{
	SegmInfo	CodeSegmInfo;
	SegmInfo	DataSegmInfo;

	//GDT初期化(オール0)
	__asm__
	(
			"xorl	%%eax,		%%eax;"
			"rep	stosl;"
			:
			: "D"( GDT_BASE ), "c"( GDT_SIZE >> 2 )
			: "%eax"
	);

	//セグメント設定
	//----------------------------------------------------------
	//カーネル用フル・アクセス・コード・セグメント
	CodeSegmInfo.u4_Base			= 0x00000000;		//0から
	CodeSegmInfo.u20b_Limit			= 0xfffff;			//4GiBまで
	CodeSegmInfo.u5b_Type			= TYPE_C_EXE_READ;	//コード、実行、読み取り
	CodeSegmInfo.u2b_DPL			= DPL0;				//ディスクリプタ特権レベル0
	CodeSegmInfo.u1b_Present		= true;				//存在する
	CodeSegmInfo.u1b_DefaultSize	= SIZE_32;			//デフォルトサイズ：32bit
	CodeSegmInfo.u1b_Granularity	= GRA_PAGE;			//サイズ：ページ単位

	//カーネル用フル・アクセス・データ・セグメント
	DataSegmInfo.u4_Base			= 0x00000000;			//0から
	DataSegmInfo.u20b_Limit			= 0xfffff;				//4GiBまで
	DataSegmInfo.u5b_Type			= TYPE_D_READ_WRITE;	//データ、読み取り、書き込み
	DataSegmInfo.u2b_DPL			= DPL0;					//ディスクリプタ特権レベル0
	DataSegmInfo.u1b_Present		= true;					//存在する
	DataSegmInfo.u1b_DefaultSize	= SIZE_32;				//デフォルトサイズ：32bit
	DataSegmInfo.u1b_Granularity	= GRA_PAGE;				//サイズ：ページ単位

	//セグメントの設定
	SetSegment( 1, CodeSegmInfo );
	SetSegment( 2, DataSegmInfo );
	//----------------------------------------------------------

	DP( "GDT::Init()\t\t\t\t\t\t\t\t\t[  OK  ]" );
}


//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■


/*******************************************************************************
	概要	：	セグメント・ディスクリプタの登録
	説明	：	GDTにセグメント・ディスクリプタを登録します。
				番号は、自動で設定します。
	Include	：	GDT.h
	引数	：	SegmInfo SegmInfo		セグメント情報構造体
				SELECTOR* P_Selector	GDTのセグメントセレクタ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		GDT::SetSegment( SegmInfo SegmInfo, SELECTOR* P_Selector )
{
	u8*		Pu8_GDT = (u8*)GDT_BASE;
	u4		u4_GDTIndex;

	//空きを検索
	for( u4_GDTIndex = USABLE_NUMBER; u4_GDTIndex < NUM_GDT; u4_GDTIndex++ )
		if( Pu8_GDT[u4_GDTIndex] == 0 )
			return SetSegment( u4_GDTIndex, SegmInfo, P_Selector );		//空いていたら、セグメントを登録して終了。

	//空いていなければエラー
	return ERROR_FULL;
}


/*******************************************************************************
	概要	：	セグメント・ディスクリプタの登録
	説明	：	GDTの指定された番号にセグメント・ディスクリプタを登録します。
	Include	：	GDT.h
	引数	：	u4 u4_Number			GDTに登録する番号(0x0001～0x1fff)
				SegmInfo SegmInfo		セグメント情報構造体
				SELECTOR* P_Selector	GDTのセグメントセレクタ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		GDT::SetSegment( u4 u4_Number, SegmInfo SegmInfo, SELECTOR* P_Selector )
{
	SegmDesc*	P_SegmDesc = (SegmDesc*)( GDT_BASE + ( u4_Number << 3 ) );	//ディスクリプタのアドレスを設定

	//エラー処理
	//GDTに登録する番号が0x0000か0x2000以上ならエラー
	if( !u4_Number || ( u4_Number & 0xffffe000 ) )
		return ERROR_NUMOVER;
	//指定のディスクリプタが空いていなければエラー
	if( *(u8*)P_SegmDesc )
		return ERROR_ALREADYUSED;
	//ディスクリプタ・タイプ判定
	if( ( SegmInfo.u5b_Type & DESC_TYPE_MASK ) == TYPE_SYSTEM )
	{
		switch( SegmInfo.u5b_Type )
		{
		case TYPE_LDT:		//LDTセグメント・ディスクリプタ
		case TYPE_TSS_32:	//TSSディスクリプタ(32ビット)
			break;

		default:			//その他はエラー
			return ERROR_INVALIDTYPE;
		}
	}

	SetSegmSub( &SegmInfo, P_SegmDesc );		//セグメント情報設定
	*P_Selector = MakeSelector( u4_Number, SELECTOR_TI_GDT, SegmInfo.u2b_DPL );	//セレクタ作成
	return SUCCESS;
}


/*******************************************************************************
	概要	：	ゲート・ディスクリプタの登録
	説明	：	GDTにゲート・ディスクリプタを登録します。
				番号は、自動で設定します。
	Include	：	GDT.h
	引数	：	GateInfo GateInfo		ゲート情報構造体
				SELECTOR* P_Selector	GDTのセグメントセレクタ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		GDT::SetGate( GateInfo GateInfo, SELECTOR* P_Selector )
{
	u8*		Pu8_GDT = (u8*)GDT_BASE;
	u4		u4_GDTIndex;

	//空きを検索
	for( u4_GDTIndex = USABLE_NUMBER; u4_GDTIndex < NUM_GDT; u4_GDTIndex++ )
		if( Pu8_GDT[u4_GDTIndex] == 0 )
			return SetGate( u4_GDTIndex, GateInfo, P_Selector );		//空いていたら、ゲートを登録して終了。

	//空いていなければエラー
	return ERROR_FULL;
}


/*******************************************************************************
	概要	：	ゲート・ディスクリプタの登録
	説明	：	GDTの指定された番号にゲート・ディスクリプタを登録します。
	Include	：	GDT.h
	引数	：	u4 u4_Number			GDTに登録する番号(0x0001～0x1fff)
				GateInfo GateInfo		ゲート情報構造体
				SELECTOR* P_Selector	GDTのセグメントセレクタ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		GDT::SetGate( u4 u4_Number, GateInfo GateInfo, SELECTOR* P_Selector )
{
	GateDesc*	P_GateDesc = (GateDesc*)( GDT_BASE + ( u4_Number << 3 ) );	//ディスクリプタのアドレスを設定

	//エラー処理
	//GDTに登録する番号が0x0000か0x2000以上ならエラー
	if( !u4_Number || ( u4_Number & 0xffffe000 ) )
		return ERROR_NUMOVER;
	//指定のディスクリプタが空いていなければエラー
	if( *(u8*)P_GateDesc )
		return ERROR_ALREADYUSED;
	//ディスクリプタ・タイプ判定
	switch( GateInfo.u5b_Type )
	{
	case TYPE_TASK_GATE:		//タスク・ゲート・ディスクリプタ
		GateInfo.u4_Offset		= 0;
		GateInfo.u1_ParamCount	= 0;
	case TYPE_CALL_GATE_32:		//コール・ゲート・ディスクリプタ(32ビット)
		break;
	default:	//その他はエラー
		return ERROR_INVALIDTYPE;
	}

	SetGateSub( &GateInfo, P_GateDesc );		//ゲート情報設定
	*P_Selector = MakeSelector( u4_Number, SELECTOR_TI_GDT, GateInfo.u2b_DPL );	//セレクタ作成
	return SUCCESS;
}


//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■


/*******************************************************************************
	概要	：	セグメント情報取得
	説明	：	GDTの指定の番号の情報を取得します。
	Include	：	GDT.h
	引数	：	SELECTOR Selector		情報を取得するセグメント・ディスクリプタのセレクタ
	戻り値	：	SegmInfo		セグメント情報
*******************************************************************************/
SegmInfo	GDT::GetSegmInfo( SELECTOR Selector )
{
	SegmDesc*	P_SegmDesc = (SegmDesc*)( GDT_BASE + ( Selector & SELECTOR_INDEX_MASK ) );	//ディスクリプタのアドレスを設定
	SegmInfo	SegmInfo = { 0, 0, 0, 0, 0, 0, 0, 0 };

	//エラー処理
	//セレクタのTIフラグが1ならエラー
	if( Selector & SELECTOR_TI_MASK )
		return SegmInfo;
	//ディスクリプタ・タイプ判定
	if( ( P_SegmDesc->u5b_Type & DESC_TYPE_MASK ) == TYPE_SYSTEM )
	{
		switch( P_SegmDesc->u5b_Type )
		{
		case TYPE_LDT:		//LDTセグメント・ディスクリプタ
		case TYPE_TSS_32:	//TSSディスクリプタ(32ビット)
			break;
		default:			//その他はエラー
			return SegmInfo;
		}
	}

	//情報取得
	GetSegmSub( &SegmInfo, P_SegmDesc );
	return SegmInfo;
}


/*******************************************************************************
	概要	：	ゲート情報取得
	説明	：	GDTの指定の番号の情報を取得します。
	Include	：	GDT.h
	引数	：	SELECTOR Selector		情報を取得するゲート・ディスクリプタのセレクタ
	戻り値	：	GateInfo		ゲート情報
*******************************************************************************/
GateInfo	GDT::GetGateInfo( SELECTOR Selector )
{
	GateDesc*	P_GateDesc = (GateDesc*)( GDT_BASE + ( Selector & SELECTOR_INDEX_MASK ) );	//ディスクリプタのアドレスを設定
	GateInfo	GateInfo = { 0 };

	//エラー処理
	//セレクタのTIフラグが1ならエラー
	if( Selector & SELECTOR_TI_MASK )
		return GateInfo;
	//ディスクリプタ・タイプ判定
	switch( P_GateDesc->u5b_Type )
	{
	case TYPE_CALL_GATE_32:	//コール・ゲート・ディスクリプタ(32ビット)
	case TYPE_TASK_GATE:	//タスク・ゲート・ディスクリプタ
		break;
	default:	//その他はエラー
		return GateInfo;
	}

	//情報取得
	GetGateSub( &GateInfo, P_GateDesc );
	return GateInfo;
}


//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■


/*******************************************************************************
	概要	：	ディスクリプタの削除
	説明	：	GDTからディスクリプタを削除します。
	Include	：	GDT.h
	引数	：	SELECTOR Selector		削除するディスクリプタのセレクタ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		GDT::ClearDescriptor( SELECTOR Selector )
{
	u8*		Pu8_Descriptor = (u8*)( GDT_BASE + ( Selector & SELECTOR_INDEX_MASK ) );	//ディスクリプタのアドレスを設定

	//エラー処理
	//セレクタのTIフラグが1ならエラー
	if( Selector & SELECTOR_TI_MASK )
		return ERROR_NOTGDT;
	//セレクタのインデックスが0ならエラー
	if( !( Selector & SELECTOR_INDEX_MASK ) )
		return ERROR_DELNULLDESC;

	//指定のディスクリプタが既に空いている。
	if( !( *Pu8_Descriptor ) )
		return SUCCESS_ALREADYFREE;

	*Pu8_Descriptor = 0;
	return SUCCESS;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

