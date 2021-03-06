/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Include/Mul_light/MsgQ.cpp
	概要		：	Mul light API - デバイス・データ・キュー クラス
	詳細		：	アプリケーションが、OSからデバイスのデータを受けとるためのキューバッファです。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"MLCommon.h"
#include	"Message.h"
#include	"MsgQ.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	GMsgQクラス
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	デバイス・データ・エンキュー
	説明	：	キューにデバイス・データを格納し、書き込みポインタをインクリメントします。
	Include	：	MsgQ.h
	引数	：	const Msg& CR_Message	格納するデータ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		MsgQ::Enqueue( const Msg& CR_Message )
{
	//書き込みポインタの次に読み込みポインタがあれば、書き込み拒否
	if( Mu1_Read == Mu1_Write + 1 )
		return ERROR_QUEUE_FULL;

	MA_MsgQ[Mu1_Write++] = CR_Message;
	return SUCCESS;
}

/*******************************************************************************
	概要	：	デキュー
	説明	：	キューからデータを取得し、読み込みポインタをインクリメントします。
	Include	：	MsgQ.h
	引数	：	Msg* P_Message	取得するデータを格納するポインタ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		MsgQ::Dequeue( Msg* P_Message )
{
	//読み込みポインタが書き込みポインタと同じ位置ならエラー。
	if( Mu1_Read == Mu1_Write )
		return ERROR_QUEUE_NULL;

	*P_Message = MA_MsgQ[Mu1_Read++];
	return SUCCESS;
}

/*******************************************************************************
	概要	：	データ読み取り
	説明	：	キューからデータを取得します。
				参照するだけで、ポインタのインクリメントは行いません。
	Include	：	MsgQ.h
	引数	：	Msg* P_Message	取得するデータを格納するポインタ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		MsgQ::Read( Msg* P_Message )
{
	//読み込みポインタが書き込みポインタと同じ位置ならエラー。
	if( Mu1_Read == Mu1_Write )
		return ERROR_QUEUE_NULL;

	*P_Message = MA_MsgQ[Mu1_Read];
	return SUCCESS;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

