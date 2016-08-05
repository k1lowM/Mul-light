/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/KLib/Image/Image.h
	概要		：
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	Imageクラス
*******************************************************************************/
class	Image
{
//メソッド
public:
//	static void ConvColor( void* Pv_Dest, void* Pv_Source, ui ui_BitCount, ui ui_Num );
	static void	Color3to4( Color4* P_Dest4, const Color3* CP_Source3, ui ui_Num );		//24ビット(3バイト)カラーから32ビット(4バイト)カラーに変換
	static void	Color4to3( Color3* P_Dest3, const Color4* CP_Source4, ui ui_Num );		//32ビット(4バイト)カラーから24ビット(3バイト)カラーに変換

	static void	VFlip( Color4* P_Color4, ui ui_Width, ui ui_Height );	//上下反転
	static void	HFlip( Color4* P_Color4, ui ui_Width, ui ui_Height );	//左右反転

	static void	TransparentColorProcessing( Color4* P_Color4, ui ui_Num, u4 u4_Transparent = 0xa785c6 );		//透明色処理
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

