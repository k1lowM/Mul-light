/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/WM/DrawObject/WMDO_Button.cpp
	概要		：	ウィンドウ管理 - ボタン描画
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"WM.h"
#include	"WMDO_Define.h"
#include	"Define/WM_Button.h"

#include	<Objects/Button.h>
#include	<Rect.h>



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数・マクロ定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
//ウィンドウリソースのベースアドレス
#define	RSRC_BUTTON_BASE		( (Color4(*)[RSRC_BUTTON_TYPE][RSRC_BUTTON_HEIGHT][RSRC_BUTTON_WIDTH])G_GMA.GetBase( Mui_BtnRsrcGMID ) )

//画像のファイルパス
#define	IMG_BUTTON_FP		"/DATA/IMAGE_4.0/WSRSRC/BUTTON/BUTTON.BMP"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
extern TM		G_TM;



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	プロトタイプ
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
namespace
{
enum
{
	BTN_FRAME,
	BTN_BG,
};
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ウィンドウ管理クラス：プライベートメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	ウィンドウ管理クラスのボタン関連の初期化
	説明	：	主にボタンリソースの読み取りを行います。
	Include	：	WM.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	WM::InitButton( u1 u1_DeviceID )
{
	char	Ac_FilePath[256];

	//ボタンリソースのメモリ確保
	if( !( Mui_BtnRsrcGMID = G_GMA.Create( RSRC_BUTTON_SIZE ) ) )
		return;			//メモリ確保失敗

	do
	{
		G_TM.DisableTS();		//タスクスイッチ禁止

		//ボタンリソース読み取り
		sprintf( Ac_FilePath, "/%d%s", u1_DeviceID, IMG_BUTTON_FP );
		if( ReadBmp( Ac_FilePath, (Color4*)RSRC_BUTTON_BASE, RSRC_BUTTON_SIZE ) < 0 )
			break;

		G_TM.EnableTS();		//タスクスイッチ許可
		return;			//正常終了
	}while( false );

	//以下、異常時の処理
	G_TM.EnableTS();		//タスクスイッチ許可
	G_GMA.Delete( Mui_BtnRsrcGMID );
	Mui_BtnRsrcGMID = 0;
	return;			//異常終了
}


/*******************************************************************************
	概要	：	ボタン描画
	説明	：	ウィンドウバッファにボタンを描画します。
	Include	：	WM.h
	引数	：	WMDO_PP *P_PP		受け渡しパラメータ
	戻り値	：	void
*******************************************************************************/
void	WM::DrawButton( WMDO_PP *P_PP )
{
	Color4	(*P_WinBuf)[WINBUF_MAX_WIDTH] = P_PP->P_WinBuf;	//描画先バッファ
	Color4	(*P_ButtonRsrc)[RSRC_BUTTON_TYPE][RSRC_BUTTON_HEIGHT][RSRC_BUTTON_WIDTH]	= RSRC_BUTTON_BASE;			//ボタンリソース
	Button*	P_Button		= (Button*)P_PP->P_Object;		//ボタンオブジェクト
	Rect	Area			= P_Button->GetArea();			//ボタンオブジェクトの領域
	Rect	ValidArea		= Area;							//有効領域
	ui		ui_ButtonPhase	= P_PP->Pu4_Option[0] & 0x03;	//ボタンフェーズ

	//エラー処理
//	if(  )
//		return;

	ValidArea.And( P_PP->ChArea );		//有効領域を設定

	//ボタン描画
	{
		ui		ui_Width	= Area.GetWidth();
		ui		ui_Height	= Area.GetHeight();
		si		si_CountX;
		si		si_CountY;
		si		si_DestXCC, si_DestYCC;		//カウント・キャッシュ
		ui		ui_RsrcXCC, ui_RsrcYCC;

		//背景描画
		for( si_CountY = Area.Msi_Top + 1; si_CountY < Area.Msi_Bottom; si_CountY++ )
		{
			if( si_CountY < ValidArea.Msi_Top )			continue;
			if( si_CountY > ValidArea.Msi_Bottom )		break;
			ui_RsrcYCC = ( si_CountY - Area.Msi_Top ) * ( RSRC_BTN_BG_HEIGHT ) / ( ui_Height ) + 1;
			for( si_CountX = Area.Msi_Left + 1; si_CountX < Area.Msi_Right; si_CountX++ )
			{
				if( si_CountX < ValidArea.Msi_Left )		continue;
				if( si_CountX > ValidArea.Msi_Right )		break;
				ui_RsrcXCC = ( si_CountX - Area.Msi_Left ) * ( RSRC_BTN_BG_WIDTH ) / ( ui_Width ) + 1;
				P_WinBuf[si_CountY][si_CountX] = P_ButtonRsrc[BTN_BG][ui_ButtonPhase][ui_RsrcYCC][ui_RsrcXCC];
			}
		}

		//フレーム描画
		{
			//フレーム上下------------------------------------------------------
			for( si_CountX = Area.Msi_Left + BTNEDGE_WIDTH; si_CountX <= Area.Msi_Right - BTNEDGE_WIDTH; si_CountX++ )
			{
				//有効領域外
				if( si_CountX < ValidArea.Msi_Left )		continue;
				if( si_CountX > ValidArea.Msi_Right )		break;

				for( si_CountY = 0; si_CountY < BTNFRAME_WIDTH; si_CountY++ )
				{
					si_DestYCC = Area.Msi_Top + si_CountY;

					//有効領域外
					if( si_DestYCC < ValidArea.Msi_Top )		continue;
					if( si_DestYCC > ValidArea.Msi_Bottom )		break;

					if( P_ButtonRsrc[BTN_FRAME][ui_ButtonPhase][si_CountY][BTNEDGE_WIDTH].u1_Alpha == 0xff )
						P_WinBuf[si_DestYCC][si_CountX] = P_ButtonRsrc[BTN_FRAME][ui_ButtonPhase][si_CountY][BTNEDGE_WIDTH];
				}
				for( si_CountY = 0; si_CountY < BTNFRAME_WIDTH; si_CountY++ )
				{
					si_DestYCC = Area.Msi_Bottom - si_CountY;

					//有効領域外
					if( si_DestYCC > ValidArea.Msi_Bottom )		continue;
					if( si_DestYCC < ValidArea.Msi_Top )		break;

					ui_RsrcYCC = RSRC_BUTTON_HEIGHT - si_CountY - 1;

					if( P_ButtonRsrc[BTN_FRAME][ui_ButtonPhase][ui_RsrcYCC][BTNEDGE_WIDTH].u1_Alpha == 0xff )
						P_WinBuf[si_DestYCC][si_CountX] = P_ButtonRsrc[BTN_FRAME][ui_ButtonPhase][ui_RsrcYCC][BTNEDGE_WIDTH];
				}
			}
			//フレーム左右------------------------------------------------------
			for( si_CountY = Area.Msi_Top + BTNEDGE_WIDTH; si_CountY <= Area.Msi_Bottom - BTNEDGE_WIDTH; si_CountY++ )
			{
				//有効領域外
				if( si_CountY < ValidArea.Msi_Top )			continue;
				if( si_CountY > ValidArea.Msi_Bottom )		break;

				for( si_CountX = 0; si_CountX < BTNFRAME_WIDTH; si_CountX++ )
				{
					si_DestXCC = Area.Msi_Left + si_CountX;

					//有効領域外
					if( si_DestXCC < ValidArea.Msi_Left )		continue;
					if( si_DestXCC > ValidArea.Msi_Right )		break;

					if( P_ButtonRsrc[BTN_FRAME][ui_ButtonPhase][BTNEDGE_WIDTH][si_CountX].u1_Alpha == 0xff )
						P_WinBuf[si_CountY][si_DestXCC] = P_ButtonRsrc[BTN_FRAME][ui_ButtonPhase][BTNEDGE_WIDTH][si_CountX];
				}
				for( si_CountX = 0; si_CountX < BTNFRAME_WIDTH; si_CountX++ )
				{
					si_DestXCC = Area.Msi_Right - si_CountX;

					//有効領域外
					if( si_DestXCC < ValidArea.Msi_Left )		continue;
					if( si_DestXCC > ValidArea.Msi_Right )		break;

					ui_RsrcXCC = RSRC_BUTTON_WIDTH - si_CountX - 1;

					if( P_ButtonRsrc[BTN_FRAME][ui_ButtonPhase][BTNEDGE_WIDTH][ui_RsrcXCC].u1_Alpha == 0xff )
						P_WinBuf[si_CountY][si_DestXCC] = P_ButtonRsrc[BTN_FRAME][ui_ButtonPhase][BTNEDGE_WIDTH][ui_RsrcXCC];
				}
			}

			//フレーム左上隅----------------------------------------------------
			for( si_CountY = 0; si_CountY < BTNEDGE_WIDTH; si_CountY++ )
			{
				si_DestYCC = Area.Msi_Top + si_CountY;

				//有効領域外
				if( si_DestYCC < ValidArea.Msi_Top )		continue;
				if( si_DestYCC > ValidArea.Msi_Bottom )		break;

				for( si_CountX = 0; si_CountX < BTNEDGE_WIDTH; si_CountX++ )
				{
					si_DestXCC = Area.Msi_Left + si_CountX;

					//有効領域外
					if( si_DestXCC < ValidArea.Msi_Left )		continue;
					if( si_DestXCC > ValidArea.Msi_Right )		break;

					if( P_ButtonRsrc[BTN_FRAME][ui_ButtonPhase][si_CountY][si_CountX].u1_Alpha == 0xff )
						P_WinBuf[si_DestYCC][si_DestXCC] = P_ButtonRsrc[BTN_FRAME][ui_ButtonPhase][si_CountY][si_CountX];
				}
			}
			//フレーム右上隅----------------------------------------------------
			for( si_CountY = 0; si_CountY < BTNEDGE_WIDTH; si_CountY++ )
			{
				si_DestYCC = Area.Msi_Top + si_CountY;

				//有効領域外
				if( si_DestYCC < ValidArea.Msi_Top )		continue;
				if( si_DestYCC > ValidArea.Msi_Bottom )		break;

				for( si_CountX = 0; si_CountX < BTNEDGE_WIDTH; si_CountX++ )
				{
					si_DestXCC = Area.Msi_Right - si_CountX;

					//有効領域外
					if( si_DestXCC > ValidArea.Msi_Right )		continue;
					if( si_DestXCC < ValidArea.Msi_Left )		break;

					ui_RsrcXCC = RSRC_BUTTON_WIDTH - si_CountX - 1;

					if( P_ButtonRsrc[BTN_FRAME][ui_ButtonPhase][si_CountY][ui_RsrcXCC].u1_Alpha == 0xff )
						P_WinBuf[si_DestYCC][si_DestXCC] = P_ButtonRsrc[BTN_FRAME][ui_ButtonPhase][si_CountY][ui_RsrcXCC];
				}
			}
			//フレーム左下隅----------------------------------------------------
			for( si_CountY = 0; si_CountY < BTNEDGE_WIDTH; si_CountY++ )
			{
				si_DestYCC = Area.Msi_Bottom - si_CountY;

				//有効領域外
				if( si_DestYCC > ValidArea.Msi_Bottom )		continue;
				if( si_DestYCC < ValidArea.Msi_Top )		break;

				ui_RsrcYCC = RSRC_BUTTON_HEIGHT - si_CountY - 1;

				for( si_CountX = 0; si_CountX < BTNEDGE_WIDTH; si_CountX++ )
				{
					si_DestXCC = Area.Msi_Left + si_CountX;
					//有効領域外
					if( si_DestXCC < ValidArea.Msi_Left )		continue;
					if( si_DestXCC > ValidArea.Msi_Right )		break;

					if( P_ButtonRsrc[BTN_FRAME][ui_ButtonPhase][ui_RsrcYCC][si_CountX].u1_Alpha == 0xff )
						P_WinBuf[si_DestYCC][si_DestXCC] = P_ButtonRsrc[BTN_FRAME][ui_ButtonPhase][ui_RsrcYCC][si_CountX];
				}
			}
			//フレーム右下隅----------------------------------------------------
			for( si_CountY = 0; si_CountY < BTNEDGE_WIDTH; si_CountY++ )
			{
				si_DestYCC = Area.Msi_Bottom - si_CountY;

				//有効領域外
				if( si_DestYCC < ValidArea.Msi_Top )		continue;
				if( si_DestYCC > ValidArea.Msi_Bottom )		break;

				ui_RsrcYCC = RSRC_BUTTON_HEIGHT - si_CountY - 1;

				for( si_CountX = 0; si_CountX < BTNEDGE_WIDTH; si_CountX++ )
				{
					si_DestXCC = Area.Msi_Right - si_CountX;

					//有効領域外
					if( si_DestXCC > ValidArea.Msi_Right )		continue;
					if( si_DestXCC < ValidArea.Msi_Left )		break;

					ui_RsrcXCC = RSRC_BUTTON_WIDTH - si_CountX - 1;

					if( P_ButtonRsrc[BTN_FRAME][ui_ButtonPhase][ui_RsrcYCC][ui_RsrcXCC].u1_Alpha == 0xff )
						P_WinBuf[si_DestYCC][si_DestXCC] = P_ButtonRsrc[BTN_FRAME][ui_ButtonPhase][ui_RsrcYCC][ui_RsrcXCC];
				}
			}
		}
	}
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	無名名前空間関数
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
namespace
{
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

