/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/MM/MM.h
	概要		：	メモリ管理ヘッダ
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	<List.h>
#include	<Flag.h>



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	メモリ領域リストクラス(Memory Area List)
*******************************************************************************/
class	MAL : public List
{
//定数
public:
	enum	Flags
	{
		USAGE_MASK		= 0x00030000,
		USAGE_FREE		= 0x00030000,		//空き
		USAGE_BUSY		= 0x00030001,		//使用中
		USAGE_RESERVED	= 0x00030002,		//予約領域(使用禁止領域)
	};

//変数
public:
	u4		Mu4_Base;
	u4		Mu4_Size;
	Flag16	M_Flag16;

//メソッド
public:
//	void	Init( u4 u4_Base, u4 u4_Size, u4 u4_Flags );
	void	Clear( void );
};


/*******************************************************************************
	メモリ割り当てクラス
	KMAクラスとGMAクラスの親クラスです。
*******************************************************************************/
class	MA
{
//定数
protected:
	enum
	{
		HEAP_LIST_MAX		= 0x400,		//メモリ割り当てリスト本体の要素数
		ID_NO_EOR			= 0xaf38c9ed,	//IDと番号を変換するための値
	};

//変数
protected:
	LM		M_LM;				//リスト管理クラス
	MAL		M_MAL[HEAP_LIST_MAX];	//リスト本体

//メソッド
public:
	ui		Create( ui ui_ByteSize );	//メモリ領域作成
	void	Delete( ui ui_ID );			//メモリ領域削除

	void*	GetBase( ui ui_ID );		//ベースアドレス取得
	ui		GetSize( ui ui_ID );		//サイズ取得

	void	ShowMemoryUsage( void );	//メモリ使用量表示
	void	ShowMemoryMap( void );		//メモリマップ表示

protected:
	ui		SearchFreeArea( ui ui_ByteSize );		//空き領域を検索
	ui		SearchEmptyElement( void );				//配列の空き要素を検索

	void	UniteArea( ui ui_No );		//空き領域結合

	ui		No2ID( ui ui_No );		//配列要素番号からIDへ変換
	ui		ID2No( ui ui_ID );		//IDから配列要素番号へ変換
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
