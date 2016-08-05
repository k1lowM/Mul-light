/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Include/Mul_light/List.h
	概要		：	リスト構造クラスヘッダ
	詳細		：	双方向リストのリスト構造クラスのヘッダです。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	リスト構造体
*******************************************************************************/
struct	List
{
public:
	List*	P_Prev;		//前のリストポインタ
	List*	P_Next;		//次のリストポインタ
};


/*******************************************************************************
	リスト管理クラス
*******************************************************************************/
class	LM
{
//定数
public:
	enum	Error
	{
		ERROR = _ERROR_,
		ERROR_NULLPOINTER,			//NULLポインタ

		SUCCESS = _SUCCESS_,
		SUCCESS_NOCHANGE,			//入れ替える必要無し。
	};

//変数
private:
	List*	MP_HeadList;
	List*	MP_TailList;

//メソッド
public:
	void	Init( List* P_FirstList = NULL );

	s4		Join( List* P_JoinList, List* P_Base = NULL );	//リスト結合(指定リストの後ろに追加)
	s4		JoinHead( List* P_JoinList );					//リスト結合(先頭に追加)
	s4		JoinTail( List* P_JoinList );					//リスト結合(末尾に追加)
	s4		Split( List* P_SplitList );						//リスト分離
	s4		Move( List* P_MoveList, List* P_Dest = NULL );	//リスト移動(指定リストの後ろに移動)
	s4		MoveHead( List* P_MoveList );					//リスト移動(先頭に移動)
	s4		MoveTail( List* P_MoveList );					//リスト移動(末尾に移動)
	s4		Exchange( List* P_List1, List* P_List2 );		//リスト入れ替え

	List*	GetHeadList( void );
	List*	GetTailList( void );
	ui		GetNumList( void );		//リスト数取得
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

