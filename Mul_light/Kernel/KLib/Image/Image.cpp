/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/KLib/Image/Image.cpp
	概要		：
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"Image.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	Imageクラス：静的パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	24ビット(3バイト)カラーから32ビット(4バイトカラー)に変換
	説明	：
	Include	：	Image.h
	引数	：	Color4* P_Dest4				32ビットカラー構造体ポインタ
				const Color3* CP_Source3	24ビットカラー構造体ポインタ
				ui ui_Num					要素数(!=バイト数)
	戻り値	：	void
*******************************************************************************/
void	Image::Color3to4( Color4* P_Dest4, const Color3* CP_Source3, ui ui_Num )
{
	ui		ui_Count = ui_Num;
	while( --ui_Count < ui_Num )
	{
		P_Dest4[ui_Count].u1_Alpha	= 0xff;
		P_Dest4[ui_Count].u1_Red	= CP_Source3[ui_Count].u1_Red;
		P_Dest4[ui_Count].u1_Green	= CP_Source3[ui_Count].u1_Green;
		P_Dest4[ui_Count].u1_Blue	= CP_Source3[ui_Count].u1_Blue;
	}
}


/*******************************************************************************
	概要	：	32ビット(4バイトカラー)から24ビット(3バイト)カラーに変換
	説明	：
	Include	：	Image.h
	引数	：	Color3* P_Dest3				24ビットカラー構造体ポインタ
				const Color4* CP_Source4	32ビットカラー構造体ポインタ
				ui ui_Num					ピクセル数、要素数(!=バイト数)
	戻り値	：	void
*******************************************************************************/
void	Image::Color4to3( Color3* P_Dest3, const Color4* CP_Source4, ui ui_Num )
{
	ui		ui_Count = 0;
	while( ui_Count++ < ui_Num )
	{
		P_Dest3[ui_Count].u1_Red	= CP_Source4[ui_Count].u1_Red;
		P_Dest3[ui_Count].u1_Green	= CP_Source4[ui_Count].u1_Green;
		P_Dest3[ui_Count].u1_Blue	= CP_Source4[ui_Count].u1_Blue;
	}
}


/*******************************************************************************
	概要	：	画像上下反転
	説明	：
	Include	：	Image.h
	引数	：	Color4* P_Color4		32ビットカラー構造体ポインタ
				ui ui_Width				幅
				ui ui_Height			高さ
	戻り値	：	void
*******************************************************************************/
void	Image::VFlip( Color4* P_Color4, ui ui_Width, ui ui_Height )
{
	ui		ui_CountX, ui_CountY;
	for( ui_CountY = 0; ui_CountY < ui_Height >> 1; ui_CountY++ )
	{
		for( ui_CountX = 0; ui_CountX < ui_Width; ui_CountX++ )
		{
			Color4	Work;
			Work = P_Color4[ui_CountY * ui_Width + ui_CountX];
			P_Color4[ui_CountY * ui_Width + ui_CountX]
				= P_Color4[( ui_Height - ui_CountY - 1 ) * ui_Width + ui_CountX];
			P_Color4[( ui_Height - ui_CountY - 1 ) * ui_Width + ui_CountX] = Work;
		}
	}
}


/*******************************************************************************
	概要	：	画像左右反転
	説明	：
	Include	：	Image.h
	引数	：	Color4* P_Color4		32ビットカラー構造体ポインタ
				ui ui_Width				幅
				ui ui_Height			高さ
	戻り値	：	void
*******************************************************************************/
void	Image::HFlip( Color4* P_Color4, ui ui_Width, ui ui_Height )
{
	ui		ui_CountX, ui_CountY;
	for( ui_CountY = 0; ui_CountY < ui_Height; ui_CountY++ )
	{
		for( ui_CountX = 0; ui_CountX < ui_Width >> 1; ui_CountX++ )
		{
			Color4	Work;
			Work = P_Color4[ui_CountY * ui_Width + ui_CountX];
			P_Color4[ui_CountY * ui_Width + ui_CountX]
				= P_Color4[ui_CountY * ui_Width + ui_Width - ui_CountX - 1];
			P_Color4[ui_CountY * ui_Width + ui_Width - ui_CountX - 1] = Work;
		}
	}
}


/*******************************************************************************
	概要	：	透明色処理
	説明	：
	Include	：	Image.h
	引数	：	Color4* P_Color4		32ビットカラー構造体ポインタ
				ui ui_Num				ピクセル数、要素数(!=バイト数)
				[u4 u4_Transparent = 0xa785c6]	透明色(RGB値を3バイトで指定)
	戻り値	：	void
*******************************************************************************/
void	Image::TransparentColorProcessing( Color4* P_Color4, ui ui_Num, u4 u4_Transparent )
{
	Color4	Transparent = { u4_Transparent };
	ui		ui_Count;
	for( ui_Count = 0; ui_Count < ui_Num; ui_Count++ )
	{
		Transparent.u1_Alpha = P_Color4[ui_Count].u1_Alpha;
		if( P_Color4[ui_Count].u4_Color == Transparent.u4_Color )
			P_Color4[ui_Count].u1_Alpha = 0;
	}
}




/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

