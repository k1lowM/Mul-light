/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Include/Mul_light/Objects/Box.cpp
	概要		：	Mul light API - ボックスクラス
	詳細		：	子オブジェクトの領域を持つオブジェクトです。
					抽象的なクラスで、これ自体にあまり意味はありません。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"MLCommon.h"
#include	"Box.h"
#include	"Single.h"
#include	"Pack.h"
#include	"Content.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ボックスクラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	背景色設定
	説明	：
	Include	：	Box.h
	引数	：	Color4 Color

				u4 u4_Color

				u1 u1_Red
				u1 u1_Green
				u1 u1_Blue
				[u1 u1_Alpha = 0xff]
	戻り値	：	void
*******************************************************************************/
void	Box::SetBGColor( Color4 Color )
{
	M_BGColor			= Color;
}
void	Box::SetBGColor( u4 u4_Color )
{
	M_BGColor.u4_Color	= u4_Color;
}
void	Box::SetBGColor( u1 u1_Red, u1 u1_Green, u1 u1_Blue, u1 u1_Alpha )
{
	M_BGColor.u1_Alpha	= u1_Alpha;
	M_BGColor.u1_Red	= u1_Red;
	M_BGColor.u1_Green	= u1_Green;
	M_BGColor.u1_Blue	= u1_Blue;
}


/*******************************************************************************
	概要	：	パディング値一括取得
	説明	：
	Include	：	Box.h
	引数	：	void
	戻り値	：	Box::Padding
*******************************************************************************/
Box::Padding	Box::GetPadding( void )
{
	return M_Padding;
}


/*******************************************************************************
	概要	：	パディング値取得
	説明	：
	Include	：	Box.h
	引数	：	PadType PaddingType		取得したいパディングの種類
	戻り値	：	u1		指定した箇所のパディング
*******************************************************************************/
u1		Box::GetPadding( PadType PaddingType )
{
	switch( PaddingType )
	{
	case TOP:
		return M_Padding.u1_Top;
	case LEFT:
		return M_Padding.u1_Left;
	case RIGHT:
		return M_Padding.u1_Right;
	case BOTTOM:
		return M_Padding.u1_Bottom;
	}
	return 0xff;
}


/*******************************************************************************
	概要	：	背景色取得
	説明	：
	Include	：	Box.h
	引数	：	void
	戻り値	：	Color4	子領域の背景色
*******************************************************************************/
Color4	Box::GetBGColor( void )
{
	return M_BGColor;
}


/*******************************************************************************
	概要	：	子領域取得
	説明	：	引数で指定した子オブジェクトの領域を返します。
	Include	：	Box.h
	引数	：	Object* P_Child		領域を取得したい子オブジェクトのポインタ
	戻り値	：	Rect	子領域
*******************************************************************************/
Rect	Box::GetChildArea( Object* P_Child )
{
	if( CheckInheritance( SINGLE ) )
		return ( (Single*)this )->Single::GetChildArea( P_Child );

	if( CheckInheritance( PACK ) )
		return ( (Pack*)this )->Pack::GetChildArea( P_Child );

	//このオブジェクト自身がボックスでない。
	Rect	ChArea;
	ChArea.Init();
	return ChArea;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ボックスクラス：プロテクティッドメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	ボックスクラスの初期化
	説明	：
	Include	：	Box.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	Box::Init( void )
{
	Object::Init();
	Object::SetObjectType( Object::BOX );

	M_Padding.u1_Top	= 0;		//上
	M_Padding.u1_Left	= 0;		//左
	M_Padding.u1_Right	= 0;		//右
	M_Padding.u1_Bottom	= 0;		//下

	M_BGColor.u4_Color	= 0xff000000;		//子領域の背景色
}


/*******************************************************************************
	概要	：	パディング設定
	説明	：
	Include	：	Box.h
	引数	：	[u1 u1_Top = 0]
				[u1 u1_Left = 0]
				[u1 u1_Right = 0]
				[u1 u1_Bottom = 0]
	戻り値	：	void
*******************************************************************************/
void	Box::SetPadding( u1 u1_Top, u1 u1_Left, u1 u1_Right, u1 u1_Bottom )
{
	M_Padding.u1_Top	= u1_Top;		//上
	M_Padding.u1_Left	= u1_Left;		//左
	M_Padding.u1_Right	= u1_Right;		//右
	M_Padding.u1_Bottom	= u1_Bottom;	//下
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
