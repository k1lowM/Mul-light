/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Include/Mul_light/DateTime.cpp
	概要		：	Mul light API - 日時クラス
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"MLCommon.h"
#include	"DateTime.h"
#include	<string.h>



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	日時クラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	日時クラスの初期化
	説明	：
	Include	：	DateTime.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	DateTime::Init( void )
{
	Mu4_Year	= 1;
	Mu1_Month	= 1;
	Mu1_Day		= 1;
	Mu1_WDay	= SUNDAY;
	Mu1_Hour	= 0;
	Mu1_Sec		= 0;
	Mu1_Min		= 0;
	Mu2_MSec	= 0;
}


/*******************************************************************************
	概要	：	日時の設定
	説明	：	日時を一括で設定します。
	Include	：	DateTime.h
	引数	：	ui		日時情報(-1のときは変更しない)
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		DateTime::SetDateTime( ui ui_Year, ui ui_Month, ui ui_Day, ui ui_WDay,
		ui ui_Hour, ui ui_Min, ui ui_Sec, ui ui_MSec )
{
	u1		Au1_Work[sizeof (DateTime)];

	//現在のデータ退避
	memcpy( Au1_Work, this, sizeof (DateTime) );

	//-1なら更新しない。
	if( ui_Year		!= (ui)-1 )		Mu4_Year	= (u4)ui_Year;		//年
	if( ui_Month	!= (ui)-1 )		Mu1_Month	= (u1)ui_Month;		//月
	if( ui_Day		!= (ui)-1 )		Mu1_Day		= (u1)ui_Day;		//日
	if( ui_WDay		!= (ui)-1 )		Mu1_WDay	= (u1)ui_WDay;		//曜日
	if( ui_Hour		!= (ui)-1 )		Mu1_Hour	= (u1)ui_Hour;		//時
	if( ui_Min		!= (ui)-1 )		Mu1_Min		= (u1)ui_Min;		//分
	if( ui_Sec		!= (ui)-1 )		Mu1_Sec		= (u1)ui_Sec;		//秒
	if( ui_MSec		!= (ui)-1 )		Mu2_MSec	= (u2)ui_MSec;		//ミリ秒

	//日時の検証
	if( Check() > 0 )	//正当
		return SUCCESS;		//そのまま終了
	else				//不当
	{
		//元に戻す
		memcpy( this, Au1_Work, sizeof (DateTime) );
		return ERROR;		//指定した日付が存在しない。
	}
}


/*******************************************************************************
	概要	：	時間を進める
	説明	：	時間を1ミリ秒進めます
	Include	：	DateTime.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	DateTime::SetForwardMSec( void )
{
	if( ++Mu2_MSec > 1000 )	//ミリ秒
	{
		Mu2_MSec = 0;
		if( ++Mu1_Sec > 60 )			//秒
		{
			Mu1_Sec = 0;
			if( ++Mu1_Min > 60 )			//分
			{
				Mu1_Min = 0;
				if( ++Mu1_Hour > 24 )			//時
				{
					Mu1_Hour = 0;
					Mu1_WDay++;
					Mu1_Day++;

					Mu1_WDay %= NUM_WEEKDAY;		//週を0～7に限定する。

					switch( Mu1_Month )
					{
					case 1:		case 3:		case 5:		case 7:		case 8:		case 10:
						if( Mu1_Day > 31 )
						{
							Mu1_Month++;
							Mu1_Day = 1;
						}
						break;
					case 4:		case 6:		case 9:		case 11:
						if( Mu1_Day > 30 )
						{
							Mu1_Month++;
							Mu1_Day = 1;
						}
						break;
					case 2:
						//通常年
						if( Mu4_Year & 0x03 )
						{
							if( Mu1_Day > 28 )
							{
								Mu1_Month++;
								Mu1_Day = 1;
							}
						}
						//閏年
						else
						{
							if( Mu1_Day > 29 )
							{
								Mu1_Month++;
								Mu1_Day = 1;
							}
						}
						break;
					case 12:
						if( Mu1_Day > 31 )
						{
							Mu4_Year++;
							Mu1_Month = 1;
							Mu1_Day = 1;
						}
						break;
					}
				}
			}
		}
	}
}


/*******************************************************************************
	概要	：	日時チェック
	説明	：	日時が正当かチェックします。
	Include	：	DateTime.h
	引数	：	void
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		DateTime::Check( void )
{
	//0エラー
	if( !Mu4_Year )
		return ERROR_YEAR_0;
	if( !Mu1_Month )
		return ERROR_MONTH_0;
	if( !Mu1_Day )
		return ERROR_DAY_0;

	//オーバーエラー
	if( Mu1_Month > 12 )
		return ERROR_MONTH_OVER12;
	if( Mu1_WDay >= NUM_WEEKDAY )
		return ERROR_NOT_WDAY;
	if( Mu1_Hour >= 24 )
		return ERROR_HOUR_ABOVE24;
	if( Mu1_Min >= 60 )
		return ERROR_MIN_ABOVE60;
	if( Mu1_Sec >= 60 )
		return ERROR_SEC_ABOVE60;
	if( Mu2_MSec >= 1000 )
		return ERROR_MSEC_ABOVE1000;

	if( Mu1_Day > 31 )
		return ERROR_DAY_OVER31;
	switch( Mu1_Month )
	{
	case 4:	case 6:	case 9:	case 11:
		if( Mu1_Day > 30 )
			return ERROR_DAY_OVER30;
		break;
	case 2:
		//通常年
		if( Mu4_Year & 0x03 )
		{
			if( Mu1_Day > 28 )
				return ERROR_DAY_OVER28;
		}
		//閏年
		else
		{
			if( Mu1_Day > 29 )
				return ERROR_DAY_OVER29;
		}
		break;
	}

	//正常
	return SUCCESS;
}


/*******************************************************************************
	概要	：	ゲッター
	説明	：	メンバ変数を取得するメソッドです。
	Include	：	DateTime.h
	引数	：	void
	戻り値	：	u4 | u1 | u2		取得するメンバ変数
*******************************************************************************/
u4		DateTime::GetYear( void ){	return Mu4_Year; }
u1		DateTime::GetMonth( void ){	return Mu1_Month; }
u1		DateTime::GetDay( void ){	return Mu1_Day; }
u1		DateTime::GetWDay( void ){	return Mu1_WDay; }
u1		DateTime::GetHour( void ){	return Mu1_Hour; }
u1		DateTime::GetMin( void ){	return Mu1_Min; }
u1		DateTime::GetSec( void ){	return Mu1_Sec; }
u2		DateTime::GetMSec( void ){	return Mu2_MSec; }



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

