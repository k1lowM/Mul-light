/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Include/Mul_light/Objects/Window.cpp
	概要		：	Mul light API - ウィンドウクラス
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"MLCommon.h"

#include	"Window.h"
#include	"Text.h"

#include	"MLStdLib.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ウィンドウクラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	メッセージのデフォルト処理
	説明	：	全ての通常メッセージを適切に処理します。
	Include	：	Window.h
	引数	：	const Msg& CR_Message		処理するメッセージ
	戻り値	：	void
*******************************************************************************/
void	Window::MsgDefProc( const Msg& CR_Message )
{
	Msg		Message;

	switch( CR_Message.M_Message )
	{
	case Msg::FINALIZE:
		//アプリケーション終了を送信
		Message.Init( Msg::SYS_QUIT );
		SendMessage( Message );
		break;

	case Msg::OBJECT_REDRAW:
		DrawingAll();
		MLStdLib::FullDrawUpdate();		//全再描画
		break;

	case Msg::WINDOW_BTN_L_RELEASE:
		switch( CR_Message.Mu1_Data[1].LL )
		{
		case BUTTON_CLOSE:
			//終了処理を送信
			Message.Init( Msg::FINALIZE );
			SendMessage( Message );
			break;
		case BUTTON_MAX:
			//最大化を送信
			break;
		case BUTTON_MIN:
			//最小化を送信
			break;
		default:
			break;
		}
		break;

	case Msg::WINDOW_BTN_REDRAW:
	{
		//システムコール
		SCD::DrawObject	SCD_DrawObject;
		SCD_DrawObject.MP_Object		= (Object*)CR_Message.Mu4_Data[0];
		SCD_DrawObject.MAu4_Option[0]	= CR_Message.Mu4_Data[1] | 0x80000000;
		SCD_DrawObject.MAu4_Option[1]	= 0;
		SC::SystemCall( SC::DRAW_OBJECT, &SCD_DrawObject );
		break;
	}

	default:
		break;
	}
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

