/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/WM/DrawObject/WMDO_Window.cpp
	概要		：	ウィンドウ管理 - ウィンドウ描画
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"WM.h"
#include	"WMDO_Define.h"
#include	"Define/WM_Window.h"

#include	<Rect.h>
#include	<string.h>



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数・マクロ定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
//ウィンドウリソースのベースアドレス
#define	RSRC_WINDOW_BASE		( (Color4(*)[RSRC_WINDOW_HEIGHT][RSRC_WINDOW_WIDTH])G_GMA.GetBase( Mui_WinRsrcGMID ) )
#define	RSRC_BUTTON_BASE		( (Color4(*)[RSRC_BUTTON_NUM][RSRC_BUTTON_HEIGHT][RSRC_BUTTON_WIDTH])( RSRC_WINDOW_BASE + RSRC_WINDOW_NUM ) )
#define	RSRC_CLOSE_BASE			( RSRC_BUTTON_BASE + 0 )
#define	RSRC_MAX_BASE			( RSRC_BUTTON_BASE + 1 )
#define	RSRC_MIN_BASE			( RSRC_BUTTON_BASE + 2 )

//画像のファイルパス
#define	IMG_CLOSEBTN_FP		"/DATA/IMAGE_4.0/WSRSRC/WINDOW/CLOSE.BMP"
#define	IMG_MAXBTN_FP		"/DATA/IMAGE_4.0/WSRSRC/WINDOW/MAX.BMP"
#define	IMG_MINBTN_FP		"/DATA/IMAGE_4.0/WSRSRC/WINDOW/MIN.BMP"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
extern TM		G_TM;



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ウィンドウ管理クラス：プライベートメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	ウィンドウ管理クラスのウィンドウ関連の初期化
	説明	：	主にウィンドウリソースの読み取りを行います。
	Include	：	WM.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	WM::InitWindow( u1 u1_DeviceID )
{
	char	Ac_FilePath[256];

	//ウィンドウリソースのメモリ確保
	if( !( Mui_WinRsrcGMID = G_GMA.Create( RSRC_WINDOW_SIZE + RSRC_BUTTON_SIZE * NUM_BUTTON_TYPE ) ) )
		return;			//メモリ確保失敗

	do
	{
		G_TM.DisableTS();		//タスクスイッチ禁止

		//ウィンドウリソース読み取り
		sprintf( Ac_FilePath, "/%d%s", u1_DeviceID, IMG_WINDOW_FP );
		if( ReadBmp( Ac_FilePath, (Color4*)RSRC_WINDOW_BASE, RSRC_WINDOW_SIZE ) < 0 )
			break;

		sprintf( Ac_FilePath, "/%d%s", u1_DeviceID, IMG_CLOSEBTN_FP );
		if( ReadBmp( Ac_FilePath, (Color4*)RSRC_CLOSE_BASE, RSRC_BUTTON_SIZE ) < 0 )
			break;

		sprintf( Ac_FilePath, "/%d%s", u1_DeviceID, IMG_MAXBTN_FP );
		if( ReadBmp( Ac_FilePath, (Color4*)RSRC_MAX_BASE, RSRC_BUTTON_SIZE ) < 0 )
			break;

		sprintf( Ac_FilePath, "/%d%s", u1_DeviceID, IMG_MINBTN_FP );
		if( ReadBmp( Ac_FilePath, (Color4*)RSRC_MIN_BASE, RSRC_BUTTON_SIZE ) < 0 )
			break;

		G_TM.EnableTS();		//タスクスイッチ許可

		DP( "WM::InitWindow()\t\t\t\t\t\t\t[  OK  ]" );
		return;			//正常終了
	}while( false );

	//以下、異常時の処理
	G_TM.EnableTS();		//タスクスイッチ許可
	G_GMA.Delete( Mui_WinRsrcGMID );
	Mui_WinRsrcGMID = 0;
	DP( "WM::InitWindow()\t\t\t\t\t\t\t[Failed]" );
	return;			//異常終了
}


