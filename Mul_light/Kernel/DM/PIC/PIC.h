/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/DM/PIC.h
	概要		：	割り込みクラスヘッダ
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Driver.h"
#include	<Device/DCD.h>



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	PICクラス
*******************************************************************************/
class	PIC : public Driver
{
//定数
public:
	enum	IRQ
	{
		//PIC0
		IRQ0_PIT,				//タイマ
		IRQ1_KEYBOARD,			//キーボード
		IRQ2_PIC1,				//PIC1のカスケード接続
		IRQ3_COM2_COM4,			//シリアルポートCOM2, COM4
		IRQ4_COM1_COM3,			//シリアルポートCOM2, COM4
		IRQ5_,				//
		IRQ6_,				//
		IRQ7_,				//
		//PIC1
		IRQ8_RTC,				//リアルタイムクロック
		IRQ9_,				//
		IRQ10_,				//
		IRQ11_,				//
		IRQ12_MOUSE,			//マウス
		IRQ13_,				//
		IRQ14_HDC1,				//ハードディスクコントローラ1
		IRQ15_HDC2,				//ハードディスクコントローラ2
	};

//変数
private:
	u1		Mu1_MasterIF;		//マスター割り込み許可フラグ(1:許可)
	u1		Mu1_SlaveIF;		//スレーブ割り込み許可フラグ(1:許可)

//メソッド
public:
	void	Init( void );
	s4		Ctrl( DCD* P_DCD );

	void	IntEnable( IRQ IRQNo );		//割り込み許可
	void	IntDisable( IRQ IRQNo );	//割り込み禁止
	void	ACK( IRQ IRQNo );			//受付完了をPICに通知
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
