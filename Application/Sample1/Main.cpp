/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Application/Sample1/Main.cpp
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
	Pack	VPack;				//垂直パック

	Text	Text1;				//テキスト1
	Text	Text2;				//テキスト2
	Text	Text3;				//テキスト3
	Text	Text4;				//テキスト4

	Button	Button1;			//ボタン1
	Text	Button1Text;		//ボタン1テキスト
	Button	Button2;			//ボタン2
	Text	Button2Text;		//ボタン2テキスト
	Button	Button3;			//ボタン3
	Text	Button3Text;		//ボタン3テキスト

	Msg		Msg;			//メッセージ受け取り変数

	//オブジェクト初期化
	{
		MainWindow.Init();				//ウィンドウ初期化
		HPack.Init( Pack::HORIZONTAL );	//水平パック初期化
		VPack.Init( Pack::VERTICAL );	//垂直パック初期化
		Text1.Init();					//テキスト1初期化
		Text2.Init();					//テキスト2初期化
		Text3.Init();					//テキスト3初期化
		Text4.Text::Init();					//テキスト4初期化
		Button1.Init();					//ボタン1初期化
		Button1Text.Init();				//ボタン1テキスト初期化
		Button2.Init();					//ボタン2初期化
		Button2Text.Init();				//ボタン2テキスト初期化
		Button3.Init();					//ボタン3初期化
		Button3Text.Init();				//ボタン3テキスト初期化
	}

	//オブジェクト初期設定
	{
		//メインウィンドウ
		MainWindow.SetChild( &VPack );	//子オブジェクト設定
		MainWindow.SetSize( 0x200, 0x100, Object::WIDTH_PIXEL, Object::HEIGHT_PIXEL );

		//垂直パック
		VPack.AddChild( &HPack );
		VPack.AddChild( &Text3 );
		VPack.AddChild( &Button3 );
		VPack.AddChild( &Text4 );

		//水平パック
		HPack.AddChild( &Text1 );
		HPack.AddChild( &Button1 );
		HPack.AddChild( &Button2 );
		HPack.AddChild( &Text2 );

		//ボタン
		Button1.SetChild( &Button1Text );
		Button2.SetChild( &Button2Text );
		Button3.SetChild( &Button3Text );

		//ラベル
		Text1.SetText( "This is Text.\nOffset:C(0), C(0)\nSize:MIN(0), MIN(0)" );		//ラベル設定
		Text2.SetText( "Text2\nabcdefghijklmn" );		//ラベル設定
		Text3.SetText( "Text3\nMul light 1.0" );		//ラベル設定
		Text4.SetText( "Text4\nThis Application is Sample." );		//ラベル設定
		Button1Text.SetText( "Button1" );
		Button1Text.SetTextColor( 0xffffffff );
		Button2Text.SetText( "Button2" );
		Button2Text.SetTextColor( 0xffffffff );
		Button3Text.SetText( "Button3" );
		Button3Text.SetTextColor( 0xffffffff );
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
