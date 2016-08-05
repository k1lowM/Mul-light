/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/WM/SendMessage/WMSM_MouseLRelease.cpp
	概要		：	ウィンドウ管理 - メッセージ送信 - Msg::MouseLRelease
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"WM.h"
#include	"Define/WM_Window.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数・マクロ定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
namespace
{
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
}




/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
namespace
{
//受け渡しパラメータ(Passing Parameters)
struct	PP
{
	ui			ui_MouseX;
	ui			ui_MouseY;
	WinList*	P_WinList;		//マウス座標のウィンドウポインタ
	Object*		P_MOO;			//マウス座標のオブジェクトポインタ
	GUI::MPT	MPT;			//マウスポインタタイプ
	Rect		WinFrame;		//ウィンドウフレーム矩形
	WinList*	P_Active;		//移動、サイズ変更中のウィンドウリストポインタ
	Flag16		Flags;			//各種フラグ
	u1			u1_WinBtnNo;	//ウィンドウボタン番号
};
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
extern TM		G_TM;
extern WM		G_WM;



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	プロトタイプ
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
namespace
{
void	WindowMove( PP* P_PP );
void	WindowResize( PP* P_PP );

void	WindowProc( PP* P_PP );
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ウィンドウ管理クラス：プライベートメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	Msg::MOUSE_L_RELEASEメッセージ送信
	説明	：
	Include	：	WM.h
	引数	：	Msg Message
				ui ui_ResX
				ui ui_ResY
	戻り値	：	void
*******************************************************************************/
void	WM::MouseLRelease( Msg Message, ui ui_ResX, ui ui_ResY )
{
	PP		PP;		//受け渡しパラメータ(Passing Parameters)

	//PP初期化・設定
	PP.ui_MouseX		= Message.Mu2_Data[0].High;				//マウス座標X
	PP.ui_MouseY		= Message.Mu2_Data[0].Low;				//			Y
	PP.P_WinList		= MPPA_SPM[PP.ui_MouseY][PP.ui_MouseX];	//マウス座標のウィンドウ
	PP.P_MOO			= MP_MOO;								//マウス座標のオブジェクト
	PP.MPT				= GUI::MPT_ARROW;						//マウスポインタタイプ
	PP.WinFrame			= M_WinFrame;							//ウィンドウフレーム矩形
	PP.P_Active			= MP_Active;							//移動、サイズ変更中のウィンドウリストポインタ
	PP.u1_WinBtnNo		= Mu1_WinBtnNo;							//
	PP.Flags			= M_Flags;								//フラグ初期化

	//ウィンドウ移動中
	if( M_Flags.CheckFlags( WINDOW_MOVE_ENABLE ) )
		WindowMove( &PP );

	//サイズ変更中
	else if( M_Flags.GetFlags( WINDOW_RESIZE_MASK ) != WINDOW_RESIZE_DISABLE )
		WindowResize( &PP );

	//通常時
	else
	{
		SendMessageSub( Message, PP.P_WinList );		//Msg::MOUSE_L_Release送信
	}

	//各種ウィンドウオブジェクト処理
	if( PP.P_MOO->CheckInheritance( Object::WINDOW ) )
		WindowProc( &PP );

	//メンバ情報セット
	M_Flags		= PP.Flags;					//フラグ

	//マウスポインタの再描画
	G_GUI.DrawMouse( PP.ui_MouseX, PP.ui_MouseY, PP.MPT );
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	無名名前空間関数
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
namespace
{
/*******************************************************************************
	概要	：	ウィンドウ移動時処理
	説明	：
	Include	：
	引数	：	PP* P_PP				受け渡しパラメータポインタ
	戻り値	：	void
*******************************************************************************/
void	WindowMove( PP* P_PP )
{
	Msg		Message;

	//PP初期化・設定
	P_PP->Flags.SetFlags( WINDOW_MOVE_DISABLE );


	//ウィンドウ座標変更
	P_PP->P_Active->P_Window->SetOffset( P_PP->WinFrame.Msi_Left, P_PP->WinFrame.Msi_Top, Object::H_LEFT, Object::V_TOP );
	Message.Init( Msg::OBJECT_REDRAW, (u4)P_PP->P_Active->P_Window );
	G_WM.SendMessageSub( Message, P_PP->P_Active );			//Msg::OBJECT_REDRAW送信
}


/*******************************************************************************
	概要	：	ウィンドウサイズ変更時処理
	説明	：
	Include	：
	引数	：	PP* P_PP				受け渡しパラメータポインタ
	戻り値	：	void
*******************************************************************************/
void	WindowResize( PP* P_PP )
{
	Msg		Message;

	//PP初期化・設定
	P_PP->Flags.SetFlags( WINDOW_RESIZE_DISABLE );


	//ウィンドウサイズ変更
	P_PP->P_Active->P_Window->SetOffset( P_PP->WinFrame.Msi_Left, P_PP->WinFrame.Msi_Top, Object::H_LEFT, Object::V_TOP );
	P_PP->P_Active->P_Window->SetSize( P_PP->WinFrame.GetWidth(), P_PP->WinFrame.GetHeight(), Object::WIDTH_PIXEL, Object::HEIGHT_PIXEL );
	Message.Init( Msg::OBJECT_REDRAW, (u4)P_PP->P_Active->P_Window );
	G_WM.SendMessageSub( Message, P_PP->P_Active );			//Msg::OBJECT_REDRAW送信
}


/*******************************************************************************
	概要	：	ウィンドウ処理
	説明	：
	Include	：
	引数	：	PP* P_PP		受け渡しパラメータポインタ
	戻り値	：	void
*******************************************************************************/
void	WindowProc( PP* P_PP )
{
	//ウィンドウボタンをリリース
	if( P_PP->u1_WinBtnNo < NUM_BUTTON_TYPE )
	{
		Msg		BtnMsg;
		//Msg::WINDOW_BTN_L_RELEASE送信
		BtnMsg.Init( Msg::WINDOW_BTN_L_RELEASE, (u4)P_PP->P_MOO, P_PP->u1_WinBtnNo );
		G_WM.SendMessageSub( BtnMsg, P_PP->P_WinList );
		//Msg::WINDOW_BTN_REDRAW送信
		BtnMsg.Init( Msg::WINDOW_BTN_REDRAW, (u4)P_PP->P_MOO, ( PH_DEFAULT << 8 ) | P_PP->u1_WinBtnNo );
		G_WM.SendMessageSub( BtnMsg, P_PP->P_WinList );
		return;
	}
}
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

