/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/DM/PS2Keyboard/PS2Keyboard.h
	概要		：	PS2キーボードクラスヘッダ
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	PS2キーボードクラス
*******************************************************************************/
class	PS2Keyboard : public Driver
{
//定数
public:
	//エラー情報
	enum	Error
	{
		ERROR = _ERROR_,
		ERROR_NULLPOINTER,			//ヌルポ。
		ERROR_INVALIDDCID,			//無効な制御ID
		ERROR_NODATA,				//キューにデータなし
		ERROR_DATAINSUFFICIENCY,	//データが不十分

		SUCCESS = _SUCCESS_,
	};

//変数
private:
	Queue1	M_KeyDataQ1;

//メソッド
public:
	static void	Task( void );

	void	Init( void );
	s4		Ctrl( DCD* P_DCD );

	s4		Read( ui* Pui_KeyDataBuf );
	s4		Enqueue( u1 u1_Data );
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
