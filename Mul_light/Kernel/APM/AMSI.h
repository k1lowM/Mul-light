/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/APM/AMSI.h
	概要		：	アプリケーション構造情報クラスヘッダ
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Appli.h"
#include	<Math.h>



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	アプリケーション・メモリ構造情報クラス
	Application Memory Structure Information
	アプリケーションのメモリ構造は以下のようになっています。
	すべてページ単位の区切りです。
	・アプリケーションクラス	：sizeof (Appli)
	・アプリケーション本体		：アプリヘッダ情報のアプリケーションサイズ - アプリヘッダのサイズ
	・ヒープ					：アプリヘッダ情報のヒープサイズ
	・スタック					：アプリヘッダ情報のスタックサイズ
	・特権0スタック				：4ページ
*******************************************************************************/
class	AMSI
{
//定数
private:
	enum
	{
		APL_CLASS_SIZE	= INT_CEIL_BIN( sizeof (Appli), 0xfff ),
		PL0_STACK_SIZE	= PAGE2BYTE( 4 ),
	};

//変数
private:
	ui		Mui_BodySize;		//アプリケーションの本体サイズ:AplHead.u4_AppliSize - sizeof (AppliHeader)
	ui		Mui_HeapSize;		//ヒープ領域のサイズ
	ui		Mui_StackSize;		//スタック領域のサイズ

	void*	MPv_Base;			//アプリケーションのベースアドレス

//メソッド
public:
	void	Init( const AppliHeader& CR_AplHead );
	void	SetBase( void* Pv_Base );

	//ゲッター
	void*	GetBase( void );			//ベースアドレス
	Appli*	GetAplClassBase( void );	//アプリケーションクラスのベースアドレス
	void*	GetBodyBase( void );		//アプリケーション本体のベースアドレス
	void*	GetHeapBase( void );		//ヒープ領域のベースアドレス
	void*	GetStackBase( void );		//スタック領域のベースアドレス
	void*	GetPL0StackBase( void );	//特権0のスタック領域のベースアドレス

	ui		GetAplClassSize( void );	//アプリケーションクラスのサイズ
	ui		GetBodySize( void );		//アプリケーション本体のサイズ
	ui		GetHeapSize( void );		//ヒープ領域のサイズ
	ui		GetStackSize( void );		//スタック領域のサイズ
	ui		GetPL0StackSize( void );	//特権0のスタック領域のサイズ

	ui		GetAllocSize( void );		//確保すべきメモリのサイズ
	u4		GetEIP( void );				//EIPに設定すべき値
	u4		GetESP( void );				//ESPに設定すべき値
	u4		GetESP0( void );			//ESP0に設定すべき値
	void	GetLDTInfo( SegmInfo* P_SegmInfo );		//LDTに設定するセグメント情報
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
