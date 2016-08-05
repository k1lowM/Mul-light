/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/DTM/LDT.cpp
	概要		：	LDT
	詳細		：	LDTはアプリケーションタスクが保持するものです。
					アプリケーション用のコード・データセグメントの2ディスクリプタしか持ちません。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"LDT.h"

#include	<string.h>



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	LDTクラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	LDTクラスの初期化
	説明	：
	Include	：	LDT.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	LDT::Init( void )
{
	memset( MA_LDT, 0, sizeof MA_LDT );
}


//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■


/*******************************************************************************
	概要	：	セグメント・ディスクリプタの登録
	説明	：	LDTにセグメント・ディスクリプタを登録します。
				番号は、自動で設定します。
	Include	：	LDT.h
	引数	：	SegmInfo SegmInfo		セグメント情報構造体
				SELECTOR* P_Selector	LDTのセグメントセレクタ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		LDT::SetSegment( SegmInfo SegmInfo, SELECTOR* P_Selector )
{
	u8*		Pu8_LDT = (u8*)MA_LDT;
	u4		u4_LDTIndex;

	//空きを検索
	for( u4_LDTIndex = USABLE_NUMBER; u4_LDTIndex < LDT_SIZE; u4_LDTIndex++ )
		if( Pu8_LDT[u4_LDTIndex] == 0 )
			return SetSegment( u4_LDTIndex, SegmInfo, P_Selector );		//空いていたら、セグメントを登録して終了。

	//空いていなければエラー
	return ERROR_FULL;
}


/*******************************************************************************
	概要	：	セグメント・ディスクリプタの登録
	説明	：	LDTの指定された番号にセグメント・ディスクリプタを登録します。
	Include	：	LDT.h
	引数	：	ui ui_Number			LDTに登録する番号(0x0001～0x1fff)
				SegmInfo SegmInfo		セグメント情報構造体
				SELECTOR* P_Selector	LDTのセグメントセレクタ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		LDT::SetSegment( ui ui_Number, SegmInfo SegmInfo, SELECTOR* P_Selector )
{
	u8*		Pu8_LDT = (u8*)MA_LDT;

	//エラー処理
	if( ui_Number >= LDT_SIZE )		//番号が大きすぎ。
		return ERROR_NUMOVER;
	if( Pu8_LDT[ui_Number] )		//指定のディスクリプタが空いていなければエラー
		return ERROR_ALREADYUSED;
	if( ( SegmInfo.u5b_Type & DESC_TYPE_MASK ) == TYPE_SYSTEM )		//ディスクリプタ・タイプエラー(コード・データセグメントディスクリプタでない。)
		return ERROR_INVALIDTYPE;

	SetSegmSub( &SegmInfo, MA_LDT + ui_Number );		//セグメント情報設定
	*P_Selector = MakeSelector( ui_Number, SELECTOR_TI_LDT, SegmInfo.u2b_DPL );	//セレクタ作成
	return SUCCESS;
}


//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■


/*******************************************************************************
	概要	：	セグメント情報取得
	説明	：	LDTの指定の番号の情報を取得します。
	Include	：	LDT.h
	引数	：	SELECTOR Selector		情報を取得するセグメント・ディスクリプタのセレクタ
	戻り値	：	SegmInfo		セグメント情報
*******************************************************************************/
SegmInfo	LDT::GetSegmInfo( SELECTOR Selector )
{
	SegmInfo	SegmInfo = { 0 };
	u4		u4_Index = Selector >> 3;

	//エラー処理
	if( ( Selector & SELECTOR_TI_MASK ) != SELECTOR_TI_LDT )		//セレクタのTIフラグが0ならエラー
		return SegmInfo;
	if( u4_Index >= LDT_SIZE )		//インデックスが、LDT_SIZE以上ならエラー
		return SegmInfo;

	//情報取得
	GetSegmSub( &SegmInfo, MA_LDT + u4_Index );
	return SegmInfo;
}


//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■


/*******************************************************************************
	概要	：	ディスクリプタの削除
	説明	：	LDTからディスクリプタを削除します。
	Include	：	LDT.h
	引数	：	SELECTOR Selector		削除するディスクリプタのセレクタ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		LDT::ClearDescriptor( SELECTOR Selector )
{
	u8*		Pu8_Descriptor = (u8*)MA_LDT;

	//エラー処理
	//セレクタのTIフラグが1ならエラー
	if( Selector & SELECTOR_TI_MASK )
		return ERROR_NOTLDT;
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

