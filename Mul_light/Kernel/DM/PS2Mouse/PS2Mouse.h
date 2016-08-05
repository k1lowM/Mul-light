/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/DM/PS2Mouse/PS2Mouse.h
	概要		：	PS2マウスクラスヘッダ
	詳細		：	PS2マウスを管理するクラスの宣言です。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/


#pragma once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	PS2マウスクラス
*******************************************************************************/
class	PS2Mouse : public Driver
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
		ERROR_INVALILDDATA,			//無効なデータ

		SUCCESS = _SUCCESS_,
	};

//変数
private:
	Queue1	M_MouseDataQ1;

//メソッド
public:
	static void	Task( void );

	void	Init( void );
	s4		Ctrl( DCD* P_DCD );

	s4		Read( u1* Pu1_XDistanceBuf, u1* Pu1_YDistanceBuf, u1* Pu1_ButtonBuf );
	s4		Enqueue( u1 u1_Data );

private:
	void	WaitKBCBusy( void );
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
