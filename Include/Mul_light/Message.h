/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Include/Mul_light/Message.h
	概要		：	Mul light API - メッセージクラス
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	メッセージ
	Message仕様
	・-1以下ならシステムメッセージ
	・+1以上なら通常のメッセージ
	・31:システム/通常, 30-12:メッセージ大別, 11-0:メッセージの詳細な種類
*******************************************************************************/
class	Msg
{
//定数
public:
	//メッセージ
	enum	Message
	{
		//システムメッセージ
		SYS_MASK	= 0x80000000,
		SYS_QUIT,				//終了

		//通常メッセージ
		GEN_MASK	= 0x00001000,		/*■全般■*/
		INITIALIZE,				//初期処理
		FINALIZE,				//終了処理

		KEY_MASK	= 0x00002000,		/*■キーボード■*/
		KEY_PRESS,				//キープレス
		KEY_RELEASE,			//キーリリース

		MOUSE_MASK	= 0x00003000,		/*■マウス■*/
		MOUSE_MOVE,				//マウス移動
		MOUSE_L_PRESS,			//左プレス
		MOUSE_L_RELEASE,		//左リリース
//		MOUSE_L_CLICK,			//左クリック
		MOUSE_R_PRESS,			//右プレス
		MOUSE_R_RELEASE,		//右リリース
//		MOUSE_R_CLICK,			//右クリック
		MOUSE_C_PRESS,			//中プレス
		MOUSE_C_RELEASE,		//中リリース
//		MOUSE_C_CLICK,			//中クリック

		WINOBJ_MASK	= 0x00004000,		/*■ウィンドウオブジェクト■*/
		OBJECT_MOUSE_OVER,		//オブジェクト	マウスオーバー
		OBJECT_MOUSE_OUT,		//				マウスアウト
		OBJECT_FOCUS_HIT,		//				フォーカスヒット
		OBJECT_FOCUS_OUT,		//				フォーカスアウト
		OBJECT_REDRAW,			//				再描画
		WINDOW_FOCUS_HIT,		//ウィンドウ	フォーカスヒット
		WINDOW_FOCUS_OUT,		//				フォーカスアウト
		WINDOW_BTN_OVER,		//			ボタン	マウスオーバー
		WINDOW_BTN_OUT,			//					マウスアウト
		WINDOW_BTN_L_PRESS,		//					左プレス
		WINDOW_BTN_L_RELEASE,		//				左リリース
//		WINDOW_BTN_CLICK,		//					クリック
		WINDOW_BTN_REDRAW,		//					再描画
	};

	//メッセージマスク
	enum	MessageMask
	{
		MSGMASK_SYSTEM		= 0x80000000,		//システム/通常
		MSGMASK_MAJOR		= 0x7ffff000,		//大分類
		MSGMASK_MINOR		= 0x00000fff,		//小分類
	};

//変数
public:
	Message	M_Message;
	union
	{
		u4		Mu4_Data[3];
		struct
		{
			u2		Low;
			u2		High;
		}Mu2_Data[3];
		struct
		{
			u1		LL;
			u1		LH;
			u1		HL;
			u1		HH;
		}Mu1_Data[3];
	};

//メソッド
public:
	void	Init( Message Message, u4 u4_Data0 = 0, u4 u4_Data1 = 0, u4 u4_Data2 = 0 );
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
