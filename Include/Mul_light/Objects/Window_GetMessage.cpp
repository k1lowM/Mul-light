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
	概要	：	メッセージ取得
	説明	：	メッセージを取得します。
				メッセージが無い場合はメッセージが来るまでタスクを休止します。
				システムメッセージがきた場合は、全てここで処理します。
	Include	：	Window.h
	引数	：	void
	戻り値	：	Msg		メッセージ
*******************************************************************************/
Msg		Window::GetMessage( void )
{
	Msg		MsgBuf;


	//メッセージ取得待ち
	while( true )
	{
		//キューが空でなく、システムメッセージでなければ取得待ちを抜ける
		if( M_MsgQ.Dequeue( &MsgBuf ) > 0
				&& !( MsgBuf.M_Message & Msg::MSGMASK_SYSTEM ) )
			break;

		//システムメッセージ処理
		switch( MsgBuf.M_Message )
		{
		case Msg::SYS_QUIT:
		{
			SCD::AplQuit	SCD_AplQuit;
			SCD_AplQuit.Mui_AppliID		= Mui_AppliID;
			SC::SystemCall( SC::APL_QUIT, &SCD_AplQuit );
			break;
		}

		default:
			break;
		}

		MLStdLib::Sleep();		//メッセージが送信されるまで待つ。
	}

	return MsgBuf;

}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

