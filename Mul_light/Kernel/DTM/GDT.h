/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/DTM/GDT.h
	概要		：	グローバル・ディスクリプタ・テーブルヘッダ
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"DT.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	GDTクラス
*******************************************************************************/
class	GDT : public DTBase
{
//変数
public:
	void	Init( void );

	//ディスクリプタの登録
	//セグメント・ディスクリプタ
	s4		SetSegment( SegmInfo SegmInfo, SELECTOR* P_Selector = NULL );				//番号自動設定
	s4		SetSegment( u4 u4_Number, SegmInfo SegmInfo, SELECTOR* P_Selector = NULL );	//番号手動指定
	//ゲート・ディスクリプタ
	s4		SetGate( GateInfo GateInfo, SELECTOR* P_Selector = NULL );					//番号自動設定
	s4		SetGate( u4 u4_Number, GateInfo GateInfo, SELECTOR* P_Selector = NULL );	//番号手動設定

	//ディスクリプタ情報取得
	SegmInfo	GetSegmInfo( SELECTOR Selector );		//セグメント情報取得
	GateInfo	GetGateInfo( SELECTOR Selector );		//ゲート情報取得

	//ディスクリプタの削除
	s4		ClearDescriptor( SELECTOR Selector );
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
