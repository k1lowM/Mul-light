/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/WM/WM_SendMessage.cpp
	概要		：	ウィンドウ管理 - メッセージ送信
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"WM.h"
#include	"Desktop.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数・マクロ定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
namespace
{
	//スクリーン・ピクセル・マップ(どこにどのウィンドウがあるかのマップ)
	enum
	{
		SPM_BASE		= 0x400000,
		SPM_SIZE		= 0x300000,
	};
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
extern TM		G_TM;
extern Desktop	G_Desktop;			//デスクトップクラスのポインタ

//マウス・ポインタ・タイプ
extern const volatile GUI::MPT	GCA_MPT[12] =
	{
			GUI::MPT_ARROW,			GUI::MPT_V_ARROW,		GUI::MPT_V_ARROW,		GUI::MPT_ARROW,
			GUI::MPT_H_ARROW,		GUI::MPT_R_DOWN_ARROW,	GUI::MPT_R_UP_ARROW,	GUI::MPT_ARROW,
			GUI::MPT_H_ARROW,		GUI::MPT_R_UP_ARROW,	GUI::MPT_R_DOWN_ARROW,	GUI::MPT_ARROW,
	};


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ウィンドウ管理クラス：静的パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	メッセージ送信
	説明	：	指定のウィンドウにメッセージを送信します。
				メッセージの種類に応じて、ポインタのアドレスを変換して送信します。
				指定のウィンドウがNULLの場合、デスクトップに
	Include	：	WM.h
	引数	：	Msg Message				メッセージ(内部のポインタは全てカーネルアドレス)
				WinList* P_DestWinList	メッセージ送信先ウィンドウリストポインタ
	戻り値	：	void
*******************************************************************************/
void	WM::SendMessageSub( Msg Message, WinList* P_DestWinList )
{
	//指定のウィンドウがNULLならデスクトップに送信して終了
	if( P_DestWinList == NULL )
	{
		G_Desktop.EnqueueMsg( Message );
		return;
	}

	//ポインタ変数を含むメッセージなら、アドレスを変換
	switch( Message.M_Message )
	{
	//オブジェクト
	case Msg::OBJECT_MOUSE_OVER:
	case Msg::OBJECT_MOUSE_OUT:
	case Msg::WINDOW_FOCUS_HIT:
	case Msg::WINDOW_FOCUS_OUT:
	case Msg::WINDOW_BTN_OVER:
	case Msg::WINDOW_BTN_OUT:
	case Msg::WINDOW_BTN_L_PRESS:
	case Msg::WINDOW_BTN_L_RELEASE:
	case Msg::WINDOW_BTN_REDRAW:
		Message.Mu4_Data[0] = (u4)KStdLib::ConvK2L( (void*)Message.Mu4_Data[0], P_DestWinList->Pv_LocalBase );
		break;
	default:
		break;
	}

	//メッセージ送信
	P_DestWinList->P_Window->SendMessage( Message );
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ウィンドウ管理クラス：プライベートメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	メッセージ送信
	説明	：	メッセージの送信を統括して行います。
				基本的には、フォーカスのあたっているウィンドウにメッセージを送ります。
				マウスのメッセージなら、Zオーダを変更したり、フォーカスを変更したり、
				ボタンオブジェクトを光らせたり、マウスポインタの種類を変更したり、
				マウスの再描画をしたり、いろいろやります。。。
	Include	：	WM.h
	引数	：	Msg Message
	戻り値	：	void
*******************************************************************************/
void	WM::SendMessage( Msg Message )
{
	//変数宣言
	//----------------------------------------------------------
	//解像度
	ui		ui_ResX		= G_GUI.GetResX();
	ui		ui_ResY		= G_GUI.GetResY();
	//----------------------------------------------------------

	switch( Message.M_Message )
	{
	//マウス左プレス
	case Msg::MOUSE_L_PRESS:
		MouseLPress( Message, ui_ResX, ui_ResY );
		break;
	//マウス左リリース
	case Msg::MOUSE_L_RELEASE:
		MouseLRelease( Message, ui_ResX, ui_ResY );
		break;

	//マウス移動
	case Msg::MOUSE_MOVE:
		MouseMove( Message, ui_ResX, ui_ResY );
		break;

	//通常処理
	default:
		SendMessageSub( Message, MP_Focus );
		break;
	}
}


/*******************************************************************************
	概要	：	共通の親オブジェクト取得
	説明	：	P_Object1とP_Object2の共通の親オブジェクトを探索して返します。
	Include	：	WM.h
	引数	：	Object* P_Object1
				Object* P_Object2
				void* Pv_LocalBase	ローカルのベースアドレス
	戻り値	：	Box*		共通のオブジェクト
*******************************************************************************/
Box*	WM::GetKinship( Object* P_Object1, Object* P_Object2, void* Pv_LocalBase ) const
{
	Box*	PA_Obj1Box[32];		//オブジェクト1の親のボックス一覧(0:最も近い親、31:最も遠い親(ウィンドウ))
	Box*	PA_Obj2Box[32];		//オブジェクト2の親のボックス一覧
	ui		ui_Obj1BoxCount = 0;
	ui		ui_Obj2BoxCount = 0;

	//Object1の親を辿ってボックスを格納
	while( true )
	{
		//ボックスを継承しているオブジェクトなら、ボックス一覧に格納
		if( P_Object1->CheckInheritance( Object::BOX ) )
			PA_Obj1Box[ui_Obj1BoxCount++] = (Box*)P_Object1;

		//親オブジェクト・ポインタ取得
		if( ( P_Object1 = P_Object1->GetParent() ) == NULL )
			break;		//親なしなら終了
		P_Object1 = (Object*)KStdLib::ConvL2K( P_Object1, Pv_LocalBase );	//アドレス変換
	}
	if( ui_Obj1BoxCount == 0 )		//ボックスが無い = 共通のオブジェクトが無い。
		return NULL;

	//Object2の親を辿ってボックスを格納
	while( true )
	{
		//ボックスを継承しているオブジェクトなら、ボックス一覧に格納
		if( P_Object2->CheckInheritance( Object::BOX ) )
			PA_Obj2Box[ui_Obj2BoxCount++] = (Box*)P_Object2;

		//親オブジェクト・ポインタ取得
		if( ( P_Object2 = P_Object2->GetParent() ) == NULL )
			break;		//親なしなら終了
		P_Object2 = (Object*)KStdLib::ConvL2K( P_Object2, Pv_LocalBase );	//アドレス変換
	}
	if( ui_Obj2BoxCount == 0 )		//ボックスが無い = 共通のオブジェクトが無い。
		return NULL;

	//上位から比較する。
	while( ui_Obj1BoxCount != 0 && ui_Obj2BoxCount != 0 )
		if( PA_Obj1Box[--ui_Obj1BoxCount] != PA_Obj2Box[--ui_Obj2BoxCount] )
			return PA_Obj1Box[++ui_Obj1BoxCount];		//共通の親有り。終了

	return PA_Obj1Box[ui_Obj1BoxCount];		//共通の親有り。終了
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

