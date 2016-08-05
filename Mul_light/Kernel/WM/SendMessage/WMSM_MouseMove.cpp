/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/WM/SendMessage/WMSM_MouseMove.cpp
	概要		：	ウィンドウ管理 - メッセージ送信 - Msg::MouseMove
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"WM.h"

#include	<Objects/Pack.h>

#include	"Define/WM_Window.h"
#include	"Define/WM_Mouse.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数・マクロ定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
namespace
{

//フラグ群(メンバ)
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
//フラグ群
enum
{
	SAME_WINDOW_MASK	= 0x00010000,		//同じウィンドウマスク
	DIFF_WINDOW			= 0x00010000,		//違うウィンドウ
	SAME_WINDOW			= 0x00010001,		//同じウィンドウ

	//今回
	NOW_DESKTOP_MASK	= 0x00100000,		//デスクトップフラグマスク
	NOW_WINDOW			= 0x00100000,		//ウィンドウ
	NOW_DESKTOP			= 0x00100010,		//デスクトップ

	//前回
	OLD_DESKTOP_MASK	= 0x04000000,		//デスクトップフラグマスク
	OLD_WINDOW			= 0x04000000,		//ウィンドウ
	OLD_DESKTOP			= 0x04000400,		//デスクトップ
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
	ui			ui_ResX;
	ui			ui_ResY;
	ui			ui_MouseXNow;
	ui			ui_MouseYNow;
	ui			ui_MouseXOld;
	ui			ui_MouseYOld;
	WinList*	P_WinListNow;		//今回のマウス座標のウィンドウポインタ
	WinList*	P_WinListOld;		//前回
	Object*		P_MOONow;			//今回のマウス座標のオブジェクトポインタ
	Object*		P_MOOOld;			//前回
	Rect		MOOAreaNow;			//今回のMOOの有効領域
	Rect		MOOAreaOld;			//前回
	GUI::MPT	MPT;				//マウスポインタタイプ
	Rect		WinFrame;			//ウィンドウフレーム矩形
	Flag16		MFlags;				//各種フラグ(メンバ)
	Flag16		Flags;				//各種フラグ
	u1			u1_WinBtnNoNow;		//今回のウィンドウボタン番号
	u1			u1_WinBtnNoOld;		//前回
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
void	Default( PP* P_PP );
void	WindowMove( PP* P_PP );
void	WindowResize( PP* P_PP );

void	WindowProc( PP* P_PP );
void	TextProc( PP* P_PP );
void	ButtonProc( PP* P_PP );

void	SendMessageSubMouseOutAll( PP* P_PP, Object* P_BeginObject, Box* P_EndObject );
void	SendMessageSubMouseOverAll( PP* P_PP, Object* P_BeginObject );

Object*	GetMOO( PP* P_PP );
}


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ウィンドウ管理クラス：プライベートメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	Msg::MOUSE_MOVEメッセージ送信
	説明	：	マウスが動いたときのメッセージ送信処理です。
				Msg::OBJECT_MOUSE_OVER/OUTの発行、ボタンオブジェクトなどの再描画メッセージ送信、
				マウスポインタの種類変更、マウスポインタ再描画、etc...
	Include	：	WM.h
	引数	：	Msg Message
				ui ui_ResX
				ui ui_ResY
	戻り値	：	void
*******************************************************************************/
void	WM::MouseMove( Msg Message, ui ui_ResX, ui ui_ResY )
{
	PP		PP;		//受け渡しパラメータ(Passing Parameters)

	//PP初期化・設定
	PP.ui_ResX			= ui_ResX;
	PP.ui_ResY			= ui_ResY;
	PP.ui_MouseXNow		= Message.Mu2_Data[0].High;		//
	PP.ui_MouseYNow		= Message.Mu2_Data[0].Low;		//
	PP.ui_MouseXOld		= Mui_MouseX;					//
	PP.ui_MouseYOld		= Mui_MouseY;					//
	PP.P_WinListNow		= NULL;							//今回のマウス座標のウィンドウ
	PP.P_WinListOld		= NULL;							//前回
	PP.P_MOONow			= NULL;							//今回のマウス座標のオブジェクト
	PP.P_MOOOld			= MP_MOO;						//前回
	PP.MOOAreaNow.Init();								//今回のMOOの有効領域
	PP.MOOAreaOld		= M_MOOArea;					//前回
	PP.MPT				= GUI::MPT_NOCHANGE;			//マウスポインタタイプ
	PP.WinFrame			= M_WinFrame;					//ウィンドウフレーム矩形
	PP.MFlags			= M_Flags;						//フラグ初期化
	PP.Flags.Init();									//フラグ初期化
	PP.u1_WinBtnNoNow	= NUM_BUTTON_TYPE;				//
	PP.u1_WinBtnNoOld	= Mu1_WinBtnNo;					//

	//ウィンドウ移動中
	if( M_Flags.CheckFlags( WINDOW_MOVE_ENABLE ) )
		WindowMove( &PP );

	//サイズ変更中
	else if( M_Flags.GetFlags( WINDOW_RESIZE_MASK ) != WINDOW_RESIZE_DISABLE )
		WindowResize( &PP );

	//通常時
	else
//		Default( &PP );
	{
		Window*	P_WindowNow;
		Window*	P_WindowOld;

		//PP初期化・設定
		PP.P_WinListNow	= MPPA_SPM[PP.ui_MouseYNow][PP.ui_MouseXNow];	//今回のマウス座標のウィンドウ
		PP.P_WinListOld	= MP_MOW;	//前回

		//フラグセット
		PP.Flags.SetFlags( SAME_WINDOW_MASK | ( ( PP.P_WinListNow == PP.P_WinListOld ) << 0 ) );	//同じウィンドウか
		PP.Flags.SetFlags( NOW_DESKTOP_MASK | ( ( PP.P_WinListNow == NULL ) << 4 ) );				//今回のMOWがデスクトップか
		PP.Flags.SetFlags( OLD_DESKTOP_MASK | ( ( PP.P_WinListOld == NULL ) << 10 ) );				//前回のMOWがデスクトップか

		P_WindowNow		= PP.P_WinListNow->P_Window;
		P_WindowOld		= PP.P_WinListOld->P_Window;

		//今回のマウス座標のウィンドウ と 前回のマウス座標のウィンドウ が同じ場合
		if( PP.Flags.CheckFlags( SAME_WINDOW ) )
		{
			//デスクトップ
			if( PP.Flags.CheckFlags( NOW_DESKTOP ) )
				SendMessageSub( Message, NULL );		//Msg::MOUSE_MOVE送信

			else
			{
				//アプリケーションの起動でマウス座標上にウィンドウが描画されたとき
				if( PP.P_MOOOld == NULL )
				{
					SendMessageSub( Message, PP.P_WinListNow );			//Msg::MOUSE_MOVE送信
					SendMessageSubMouseOverAll( &PP, P_WindowNow );		//P_Windowから今回のマウス座標までMsg::OBJECT_MOUSE_OVERを送信
				}

				//通常、前回のMOOはNULLでない。
				else
				{
					//前回のMOOの有効領域から外れていない。
					if( PP.MOOAreaOld.CheckInArea( PP.ui_MouseXNow, PP.ui_MouseYNow ) )
					{
						//前回のMOOが子を持つオブジェクトのとき
						if( PP.P_MOOOld->CheckInheritance( Object::BOX ) )
						{
							PP.P_MOONow = GetMOO( &PP );		//今回のMOOを取得

							//今回のMOOと前回のMOOが同じ
							if( PP.P_MOONow == PP.P_MOOOld )
								SendMessageSub( Message, PP.P_WinListNow );		//Msg::MOUSE_MOVE送信
							//今回のMOOと前回のMOOが異なる(今回のMOO == 前回のMOOの子孫)
							else
							{
								SendMessageSub( Message, PP.P_WinListNow );		//Msg::MOUSE_MOVE送信
								SendMessageSubMouseOverAll( &PP, PP.P_MOOOld );	//前回のMOOから今回のマウス座標までMsg::OBJECT_MOUSE_OVERを送信
							}
						}
						//前回のMOOが子を持たないオブジェクトのとき
						else
						{
							PP.P_MOONow = PP.P_MOOOld;					//今回のMOOを前回と同じに。
							SendMessageSub( Message, PP.P_WinListNow );		//Msg::MOUSE_MOVE送信
						}
					}

					//前回のMOOの有効領域から外れている。
					else
					{
						Box*	P_Kinship;			//前回のMOOと、今回のMOOの共通の親
						PP.P_MOONow = GetMOO( &PP );					//今回のMOOを取得
						P_Kinship = GetKinship( PP.P_MOONow, PP.P_MOOOld,
								PP.P_WinListNow->Pv_LocalBase );		//共通の親を取得
						SendMessageSubMouseOutAll( &PP, MP_MOO, P_Kinship );	//前回のMOOからP_KinshipまでMsg::OBJECT_MOUSE_OUTを送信
						SendMessageSub( Message, PP.P_WinListNow );			//Msg::MOUSE_MOVE送信
						SendMessageSubMouseOverAll( &PP, P_Kinship );			//共通の親から今回のマウス座標までMsg::OBJECT_MOUSE_OVERを送信
					}
				}
			}
		}
		//前回のマウス座標のウィンドウ と 今回のマウス座標のウィンドウ が違う場合
		else
		{
			SendMessageSubMouseOutAll( &PP, PP.P_MOOOld, NULL );	//前回のMOOから前回のウィンドウまで全てにMsg::OBJECT_MOUSE_OUTを送信
			SendMessageSub( Message, PP.P_WinListNow );			//Msg::MOUSE_MOVE送信
			SendMessageSubMouseOverAll( &PP, P_WindowNow );		//P_Windowから今回のマウス座標までMsg::OBJECT_MOUSE_OVERを送信
		}

		//各種処理
		{
			WindowProc( &PP );
//			TextProc( &PP );
//			ButtonProc( &PP );
		}
	}


	//メンバ情報セット
	M_Flags			= PP.MFlags;				//フラグ
	Mui_MouseX		= PP.ui_MouseXNow;			//
	Mui_MouseY		= PP.ui_MouseYNow;			//
	MP_MOW			= PP.P_WinListNow;			//マウス座標ウィンドウリスト
	MP_MOO			= PP.P_MOONow;				//マウス座標オブジェクト
	M_MOOArea		= PP.MOOAreaNow;			//MOOの有効領域
	M_WinFrame		= PP.WinFrame;				//ウィンドウフレーム矩形
	Mu1_WinBtnNo	= PP.u1_WinBtnNoNow;		//ウィンドウボタン番号

	//マウスポインタの再描画
	G_GUI.DrawMouse( PP.ui_MouseXNow, PP.ui_MouseYNow, PP.MPT );

}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	無名名前空間関数
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
namespace
{
/*******************************************************************************
	概要	：	通常処理
	説明	：
	Include	：
	引数	：	PP* P_PP				受け渡しパラメータポインタ
	戻り値	：	void
*******************************************************************************
void	Default( PP* P_PP )
{
}


/*******************************************************************************
	概要	：	ウィンドウ移動時処理
	説明	：
	Include	：
	引数	：	PP* P_PP				受け渡しパラメータポインタ
	戻り値	：	void
*******************************************************************************/
void	WindowMove( PP* P_PP )
{
	si		si_Cache;

	//PP初期化・設定

	//ウィンドウフレーム移動＆制御
	P_PP->WinFrame.Move( P_PP->ui_MouseXNow - P_PP->ui_MouseXOld, P_PP->ui_MouseYNow - P_PP->ui_MouseYOld );
	if( P_PP->WinFrame.Msi_Top < 0 )
		P_PP->WinFrame.Move( 0, -P_PP->WinFrame.Msi_Top );
	else if( ( si_Cache = P_PP->ui_ResY - P_PP->WinFrame.Msi_Top - WIN_LIMIT_BOTTOM ) < 0 )
		P_PP->WinFrame.Move( 0, si_Cache );
	if( ( si_Cache = P_PP->WinFrame.Msi_Right - WIN_LIMIT_LEFT ) < 0 )
		P_PP->WinFrame.Move( -si_Cache, 0 );
	else if( ( si_Cache = P_PP->ui_ResX - P_PP->WinFrame.Msi_Left - WIN_LIMIT_RIGHT ) < 0 )
		P_PP->WinFrame.Move( si_Cache, 0 );

	G_GUI.DrawWindowFrame( P_PP->WinFrame );	//ウィンドウフレーム描画
}


/*******************************************************************************
	概要	：	ウィンドウサイズ変更時処理
	説明	：	ウィンドウサイズ変更中にマウスが動いたときの処理です。
				最小サイズ、最大サイズの制限などを行います。
	Include	：
	引数	：	PP* P_PP				受け渡しパラメータポインタ
	戻り値	：	void
*******************************************************************************/
void	WindowResize( PP* P_PP )
{
	//PP初期化・設定

	//マウスの移動量
	si		si_MouseXDist	= P_PP->ui_MouseXNow - P_PP->ui_MouseXOld;
	si		si_MouseYDist	= P_PP->ui_MouseYNow - P_PP->ui_MouseYOld;

	u4		u4_Flags		= (u2)P_PP->MFlags.GetFlags( WINDOW_RESIZE_MASK );		//フラグ取得
	si		si_Cache;

	if( u4_Flags & WINDOW_RESIZE_TOP )
	{
		P_PP->WinFrame.Msi_Top += si_MouseYDist;
		if( P_PP->WinFrame.Msi_Top > ( si_Cache = P_PP->WinFrame.Msi_Bottom - WINDOW_MIN_HEIGHT )			//最小高さより小さい
			|| P_PP->WinFrame.Msi_Top < ( si_Cache = P_PP->WinFrame.Msi_Bottom - WINDOW_MAX_HEIGHT + 1 ) )	//最大高さより大きい
			P_PP->WinFrame.Msi_Top = si_Cache;
		if( P_PP->WinFrame.Msi_Top < ( si_Cache = WIN_LIMIT_TOP )											//上限より上
			|| P_PP->WinFrame.Msi_Top > ( si_Cache = P_PP->ui_ResY - WIN_LIMIT_BOTTOM ) )					//下限より下
			P_PP->WinFrame.Msi_Top = si_Cache;
	}
	else if( u4_Flags & WINDOW_RESIZE_BOTTOM )
	{
		P_PP->WinFrame.Msi_Bottom += si_MouseYDist;
		if( P_PP->WinFrame.Msi_Bottom < ( si_Cache = P_PP->WinFrame.Msi_Top + WINDOW_MIN_HEIGHT )			//最小高さより小さい
			|| P_PP->WinFrame.Msi_Bottom > ( si_Cache = P_PP->WinFrame.Msi_Top + WINDOW_MAX_HEIGHT - 1 ) )	//最大高さより大きい
			P_PP->WinFrame.Msi_Bottom = si_Cache;
	}

	if( u4_Flags & WINDOW_RESIZE_LEFT )
	{
		P_PP->WinFrame.Msi_Left += si_MouseXDist;
		if( P_PP->WinFrame.Msi_Left > ( si_Cache = P_PP->WinFrame.Msi_Right - WINDOW_MIN_WIDTH )			//最小幅より小さい
			|| P_PP->WinFrame.Msi_Left < ( si_Cache = P_PP->WinFrame.Msi_Right - WINDOW_MAX_WIDTH + 1 ) )	//最大幅より大きい
			P_PP->WinFrame.Msi_Left = si_Cache;
		if( P_PP->WinFrame.Msi_Left > ( si_Cache = P_PP->ui_ResX - WIN_LIMIT_RIGHT ) )						//右限より右
			P_PP->WinFrame.Msi_Left = si_Cache;
	}
	else if( u4_Flags & WINDOW_RESIZE_RIGHT )
	{
		P_PP->WinFrame.Msi_Right += si_MouseXDist;
		if( P_PP->WinFrame.Msi_Right < ( si_Cache = P_PP->WinFrame.Msi_Left +  WINDOW_MIN_WIDTH )			//最小幅より小さい
			||  P_PP->WinFrame.Msi_Right > ( si_Cache = P_PP->WinFrame.Msi_Left + WINDOW_MAX_WIDTH - 1 ) )	//最大幅より大きい
			P_PP->WinFrame.Msi_Right = si_Cache;
		if( P_PP->WinFrame.Msi_Right < ( si_Cache = WIN_LIMIT_LEFT ) )										//左限より左
			P_PP->WinFrame.Msi_Right = si_Cache;
	}

	G_GUI.DrawWindowFrame( P_PP->WinFrame );	//ウィンドウフレーム描画
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
	//PP初期化・設定
	P_PP->MPT			= GUI::MPT_ARROW;						//マウスポインタタイプ


	//ウィンドウボタンアウト、再描画メッセージ送信(前回のMOOがウィンドウの場合)
	if( P_PP->P_MOOOld->CheckInheritance( Object::WINDOW ) )
	{
		//前回と違うウィンドウの場合
		//＆前回どれかにヒットしていたとき
		if( P_PP->P_MOONow != P_PP->P_MOOOld
				&& P_PP->u1_WinBtnNoOld < NUM_BUTTON_TYPE )
		{
			Msg		BtnMsg;
			//Msg::WINDOW_BTN_OUT送信
			BtnMsg.Init( Msg::WINDOW_BTN_OUT, (u4)P_PP->P_MOOOld, P_PP->u1_WinBtnNoOld );
			G_WM.SendMessageSub( BtnMsg, P_PP->P_WinListOld );
			//Msg::WINDOW_BTN_REDRAW送信
			BtnMsg.Init( Msg::WINDOW_BTN_REDRAW, (u4)P_PP->P_MOOOld, ( PH_DEFAULT << 8 ) | P_PP->u1_WinBtnNoOld );
			G_WM.SendMessageSub( BtnMsg, P_PP->P_WinListOld );
		}
	}


	//今回のMOOがウィンドウの場合
	if( P_PP->P_MOONow->CheckInheritance( Object::WINDOW ) )
	{
		Rect		WinArea = P_PP->MOOAreaNow;		//ウィンドウ領域

		//拡縮カーソル処理
		{
			ui			ui_MPT = 0;							//マウスポインタタイプ番号(配列要素番号)

			if( (si)P_PP->ui_MouseYNow < WinArea.Msi_Top + WINFRAME_WIDTH )			//ウィンドウの上にヒット
				ui_MPT |= MPT_T_ARROW;
			else if( (si)P_PP->ui_MouseYNow > WinArea.Msi_Bottom - WINFRAME_WIDTH )	//ウィンドウの下にヒット
				ui_MPT |= MPT_B_ARROW;

			if( (si)P_PP->ui_MouseXNow < WinArea.Msi_Left + WINFRAME_WIDTH )			//ウィンドウの左にヒット
				ui_MPT |= MPT_L_ARROW;
			else if( (si)P_PP->ui_MouseXNow > WinArea.Msi_Right - WINFRAME_WIDTH )	//ウィンドウの右にヒット
				ui_MPT |= MPT_R_ARROW;

			P_PP->MPT = GCA_MPT[ui_MPT];		//マウス・ポインタ・タイプ更新
		}

		//ウィンドウボタンアウト、オーバー、再描画メッセージ送信
		{
			//前回と同じウィンドウの場合
			if( P_PP->P_MOONow == P_PP->P_MOOOld )
			{
				ui		ui_BtnCount;
				Rect	WinBtnArea;		//ウィンドウボタン領域

				//閉じるボタン(初期値)に領域を設定
				WinBtnArea.Msi_Top		= P_PP->MOOAreaNow.Msi_Top + BUTTON_TOP;
				WinBtnArea.Msi_Bottom	= WinBtnArea.Msi_Top + RSRC_BUTTON_HEIGHT;
				WinBtnArea.Msi_Right	= P_PP->MOOAreaNow.Msi_Right - BUTTON_RIGHT;
				WinBtnArea.Msi_Left		= WinBtnArea.Msi_Right - RSRC_BUTTON_WIDTH;

				//前回どれかにヒットしていたとき
				if( P_PP->u1_WinBtnNoOld < NUM_BUTTON_TYPE )
				{
					//前回重なっていたボタンへ領域を移動
					WinBtnArea.Move( -( ( RSRC_BUTTON_WIDTH + BUTTON_SPACING ) * P_PP->u1_WinBtnNoOld ), 0 );

					//前回のボタンが今のマウス座標と重なっていない
					if( !WinBtnArea.CheckInArea( P_PP->ui_MouseXNow, P_PP->ui_MouseYNow )
							&& WinBtnArea.Msi_Left > P_PP->MOOAreaNow.Msi_Left + BUTTON_RIGHT )
					{
						Msg		BtnMsg;
						//Msg::WINDOW_BTN_OUT送信
						BtnMsg.Init( Msg::WINDOW_BTN_OUT, (u4)P_PP->P_MOONow, P_PP->u1_WinBtnNoOld );
						G_WM.SendMessageSub( BtnMsg, P_PP->P_WinListNow );
						//Msg::WINDOW_BTN_REDRAW送信
						BtnMsg.Init( Msg::WINDOW_BTN_REDRAW, (u4)P_PP->P_MOONow, ( PH_DEFAULT << 8 ) | P_PP->u1_WinBtnNoOld );
						G_WM.SendMessageSub( BtnMsg, P_PP->P_WinListNow );
					}

					//閉じるボタン(初期値)に領域を設定
					WinBtnArea.Msi_Right	= P_PP->MOOAreaNow.Msi_Right - BUTTON_RIGHT;
					WinBtnArea.Msi_Left		= WinBtnArea.Msi_Right - RSRC_BUTTON_WIDTH;

					//ボタン三種で今回重なったボタンのメッセージを送る。
					{
						for( ui_BtnCount = 0; ui_BtnCount < NUM_BUTTON_TYPE; ui_BtnCount++ )
						{
							//ヒット！
							if( WinBtnArea.CheckInArea( P_PP->ui_MouseXNow, P_PP->ui_MouseYNow )
									&& WinBtnArea.Msi_Left > P_PP->MOOAreaNow.Msi_Left + BUTTON_RIGHT )
							{
								//前回と違う種類のみ
								if( ui_BtnCount != P_PP->u1_WinBtnNoOld )
								{
									Msg		BtnMsg;
									//Msg::WINDOW_BTN_OVER送信
									BtnMsg.Init( Msg::WINDOW_BTN_OVER, (u4)P_PP->P_MOONow, ui_BtnCount );
									G_WM.SendMessageSub( BtnMsg, P_PP->P_WinListNow );
									//Msg::WINDOW_BTN_REDRAW送信
									BtnMsg.Init( Msg::WINDOW_BTN_REDRAW, (u4)P_PP->P_MOONow, ( PH_LIGHT << 8 ) | ui_BtnCount );
									G_WM.SendMessageSub( BtnMsg, P_PP->P_WinListNow );
								}
								break;
							}

							//次のボタンへ領域を移動
							WinBtnArea.Move( -( RSRC_BUTTON_WIDTH + BUTTON_SPACING ), 0 );
						}
						P_PP->u1_WinBtnNoNow = ui_BtnCount;		//番号を設定
					}
				}
				//前回どれにもヒットしていないとき
				else
				{
					//ボタン三種で今回重なったボタンのメッセージを送る。
					for( ui_BtnCount = 0; ui_BtnCount < NUM_BUTTON_TYPE; ui_BtnCount++ )
					{
						//ヒット！
						if( WinBtnArea.CheckInArea( P_PP->ui_MouseXNow, P_PP->ui_MouseYNow )
								&& WinBtnArea.Msi_Left > P_PP->MOOAreaNow.Msi_Left + BUTTON_RIGHT )
						{
							Msg		BtnMsg;
							//Msg::WINDOW_BTN_OVER送信
							BtnMsg.Init( Msg::WINDOW_BTN_OVER, (u4)P_PP->P_MOONow, ui_BtnCount );
							G_WM.SendMessageSub( BtnMsg, P_PP->P_WinListNow );
							//Msg::WINDOW_BTN_REDRAW送信
							BtnMsg.Init( Msg::WINDOW_BTN_REDRAW, (u4)P_PP->P_MOONow, ( PH_LIGHT << 8 ) | ui_BtnCount );
							G_WM.SendMessageSub( BtnMsg, P_PP->P_WinListNow );
							break;
						}

						//次のボタンへ領域を移動
						WinBtnArea.Move( -( RSRC_BUTTON_WIDTH + BUTTON_SPACING ), 0 );
					}
					P_PP->u1_WinBtnNoNow = ui_BtnCount;		//番号を設定
				}
			}
		}
	}
}


/*******************************************************************************
	概要	：	テキスト処理
	説明	：
	Include	：
	引数	：	PP* P_PP		受け渡しパラメータポインタ
	戻り値	：	void
*******************************************************************************/
void	TextProc( PP* P_PP )
{
	//今回のMOOがテキストの場合
	if( P_PP->P_MOONow->CheckInheritance( Object::TEXT ) )
	{
//		Rect		ObjectArea = P_PP->MOOAreaNow;		//オブジェクト領域
//		DP( "Text:MX:%#x, MY:%#x", P_PP->ui_MouseXNow, P_PP->ui_MouseYNow );
//		P_PP->MPT = GUI::MPT_CURSOR;		//マウス・ポインタ・タイプ更新
	}
}


/*******************************************************************************
	概要	：	ボタン処理
	説明	：
	Include	：
	引数	：	PP* P_PP		受け渡しパラメータポインタ
	戻り値	：	void
*******************************************************************************/
void	ButtonProc( PP* P_PP )
{
	//今回のMOOがボタンの場合
	if( P_PP->P_MOONow->CheckInheritance( Object::BOX ) )
	{
	}
}


/*******************************************************************************
	概要	：	Msg::OBJECT_MOUSE_OUT送信(指定オブジェクトから指定オブジェクトまで)
	説明	：	P_BeginObjectから親をたどり、P_EndObjectの直前まですべてにMsg::OBJECT_MOUSE_OUTを送信します。
				マウスが動く以前にマウスポインタが指していたオブジェクトへメッセージを送信するための関数です。
				※P_EndObjectにはMsg::OBJECT_MOUSE_OUTは送信しません
	Include	：
	引数	：	PP* P_PP				受け渡しパラメータポインタ
				Object* P_BeginObject	Msg::OBJECT_MOUSE_OUTを送信する最初のオブジェクトポインタ(カーネルアドレス)
				Box* P_EndObject		Msg::OBJECT_MOUSE_OUTを送信する最後のオブジェクトポインタ(カーネルアドレス)
	戻り値	：	void
*******************************************************************************/
void	SendMessageSubMouseOutAll( PP* P_PP, Object* P_BeginObject, Box* P_EndObject )
{
	Object*	P_Object = P_BeginObject;
	Msg		Message;
	Message.Init( Msg::OBJECT_MOUSE_OUT );

	//デスクトップ
	if( P_PP->Flags.CheckFlags( OLD_DESKTOP ) )
	{
		//Msg::OBJECT_MOUSE_OUT送信
		Message.Mu4_Data[0] = (u4)P_Object;
		G_WM.SendMessageSub( Message, P_PP->P_WinListOld );
		return;
	}

	//P_EndObjectまでオブジェクトの親をたどる
	for( ; P_Object != P_EndObject; P_Object = (Object*)KStdLib::ConvL2K( P_Object->GetParent(), P_PP->P_WinListOld->Pv_LocalBase ) )
	{
		//Msg::OBJECT_MOUSE_OUT送信
		Message.Mu4_Data[0] = (u4)P_Object;
		G_WM.SendMessageSub( Message, P_PP->P_WinListOld );

		//最終的にウィンドウまで送信したら終了
		if( P_Object->GetObjectType() == Object::WINDOW )
			break;
	}
}


/*******************************************************************************
	概要	：	Msg::OBJECT_MOUSE_OVER送信(マウス座標のウィンドウからオブジェクトまですべて)
	説明	：	P_BeginObjectからマウス座標のオブジェクトまで順にMsg::OBJECT_MOUSE_OVERを送信します。
				現状、デスクトップはNULL扱いなので注意。
	Include	：
	引数	：	PP* P_PP				受け渡しパラメータポインタ
				Object* P_BeginObject	Msg::OBJECT_MOUSE_OVERを送信する最初のオブジェクトポインタ(カーネルアドレス)
	戻り値	：	void
*******************************************************************************/
void	SendMessageSubMouseOverAll( PP* P_PP, Object* P_BeginObject )
{
	Object*			P_Object = P_BeginObject;	//順にたどるオブジェクトポインタ
	Rect			WinArea;			//ウィンドウの領域(画面左上基準)
	Rect			ObjArea;			//オブジェクトの領域(画面左上基準)
	Box::Padding	PadBuf;				//パディング
	Msg				Message;

	Message.Init( Msg::OBJECT_MOUSE_OVER );


	//デスクトップ	あ～めんど・・デスクトップもウィンドウにしたい。カーネル内でもいいからウィンドウにできんか？ローカルで動くことを想定したAPIだから無理？
	if( P_PP->Flags.CheckFlags( NOW_DESKTOP ) )
	{
		//Msg::OBJECT_MOUSE_OVER送信
		Message.Mu4_Data[0] = NULL;		//マウスポインタが重なった対象のオブジェクトのポインタ
		G_WM.SendMessageSub( Message, P_PP->P_WinListNow );
		P_PP->P_MOONow = NULL;		//マウスオーバーオブジェクトを更新
		P_PP->MOOAreaNow.Init();
		return;
	}

	//領域を設定
	WinArea = P_PP->P_WinListNow->P_Window->GetArea();
	ObjArea = P_BeginObject->GetArea();
	if( !P_BeginObject->CheckInheritance( Object::WINDOW ) )	//ウィンドウを継承したオブジェクトでなければ
		ObjArea.Move( WinArea.Msi_Left, WinArea.Msi_Top );		//	バッファ左上基準から画面左上基準に

	//P_BeginObjectからマウスオーバーオブジェクトまで順番にMsg::MOSUE_OVERを送信する。
	while( true )
	{
	_OnceMore:		//もう一度実行ラベル。

		//Msg::OBJECT_MOUSE_OVER送信
		Message.Mu4_Data[0] = (u4)P_Object;		//マウスポインタが重なった対象のオブジェクトのポインタ
		G_WM.SendMessageSub( Message, P_PP->P_WinListNow );

		//受け渡しパラメータ更新
		P_PP->P_MOONow		= P_Object;
		P_PP->MOOAreaNow	= ObjArea;

		//Boxクラスを継承したオブジェクトでなければ終了。
		if( !P_Object->CheckInheritance( Object::BOX ) )
			break;

		//マウスポインタが子領域内でなければ終了。
		PadBuf = ( (Box*)P_Object )->GetPadding();			//パディング取得
		ObjArea.Contract( PadBuf.u1_Top, PadBuf.u1_Left,
				PadBuf.u1_Right, PadBuf.u1_Bottom );		//パディング分、領域を縮小
		if( !ObjArea.CheckInArea( P_PP->ui_MouseXNow, P_PP->ui_MouseYNow ) )	//子領域外なら終了
			break;

		//子オブジェクトにヒットしたか？(シングルボックス)
		if( P_Object->CheckInheritance( Object::SINGLE ) )
		{
			Object*	P_Child;	//子オブジェクト
			Rect	ChArea;		//子オブジェクトの領域

			//子オブジェクト・ポインタ取得
			if( ( P_Child = ( (Single*)P_Object )->GetChild() ) == NULL )
				break;		//子無しなら終了
			P_Child = (Object*)KStdLib::ConvL2K( P_Child, P_PP->P_WinListNow->Pv_LocalBase );		//アドレス変換

			//子領域と、子オブジェクトの領域から、有効な領域を算出
			ChArea = P_Child->GetArea();						//子オブジェクトの領域を取得
			ChArea.Move( WinArea.Msi_Left, WinArea.Msi_Top );	//バッファ左上基準から画面左上基準に
			ObjArea.And( ChArea );								//有効領域の算出

			//有効な領域外なら終了
			if( !ObjArea.CheckInArea( P_PP->ui_MouseXNow, P_PP->ui_MouseYNow ) )
				break;

			//ヒットした子オブジェクトについてもう一度処理する。
			P_Object = P_Child;
			continue;
		}

		//子オブジェクトにヒットしたか？(パックボックス)
		if( P_Object->CheckInheritance( Object::PACK ) )
		{
			Rect		ChArea;		//子オブジェクトの領域
			Pack*		P_Pack = (Pack*)P_Object;
			ObjList*	P_ObjList;
			ObjList*	P_TailList;

			//先頭の子オブジェクト・ポインタ取得
			if( ( P_ObjList = P_Pack->GetHeadChildList() ) == NULL )
				goto _Finish;		//子無しなら終了

			P_TailList = (ObjList*)KStdLib::ConvL2K( P_Pack->GetTailChildList(), P_PP->P_WinListNow->Pv_LocalBase );		//末尾のリストを取得

			//子オブジェクトにヒットしたかどうか全てを調べる
			for( ; ; P_ObjList = (ObjList*)P_ObjList->P_Next )
			{
				P_ObjList = (ObjList*)KStdLib::ConvL2K( P_ObjList, P_PP->P_WinListNow->Pv_LocalBase );		//アドレス変換

				ChArea = P_ObjList->M_ChildArea;					//子オブジェクトの領域を取得
				ChArea.Move( WinArea.Msi_Left, WinArea.Msi_Top );	//バッファ左上基準から画面左上基準に

				//子オブジェクトの領域外なら次へ。
				if( !ChArea.CheckInArea( P_PP->ui_MouseXNow, P_PP->ui_MouseYNow ) )
				{
					//すべて調べたら終了
					if( P_ObjList == P_TailList )
						goto _Finish;
					continue;		//そうでなければ次へ。
				}

				ObjArea.And( ChArea );		//子オブジェクトとの有効領域を算出

				//ヒットした子オブジェクトについてもう一度処理する。
				P_Object = (Object*)KStdLib::ConvL2K( P_ObjList->MP_Child, P_PP->P_WinListNow->Pv_LocalBase );		//アドレス変換
				goto _OnceMore;
			}
		}
	}
_Finish:
	return;
}


/*******************************************************************************
	概要	：	指定座標のオブジェクト取得
	説明	：	SPM -> ウィンドウ -> オブジェクトと、オブジェクトを順にたどり、指定座標のオブジェクトを返します。
	Include	：
	引数	：	PP* P_PP		受け渡しパラメータポインタ
	戻り値	：	Object*		指定座標のオブジェクト
*******************************************************************************/
Object*	GetMOO( PP* P_PP )
{
	Window*			P_Window	= P_PP->P_WinListNow->P_Window;		//指定座標のウィンドウ
	Object*			P_Object	= P_Window;		//順にたどるオブジェクトポインタ
	Rect			WinArea;			//ウィンドウの領域(画面左上基準)
	Rect			ObjArea;			//オブジェクトの領域(画面左上基準)
	Box::Padding	PadBuf;				//パディング

	//デスクトップ
	if( P_Window == NULL )
		return P_Window;

	//領域を設定
	WinArea = P_Window->GetArea();
	ObjArea = WinArea;

	//ウィンドウから順にマウスオーバーオブジェクトまでたどる
	while( true )
	{
	_OnceMore:		//もう一度実行ラベル。

		//受け渡しパラメータ更新
		P_PP->P_MOONow		= P_Object;
		P_PP->MOOAreaNow	= ObjArea;

		//Boxクラスを継承したオブジェクトでなければ終了。
		if( !P_Object->CheckInheritance( Object::BOX ) )
			break;

		//マウスポインタが子領域内でなければ終了。
		PadBuf = ( (Box*)P_Object )->GetPadding();			//パディング取得
		ObjArea.Contract( PadBuf.u1_Top, PadBuf.u1_Left,
				PadBuf.u1_Right, PadBuf.u1_Bottom );		//パディング分、領域を縮小
		if( !ObjArea.CheckInArea( P_PP->ui_MouseXNow, P_PP->ui_MouseYNow ) )	//子領域外なら終了
			break;

		//子オブジェクトにヒットしたか？(シングルボックス)
		if( P_Object->CheckInheritance( Object::SINGLE ) )
		{
			Object*	P_Child;	//子オブジェクト
			Rect	ChArea;		//子オブジェクトの領域

			//子オブジェクト・ポインタ取得
			if( ( P_Child = ( (Single*)P_Object )->GetChild() ) == NULL )
				break;		//子無しなら終了
			P_Child = (Object*)KStdLib::ConvL2K( P_Child, P_PP->P_WinListNow->Pv_LocalBase );		//アドレス変換

			//子領域と、子オブジェクトの領域から、有効な領域を算出
			ChArea = P_Child->GetArea();						//子オブジェクトの領域を取得
			ChArea.Move( WinArea.Msi_Left, WinArea.Msi_Top );	//バッファ左上基準から画面左上基準に
			ObjArea.And( ChArea );								//有効領域の算出

			//有効な領域外なら終了
			if( !ObjArea.CheckInArea( P_PP->ui_MouseXNow, P_PP->ui_MouseYNow ) )
				break;

			//ヒットした子オブジェクトについてもう一度処理する。
			P_Object = P_Child;
			continue;
		}

		//子オブジェクトにヒットしたか？(パックボックス)
		if( P_Object->CheckInheritance( Object::PACK ) )
		{
			Rect		ChArea;		//子オブジェクトの領域
			Pack*		P_Pack = (Pack*)P_Object;
			ObjList*	P_ObjList;
			ObjList*	P_TailList;

			//先頭の子オブジェクト・ポインタ取得
			if( ( P_ObjList = P_Pack->GetHeadChildList() ) == NULL )
				goto _Finish;		//子無しなら終了

			P_TailList = (ObjList*)KStdLib::ConvL2K( P_Pack->GetTailChildList(), P_PP->P_WinListNow->Pv_LocalBase );		//末尾のリストを取得

			//子オブジェクトにヒットしたかどうか全てを調べる
			for( ; ; P_ObjList = (ObjList*)P_ObjList->P_Next )
			{
				P_ObjList = (ObjList*)KStdLib::ConvL2K( P_ObjList, P_PP->P_WinListNow->Pv_LocalBase );		//アドレス変換

				ChArea = P_ObjList->M_ChildArea;					//子オブジェクトの領域を取得
				ChArea.Move( WinArea.Msi_Left, WinArea.Msi_Top );	//バッファ左上基準から画面左上基準に

				//子オブジェクトの領域外なら次へ。
				if( !ChArea.CheckInArea( P_PP->ui_MouseXNow, P_PP->ui_MouseYNow ) )
				{
					//すべて調べたら終了
					if( P_ObjList == P_TailList )
						goto _Finish;

					continue;		//そうでなければ次へ。
				}

				ObjArea.And( ChArea );		//子オブジェクトとの有効領域を算出

				//ヒットした子オブジェクトについてもう一度処理する。
				P_Object = (Object*)KStdLib::ConvL2K( P_ObjList->MP_Child, P_PP->P_WinListNow->Pv_LocalBase );		//アドレス変換
				goto _OnceMore;
			}
		}
	}

_Finish:
	return P_Object;
}
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
