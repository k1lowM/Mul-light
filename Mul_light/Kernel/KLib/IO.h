/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/KLib/IO.h
	概要		：	I/O関数群ヘッダ
	詳細		：	周辺装置のインプット・アウトプットを行う関数の宣言です。
					ここでは、あえてオーバーロードをしていません。
					入出力はシビアなのです。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	I/Oクラス
*******************************************************************************/
class	IO
{
//メソッド
public:
	//入力
	static u1		In1( u2 u2_Port );
	static u2		In2( u2 u2_Port );
	static u4		In4( u2 u2_Port );

	//連続入力
	static void	Ins1( u2 u2_Port, void* Pv_Dest, ui ui_Count );
	static void	Ins2( u2 u2_Port, void* Pv_Dest, ui ui_Count );
	static void	Ins4( u2 u2_Port, void* Pv_Dest, ui ui_Count );

	//出力
	static void	Out1( u2 u2_Port, u1 u1_Source );
	static void	Out2( u2 u2_Port, u2 u2_Source );
	static void	Out4( u2 u2_Port, u4 u4_Source );

	//連続出力
	static void	Outs1( u2 u2_Port, void* Pv_Src, ui ui_Count );
	static void	Outs2( u2 u2_Port, void* Pv_Src, ui ui_Count );
	static void	Outs4( u2 u2_Port, void* Pv_Src, ui ui_Count );
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

