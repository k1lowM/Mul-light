/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Include/Mul_light/Objects/Button.cpp
	概要		：	Mul light API - ボタンクラス
	詳細		：	ボタン機能を持つシングルボックスです。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"MLCommon.h"
#include	"Button.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数・マクロ定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
namespace
{
	enum
	{
		//ボタンのパディング情報
		BTNPAD_TOP		= 5,
		BTNPAD_LEFT		= 5,
		BTNPAD_RIGHT	= BTNPAD_LEFT,
		BTNPAD_BOTTOM	= BTNPAD_TOP,
	};
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ボタンクラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	ボタンクラスの初期化
	説明	：
	Include	：	Button.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	Button::Init( void )
{
	Single::Init();
	Object::SetObjectType( Object::BUTTON );
	Object::SetOffset( 0, 0, H_CENTER, V_CENTER );
	Object::SetSize( 0, 0, WIDTH_MAX, HEIGHT_MAX );

	Box::SetBGColor( 0 );
	Box::SetPadding( BTNPAD_TOP, BTNPAD_LEFT, BTNPAD_RIGHT, BTNPAD_BOTTOM );
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

