/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/WM/DrawObject/WMDO_Main.cpp
	概要		：	ウィンドウ管理 - ウィンドウオブジェクト描画
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"WM.h"
#include	"WMDO_Define.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ウィンドウ管理クラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	オブジェクト描画
	説明	：	ウィンドウバッファに各種オブジェクトを描画します。
				LDTを使ったアプリケーションがシステムコールを通して使う関数です。
				カーネルは使用禁止！
	Include	：	WM.h
	引数	：	Object* P_Object		描画するオブジェクトのポインタ(ローカルポインタ)
				Rect ChArea				対象の親の子領域
				u4* Pu4_Option			オプション(カーネルポインタ。参照先は配列。)
				void* Pv_LocalBase		ローカルのベースアドレス
	戻り値	：	void
*******************************************************************************/
void	WM::DrawObject( Object* P_Object, Rect ChArea, u4* Pu4_Option, void* Pv_LocalBase )
{
	WMDO_PP		PP;		//受け渡しパラメータ(Passing Parameters)

	P_Object = (Object*)KStdLib::ConvL2K( P_Object, Pv_LocalBase );

	//PP初期化・設定
	PP.P_WinList	= (WinList*)( ( (Window*)KStdLib::ConvL2K( P_Object->GetWindow(), Pv_LocalBase ) )->GetKWinList() );
	PP.P_WinBuf		= (Color4 (*)[1024])G_GMA.GetBase( PP.P_WinList->ui_WinBufGMID );
	PP.P_Object		= P_Object;
	PP.ChArea		= ChArea;
	PP.Pu4_Option	= Pu4_Option;
//	PP.DrawArea		= PP.P_Object->GetArea();
	PP.Pv_LocalBase	= Pv_LocalBase;

	//オブジェクトタイプによって処理分岐。
	switch( P_Object->GetObjectType() )
	{
	case Object::WINDOW:
		DrawWindow( &PP );
		break;
	case Object::BUTTON:
		DrawButton( &PP );
		break;
	case Object::TEXT:
		DrawText( &PP );
		break;
	default:
		return;
	}

//	ここでは描画しない。
//	//描画
//	{
//		PP.DrawArea.Move( PP.P_WinList->P_Window->GetOffsetX(), PP.P_WinList->P_Window->GetOffsetY() );
//		DP( "%#x, %#x, %#x, %#x", PP.DrawArea.Msi_Top, PP.DrawArea.Msi_Left, PP.DrawArea.Msi_Right, PP.DrawArea.Msi_Bottom );
//		G_GUI.DrawUpdate( PP.DrawArea );
//	}
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

