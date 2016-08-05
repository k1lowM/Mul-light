/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/APM/AMSI.cpp
	概要		：	アプリケーション・メモリ構造情報クラス
	詳細		：	アプリケーションのメモリ構造情報を管理するクラスです。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"AMSI.h"
#include	"LDT.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
namespace
{
	enum
	{
		STACK_MAX_SIZE	= PAGE2BYTE( 0x100 ),	//スタック最大サイズ
		STACK_MIN_SIZE	= PAGE2BYTE( 0x1 ),		//スタック最小サイズ
		HEAP_MAX_SIZE	= PAGE2BYTE( 0x2000 ),	//ヒープ領域最大サイズ
		HEAP_MIN_SIZE	= PAGE2BYTE( 0x00 ),	//ヒープ領域最小サイズ
	};
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	アプリケーション・メモリ構造情報クラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	アプリケーション・メモリ構造情報クラスの初期化
	説明	：
	Include	：	AMSI.h
	引数	：	const AppliHeader& CR_AplHead	アプリケーション情報
	戻り値	：	void
*******************************************************************************/
void	AMSI::Init( const AppliHeader& CR_AplHead )
{
	//スタックサイズ修正
	if( CR_AplHead.u4_StackSize < STACK_MIN_SIZE )
		Mui_StackSize = STACK_MIN_SIZE;
	else if( CR_AplHead.u4_StackSize > STACK_MAX_SIZE )
		Mui_StackSize = STACK_MAX_SIZE;
	else
		Mui_StackSize = CR_AplHead.u4_StackSize;
	Mui_StackSize = INT_CEIL_BIN( Mui_StackSize, 0xfff );

	//ヒープサイズ修正
	if( CR_AplHead.u4_HeapSize < HEAP_MIN_SIZE )
		Mui_HeapSize = STACK_MIN_SIZE;
	else if( CR_AplHead.u4_HeapSize > HEAP_MAX_SIZE )
		Mui_HeapSize = STACK_MAX_SIZE;
	else
		Mui_HeapSize = CR_AplHead.u4_HeapSize;
	Mui_HeapSize = INT_CEIL_BIN( Mui_HeapSize, 0xfff );

	Mui_BodySize = INT_CEIL_BIN( CR_AplHead.u4_AppliSize - sizeof (AppliHeader), 0xfff );
	DP( "Mui_BodySize:%#x", Mui_BodySize );
}


/*******************************************************************************
	概要	：	ベースアドレス設定
	説明	：
	Include	：	AMSI.h
	引数	：	void* Pv_Base		ベースアドレス
	戻り値	：	void
*******************************************************************************/
void	AMSI::SetBase( void* Pv_Base )
{
	MPv_Base = Pv_Base;
}


//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■


/*******************************************************************************
	概要	：	ベースアドレス取得
	説明	：
	Include	：	AMSI.h
	引数	：	void
	戻り値	：	void*
*******************************************************************************/
void*	AMSI::GetBase( void )
{
	return MPv_Base;
}


/*******************************************************************************
	概要	：	アプリケーションクラスのベースアドレス取得
	説明	：
	Include	：	AMSI.h
	引数	：	void
	戻り値	：	Appli*
*******************************************************************************/
Appli*	AMSI::GetAplClassBase( void )
{
	return (Appli*)MPv_Base;
}


/*******************************************************************************
	概要	：	アプリケーション本体のベースアドレス取得
	説明	：
	Include	：	AMSI.h
	引数	：	void
	戻り値	：	void*
*******************************************************************************/
void*	AMSI::GetBodyBase( void )
{
	return (void*)( (u1*)MPv_Base + APL_CLASS_SIZE );
}


/*******************************************************************************
	概要	：	ヒープ領域のベースアドレス取得
	説明	：
	Include	：	AMSI.h
	引数	：	void
	戻り値	：	void*
*******************************************************************************/
void*	AMSI::GetHeapBase( void )
{
	return (void*)( (u1*)MPv_Base + APL_CLASS_SIZE + Mui_BodySize );
}


/*******************************************************************************
	概要	：	スタック領域のベースアドレス取得
	説明	：
	Include	：	AMSI.h
	引数	：	void
	戻り値	：	void*
*******************************************************************************/
void*	AMSI::GetStackBase( void )
{
	return (void*)( (u1*)MPv_Base + APL_CLASS_SIZE + Mui_BodySize + Mui_HeapSize );
}


/*******************************************************************************
	概要	：	特権0のスタック領域のベースアドレス取得
	説明	：
	Include	：	AMSI.h
	引数	：	void
	戻り値	：	void*
*******************************************************************************/
void*	AMSI::GetPL0StackBase( void )
{
	return (void*)( (u1*)MPv_Base + APL_CLASS_SIZE + Mui_BodySize + Mui_HeapSize + Mui_StackSize );
}


//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■


/*******************************************************************************
	概要	：	アプリケーションクラスのサイズ取得
	説明	：
	Include	：	AMSI.h
	引数	：	void
	戻り値	：	ui
*******************************************************************************/
ui		AMSI::GetAplClassSize( void )
{
	return APL_CLASS_SIZE;
}


/*******************************************************************************
	概要	：	アプリケーション本体のサイズ取得
	説明	：
	Include	：	AMSI.h
	引数	：	void
	戻り値	：	ui
*******************************************************************************/
ui		AMSI::GetBodySize( void )
{
	return Mui_BodySize;
}


/*******************************************************************************
	概要	：	ヒープ領域のサイズ取得
	説明	：
	Include	：	AMSI.h
	引数	：	void
	戻り値	：	ui
*******************************************************************************/
ui		AMSI::GetHeapSize( void )
{
	return Mui_HeapSize;
}


/*******************************************************************************
	概要	：	スタック領域のサイズ取得
	説明	：
	Include	：	AMSI.h
	引数	：	void
	戻り値	：	ui
*******************************************************************************/
ui		AMSI::GetStackSize( void )
{
	return Mui_StackSize;
}


/*******************************************************************************
	概要	：	特権0のスタック領域のサイズ取得
	説明	：
	Include	：	AMSI.h
	引数	：	void
	戻り値	：	ui
*******************************************************************************/
ui		AMSI::GetPL0StackSize( void )
{
	return PL0_STACK_SIZE;
}


//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■


/*******************************************************************************
	概要	：	確保すべきメモリのサイズ取得
	説明	：
	Include	：	AMSI.h
	引数	：	void
	戻り値	：	ui
*******************************************************************************/
ui		AMSI::GetAllocSize( void )
{
	return APL_CLASS_SIZE + Mui_BodySize + Mui_HeapSize + Mui_StackSize + PL0_STACK_SIZE;
}


/*******************************************************************************
	概要	：	EIPに設定すべき値取得
	説明	：
	Include	：	AMSI.h
	引数	：	void
	戻り値	：	u4
*******************************************************************************/
u4		AMSI::GetEIP( void )
{
	return 0;		//LDTでセグメントが個別にあるため、0でOK
}


/*******************************************************************************
	概要	：	ESPに設定すべき値取得
	説明	：
	Include	：	AMSI.h
	引数	：	void
	戻り値	：	u4
*******************************************************************************/
u4		AMSI::GetESP( void )
{
	return Mui_BodySize + Mui_HeapSize + Mui_StackSize;
}


/*******************************************************************************
	概要	：	ESP0に設定すべき値取得
	説明	：
	Include	：	AMSI.h
	引数	：	void
	戻り値	：	u4
*******************************************************************************/
u4		AMSI::GetESP0( void )
{
	return (u4)MPv_Base + APL_CLASS_SIZE + Mui_BodySize + Mui_HeapSize + Mui_StackSize + PL0_STACK_SIZE;
}


/*******************************************************************************
	概要	：	LDTに設定するセグメント情報取得
	説明	：
	Include	：	AMSI.h
	引数	：	SegmInfo* P_SegmInfo
	戻り値	：	SegmInfo*
*******************************************************************************/
void	AMSI::GetLDTInfo( SegmInfo* P_SegmInfo )
{
	//コード・セグメント
	P_SegmInfo[0].u4_Base			= (u4)GetBodyBase();	//
	P_SegmInfo[0].u20b_Limit		= BYTE2PAGE_CEIL( Mui_BodySize + Mui_HeapSize + Mui_StackSize );	//
	P_SegmInfo[0].u5b_Type			= LDT::TYPE_C_EXE_READ;	//コード、実行、読み取り
	P_SegmInfo[0].u2b_DPL			= LDT::DPL3;			//ディスクリプタ特権レベル3
	P_SegmInfo[0].u1b_Present		= true;					//存在する
	P_SegmInfo[0].u1b_DefaultSize	= LDT::SIZE_32;			//デフォルトサイズ：32bit
	P_SegmInfo[0].u1b_Granularity	= LDT::GRA_PAGE;		//サイズ：ページ単位

	//データ・セグメント
	P_SegmInfo[1].u4_Base			= (u4)GetBodyBase();		//
	P_SegmInfo[1].u20b_Limit		= BYTE2PAGE_CEIL( Mui_BodySize + Mui_HeapSize + Mui_StackSize );	//
	P_SegmInfo[1].u5b_Type			= LDT::TYPE_D_READ_WRITE;	//データ、読み取り、書き込み
	P_SegmInfo[1].u2b_DPL			= LDT::DPL3;				//ディスクリプタ特権レベル3
	P_SegmInfo[1].u1b_Present		= true;						//存在する
	P_SegmInfo[1].u1b_DefaultSize	= LDT::SIZE_32;				//デフォルトサイズ：32bit
	P_SegmInfo[1].u1b_Granularity	= LDT::GRA_PAGE;			//サイズ：ページ単位

	DP( "P_SegmInfo[0].u4_Base:%#x", P_SegmInfo[0].u4_Base );
	DP( "P_SegmInfo[1].u4_Base:%#x", P_SegmInfo[1].u4_Base );
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