/*******************************************************************************
	概要	：	ウィンドウ描画
	説明	：	リソースからバッファにウィンドウを描画します。
	Include	：	WM.h
	引数	：	WMDO_PP *P_PP		受け渡しパラメータ
	戻り値	：	void
*******************************************************************************/
void	WM::DrawWindow( WMDO_PP *P_PP )
{
	Color4	(*P_WinBuf)[WINBUF_MAX_WIDTH] = P_PP->P_WinBuf;
	Color4	(*P_WinRsrc)[RSRC_WINDOW_HEIGHT][RSRC_WINDOW_WIDTH]		= RSRC_WINDOW_BASE;			//ウィンドウリソース
	Color4	(*P_ButtonRsrc)[RSRC_BUTTON_NUM][RSRC_BUTTON_HEIGHT][RSRC_BUTTON_WIDTH]	= RSRC_BUTTON_BASE;			//ウィンドウボタンリソース
	Window*	P_Window	= (Window*)P_PP->P_Object;		//ウィンドウポインタ
	Rect	Area		= P_PP->P_Object->GetArea();	//ウィンドウ領域
	const ui	Cui_WinWidth	= Area.GetWidth();		//ウィンドウ幅
	const ui	Cui_WinHeight	= Area.GetHeight();		//ウィンドウ高さ
	ui		ui_CountX, ui_CountY;						//カウンタ
	ui		ui_RsrcCC;									//リソース用カウントキャッシュ
	bool	b_Gray = ( P_PP->P_WinList == M_WinLM.GetHeadList() );		//グレーなら0、通常なら1


	//ボタンの再描画
	if( P_PP->Pu4_Option[0] != 0 )
	{
		//ボタン描画
		{
			Rect	DrawArea;
			ui		ui_ButtonCount	= (u1)P_PP->Pu4_Option[0];
			ui		ui_ButtonPhase	= (u1)( P_PP->Pu4_Option[0] >> 8 );
			si		si_ButtonX		= Cui_WinWidth - RSRC_BUTTON_WIDTH - BUTTON_RIGHT - ( RSRC_BUTTON_WIDTH + BUTTON_SPACING ) * ui_ButtonCount;
			si		si_ButtonY		= BUTTON_TOP;

			for( ui_CountY = 0; ui_CountY < RSRC_BUTTON_HEIGHT; ui_CountY++ )
				for( ui_CountX = 0; ui_CountX < RSRC_BUTTON_WIDTH; ui_CountX++ )
					if( P_ButtonRsrc[ui_ButtonCount][ui_ButtonPhase][ui_CountY][ui_CountX].u1_Alpha == 0xff )
						P_WinBuf[ui_CountY + si_ButtonY][ui_CountX + si_ButtonX] = P_ButtonRsrc[ui_ButtonCount][ui_ButtonPhase][ui_CountY][ui_CountX];

			DrawArea = Area;
			DrawArea.Msi_Top		+= si_ButtonY;
			DrawArea.Msi_Left		+= si_ButtonX;
			DrawArea.Msi_Right		= DrawArea.Msi_Left + RSRC_BUTTON_WIDTH - 1;
			DrawArea.Msi_Bottom		= DrawArea.Msi_Top + RSRC_BUTTON_HEIGHT - 1;
			G_GUI.DrawUpdate( DrawArea );
		}
		return;
	}


	//ウィンドウ0クリア
	memset( P_WinBuf, 0, WINBUF_SIZE );

	//背景色描画
	{
		Color4			BGColor = P_Window->GetBGColor();
		Box::Padding	Pad = P_Window->GetPadding();
		if( BGColor.u1_Alpha == 0xff )
			for( ui_CountY = Pad.u1_Top; ui_CountY <= Cui_WinHeight - Pad.u1_Bottom; ui_CountY++ )
				for( ui_CountX = Pad.u1_Left; ui_CountX <= Cui_WinWidth - Pad.u1_Right; ui_CountX++ )
					P_WinBuf[ui_CountY][ui_CountX] = BGColor;
	}

	//キャプションバー描画
	{
		for( ui_CountY = 0; ui_CountY < CPTBAR_HEIGHT; ui_CountY++ )
		{
			ui_RsrcCC	= ui_CountY * RSRC_WINDOW_WIDTH;
			//キャプションバーの端
			for( ui_CountX = 0; ui_CountX < CPTBAR_EDGE_WIDTH; ui_CountX++ )
			{
				//左上角(3*20)
				if( P_WinRsrc[b_Gray][ui_CountY][ui_CountX].u1_Alpha != 0 )		//透明でなければ描画
					P_WinBuf[ui_CountY][ui_CountX] = P_WinRsrc[b_Gray][ui_CountY][ui_CountX];
				//右上角(3*20)
				if( P_WinRsrc[b_Gray][ui_CountY][RSRC_WINDOW_WIDTH - ui_CountX - 1].u1_Alpha != 0 )		//透明でなければ描画
					P_WinBuf[ui_CountY][Cui_WinWidth - ui_CountX] = P_WinRsrc[b_Gray][ui_CountY][RSRC_WINDOW_WIDTH - ui_CountX - 1];
			}
			//キャプションバー
			for( ui_CountX = CPTBAR_EDGE_WIDTH; ui_CountX <= Cui_WinWidth - CPTBAR_EDGE_WIDTH; ui_CountX++ )
				P_WinBuf[ui_CountY][ui_CountX] = P_WinRsrc[b_Gray][ui_CountY][CPTBAR_EDGE_WIDTH];
		}
	}

	//フレーム描画
	{
		//左右
		for( ui_CountY = CPTBAR_HEIGHT; ui_CountY <= Cui_WinHeight - WINFRAME_WIDTH; ui_CountY++ )
		{
			ui_RsrcCC	= CPTBAR_HEIGHT + ( ( ui_CountY - CPTBAR_HEIGHT ) % ( RSRC_WINDOW_HEIGHT - CPTBAR_HEIGHT - WINFRAME_WIDTH ) );
			for( ui_CountX = 0; ui_CountX < WINFRAME_WIDTH; ui_CountX++ )
			{
				P_WinBuf[ui_CountY][ui_CountX] = P_WinRsrc[b_Gray][ui_RsrcCC][ui_CountX];
				P_WinBuf[ui_CountY][Cui_WinWidth - ui_CountX].u4_Color = P_WinRsrc[b_Gray][ui_RsrcCC][RSRC_WINDOW_WIDTH - ui_CountX - 1].u4_Color;
			}
		}

		//下＆下隅
		for( ui_CountY = Cui_WinHeight - WINFRAME_WIDTH; ui_CountY < Cui_WinHeight; ui_CountY++ )
		{
			ui_RsrcCC	= ui_CountY - Cui_WinHeight + RSRC_WINDOW_HEIGHT;
			for( ui_CountX = WINFRAME_WIDTH; ui_CountX <= Cui_WinWidth - WINFRAME_WIDTH; ui_CountX++ )
				P_WinBuf[ui_CountY + 1][ui_CountX] = P_WinRsrc[b_Gray][ui_RsrcCC][WINFRAME_WIDTH + ( ( ui_CountX - WINFRAME_WIDTH ) % ( RSRC_WINDOW_WIDTH - ( WINFRAME_WIDTH << 1 ) ) )];

			for( ui_CountX = 0; ui_CountX < WINFRAME_WIDTH; ui_CountX++ )
			{
				P_WinBuf[ui_CountY + 1][ui_CountX] = P_WinRsrc[b_Gray][ui_RsrcCC][ui_CountX];
				P_WinBuf[ui_CountY + 1][Cui_WinWidth - ui_CountX] = P_WinRsrc[b_Gray][ui_RsrcCC][RSRC_WINDOW_WIDTH - ui_CountX - 1];
			}
		}
	}

	//キャプション描画
	{

	}

	//各種ボタン描画
	{
		si		si_ButtonX, si_ButtonY = BUTTON_TOP;
		ui		ui_ButtonCount = 0;

		//三種類描画
		for(	ui_ButtonCount = 0, si_ButtonX = Cui_WinWidth - RSRC_BUTTON_WIDTH - BUTTON_RIGHT;	//初期化式
				ui_ButtonCount < NUM_BUTTON_TYPE && si_ButtonX > BUTTON_RIGHT;						//継続条件式
				ui_ButtonCount++, si_ButtonX -= RSRC_BUTTON_WIDTH + BUTTON_SPACING )				//再初期化式
		{
			for( ui_CountY = 0; ui_CountY < RSRC_BUTTON_HEIGHT; ui_CountY++ )
				for( ui_CountX = 0; ui_CountX < RSRC_BUTTON_WIDTH; ui_CountX++ )
					if( P_ButtonRsrc[ui_ButtonCount][b_Gray][ui_CountY][ui_CountX].u1_Alpha == 0xff )
						P_WinBuf[ui_CountY + si_ButtonY][ui_CountX + si_ButtonX] = P_ButtonRsrc[ui_ButtonCount][b_Gray][ui_CountY][ui_CountX];
		}
	}

	//描画領域設定
//	P_PP->DrawArea.Move( -P_PP->P_WinList->P_Window->GetOffsetX(), -P_PP->P_WinList->P_Window->GetOffsetY() );
}


