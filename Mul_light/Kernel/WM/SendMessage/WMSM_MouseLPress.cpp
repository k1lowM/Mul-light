/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/WM/SendMessage/WMSM_MouseLPress.cpp
	概要		：	ウィンドウ管理 - メッセージ送信 - Msg::MouseLPress
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
	Flag16		Flags;			//各種フラグ
	WinList*	P_Active;		//移動、サイズ変更中のウィンドウリストポインタ
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
void	WindowProc( PP* P_PP );
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ウィンドウ管理クラス：プライベートメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	Msg::MOUSE_L_PRESSメッセージ送信
	説明	：
	Include	：	WM.h
	引数	：	Msg Message
				ui ui_ResX
				ui ui_ResY
	戻り値	：	void
*******************************************************************************/
void	WM::MouseLPress( Msg Message, ui ui_ResX, ui ui_ResY )
{
	PP		PP;		//受け渡しパラメータ(Passing Parameters)

	//PP初期化・設定
	PP.ui_MouseX		= Message.Mu2_Data[0].High;				//マウス座標X
	PP.ui_MouseY		= Message.Mu2_Data[0].Low;				//			Y
	PP.P_WinList		= MPPA_SPM[PP.ui_MouseY][PP.ui_MouseX];	//マウス座標のウィンドウ
	PP.P_MOO			= MP_MOO;								//マウス座標のオブジェクト
	PP.MPT				= GUI::MPT_NOCHANGE;					//マウスポインタタイプ
	PP.WinFrame			= M_WinFrame;							//ウィンドウフレーム矩形初期化
	PP.P_Active			= PP.P_WinList;							//移動、サイズ変更中のウィンドウリストポインタ
	PP.u1_WinBtnNo		= Mu1_WinBtnNo;							//
	PP.Flags			= M_Flags;								//フラグ初期化

	SendMessageSub( Message, PP.P_WinList );		//Msg::MOUSE_L_PRESS送信

	//各種ウィンドウオブジェクト処理
	if( PP.P_WinList != NULL )
	{
		if( PP.P_MOO->CheckInheritance( Object::WINDOW ) )
			WindowProc( &PP );
	}

	//アクティブウィンドウが最前面になければ最前面に移動
	{
		WinList*	P_FrontWinList = (WinList*)M_WinLM.GetHeadList();
		if( PP.P_Active != P_FrontWinList && PP.P_Active != NULL )
		{
//			Msg		WinMsg;
//			//現在最前面にあるウィンドウをグレーにする。
//			//Msg::WINDOW_BTN_REDRAW送信
//			WinMsg.Init( Msg::OBJECT_REDRAW, (u4)( P_FrontWinList->P_Window ) );
//			G_WM.SendMessageSub( WinMsg, P_FrontWinList );
//
//			//アクティブウィンドウを最前面にする。
//			//Msg::WINDOW_BTN_REDRAW送信
//			WinMsg.Init( Msg::OBJECT_REDRAW, (u4)PP.P_MOO );
//			G_WM.SendMessageSub( WinMsg, PP.P_WinList );

			M_WinLM.MoveHead( PP.P_Active );	//ウィンドウを先頭に移動
//			G_GUI.DrawUpdate( P_FrontWinList->P_Window->GetArea() );		//再描画
			G_GUI.DrawUpdate( PP.P_Active->P_Window->GetArea() );		//再描画
		}
	}

	//メンバ情報セット
	MP_Focus	= PP.P_WinList;			//フォーカスウィンドウリスト
	MP_FocusObj	= PP.P_MOO;				//フォーカスオブジェクト
	M_Flags		= PP.Flags;				//フラグ
	M_WinFrame	= PP.WinFrame;			//ウィンドウフレーム矩形
	MP_Active	= PP.P_Active;			//移動、サイズ変更中のウィンドウリストポインタ

	//マウスポインタの再描画
	G_GUI.DrawMouse( PP.ui_MouseX, PP.ui_MouseY, PP.MPT );
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	無名名前空間関数
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
namespace
{
/*******************************************************************************
	概要	：	ウィンドウ処理
	説明	：
	Include	：
	引数	：	PP* P_PP		受け渡しパラメータポインタ
	戻り値	：	void
*******************************************************************************/
void	WindowProc( PP* P_PP )
{
	Rect		WinArea = P_PP->P_MOO->GetArea();		//ウィンドウ領域

	//サイズ変更
	if( (si)P_PP->ui_MouseY < WinArea.Msi_Top + WINFRAME_WIDTH )			//ウィンドウの上にヒット
		P_PP->Flags.SetFlags( WINDOW_RESIZE_TOP );
	else if( (si)P_PP->ui_MouseY > WinArea.Msi_Bottom - WINFRAME_WIDTH )	//ウィンドウの下にヒット
		P_PP->Flags.SetFlags( WINDOW_RESIZE_BOTTOM );
	if( (si)P_PP->ui_MouseX < WinArea.Msi_Left + WINFRAME_WIDTH )			//ウィンドウの左にヒット
		P_PP->Flags.SetFlags( P_PP->Flags.GetFlags( WINDOW_RESIZE_MASK ) | WINDOW_RESIZE_LEFT );
	else if( (si)P_PP->ui_MouseX > WinArea.Msi_Right - WINFRAME_WIDTH )	//ウィンドウの右にヒット
		P_PP->Flags.SetFlags( P_PP->Flags.GetFlags( WINDOW_RESIZE_MASK ) | WINDOW_RESIZE_RIGHT );

	if( P_PP->Flags.GetFlags( WINDOW_RESIZE_MASK ) & 0xffff )
	{
		//ウィンドウフレーム描画
		P_PP->WinFrame = P_PP->P_WinList->P_Window->GetArea();
		G_GUI.DrawWindowFrame( P_PP->WinFrame, false );

		return;
	}

	//ウィンドウボタンをプレス
	if( P_PP->u1_WinBtnNo < NUM_BUTTON_TYPE )
	{
		Msg		BtnMsg;
		//Msg::WINDOW_BTN_L_PRESS送信
		BtnMsg.Init( Msg::WINDOW_BTN_L_PRESS, (u4)P_PP->P_MOO, P_PP->u1_WinBtnNo );
		G_WM.SendMessageSub( BtnMsg, P_PP->P_WinList );
		//Msg::WINDOW_BTN_REDRAW送信
		BtnMsg.Init( Msg::WINDOW_BTN_REDRAW, (u4)P_PP->P_MOO, ( PH_PRESS << 8 ) | P_PP->u1_WinBtnNo );
		G_WM.SendMessageSub( BtnMsg, P_PP->P_WinList );
		return;
	}

	//ウィンドウ移動(キャプションバーにヒット)
	if( (si)P_PP->ui_MouseY < WinArea.Msi_Top + CPTBAR_HEIGHT )
	{
		//PP初期化・設定
		P_PP->MPT			= GUI::MPT_MOVE;						//マウスポインタタイプ
		P_PP->Flags.SetFlags( WINDOW_MOVE_ENABLE );

		//ウィンドウフレーム描画
		P_PP->WinFrame = P_PP->P_WinList->P_Window->GetArea();
		G_GUI.DrawWindowFrame( P_PP->WinFrame, false );
		return;
	}
}
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

