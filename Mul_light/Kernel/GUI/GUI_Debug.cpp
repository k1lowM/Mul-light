/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/GUI/GUI.cpp
	概要		：	描画関連全般
	詳細		：	描画関連のすべてを司ります。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"GUI.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
extern const u1		GCu1_DebugFont[128][16];



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	GUIクラス：パブリックメソッド(デバッグ用)
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	デバッグ用矩形表示
	説明	：	省略
	Include	：	GUI.h
	引数	：	省略
	戻り値	：	void
*******************************************************************************/
void	GUI::DebugPrintRect( s4 s4_X, s4 s4_Y, u4 u4_Width, u4 u4_Height, u4 u4_Color )
{
	s4		x, y;
	u1*		Pu1_Cache;

	for( y = s4_Y; y < (s4)( s4_Y + u4_Height ); y++ )
	{
		if( (u4)y > Mui_ResY )
			continue;

		for( x = s4_X; x < (s4)( s4_X + u4_Width ); x++ )
		{
			if( (u4)x > Mui_ResX )
				continue;

			Pu1_Cache = (u1*)MPv_VRAM;
			Pu1_Cache += ( y * Mui_ResX + x ) * Mui_BytePerDot;

			Pu1_Cache[0] = (u1)( u4_Color );		//青
			Pu1_Cache[1] = (u1)( u4_Color >> 8 );	//緑
			Pu1_Cache[2] = (u1)( u4_Color >> 16 );	//赤
		}
	}
}


/*******************************************************************************
	概要	：	デバッグ用文字表示
	説明	：	省略
	Include	：	GUI.h
	引数	：	省略
	戻り値	：	void
*******************************************************************************/
void	GUI::DebugPrintChar( char c_Char, u4 u4_X, u4 u4_Y, u4 u4_Color )
{
	u4		x, y;
	u1*		Pu1_Cache;
	u1		u1_BitMask;


	u4_Y <<= 4;
	u4_X <<= 3;

	if( u4_Y > Mui_ResY )
		return;
	if( u4_X > Mui_ResX )
		return;

	for( y = u4_Y; y < u4_Y + 16; y++ )
	{
		for( x = u4_X, u1_BitMask = 0x80; u1_BitMask; x++, u1_BitMask >>= 1 )
		{
			Pu1_Cache = (u1*)MPv_VRAM;
			Pu1_Cache += ( y * Mui_ResX + x ) * Mui_BytePerDot;

			//ビットが1なら指定色を書き込み
			if( GCu1_DebugFont[(u1)c_Char][y - u4_Y] & u1_BitMask )
			{
				Pu1_Cache[0] = (u1)( u4_Color );		//青
				Pu1_Cache[1] = (u1)( u4_Color >> 8 );	//緑
				Pu1_Cache[2] = (u1)( u4_Color >> 16 );	//赤
			}
			else
			{
				Pu1_Cache[0] = 0;
				Pu1_Cache[1] = 0;
				Pu1_Cache[2] = 0;
			}
		}
	}
}


/*******************************************************************************
	概要	：	デバッグ用文字列表示
	説明	：	省略
	Include	：	GUI.h
	引数	：	省略
	戻り値	：	void
*******************************************************************************/
void	GUI::DebugPrintString( const char* CPc_String, u4 u4_X, u4 u4_Y, u4 u4_Color )
{
	ui		ui_StartX = u4_X;
	ui		ui_CharCount;

	for( ui_CharCount = 0; CPc_String[ui_CharCount]; ui_CharCount++ )
	{
		switch( CPc_String[ui_CharCount] )
		{
		case '\t':
			u4_X += 4;
			u4_X &= ~3;
			continue;
		case '\n':
			u4_Y++;
			u4_X = ui_StartX;
			continue;
		default:
			u4_X++;
			DebugPrintChar( CPc_String[ui_CharCount], u4_X, u4_Y, u4_Color );
		}
	}
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

