/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/DTM/DT.h
	概要		：	ディスクリプタ・テーブルヘッダ
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	型定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
typedef	u2		SELECTOR;



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Descriptor.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	ディスクリプタ・テーブル・ベースクラス
*******************************************************************************/
class	DTBase
{
//定数
public:
	//エラー情報
	enum	Error
	{
		ERROR = _ERROR_,
		ERROR_DELNULLDESC,		//NULLディスクリプタ削除エラー
		ERROR_NOTGDT,			//GDTのセレクタでない(TIフラグが1)
		ERROR_NOTLDT,			//LDTのセレクタでない(TIフラグが0)
		ERROR_FULL,				//空きなし
		ERROR_NUMOVER,			//登録する番号が大きすぎ
		ERROR_ALREADYUSED,		//指定の番号が使用中
		ERROR_INVALIDTYPE,		//無効なディスクリプタ・タイプ

		SUCCESS = _SUCCESS_,
		SUCCESS_ALREADYFREE,	//指定のディスクリプタが既に空き
	};

	//セレクタ関連
	enum	SelectorRPL
	{
		RPL0,
		RPL1,
		RPL2,
		RPL3,
	};
	enum	Selector
	{
		SELECTOR_RPL_MASK		= 0x0003,
		SELECTOR_TI_GDT			= 0x0000,
		SELECTOR_TI_LDT			= 0x0004,
		SELECTOR_TI_MASK		= 0x0004,
		SELECTOR_INDEX_MASK		= 0xfff8,
	};

	//ディスクリプタ・タイプ、セグメント・タイプ
	enum	DescType
	{
		//システムセグメント
		//16bit
		TYPE_RESERVED_0,		//予約0
		TYPE_TSS_16,			//TSS
		TYPE_LDT,				//LDT
		TYPE_TSS_16_BUSY,		//TSS(ビジー)
		TYPE_CALL_GATE_16,		//コールゲート
		TYPE_TASK_GATE,			//タスクゲート
		TYPE_INT_GATE_16,		//割り込みゲート
		TYPE_TRAP_GATE_16,		//トラップゲート

		//32bit
		TYPE_RESERVED_8,		//予約8
		TYPE_TSS_32,			//TSS
		TYPE_RESERVED_10,		//予約10
		TYPE_TSS_32_BUSY,		//TSS(ビジー)
		TYPE_CALL_GATE_32,		//コールゲート
		TYPE_RESERVED_13,		//予約13
		TYPE_INT_GATE_32,		//割り込みゲート
		TYPE_TRAP_GATE_32,		//トラップゲート

		//コード・データセグメント
		//データセグメント
		TYPE_D_READ,				//読み込み専用
		TYPE_D_READ_A,				//読み込み専用(アクセス)
		TYPE_D_READ_WRITE,			//読み書き
		TYPE_D_READ_WRITE_A,		//読み書き(アクセス)
		TYPE_D_READ_XDOWN,			//エクスパンドダウン
		TYPE_D_READ_XDOWN_A,		//エクスパンドダウン(アクセス)
		TYPE_D_READ_WRITE_XDOWN,	//読み書き、エクスパンドダウン
		TYPE_D_READ_WRITE_XDOWN_A,	//読み書き、エクスパンドダウン(アクセス)

		//コードセグメント
		TYPE_C_EXE,					//実行専用
		TYPE_C_EXE_A,				//実行専用(アクセス)
		TYPE_C_EXE_READ,			//実行、読み込み
		TYPE_C_EXE_READ_A,			//実行、読み込み(アクセス)
		TYPE_C_EXE_CONF,			//実行、コンフォーミング
		TYPE_C_EXE_CONF_A,			//実行、コンフォーミング(アクセス)
		TYPE_C_EXE_READ_CONF,		//実行、読み込み、コンフォーミング
		TYPE_C_EXE_READ_CONF_A,		//実行、読み込み、コンフォーミング(アクセス)
	};

	//属性
	enum	DPL
	{
		DPL0,
		DPL1,
		DPL2,
		DPL3,
	};
	enum	Attr
	{
		NOTPRESENT	= FALSE0,
		PRESENT		= TRUE1,
		SIZE_16		= FALSE0,
		SIZE_32		= TRUE1,
		GRA_BYTE	= FALSE0,
		GRA_PAGE	= TRUE1,
	};

protected:
	//ディスクリプタのマスク
	enum	Descriptor
	{
		//ディスクリプタ・タイプ
		DESC_TYPE_MASK			= 0x10,		//マスク
		TYPE_SYSTEM				= 0x00,		//システム・ディスクリプタ
		TYPE_CODE_DATA			= 0x10,		//コード,データセグメント
	};


//メソッド
protected:
	SELECTOR	MakeSelector( u2 u2_Number, u1 u1_TableIndicator, u1 u1_RPL );
	void	SetSegmSub( SegmInfo* P_SegmInfo, SegmDesc* P_SegmDesc );		//セグメント情報をセグメントディスクリプタに設定
	void	SetGateSub( GateInfo* P_GateInfo, GateDesc* P_GateDesc );		//ゲート情報をゲートディスクリプタに設定
	void	GetSegmSub( SegmInfo* P_SegmInfo, SegmDesc* P_SegmDesc );		//セグメント情報をセグメントディスクリプタから取得
	void	GetGateSub( GateInfo* P_GateInfo, GateDesc* P_GateDesc );		//ゲート情報をゲートディスクリプタから取得
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
