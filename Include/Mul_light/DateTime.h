/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Include/Mul_light/DateTime.h
	概要		：	Mul light API - 日時クラスヘッダ
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数・マクロ定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	日時クラス
*******************************************************************************/
class	DateTime
{
//定数
public:
	//エラー情報
	enum	Error
	{
		ERROR = _ERROR_,
		ERROR_YEAR_0,			//年が0
		ERROR_MONTH_0,			//月が0
		ERROR_MONTH_OVER12,		//月が12より大きい
		ERROR_DAY_0,			//日が0
		ERROR_DAY_OVER28,		//日が28より大きい
		ERROR_DAY_OVER29,		//日が29より大きい
		ERROR_DAY_OVER30,		//日が30より大きい
		ERROR_DAY_OVER31,		//日が31より大きい
		ERROR_NOT_WDAY,			//週番号が0～6でない
		ERROR_HOUR_ABOVE24,		//時間が24以上
		ERROR_MIN_ABOVE60,		//分が60以上
		ERROR_SEC_ABOVE60,		//秒が60以上
		ERROR_MSEC_ABOVE1000,	//ミリ秒が1000以上

		SUCCESS = _SUCCESS_,
	};

	//曜日
	enum	WeekDay
	{
		SUNDAY,
		MONDAY,
		TUESDAY,
		WEDNESDAY,
		THURSDAY,
		FRIDAY,
		SATURDAY,

		NUM_WEEKDAY
	};

//変数
private:
	u4		Mu4_Year;		//年
	u1		Mu1_Month;		//月
	u1		Mu1_Day;		//日
	u1		Mu1_WDay;		//曜日
	u1		Mu1_Hour;		//時
	u1		Mu1_Min;		//分
	u1		Mu1_Sec;		//秒
	u2		Mu2_MSec;		//ミリ秒

//メソッド
public:
	void	Init( void );

	s4		SetDateTime( ui ui_Year, ui ui_Month, ui ui_Day, ui ui_WDay, ui ui_Hour, ui ui_Min, ui ui_Sec, ui ui_MSec );
	void	SetForwardMSec( void );
	s4		Check( void );

	u4		GetYear( void );
	u1		GetMonth( void );
	u1		GetDay( void );
	u1		GetWDay( void );
	u1		GetHour( void );
	u1		GetMin( void );
	u1		GetSec( void );
	u2		GetMSec( void );
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
