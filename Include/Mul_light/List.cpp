/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Include/Mul_light/List.cpp
	概要		：	リスト構造クラス
	詳細		：	双方向リストのリスト構造クラスです。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"MLCommon.h"
#include	"List.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	リスト管理クラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	リスト構造初期化
	説明	：
	Include	：	List.h
	引数	：	[List* P_FirstList = NULL]
	戻り値	：	void
*******************************************************************************/
void	LM::Init( List* P_FirstList )
{
	MP_HeadList = MP_TailList = P_FirstList;
	P_FirstList->P_Prev = P_FirstList->P_Next = P_FirstList;
}


/*******************************************************************************
	概要	：	リスト結合(指定リストの後ろに追加)
	説明	：	指定したリストの後ろに新しいリストを挿入・結合します。
	Include	：	List.h
	引数	：	List* P_JoinList			追加するリスト
				[List* P_ListBase = NULL]	追加する基準になるリスト
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		LM::Join( List* P_JoinList, List* P_ListBase )
{
	//エラー処理
	if( P_JoinList == NULL )
		return ERROR_NULLPOINTER;

	//末尾ならJoinTailにお任せ。
	if( P_ListBase == NULL || P_ListBase == MP_TailList )
		return JoinTail( P_JoinList );

	//先頭ならJoinHeadにお任せ。
	if( P_ListBase == MP_HeadList )
		return JoinHead( P_JoinList );

	//リスト数が0なら全部初期化
	if( GetNumList() == 0 )
	{
		Init( P_JoinList );
		return SUCCESS;
	}

	//リスト連結
	P_JoinList->P_Prev = P_ListBase;
	P_JoinList->P_Next = P_ListBase->P_Next;
	P_ListBase->P_Next->P_Prev = P_JoinList;
	P_ListBase->P_Next = P_JoinList;
	return SUCCESS;
}


/*******************************************************************************
	概要	：	リスト結合(先頭に追加)
	説明	：	リストの先頭に新しいリストを挿入・結合します。
	Include	：	List.h
	引数	：	List* P_JoinList			追加するリスト
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		LM::JoinHead( List* P_JoinList )
{
	//エラー処理
	if( P_JoinList == NULL )
		return ERROR_NULLPOINTER;

	//リスト数が0なら全部初期化
	if( GetNumList() == 0 )
	{
		Init( P_JoinList );
		return SUCCESS;
	}

	//リスト連結
	P_JoinList->P_Prev = MP_TailList;
	P_JoinList->P_Next = MP_HeadList;
	MP_TailList->P_Next = P_JoinList;
	MP_HeadList->P_Prev = P_JoinList;
	MP_HeadList = P_JoinList;
	return SUCCESS;
}


/*******************************************************************************
	概要	：	リスト結合(末尾に追加)
	説明	：	リストの末尾に新しいリストを挿入・結合します。
	Include	：	List.h
	引数	：	List* P_JoinList			追加するリスト
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		LM::JoinTail( List* P_JoinList )
{
	//エラー処理
	if( P_JoinList == NULL )
		return ERROR_NULLPOINTER;

	//リスト数が0なら全部初期化
	if( GetNumList() == 0 )
	{
		Init( P_JoinList );
		return SUCCESS;
	}

	//リスト連結
	P_JoinList->P_Prev = MP_TailList;
	P_JoinList->P_Next = MP_HeadList;
	MP_TailList->P_Next = P_JoinList;
	MP_HeadList->P_Prev = P_JoinList;
	MP_TailList = P_JoinList;
	return SUCCESS;
}


/*******************************************************************************
	概要	：	リスト分離
	説明	：	指定のリストをリスト構造から切り離します
	Include	：	List.h
	引数	：	List* P_SplitList			分離するリスト
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		LM::Split( List* P_SplitList )
{
	//エラー処理
	if( P_SplitList == NULL )
		return ERROR_NULLPOINTER;

	//先頭と末尾の処理
	if( P_SplitList == MP_HeadList && P_SplitList == MP_TailList )
		MP_HeadList = MP_TailList = NULL;		//一つしかなければ、NULLに設定。
	else if( P_SplitList == MP_HeadList )		//先頭のリストなら、
		MP_HeadList = P_SplitList->P_Next;		//　対象の次のリストを先頭に設定。
	else if( P_SplitList == MP_TailList )		//末尾のリストなら、
		MP_TailList = P_SplitList->P_Prev;		//　対象の前のリストを末尾に設定。

	//リスト分離
	P_SplitList->P_Prev->P_Next = P_SplitList->P_Next;	//対象の前のリストと、
	P_SplitList->P_Next->P_Prev = P_SplitList->P_Prev;	//　次のリストを連結。
//	P_SplitList->P_Prev = NULL;		//対象のリストを無効化
//	P_SplitList->P_Next = NULL;		//対象のリストを無効化

	return SUCCESS;
}


/*******************************************************************************
	概要	：	リスト移動(指定リストの後ろに移動)
	説明	：	指定したリストの後ろに既存のリストを移動します。
	Include	：	List.h
	引数	：	List* P_MoveList			移動するリスト
				[List* P_Dest = NULL]		移動先のリスト
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		LM::Move( List* P_MoveList, List* P_Dest )
{
	s4		s4_Return;
	if( ( s4_Return = Split( P_MoveList ) ) < 0 )	//一旦分離して、
		return s4_Return;
	return Join( P_MoveList, P_Dest );				//再結合
}


/*******************************************************************************
	概要	：	リスト移動(先頭に移動)
	説明	：	リストの先頭に既存のリストを移動します。
	Include	：	List.h
	引数	：	List* P_MoveList			移動するリスト
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		LM::MoveHead( List* P_MoveList )
{
	s4		s4_Return;
	if( ( s4_Return = Split( P_MoveList ) ) < 0 )	//一旦分離して、
		return s4_Return;
	return JoinHead( P_MoveList );					//再結合
}


/*******************************************************************************
	概要	：	リスト移動(末尾に移動)
	説明	：	リストの末尾に既存のリストを移動します。
	Include	：	List.h
	引数	：	List* P_MoveList		移動するリスト
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		LM::MoveTail( List* P_MoveList )
{
	s4		s4_Return;
	if( ( s4_Return = Split( P_MoveList ) ) < 0 )	//一旦分離して、
		return s4_Return;
	return JoinTail( P_MoveList );					//再結合
}


/*******************************************************************************
	概要	：	リスト入れ替え
	説明	：	リスト1とリスト2を入れ替えます
	Include	：	List.h
	引数	：	List* P_List1		入れ替えるリスト1
				List* P_List2		入れ替えるリスト2
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		LM::Exchange( List* P_List1, List* P_List2 )
{
	List	ListWork;

	//エラー処理
	if( P_List1 == NULL || P_List2 == NULL )
		return ERROR_NULLPOINTER;
	if( P_List1 == P_List2 )	//まったく同じなら入れ替える必要無し。
		return SUCCESS_NOCHANGE;

	//入れ替え
	ListWork.P_Prev = P_List1->P_Prev;	ListWork.P_Next = P_List1->P_Next;	//1からワークへ
	P_List1->P_Prev = P_List2->P_Prev;	P_List1->P_Next = P_List2->P_Next;	//2から1へ
	P_List2->P_Prev = ListWork.P_Prev;	P_List2->P_Next = ListWork.P_Next;	//ワークから2へ

	//先頭と末尾の変更
	if( P_List1 == MP_HeadList )		//リスト1が先頭
		MP_HeadList = P_List2;
	else if( P_List2 == MP_HeadList )	//リスト2が先頭
		MP_HeadList = P_List1;

	if( P_List1 == MP_TailList )		//リスト1が終端
		MP_TailList = P_List2;
	else if( P_List2 == MP_TailList )	//リスト2が終端
		MP_TailList = P_List1;

	return SUCCESS;
}


//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■


/*******************************************************************************
	概要	：	リストの先頭のポインタを取得
	説明	：
	Include	：	List.h
	引数	：	void
	戻り値	：	List*		リストの先頭のポインタ
*******************************************************************************/
List*	LM::GetHeadList( void )
{
	return MP_HeadList;
}


/*******************************************************************************
	概要	：	リストの末尾のポインタを取得
	説明	：
	Include	：	List.h
	引数	：	void
	戻り値	：	List*		リストの末尾のポインタ
*******************************************************************************/
List*	LM::GetTailList( void )
{
	return MP_TailList;
}


/*******************************************************************************
	概要	：	リスト数取得
	説明	：
	Include	：	List.h
	引数	：	void
	戻り値	：	ui		リスト数
*******************************************************************************/
ui		LM::GetNumList( void )
{
	List*	P_CurrentList;
	ui		ui_NumList = 0;

	//先頭ポインタか末尾ポインタがNULLなら0
	if( MP_HeadList == NULL || MP_TailList == NULL )
		return ui_NumList;

	for( P_CurrentList = MP_HeadList; P_CurrentList != MP_TailList; P_CurrentList = P_CurrentList->P_Next )
		ui_NumList++;

	return ++ui_NumList;		//一つ目を考慮したインクリメント
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

