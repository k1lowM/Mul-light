/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Include/Mul_light/Objects/Box.h
	概要		：	Mul light API - ボックスクラス
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Object.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	ボックスクラス
*******************************************************************************/
class	Box : public Object
{
//定数
public:
	//パディングタイプ
	enum	PadType
	{
		TOP,
		LEFT,
		RIGHT,
		BOTTOM,
	};

//クラス・構造体
public:
	//子領域までのパディング
	struct	Padding
	{
		u1		u1_Top;			//上
		u1		u1_Left;		//左
		u1		u1_Right;		//右
		u1		u1_Bottom;		//下
	};

//変数
protected:
	Padding	M_Padding;
	Color4	M_BGColor;		//子領域の背景色

//メソッド
public:
	void	SetBGColor( Color4 Color );
	void	SetBGColor( u4 u4_Color );
	void	SetBGColor( u1 u1_Red, u1 u1_Green, u1 u1_Blue, u1 u1_Alpha = 0xff );

	Padding	GetPadding( void );
	u1		GetPadding( PadType PaddingType );
	Color4	GetBGColor( void );
	Rect	GetChildArea( Object* P_Child );		//子領域取得

protected:
	void	Init( void );

	void	SetPadding( u1 u1_Top = 0, u1 u1_Left = 0, u1 u1_Right = 0, u1 u1_Bottom = 0 );
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
