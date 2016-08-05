/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/DM/PIT.cpp
	概要		：	PITクラス
	詳細		：	PITを管理するクラスです。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"DM.h"
#include	"PIT.h"
#include	"RDTM/RDTM.h"		//現在日時管理
#include	"TM/TM.h"			//タスク管理



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数・マクロ定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#define	COUNT_VALUE		( 0x4a9 )		//カウント値(1000count/s)

#define	BIN_COUNT		( 0x00 )
#define	BCD_COUNT		( 0x01 )

#define	MODE0			( 0x00 )
#define	MODE1			( 0x02 )
#define	MODE2			( 0x04 )
#define	MODE3			( 0x06 )
#define	MODE4			( 0x08 )
#define	MODE5			( 0x0a )

#define	LATCH			( 0x00 )
#define	LOWBYTE			( 0x10 )
#define	HIGHBYTE		( 0x20 )
#define	LOWHIGH			( 0x30 )

#define	COUNTER0		( 0x00 )
#define	COUNTER1		( 0x40 )
#define	COUNTER2		( 0x80 )
#define	READBACK		( 0xc0 )


/*******************************************************************************
	I/Oポート
*******************************************************************************/
#define	PORT_PIT_COUNTER0	( 0x0040 )		//カウンタ0
#define	PORT_PIT_COUNTER1	( 0x0041 )		//カウンタ1
#define	PORT_PIT_COUNTER2	( 0x0042 )		//カウンタ2
#define	PORT_PIT_CONTROL	( 0x0043 )		//コントロール



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
//PIT		G_PIT;

extern RDTM		G_RDTM;
extern TM		G_TM;
extern PIC	G_PIC;



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	関数
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	PIT割り込みルーチン
	説明	：	PITの割り込みルーチンです。
	Include	：	-
	引数	：	ARG_CONST_REGI_STAT
	戻り値	：	void
*******************************************************************************/
void	Int20( ARG_CONST_REGI_STAT )
{
	G_PIC.ACK( G_PIC.IRQ0_PIT );

	//時間を進める
	G_RDTM.SetForward();

	//タスク管理のスケジューラを呼び出し
	G_TM.Scheduler();
}


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	PITクラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	PITクラスの初期化
	説明	：
	Include	：	PIT.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	PIT::Init( void )
{
	IO::Out1( PORT_PIT_CONTROL, BIN_COUNT | MODE2 | LOWHIGH | COUNTER0 );
	IO::Out1( PORT_PIT_COUNTER0, (u1)COUNT_VALUE );				//カウンタ値Low
	IO::Out1( PORT_PIT_COUNTER0, (u1)( COUNT_VALUE >> 8 ) );	//カウンタ値High

	G_PIC.IntEnable( G_PIC.IRQ0_PIT );
}


/*******************************************************************************
	概要	：	デバイス制御
	説明	：	デバイスの制御を統括して行う関数です。
	Include	：	PIT.h
	引数	：	DCD* P_DCD
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		PIT::Ctrl( DCD* P_DCD )
{
	//未実装
	return 0;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

