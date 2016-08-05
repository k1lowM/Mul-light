/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Include/Mul_light/Objects/Text.cpp
	概要		：	Mul light API - テキストクラス
	詳細		：	文字列を扱うクラスです。
					ボタンのラベルにしたり、様々な場面で使用します。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"MLCommon.h"
#include	"Text.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	テキストクラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	テキストクラスの初期化
	説明	：
	Include	：	Text.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	Text::Init( void )
{
	Content::Init();
	Object::SetObjectType( Object::TEXT );
	Object::SetOffset();		//C:0, C:0
	Object::SetSize( 0, 0, WIDTH_MIN, HEIGHT_MIN );

	MCPc_Text				= NULL;
	M_TextColor.u4_Color	= 0xff000000;
}


/*******************************************************************************
	概要	：	文字列設定
	説明	：
	Include	：	Text.h
	引数	：	const char* CPc_Text		表示する文字列のアドレス
	戻り値	：	void
*******************************************************************************/
void	Text::SetText( const char* CPc_Text )
{
	//エラー処理
	if( CPc_Text == NULL )
		return;

	//文字列のアドレス設定
	MCPc_Text = CPc_Text;
}


/*******************************************************************************
	概要	：	文字色設定
	説明	：
	Include	：	Text.h
	引数	：	Color4 Color

				u4 u4_Color

				u1 u1_Red
				u1 u1_Green
				u1 u1_Blue
				[u1 u1_Alpha = 0xff]
	戻り値	：	void
*******************************************************************************/
void	Text::SetTextColor( Color4 Color )
{
	M_TextColor				= Color;
}
void	Text::SetTextColor( u4 u4_Color )
{
	M_TextColor.u4_Color	= u4_Color;
}
void	Text::SetTextColor( u1 u1_Red, u1 u1_Green, u1 u1_Blue, u1 u1_Alpha )
{
	M_TextColor.u1_Blue		= u1_Blue;
	M_TextColor.u1_Green	= u1_Green;
	M_TextColor.u1_Red		= u1_Red;
	M_TextColor.u1_Alpha	= u1_Alpha;
}


/*******************************************************************************
	概要	：	文字列取得
	説明	：
	Include	：	Text.h
	引数	：	void
	戻り値	：	const char*		文字列のアドレス
*******************************************************************************/
const char*	Text::GetText( void )
{
	return MCPc_Text;
}


/*******************************************************************************
	概要	：	文字色取得
	説明	：
	Include	：	Text.h
	引数	：	void
	戻り値	：	Color4		文字色
*******************************************************************************/
Color4	Text::GetTextColor( void )
{
	return M_TextColor;
}


/*******************************************************************************
	概要	：	一行あたりの最長文字数を取得
	説明	：	現状では、asciiコードしか考慮されていません。
				そのため、単にバイト数で返ります。(utf-8に対応したい。)
	Include	：	Text.h
	引数	：	void
	戻り値	：	ui		一行あたりの最長文字数
*******************************************************************************/
ui		Text::GetMaxLength( void )
{
	ui		ui_StrCount		= 0;	//文字列のカウンタ
	ui		ui_StrCountLine	= 0;	//一行の文字カウンタ
	ui		ui_MaxLength	= 0;	//一行あたりの最長文字数

	//NULLエラー
	if( MCPc_Text == NULL )
		return 0;

	//文字列の終端までループ。
	for( ui_StrCount = 0, ui_StrCountLine = 0; MCPc_Text[ui_StrCount] != '\0'; ui_StrCount++, ui_StrCountLine++ )
	{
		//改行コードがあれば、最長文字数を更新、行の文字カウンタをリセット
		if( MCPc_Text[ui_StrCount] == '\n' )
		{
			//現在の最長文字数より大きければ最長文字数を更新
			if( ui_MaxLength < ui_StrCountLine )
				ui_MaxLength = ui_StrCountLine;
			ui_StrCountLine = 0;		//一行の文字カウンタをリセット
		}
	}

	//最終行のための処理(ソースのスマートさより、処理速度を優先。)
	if( ui_MaxLength < ui_StrCountLine )
		ui_MaxLength = ui_StrCountLine;

	return ui_MaxLength;
}


/*******************************************************************************
	概要	：	テキストの行数を取得
	説明	：	現状では、asciiコードしか考慮されていません。
				そのため、単にバイト数で返ります。(utf-8に対応したい。)
	Include	：	Text.h
	引数	：	void
	戻り値	：	ui		テキストの行数
*******************************************************************************/
ui		Text::GetNumLine( void )
{
	ui		ui_StrCount;		//文字列のカウンタ
	ui		ui_LineCount;		//行数のカウンタ

	//NULLエラー
	if( MCPc_Text == NULL )
		return 0;

	//改行コードをカウント
	for( ui_StrCount = 0, ui_LineCount = 1; MCPc_Text[ui_StrCount] != '\0'; ui_StrCount++ )
		if( MCPc_Text[ui_StrCount] == '\n' )		//改行コードがあれば、
			ui_LineCount++;						//	行数をインクリメント

	return ui_LineCount;
}


/*******************************************************************************
	概要	：	テキストの幅の取得
	説明	：	テキスト全体の幅をピクセル単位で返します。
	Include	：	Text.h
	引数	：	void
	戻り値	：	ui		テキストの幅(ピクセル単位)
*******************************************************************************/
ui		Text::GetWidth( void )
{
	return GetMaxLength() << 3;		//一文字：幅8ピクセル
}


/*******************************************************************************
	概要	：	テキストの高さの取得
	説明	：	テキスト全体の高さをピクセル単位で返します。
	Include	：	Text.h
	引数	：	void
	戻り値	：	ui		テキストの高さ(ピクセル単位)
*******************************************************************************/
ui		Text::GetHeight( void )
{
	return GetNumLine() << 4;		//一文字：高さ16ピクセル
}


/*******************************************************************************
	概要	：	オブジェクト描画
	説明	：
	Include	：	Object.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	Text::Drawing( void )
{
	UpdateArea();		//オブジェクト領域更新

	//システムコール
	SCD::DrawObject	SCD_DrawObject;
	SCD_DrawObject.MP_Object		= this;
	SCD_DrawObject.M_ChArea			= GetValidArea();
	SCD_DrawObject.MAu4_Option[0]	= M_TextColor.u4_Color;
	SCD_DrawObject.MAu4_Option[1]	= 0x12345678;
	SC::SystemCall( SC::DRAW_OBJECT, &SCD_DrawObject );
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

