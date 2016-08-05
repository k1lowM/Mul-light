/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Include/Mul_light/Objects/Single.h
	概要		：	Mul light API - シングルクラス
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Box.h"
#include	"Rect.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	シングルクラス
*******************************************************************************/
class	Single : public Box
{
//定数
public:
	enum
	{
		ERROR = _ERROR_,
		ERROR_NULLPOINTER,			//NULLポインタ
		ERROR_NOTOBJECT,			//オブジェクトでない

		SUCCESS = _SUCCESS_,
	};

//変数
protected:
	Object*	MP_Child;			//子オブジェクトのポインタ

//メソッド
public:
	s4		SetChild( Object* P_Child );	//子オブジェクト設定

	Object*	GetChild( void );		//子オブジェクト取得
	void	GetMinSize( ui* Pui_Width, ui* Pui_Height );		//最小サイズ取得
	Rect	GetChildArea( Object* P_Child );		//子領域取得

protected:
	void	Init( void );
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
