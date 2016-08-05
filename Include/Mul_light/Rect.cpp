/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Include/Mul_light/Rect.cpp
	概要		：	Mul light API - 矩形領域クラス
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"MLCommon.h"
#include	"Rect.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	矩形領域クラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	矩形領域の初期化
	説明	：	矩形領域を引数で初期化します。
	Include	：	Rect.h
	引数	：	[si si_Top = 0]
				[si si_Left = 0]
				[si si_Right = 0]
				[si si_Bottom = 0]
	戻り値	：	void
*******************************************************************************/
void	Rect::Init( si si_Top, si si_Left, si si_Right, si si_Bottom )
{
	Msi_Top		= si_Top;
	Msi_Left	= si_Left;
	Msi_Right	= si_Right;
	Msi_Bottom	= si_Bottom;
}


/*******************************************************************************
	概要	：	矩形領域の設定
	説明	：	矩形領域を一括で設定します。初期化と同じです。
	Include	：	Rect.h
	引数	：	[si si_Top = 0]
				[si si_Left = 0]
				[si si_Right = 0]
				[si si_Bottom = 0]
	戻り値	：	void
*******************************************************************************/
void	Rect::SetRect( si si_Top, si si_Left, si si_Right, si si_Bottom )
{
	Msi_Top		= si_Top;
	Msi_Left	= si_Left;
	Msi_Right	= si_Right;
	Msi_Bottom	= si_Bottom;
}


/*******************************************************************************
	概要	：	矩形領域の拡大
	説明	：	矩形領域を拡張します。
				Top,Leftは減算し、Right,Bottomは加算して拡張します。
	Include	：	Rect.h
	引数	：	[si si_Top = 0]
				[si si_Left = 0]
				[si si_Right = 0]
				[si si_Bottom = 0]
	戻り値	：	void
*******************************************************************************/
void	Rect::Expand( si si_Top, si si_Left, si si_Right, si si_Bottom )
{
	Msi_Top		-= si_Top;
	Msi_Left	-= si_Left;
	Msi_Right	+= si_Right;
	Msi_Bottom	+= si_Bottom;
}


/*******************************************************************************
	概要	：	矩形領域の縮小
	説明	：	矩形領域を縮小します。
				Top,Leftは加算し、Right,Bottomは減算して縮小します。
	Include	：	Rect.h
	引数	：	[si si_Top = 0]
				[si si_Left = 0]
				[si si_Right = 0]
				[si si_Bottom = 0]
	戻り値	：	void
*******************************************************************************/
void	Rect::Contract( si si_Top, si si_Left, si si_Right, si si_Bottom )
{
	Msi_Top		+= si_Top;
	Msi_Left	+= si_Left;
	Msi_Right	-= si_Right;
	Msi_Bottom	-= si_Bottom;
}


/*******************************************************************************
	概要	：	矩形領域の移動
	説明	：	矩形領域を移動します。
				Top,Leftは加算し、Right,Bottomは減算して縮小します。
	Include	：	Rect.h
	引数	：	[si si_DistanceX = 0]
				[si si_DistanceY = 0]
	戻り値	：	void
*******************************************************************************/
void	Rect::Move( si si_DistanceX, si si_DistanceY )
{
	Msi_Top		+= si_DistanceY;
	Msi_Left	+= si_DistanceX;
	Msi_Right	+= si_DistanceX;
	Msi_Bottom	+= si_DistanceY;
}


