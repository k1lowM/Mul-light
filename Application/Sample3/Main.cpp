/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Application/Sample3/Main.cpp
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
	Pack	HPack;				//水平パック

	Text	Text1;				//テキスト1
	Text	Text2;				//テキスト2

	Button	Button1;			//ボタン1
	Text	Button1Text;		//ボタン1テキスト
	Button	Button2;			//ボタン2
	Text	Button2Text;		//ボタン2テキスト

	Msg		Msg;			//メッセージ受け取り変数

	//オブジェクト初期化
	{
		MainWindow.Init();				//ウィンドウ初期化
		HPack.Init( Pack::HORIZONTAL );	//水平パック初期化
		Text1.Init();					//テキスト1初期化
		Text2.Init();					//テキスト2初期化
		Button1.Init();					//ボタン1初期化
		Button1Text.Init();				//ボタン1テキスト初期化
		Button2.Init();					//ボタン2初期化
		Button2Text.Init();				//ボタン2テキスト初期化
	}

	//オブジェクト初期設定
	{
		//メインウィンドウ
		MainWindow.SetOffset( 0x10, 0x10, Object::H_LEFT, Object::V_TOP );
		MainWindow.SetSize( 0x200, 0x80, Object::WIDTH_PIXEL, Object::HEIGHT_PIXEL );
		MainWindow.SetChild( &HPack );	//子オブジェクト設定

		//水平パック
		HPack.AddChild( &Button1 );
		HPack.AddChild( &Text1 );
		HPack.AddChild( &Button2 );
		HPack.AddChild( &Text2 );

		//ボタン
		Button1.SetSize( 0, 0, Object::WIDTH_MIN, Object::HEIGHT_MIN );
		Button1.SetChild( &Button1Text );
		Button2.SetChild( &Button2Text );

		//ラベル
		Text1.SetText( "TextColor\nR-0xff, G-0x7f, B-0x00" );		//ラベル設定
		Text1.SetTextColor( 0xffff7f00 );
		Text2.SetText( "Text2\nSample3\nline feed sample." );		//ラベル設定
		Text2.SetTextColor( 0xff003fff );
		Button1Text.SetText( "Button1" );
		Button1Text.SetTextColor( 0xffffffff );
		Button2Text.SetText( "Button2" );
		Button2Text.SetTextColor( 0xffffffff );
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
