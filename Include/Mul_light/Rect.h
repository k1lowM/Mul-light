/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Include/Mul_light/Rect.h
	概要		：	Mul light API - 矩形領域クラスヘッダ
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	矩形領域クラス
	Rect : Rectangle(長方形、矩形)
	左上を基点とした領域です。
*******************************************************************************/
class	Rect
{
//定数
public:
	//エラー情報
	enum	Error
	{
		ERROR = _ERROR_,
		ERROR_NO_OVERLAP,		//重複部分無し

		SUCCESS = _SUCCESS_,
	};

//変数
public:
	si		Msi_Top;
	si		Msi_Left;
	si		Msi_Right;
	si		Msi_Bottom;

//メソッド
public:
	//セッター系
	void	Init( si si_Top = 0, si si_Left = 0, si si_Right = 0, si si_Bottom = 0 );		//初期化
	void	SetRect( si si_Top = 0, si si_Left = 0, si si_Right = 0, si si_Bottom = 0 );	//領域セット
	void	Expand( si si_Top = 0, si si_Left = 0, si si_Right = 0, si si_Bottom = 0 );		//領域拡大
	void	Contract( si si_Top = 0, si si_Left = 0, si si_Right = 0, si si_Bottom = 0 );	//領域縮小
	void	Move( si si_DistanceX = 0, si si_DistanceY = 0 );								//領域移動

	void	ExchangeTB( void );				//上下入れ替え
	void	ExchangeLR( void );				//左右入れ替え

	void	Matching( void );				//領域補正

	s4		And( Rect& R_Rect );			//2領域のandを設定
	s4		And( si si_Top, si si_Left, si si_Right, si si_Bottom );	//2領域のandを設定
	void	Or( Rect& R_Rect );				//2領域のorを設定

	//ゲッター系
	si		GetWidth( void );		//幅取得
	si		GetHeight( void );		//高さ取得
	si		GetAreaSize( void );	//面積取得

	//その他
	bool	CheckInArea( si si_X, si si_Y );		//領域内チェック
	bool	CheckOverlap( Rect& R_Rect );			//重複チェック
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
