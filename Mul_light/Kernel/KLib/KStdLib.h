/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/KLib/KStdLib.h
	概要		：	カーネル専用標準ライブラリヘッダ
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	カーネル標準ライブラリクラス
*******************************************************************************/
class	KStdLib
{
//メソッド
public:
	//カーネル<-->ローカル間アドレス変換
	static void*	ConvK2L( const void* CPv_K2LAddr, const void* CPv_LocalAddr );
	static void*	ConvL2K( const void* CPv_L2KAddr, const void* CPv_LocalAddr );
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