/*******************************************************************************
	概要	：	ウィンドウ描画
	説明	：	リソースからバッファにウィンドウを描画します。
	Include	：	WM.h
	引数	：	Color4* P_WinBuf		描画先のウィンドウバッファ
				Window* P_Window		ウィンドウクラスのポインタ(カーネルポインタ)
				void* Pv_LocalBase		ローカルのベースアドレス
	戻り値	：	void
*******************************************************************************
void	WM::DrawWindow( Color4* P_WinBuf, Window* P_Window, void* Pv_LocalBase )
{
	Rect	Area			= P_Window->GetArea();
	Color4*	P_WinRsrc		= RSRC_WINDOW_BASE;			//ウィンドウリソース
	ui		ui_WinWidth		= Area.GetWidth();
	ui		Cui_WinHeight	= Area.GetHeight();
	ui		ui_CountX, ui_CountY;
	ui		ui_RsrcCC;			//リソース用カウントキャッシュ
	ui		ui_BufCC;			//バッファ用カウントキャッシュ


	enum
	{
		CPTBAR_EDGE_WIDTH		= 3,		//キャプションバーの端の幅
		CPTBAR_HEIGHT			= 20,		//キャプションバーの高さ
		WINFRAME_WIDTH				= 2,		//ウィンドウフレームの幅(左、右、下)
	};

	DP( "WM::DrawWindow():Start!" );

	//ウィンドウ0クリア
	memset( P_WinBuf, 0, WINBUF_SIZE );



	//キャプションバー描画
	{
		for( ui_CountY = 0; ui_CountY < CPTBAR_HEIGHT; ui_CountY++ )
		{
			ui_RsrcCC	= ui_CountY * RSRC_WINDOW_WIDTH;
			ui_BufCC	= ui_CountY * WINBUF_MAX_WIDTH;
#if 1	//どっちが早いか。。それともアセンブラか。
			//左上角(3*20)
			for( ui_CountX = 0; ui_CountX < CPTBAR_EDGE_WIDTH; ui_CountX++ )
				if( P_WinRsrc[ui_RsrcCC + ui_CountX].u1_Alpha != 0 )		//透明でなければ描画
					P_WinBuf[ui_BufCC + ui_CountX] = P_WinRsrc[ui_RsrcCC + ui_CountX];
			//右上角(3*20)
			for( ui_CountX = 0; ui_CountX < CPTBAR_EDGE_WIDTH; ui_CountX++ )
				if( P_WinRsrc[ui_RsrcCC + RSRC_WINDOW_WIDTH + ui_CountX - CPTBAR_EDGE_WIDTH].u1_Alpha != 0 )		//透明でなければ描画
					P_WinBuf[ui_BufCC + ui_WinWidth + ui_CountX - CPTBAR_EDGE_WIDTH] = P_WinRsrc[ui_RsrcCC + RSRC_WINDOW_WIDTH + ui_CountX - CPTBAR_EDGE_WIDTH];
#else
			//キャプションバーの端
			for( ui_CountX = 0; ui_CountX < CPTBAR_EDGE_WIDTH; ui_CountX++ )
			{
				//左上角(3*20)
				if( P_WinRsrc[ui_RsrcCC + ui_CountX].u1_Alpha != 0 )		//透明でなければ描画
					P_WinBuf[ui_BufCC + ui_CountX].u4_Color = P_WinRsrc[ui_RsrcCC + ui_CountX].u4_Color;
				//右上角(3*20)
				if( P_WinRsrc[ui_RsrcCC + RSRC_WINDOW_WIDTH + ui_CountX - CPTBAR_EDGE_WIDTH].u1_Alpha != 0 )		//透明でなければ描画
					P_WinBuf[ui_BufCC + ui_WinWidth + ui_CountX - CPTBAR_EDGE_WIDTH].u4_Color = P_WinRsrc[ui_RsrcCC + RSRC_WINDOW_WIDTH + ui_CountX - CPTBAR_EDGE_WIDTH].u4_Color;
			}
#endif
			//キャプションバー
			for( ui_CountX = CPTBAR_EDGE_WIDTH; ui_CountX < ui_WinWidth - CPTBAR_EDGE_WIDTH; ui_CountX++ )
				P_WinBuf[ui_BufCC + ui_CountX] = P_WinRsrc[ui_RsrcCC + CPTBAR_EDGE_WIDTH];
		}
	}

	//背景色描画
	{
		Color4			BGColor = P_Window->GetBGColor();
		Box::Padding	Pad = P_Window->GetPadding();
		ui				ui_BufCC;
		if( BGColor.u1_Alpha == 0xff )
		{
			for( ui_CountY = Pad.u1_Top; ui_CountY < Cui_WinHeight - Pad.u1_Bottom; ui_CountY++ )
			{
				ui_BufCC	= ui_CountY * WINBUF_MAX_WIDTH;
				for( ui_CountX = Pad.u1_Left; ui_CountX < ui_WinWidth - Pad.u1_Right; ui_CountX++ )
					P_WinBuf[ui_BufCC + ui_CountX] = BGColor;
			}
		}
	}

	//フレーム描画
	{
		for( ui_CountY = CPTBAR_HEIGHT; ui_CountY < Cui_WinHeight - WINFRAME_WIDTH; ui_CountY++ )
		{
			ui_RsrcCC	= ( CPTBAR_HEIGHT + ( ( ui_CountY - CPTBAR_HEIGHT ) % ( RSRC_WINDOW_HEIGHT - CPTBAR_HEIGHT - WINFRAME_WIDTH ) ) ) * RSRC_WINDOW_WIDTH;
			ui_BufCC	= ui_CountY * WINBUF_MAX_WIDTH;
#if 1	//どっちが早いか。。それともアセンブラか。
			//左
			for( ui_CountX = 0; ui_CountX < WINFRAME_WIDTH; ui_CountX++ )
				P_WinBuf[ui_BufCC + ui_CountX] = P_WinRsrc[ui_RsrcCC + ui_CountX];
			//右
			for( ui_CountX = 0; ui_CountX < WINFRAME_WIDTH; ui_CountX++ )
				P_WinBuf[ui_BufCC + ui_CountX + ui_WinWidth - WINFRAME_WIDTH].u4_Color = P_WinRsrc[ui_RsrcCC + ui_CountX + RSRC_WINDOW_WIDTH - WINFRAME_WIDTH].u4_Color;
#else
			for( ui_CountX = 0; ui_CountX < WINFRAME_WIDTH; ui_CountX++ )
			{
				P_WinBuf[ui_BufCC + ui_CountX] = P_WinRsrc[ui_RsrcCC + ui_CountX];
				P_WinBuf[ui_BufCC + ui_CountX + ui_WinWidth - WINFRAME_WIDTH].u4_Color = P_WinRsrc[ui_RsrcCC + ui_CountX + RSRC_WINDOW_WIDTH - WINFRAME_WIDTH].u4_Color;
			}
#endif
		}

		//下
		for( ui_CountY = Cui_WinHeight - WINFRAME_WIDTH; ui_CountY < Cui_WinHeight; ui_CountY++ )
		{
			ui_RsrcCC	= ( ui_CountY - Cui_WinHeight + RSRC_WINDOW_HEIGHT ) * RSRC_WINDOW_WIDTH;
			ui_BufCC	= ui_CountY * WINBUF_MAX_WIDTH;
			for( ui_CountX = WINFRAME_WIDTH; ui_CountX < Cui_WinHeight - WINFRAME_WIDTH; ui_CountX++ )
				P_WinBuf[ui_BufCC + ui_CountX] = P_WinRsrc[ui_RsrcCC + WINFRAME_WIDTH + ( ( ui_CountX - WINFRAME_WIDTH ) % ( RSRC_WINDOW_WIDTH - WINFRAME_WIDTH - WINFRAME_WIDTH ) )];
		}

		//下隅
		for( ui_CountY = Cui_WinHeight - WINFRAME_WIDTH; ui_CountY < Cui_WinHeight; ui_CountY++ )
		{
			ui_RsrcCC	= ( ui_CountY - Cui_WinHeight + RSRC_WINDOW_HEIGHT ) * RSRC_WINDOW_WIDTH;
			ui_BufCC	= ui_CountY * WINBUF_MAX_WIDTH;
#if 1	//どっちが早いか。。それともアセンブラか。
			//左
			for( ui_CountX = 0; ui_CountX < WINFRAME_WIDTH; ui_CountX++ )
				P_WinBuf[ui_BufCC + ui_CountX] = P_WinRsrc[ui_RsrcCC + ui_CountX];
			//右
			for( ui_CountX = 0; ui_CountX < WINFRAME_WIDTH; ui_CountX++ )
				P_WinBuf[ui_BufCC + ui_CountX + ui_WinWidth - WINFRAME_WIDTH] = P_WinRsrc[ui_RsrcCC + ui_CountX + RSRC_WINDOW_WIDTH - WINFRAME_WIDTH];
#else
			for( ui_CountX = 0; ui_CountX < WINFRAME_WIDTH; ui_CountX++ )
			{
				P_WinBuf[ui_BufCC + ui_CountX] = P_WinRsrc[ui_RsrcCC + ui_CountX];
				P_WinBuf[ui_BufCC + ui_CountX + ui_WinWidth - WINFRAME_WIDTH] = P_WinRsrc[ui_RsrcCC + ui_CountX + RSRC_WINDOW_WIDTH - WINFRAME_WIDTH];
			}
#endif
		}
	}




	DP( "WM::DrawWindow():Finished!" );
//G_GUI.FullDrawUpdate();
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

