/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	Text.h
	概要		：	Mul light API - テキストクラス
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Content.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	テキストクラス
*******************************************************************************/
class	Text : public Content
{
//変数
protected:
	const char*	MCPc_Text;		//表示する文字列の先頭アドレス
	Color4		M_TextColor;	//文字色

//メソッド
public:
	void	Init( void );

	void	SetText( const char* CPc_Text );
	void	SetTextColor( Color4 Color );
	void	SetTextColor( u4 u4_Color );
	void	SetTextColor( u1 u1_Red, u1 u1_Green, u1 u1_Blue, u1 u1_Alpha = 0xff );

	const char*	GetText( void );		//テキスト取得
	Color4	GetTextColor( void );		//文字色取得
	ui		GetMaxLength( void );		//一行あたりの最長文字数を取得
	ui		GetNumLine( void );			//テキストの行数を取得
	ui		GetWidth( void );			//テキストの幅の取得
	ui		GetHeight( void );			//テキストの高さの取得

	void	Drawing( void );			//テキスト描画
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
