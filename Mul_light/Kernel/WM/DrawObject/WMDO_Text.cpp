/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/WM/DrawObject/WMDO_Text.cpp
	概要		：	ウィンドウ管理 - テキスト描画
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"WM.h"
#include	"WMDO_Define.h"

#include	<Objects/Text.h>
#include	<Rect.h>



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
extern const u1		GCu1_DebugFont[128][16];



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	プロトタイプ
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
namespace
{
void	DrawChar( Color4 (*P_WinBuf)[1024], char c_Char, ui ui_X, ui ui_Y, Color4 TextColor, Rect& R_ValidArea );
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ウィンドウ管理クラス：プライベートメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	テキスト描画
	説明	：	ウィンドウバッファに文字列を描画します。
	Include	：	WM.h
	引数	：	WMDO_PP *P_PP		受け渡しパラメータ
	戻り値	：	void
*******************************************************************************/
void	WM::DrawText( WMDO_PP *P_PP )
{
	Color4		(*P_WinBuf)[WINBUF_MAX_WIDTH] = P_PP->P_WinBuf;	//描画先バッファ
	Text*		P_Text		= (Text*)P_PP->P_Object;			//テキストオブジェクト
	const char*	CPc_Text	= (const char*)KStdLib::ConvL2K( P_Text->GetText(), P_PP->Pv_LocalBase );		//文字列
	ui			ui_CharCount;									//文字カウンタ
	ui			ui_StrX = 0, ui_StrY = 0;						//
	Rect		Area		= P_Text->GetArea();				//テキストオブジェクトの領域
	Rect		ValidArea	= Area;								//有効領域
	Color4		TextColor	= { P_PP->Pu4_Option[0] };

	//エラー処理
	if( P_Text->GetText() == NULL )		//文字列が設定されていなければエラー
		return;
	if( TextColor.u1_Alpha == 0 )		//透明なら描画しない。
		return;

	ValidArea.And( P_PP->ChArea );		//有効領域を設定

	//文字列ループ
	for( ui_CharCount = 0; CPc_Text[ui_CharCount] != '\0'; ui_CharCount++ )
	{
		//改行コードならXをリセット、Yをインクリメント
		if( CPc_Text[ui_CharCount] == '\n' )
		{
			ui_StrY++;
			ui_StrX = 0;
			continue;
		}


		//文字描画
		DrawChar( P_WinBuf, CPc_Text[ui_CharCount],
				Area.Msi_Left + ( ui_StrX++ << 3 ), Area.Msi_Top + ( ui_StrY << 4 ),
				TextColor, ValidArea );
	}
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	無名名前空間関数
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
namespace
{
/*******************************************************************************
	概要	：	文字描画
	説明	：	ウィンドウバッファに文字を表示します。
				現状、デバッグ用フォントを使っています。。。
	Include	：
	引数	：	Color4 (*P_WinBuf)[1024]	描画先のウィンドウバッファ
				char c_Char					描画する文字
				ui ui_X						描画先座標
				ui ui_Y						描画先座標
				Color4 TextColor			文字色
				Rect& R_ValidArea			有効領域
	戻り値	：	void
*******************************************************************************/
void	DrawChar( Color4 (*P_WinBuf)[1024], char c_Char, ui ui_X, ui ui_Y, Color4 TextColor, Rect& R_ValidArea )
{
	ui		ui_CharX, ui_CharY;
	u1		u1_BitMask;

	//文字描画
	for( ui_CharY = ui_Y; ui_CharY < ui_Y + 16; ui_CharY++ )
		if( R_ValidArea.Msi_Top <= (si)ui_CharY && (si)ui_CharY <= R_ValidArea.Msi_Bottom )
			for( ui_CharX = ui_X, u1_BitMask = 0x80; ui_CharX < ui_X + 8; ui_CharX++, u1_BitMask >>= 1 )
				if( GCu1_DebugFont[(u1)c_Char][ui_CharY - ui_Y] & u1_BitMask		//ビットが1なら指定色を書き込み
					&& R_ValidArea.Msi_Left <= (si)ui_CharX && (si)ui_CharX <= R_ValidArea.Msi_Right )
					P_WinBuf[ui_CharY][ui_CharX]	= TextColor;
}
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

