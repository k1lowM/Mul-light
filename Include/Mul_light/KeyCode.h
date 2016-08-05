/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Include/Mul_light/Message.h
	概要		：	Mul light API - キーコード定義
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	キーコード
	asciiコードに合わせてあります。
*******************************************************************************/
namespace	KC
{
	enum	KeyCode
	{
		BS = 0x08,		//08

		ENTER = 0x0a,		//0a
		TAB,		//0b

		SPACE = 0x20,		//20

		COMMA = 0x2c,		//2c	,
		HYPHEN,		//2d	-
		PERIOD,		//2e	.
		SLASH,		//2f	/
		KEY_0,		//30
		KEY_1,		//31
		KEY_2,		//32
		KEY_3,		//33
		KEY_4,		//34
		KEY_5,		//35
		KEY_6,		//36
		KEY_7,		//37
		KEY_8,		//38
		KEY_9,		//39
		COLON,		//3a	:
		SEMICOLON,	//3b	;

		ATMARK = 0x40,		//40	@
		A,			//41
		B,			//42
		C,			//43
		D,			//44
		E,			//45
		F,			//46
		G,			//47
		H,			//48
		I,			//49
		J,			//4a
		K,			//4b
		L,			//4c
		M,			//4d
		N,			//4e
		O,			//4f
		P,			//50
		Q,			//51
		R,			//52
		S,			//53
		T,			//54
		U,			//55
		V,			//56
		W,			//57
		X,			//58
		Y,			//59
		Z,			//5a
		LEFT_SB,	//5b	[
		BACKSLASH,	//5c
		RIGHT_SB,	//5d	]
		HAT,		//5e	^

		DELETE = 0x7f,		//7f


		//以下、asciiコード外
		TEN_ENTER = 0x8a,	//8a
		//0x80から0x9fまでは一応予約。

		TEN_ASTERISK = 0xaa,	//aa
		TEN_PLUS,	//ab
		ac,
		TEN_HYPHEN,	//ad	-
		TEN_PERIOD,	//ae	.
		TEN_SLASH,	//af	/
		TEN_0,		//b0
		TEN_1,		//b1
		TEN_2,		//b2
		TEN_3,		//b3
		TEN_4,		//b4
		TEN_5,		//b5
		TEN_6,		//b6
		TEN_7,		//b7
		TEN_8,		//b8
		TEN_9,		//b9
		ba,
		bb,
		bc,
		bd,
		be,
		bf,

		//ファンクションキー＆**Lockキー
		ESC = 0xc0,	//c0
		F1,			//c1
		F2,			//c2
		F3,			//c3
		F4,			//c4
		F5,			//c5
		F6,			//c6
		F7,			//c7
		F8,			//c8
		F9,			//c9
		F10,		//ca
		F11,		//cb
		F12,		//cc
		CAPSLOCK,	//cd
		NUMLOCK,	//ce
		SCROLLLOCK,	//cf

		//特殊キー群
		KANJI = 0xd0,		//d0
		NONCONVERT,	//d1	無変換
		CONVERT,	//d2	変換
		KANA,		//d3	カタカナ ひらがな ローマ字
		MENU,		//d4
		INSERT,		//d5
		PRINTSCREEN,	//d6
		PAUSE,		//d7
		BREAK,		//d8
		d9,
		da,
		db,
		dc,
		dd,
		de,
		df,

		UP = 0xe0,			//e0	↑
		DOWN,		//e1	↓
		LEFT,		//e2	←
		RIGHT,		//e3	→
		HOME,		//e4
		END,		//e5
		PAGEUP,		//e6
		PAGEDOWN,	//e7
		e8,
		e9,
		ea,
		eb,
		ec,
		ed,
		ee,
		ef,

		//左右にあるキー
		LCTRL = 0xf0,		//f0
		LSHIFT,		//f1
		LALT,		//f2
		LWIN,		//f3

		RCTRL = 0xf8,		//f8
		RSHIFT,		//f9
		RALT,		//fa
		RWIN,		//fb
	};
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
