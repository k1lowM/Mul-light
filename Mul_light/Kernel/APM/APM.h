/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/APM/APM.h
	概要		：	アプリケーション管理ヘッダ
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"TM/TM.h"
#include	"Appli.h"
#include	"DM.h"
#include	<Message.h>



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	アプリケーション管理クラス
*******************************************************************************/
class	APM
{
//定数
public:
	//エラー情報
	enum	Error
	{
		ERROR = _ERROR_,
		ERROR_APPLI_FULLUSED,		//すべて使用中
		ERROR_UNDEFINED_TYPE,		//デバイスタイプ未定義
		ERROR_INVALID_STACKSIZE,	//無効スタックサイズ
		ERROR_INVALID_HEAPSIZE,		//無効ヒープサイズ
		ERROR_READ_FAILED,			//リード失敗
		ERROR_KMA_FAILED,			//カーネルのメモリ確保失敗
		ERROR_GMA_FAILED,			//グローバルメモリ確保失敗
		ERROR_APPLIINIT_FAILED,		//アプリケーションクラス初期化失敗
		ERROR_ADDTASK_FAILED,		//タスク追加失敗

		SUCCESS = _SUCCESS_,
	};

private:
	enum
	{
		MAX_NUM_APPLI	= 256,			//最大アプリケーション数
		ID_NO_EOR		= 0x6df9c75a,	//IDと番号を変換するための値
	};

//変数
private:
	ui			MAui_AppliGMID[MAX_NUM_APPLI];	//アプリケーションのメモリID(グローバル)
	Queue1		M_QuitAppliQ;					//アプリケーション終了待ちキュー

//メソッド
public:
	static void	Task( void );

	void	Init( void );
	void	Main( void );

	s4		StartAppli( const char* CPc_AppliFP, ui* Pui_AppliID = NULL );	//アプリケーション実行開始
	void	QuitAppli( ui ui_AppliID );			//アプリケーション終了

	Appli*	GetAppli( ui ui_AppliID );

private:
	s4		DD2Msg( const DD& CR_DeviceData, Msg* P_Message );		//デバイスデータをメッセージに変換
	ui		SearchFree( void );			//空きを検索
	ui		No2ID( u1 u1_No );
	ui		ID2No( ui ui_ID );
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
