/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/WM/WM.h
	概要		：	ウィンドウ管理ヘッダ
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"TM.h"
#include	"Appli.h"
#include	"DM.h"
#include	"GUI.h"

#include	<Objects/Window.h>



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	前方宣言
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
struct WMDO_PP;


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数・マクロ定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#define	IMG_WINDOW_FP		"/DATA/IMAGE_4.0/WSRSRC/WINDOW/WINDOW.BMP"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	ウィンドウリスト構造体
*******************************************************************************/
struct	WinList : public List
{
	ui		ui_WinListKMID;	//このリスト自体のメモリID
	Window*	P_Window;		//ウィンドウクラスのポインタ(カーネルポインタ)
	ui		ui_WinBufGMID;	//ウィンドウのバッファのメモリID
	ui		ui_AppliID;		//アプリケーションID
	void*	Pv_LocalBase;	//ローカルのベースアドレス
};


/*******************************************************************************
	ウィンドウ管理クラス
*******************************************************************************/
class	WM
{
	friend class GUI;

//定数
public:
	//エラー情報
	enum	Error
	{
		ERROR = _ERROR_,
		ERROR_MEMORYALLOC,
		ERROR_READFAILED,			//ファイル読み取り失敗
		ERROR_INVALIDBITCOUNT,		//無効色数
		ERROR_INVALIDMESSAGE,		//無効メッセージ
		ERROR_NULLPOINTER,			//NULLポインタエラー

		SUCCESS = _SUCCESS_,
	};

private:
	enum
	{
		WINBUF_MAX_WIDTH		= 1024,
		WINBUF_MAX_HEIGHT		= 768,
		WINBUF_SIZE				= WINBUF_MAX_WIDTH * WINBUF_MAX_HEIGHT * sizeof (Color4),
	};

	//フラグ群
	enum
	{
		//ウィンドウサイズ変更中フラグ
		WINDOW_RESIZE_MASK		= 0x000f0000,
		WINDOW_RESIZE_DISABLE	= 0x000f0000,
		WINDOW_RESIZE_TOP		= 0x000f0001,
		WINDOW_RESIZE_BOTTOM	= 0x000f0002,
		WINDOW_RESIZE_LEFT		= 0x000f0004,
		WINDOW_RESIZE_RIGHT		= 0x000f0008,
		WINDOW_RESIZE_TL		= WINDOW_RESIZE_TOP | WINDOW_RESIZE_LEFT,
		WINDOW_RESIZE_BL		= WINDOW_RESIZE_BOTTOM | WINDOW_RESIZE_LEFT,
		WINDOW_RESIZE_TR		= WINDOW_RESIZE_TOP | WINDOW_RESIZE_RIGHT,
		WINDOW_RESIZE_BR		= WINDOW_RESIZE_BOTTOM | WINDOW_RESIZE_RIGHT,

		//ウィンドウ移動中フラグ
		WINDOW_MOVE_MASK		= 0x00100000,
		WINDOW_MOVE_DISABLE		= 0x00100000,
		WINDOW_MOVE_ENABLE		= 0x00100010,
	};

//変数
private:
	WinList*	(*MPPA_SPM)[WINBUF_MAX_WIDTH];	//スクリーン・ピクセル・マップ
	LM			M_WinLM;			//ウィンドウリスト管理
	ui			Mui_WinRsrcGMID;	//ウィンドウリソースGMID
	ui			Mui_BtnRsrcGMID;	//ボタンリソースGMID
	WinList*	MP_Active;			//移動、サイズ変更中のウィンドウリストポインタ(通常は最前面のウィンドウ。)
	WinList*	MP_Focus;			//キー情報などの送信先ウィンドウリストポインタ
	Object*		MP_FocusObj;		//キー情報などの送信先オブジェクトポインタ
	Flag16		M_Flags;			//フラグ群

	//マウス情報
	ui			Mui_MouseX, Mui_MouseY;	//マウス座標
	u4			Mu4_MouseButton;		//31-3:予約, 2:中央, 1:右, 0:左
	WinList*	MP_MOW;					//マウス座標のウィンドウリスト(Mouse Over Window list)
	Object*		MP_MOO;					//マウスが重なったオブジェクトのポインタ(Mouse Over Object)
	Rect		M_MOOArea;				//MOOの有効領域

	//その他
	Rect		M_WinFrame;			//ウィンドウフレーム矩形
	u1			Mu1_WinBtnNo;		//ウィンドウボタン番号

//メソッド
public:
	static void	Task( void );
	static void	SendMessageSub( Msg Message, WinList* P_DestWinList );

	void	Init( void );
	void	Main( void );

	s4		RegiWindow( ui ui_AppliID, Window* P_Window );
	void	DeleteWindow( ui ui_AppliID );
	void	DrawObject( Object* P_Object, Rect ChArea, u4* Pu4_Option, void* Pv_LocalBase );

	ui		GetMouseX( void );
	ui		GetMouseY( void );

private:
	//各種初期化
	void	InitWindow( u1 u1_DeviceID );
	void	InitButton( u1 u1_DeviceID );

	//メッセージ送信関連
	void	SendMessage( Msg Message );			//メッセージ送信処理

	void	MouseMove( Msg Message, ui ui_ResX, ui ui_ResY );
	void	MouseLPress( Msg Message, ui ui_ResX, ui ui_ResY );
	void	MouseLRelease( Msg Message, ui ui_ResX, ui ui_ResY );

	//オブジェクト描画関連
	void	DrawWindow( WMDO_PP *P_PP );
	void	DrawButton( WMDO_PP *P_PP );
	void	DrawText( WMDO_PP *P_PP );

	//その他
	Box*	GetKinship( Object* P_Object1, Object* P_Object2, void* Pv_LocalBase ) const;		//共通の親オブジェクト取得
	void*	GetLocalBase( Window* P_Window );
	s4		ReadBmp( const char* CPc_ImageFP, Color4* P_Dest, const ui Cui_BufSize );
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
