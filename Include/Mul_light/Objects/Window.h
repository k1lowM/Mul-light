/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Include/Mul_light/Objects/Window.h
	概要		：	Mul light API - ウィンドウクラス
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*******************************************************************************
	■ウィンドウクラスにおけるオブジェクトクラスのメンバ変数の取扱い
	M_ObjectType	：	Object::WINDOW		オブジェクト・タイプ。通常どおり
	MP_Window		：	this				所属ウィンドウ。自身のポインタ
	MP_Parent		：	NULL				親オブジェクト。親は無し(ウィンドウクラスのみ)

	Msi_OffsetX/Y	：	デスクトップとH/VBPを基準とする。
	Mui_Width/H		：	デスクトップとW/HTypeを基準とする。

	M_Area			：	画面左上を基準とした領域(通常はウィンドウバッファの左上が基準)

	M_Flags	H/VBP	：	デスクトップを基準とする。
			W/HType	：	デスクトップを基準とする。単位によらず、最大値は解像度と同じ。
*******************************************************************************/


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Caption.h"
#include	"Message.h"
#include	"Flag.h"
#include	"MsgQ.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	ウィンドウクラス
*******************************************************************************/
class	Window : public Caption
{
//定数
public:
	//エラー情報
	enum	Error
	{
		ERROR = _ERROR_,
		ERROR_REGIFAILED,		//ウィンドウ登録失敗
		WINDOW_ERROR_MAX,

		SUCCESS = _SUCCESS_,
		WINDOW_SUCCESS_MAX,
	};

	//ボタンタイプ
	enum
	{
		BUTTON_CLOSE,			//閉じるボタン
		BUTTON_MAX,				//最大化ボタン
		BUTTON_MIN,				//最小化ボタン
		NUM_BUTTON_TYPE,		//ボタンタイプ数
	};

	//フラグ類
	enum	StateFlags
	{
		STATE_VARIABLE	= 0x00030000,		//ウィンドウサイズ可変(通常)
		STATE_MIN		= 0x00030002,		//ウィンドウ最小化中
		STATE_MAX		= 0x00030003,		//ウィンドウ最大化中
	};

//変数
protected:
	MsgQ	M_MsgQ;				//メッセージ・キュー
	ui		Mui_AppliID;		//アプリケーションID
	Flag16	M_StateFlags;		//状態フラグ類

	void*	MPv_KWinList;		//ウィンドウリストポインタ(カーネルポインタ)(カーネル用変数なので、アプリケーションやAPIが触ってはいけない。)

//メソッド
public:
	void	Init( void );

	s4		RegiWindow( ui ui_AppliID );			//ウィンドウ登録

	s4		UpdateArea( void );
	s4		UpdateAreaAll( void );

	ui		GetAppliID( void );
	Flag16	GetStateFlags( void );

	//メッセージ
	s4		SendMessage( const Msg& CR_Message );	//メッセージ送信
	Msg		GetMessage( void );						//メッセージ取得
	void	MsgDefProc( const Msg& CR_Message );	//メッセージのデフォルト処理(どのメッセージがきても、適切に処理する。)

//カーネル用メソッド
public:
	void	SetKWinList( void* Pv_WinList );	//ウィンドウリストポインタ設定
	void*	GetKWinList( void );				//ウィンドウリストポインタ取得
};


/*******************************************************************************
	(SCD)ウィンドウ登録クラス
*******************************************************************************/
namespace	SCD
{
	class	RegiWindow : public SCDBase
	{
	public:
		Window*	MP_Window;
		ui		Mui_AppliID;		//登録するウィンドウのアプリケーションID
	};
}


/*******************************************************************************
	(SCD)アプリケーションクラス
*******************************************************************************/
namespace	SCD
{
	class	AplQuit : public SCDBase
	{
	public:
		ui		Mui_AppliID;		//終了するアプリケーションID
	};
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
