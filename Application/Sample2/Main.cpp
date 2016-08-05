/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Application/Sample2/Main.cpp
	概要		：	サンプル
	詳細		：	アプリケーションのサンプルプログラムです。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	<Mul_light.h>
#include	<stdio.h>



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	関数
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	メイン関数です。
	説明	：
	Include	：
	引数	：	ui ui_AppliID		アプリケーションのID
	戻り値	：	void
*******************************************************************************/
void	ML_Main( ui ui_AppliID )
{
	Window	MainWindow;			//ウィンドウ

	Text	Text1;				//テキスト1

	Msg		Msg;			//メッセージ受け取り変数

	//オブジェクト初期化
	{
		MainWindow.Init();				//ウィンドウ初期化
		Text1.Init();					//テキスト1初期化
	}

	//オブジェクト初期設定
	{
		//メインウィンドウ
		MainWindow.SetChild( &Text1 );	//子オブジェクト設定
		MainWindow.SetSize( 0, 0, Object::WIDTH_MIN, Object::HEIGHT_MIN );

		//テキスト
		Text1.SetText( "Application : Sample2" );		//ラベル設定
	}


	//ウィンドウ登録
	MainWindow.RegiWindow( ui_AppliID );


	//メインループ
	while( true )
	{
		Msg = MainWindow.GetMessage();		//メッセージ取得
		switch( Msg.M_Message )
		{
		case Msg::MOUSE_MOVE:
			break;
		default:
			MainWindow.MsgDefProc( Msg );
			break;
		}
	}
}





/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
