/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/DM/RTC.cpp
	概要		：	RTCクラス
	詳細		：	RTCを管理するクラスです。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"DM.h"
#include	"RTC.h"

//デバッグ用
#include	<stdio.h>
#include	"GUI/GUI.h"


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数・マクロ定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
//レジスタ指定
#define	RTC_SECOND		( 0x00 )		//秒
#define	RTC_MINUTE		( 0x02 )		//分
#define	RTC_HOUR		( 0x04 )		//時
#define	RTC_WEEKDAY		( 0x06 )		//曜日
#define	RTC_DAY			( 0x07 )		//日
#define	RTC_MONTH		( 0x08 )		//月
#define	RTC_YEAR_LOW	( 0x09 )		//年	西暦年(下2桁)
#define	RTC_YEAR_HIGH	( 0x32 )		//年	西暦年(上2桁)

#define	RTC_REG_A		( 0x0a )		//レジスタA
#define	RTC_REG_B		( 0x0b )		//レジスタB
#define	RTC_REG_C		( 0x0c )		//レジスタC
#define	RTC_REG_D		( 0x0d )		//レジスタD


/*******************************************************************************
	I/Oポート
*******************************************************************************/
#define	PORT_RTC_ADDR		( 0x0070 )		//アドレス指定レジスタ
#define	PORT_RTC_DATA		( 0x0071 )		//データレジスタ



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
//RTC		G_RTC;



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	RTCクラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	RTCクラスの初期化
	説明	：
	Include	：	RTC.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	RTC::Init( void )
{
/*
	u1		u1_RTCData;

	//バイナリモードに設定（時分秒がBCDからかわらん・・・）
	//----------------------------------------------------------
	IO::Out1( PORT_RTC_ADDR, RTC_REG_B );
	u1_RTCData = IO::In1( PORT_RTC_DATA );

	u1_RTCData |= 0b00000100;

	IO::Out1( PORT_RTC_ADDR, RTC_REG_B );
	IO::Out1( PORT_RTC_DATA, u1_RTCData );
	//----------------------------------------------------------
*/
}


/*******************************************************************************
	概要	：	デバイス制御
	説明	：	デバイスの制御を統括して行う関数です。
	Include	：	RTC.h
	引数	：	DCD* P_DCD
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		RTC::Ctrl( DCD* P_DCD )
{
	//未実装
	return 0;
}


/*******************************************************************************
	概要	：	日時取得
	説明	：
	Include	：	RTC.h
	引数	：	DateTime* P_DateTime	日付クラスのポインタ
	戻り値	：	void
*******************************************************************************/
void	RTC::GetDateTime( DateTime* P_DateTime )
{
//	DateTime	DateTime;
	u4		u4_Year;
	u1		u1_Month, u1_Day, u1_WDay;
	u1		u1_Hour, u1_Min, u1_Sec;

	//取得
	while( true )
	{
		IO::Out1( PORT_RTC_ADDR, RTC_SECOND );		//秒
		u1_Sec = BCD2Dec( IO::In1( PORT_RTC_DATA ) );
		IO::Out1( PORT_RTC_ADDR, RTC_MINUTE );		//分
		u1_Min = BCD2Dec( IO::In1( PORT_RTC_DATA ) );
		IO::Out1( PORT_RTC_ADDR, RTC_HOUR );		//時
		u1_Hour = BCD2Dec( IO::In1( PORT_RTC_DATA ) );
		IO::Out1( PORT_RTC_ADDR, RTC_WEEKDAY );		//曜日
		u1_WDay = IO::In1( PORT_RTC_DATA ) - 1;
		IO::Out1( PORT_RTC_ADDR, RTC_DAY );			//日
		u1_Day = BCD2Dec( IO::In1( PORT_RTC_DATA ) );
		IO::Out1( PORT_RTC_ADDR, RTC_MONTH );		//月
		u1_Month = BCD2Dec( IO::In1( PORT_RTC_DATA ) );
		IO::Out1( PORT_RTC_ADDR, RTC_YEAR_HIGH );	//年(上2桁)
		u4_Year = BCD2Dec( IO::In1( PORT_RTC_DATA ) ) * 100;
		IO::Out1( PORT_RTC_ADDR, RTC_YEAR_LOW );	//年(下2桁)
		u4_Year += BCD2Dec( IO::In1( PORT_RTC_DATA ) );

		//ちょうど59秒から0秒になっていなければ終了。
		IO::Out1( PORT_RTC_ADDR, RTC_SECOND );
		if( u1_Sec <= BCD2Dec( IO::In1( PORT_RTC_DATA ) ) )
			break;
	}

	//設定
	P_DateTime->SetDateTime( u4_Year, u1_Month, u1_Day, u1_WDay, u1_Hour, u1_Min, u1_Sec, 0 );
}


/*******************************************************************************
	概要	：	RTCに日時設定
	説明	：
	Include	：	RTC.h
	引数	：	DateTime DateTime	設定する日時のクラス
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		RTC::SetDateTime( DateTime DateTime )
{
	//未作成
	return 0;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	RTCクラス：プライベートメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	BCDから10進数へ変換
	説明	：	1バイトのBCD値を1バイトの10進数に変換します。
	Include	：	RTC.h
	引数	：	u1		10進数に変換するBCD値
	戻り値	：	u1		10進数に変換した値
*******************************************************************************/
u1		RTC::BCD2Dec( u1 u1_BCD )
{
	return ( u1_BCD & 0x0f ) + ( u1_BCD >> 4 ) * 10;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