/*******************************************************************************
	概要	：	領域補正
	説明	：	上下左右を正しい状態に補正します。
				( Top <= Bottom && Left <= Right )の状態に補正します。
	Include	：	Rect.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	Rect::Matching( void )
{
	if( Msi_Top > Msi_Bottom )
		ExchangeTB();
	if( Msi_Left > Msi_Right )
		ExchangeLR();
}


/*******************************************************************************
	概要	：	上下入れ替え、左右入れ替え
	説明	：
	Include	：	Rect.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	Rect::ExchangeTB( void )
{
	si si_ExchgWork	= Msi_Top;
	Msi_Top			= Msi_Bottom;
	Msi_Bottom		= si_ExchgWork;
}
void	Rect::ExchangeLR( void )
{
	si si_ExchgWork	= Msi_Left;
	Msi_Left		= Msi_Right;
	Msi_Right		= si_ExchgWork;
}


/*******************************************************************************
	概要	：	2領域のandを設定
	説明	：	メンバと引数の二つの領域の重なる部分を矩形領域として設定します。
				まったく重なっていない場合は、変更されません。
	Include	：	Rect.h
	引数	：	Rect& R_Rect
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		Rect::And( Rect& R_Rect )
{
	//両領域とも補正
	Matching();
	R_Rect.Matching();

	//重複確認
	if( !CheckOverlap( R_Rect ) )
		return ERROR_NO_OVERLAP;		//重なっていない

	if( Msi_Top < R_Rect.Msi_Top )
		Msi_Top = R_Rect.Msi_Top;
	if( Msi_Left < R_Rect.Msi_Left )
		Msi_Left = R_Rect.Msi_Left;
	if( Msi_Right > R_Rect.Msi_Right )
		Msi_Right = R_Rect.Msi_Right;
	if( Msi_Bottom > R_Rect.Msi_Bottom )
		Msi_Bottom = R_Rect.Msi_Bottom;
	return SUCCESS;
}

/*******************************************************************************
	概要	：	2領域のandを設定
	説明	：
	Include	：	Rect.h
	引数	：	si si_Top, si si_Left, si si_Right, si si_Bottom
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		Rect::And( si si_Top, si si_Left, si si_Right, si si_Bottom )
{
	Rect	Rect;
	Rect.Init( si_Top, si_Left, si_Right, si_Bottom );
	return And( Rect );
}


/*******************************************************************************
	概要	：	2領域のorを設定
	説明	：	メンバと引数の二つの領域を包含する部分を矩形領域として設定します。
	Include	：	Rect.h
	引数	：	Rect& R_Rect
	戻り値	：	void
*******************************************************************************/
void	Rect::Or( Rect& R_Rect )
{
}



/*******************************************************************************
	概要	：	幅取得
	説明	：	矩形の幅を返します。
	Include	：	Rect.h
	引数	：	void
	戻り値	：	si		矩形の幅( Right - Left )
*******************************************************************************/
si		Rect::GetWidth( void )
{
	return Msi_Right - Msi_Left;
}


/*******************************************************************************
	概要	：	高さ取得
	説明	：	矩形の高さを返します。
	Include	：	Rect.h
	引数	：	void
	戻り値	：	si		矩形の高さ( Bottom - Top )
*******************************************************************************/
si		Rect::GetHeight( void )
{
	return Msi_Bottom - Msi_Top;
}


/*******************************************************************************
	概要	：	面積取得
	説明	：	矩形の高さを返します。
	Include	：	Rect.h
	引数	：	void
	戻り値	：	si		矩形の面積( Width * Height )
*******************************************************************************/
si		Rect::GetAreaSize( void )
{
	return ( Msi_Right - Msi_Left ) * ( Msi_Bottom - Msi_Top );
}



/*******************************************************************************
	概要	：	領域内チェック
	説明	：	指定の座標が領域内かどうか調べます。
	Include	：	Rect.h
	引数	：	void
	戻り値	：	bool	true:領域内、false:領域外
*******************************************************************************/
bool	Rect::CheckInArea( si si_X, si si_Y )
{
	if( Msi_Top <= si_Y && si_Y <= Msi_Bottom && Msi_Left <= si_X && si_X <= Msi_Right )
		return true;
	else
		return false;
}


/*******************************************************************************
	概要	：	重複チェック
	説明	：	メンバと引数の二つの領域が重なっているか調べます。
	Include	：	Rect.h
	引数	：	Rect& R_Rect
	戻り値	：	bool		true:重なっている　false:重なっていない
*******************************************************************************/
bool	Rect::CheckOverlap( Rect& R_Rect )
{
	if( ( Msi_Bottom < R_Rect.Msi_Top && Msi_Top < R_Rect.Msi_Bottom )
		 || ( Msi_Bottom > R_Rect.Msi_Top && Msi_Top > R_Rect.Msi_Bottom )
		 || ( Msi_Right < R_Rect.Msi_Left && Msi_Left < R_Rect.Msi_Right )
		 || ( Msi_Right > R_Rect.Msi_Left && Msi_Left > R_Rect.Msi_Right ) )
		return false;
	else
		return true;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

