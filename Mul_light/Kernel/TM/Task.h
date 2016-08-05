/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/TM/Task.h
	概要		：	タスククラスヘッダ
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"TSS.h"
#include	"LDT.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	タスククラス
	カーネルが直接使う、システムタスクを想定したクラスです。
*******************************************************************************/
class	Task : public TSS
{
//定数
public:
	enum	Error
	{
		ERROR = _ERROR_,
		ERROR_NULL_POINTER,			//NULLポインタエラー
		ERROR_INVALIDLDT,			//無効セグメントLDT

		SUCCESS = _SUCCESS_,
		SUCCESS_ALREADYFREE,		//指定のディスクリプタが既に空き
	};

	//タスクのフラグ(32ビット)
	enum	Flags
	{
		//実行(可能)状態 or 休止状態
		STATE_MASK		= 0x00010000,
		STATE_SLEEP		= 0x00010000,		//休止状態
		STATE_ACTIVE	= 0x00010001,		//実行(可能)状態
	};

//変数
public:
	Task*	MP_PrevTask;		//前に実行したタスクのポインタ
	Task*	MP_NextTask;		//次に実行するタスクのポインタ
	Flag16	M_Flag16;			//フラグ

protected:
	ui			Mui_AllottedTime;	//持ち時間
	SELECTOR	M_TSSSelector;		//TSSディスクリプタのセグメント・セレクタ
	char		MAc_Caption[256\
		- sizeof (TSS) - sizeof (Task*) - sizeof (Task*)\
		- sizeof (ui) - sizeof (Flag16) - sizeof (SELECTOR)];	//タスクのキャプション(256バイトに揃える役割を持つ。)

//メソッド
public:
	s4			Init( u4 u4_EIP, u4 u4_ESP, const char* CPc_Caption, u1 u1_AllottedTime = 0x20 );
	void		Finalizer( void );

	//セッター
	void		SetAllottedTime( u1 u1_AllottedTime = 0x20, bool b_System = false );		//持ち時間の設定
	s4			SetCaption( const char* CPc_Caption );		//キャプションの設定

	//ゲッター
	ui			GetAllottedTime( void );	//持ち時間の取得
	SELECTOR	GetTSSSelector( void );		//TSSセレクタの取得
	void		GetCaption( char* Pc_Dest );	//キャプション取得
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
