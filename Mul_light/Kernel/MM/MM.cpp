/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/MM/MM.cpp
	概要		：	メモリ管理
	詳細		：	メモリのすべてを管理します。
					メモリ全体の使用可能な領域と使用不可能な領域を管理し、
					タスクなどに割り当てます。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"MM.h"
#include	"TM.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
extern TM		G_TM;



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	メモリ領域リストクラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	メモリ領域リストの0クリア
	説明	：
	Include	：	MM.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	MAL::Clear( void )
{
	P_Prev = P_Next = NULL;
	Mu4_Base = Mu4_Size = 0;
	M_Flag16.Init();
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	メモリ割り当てクラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	メモリ領域確保
	説明	：	指定のサイズの空き領域を検索して、ベースアドレスを返します。
	Include	：	MM.h
	引数	：	ui ui_ByteSize		確保する領域のサイズ(Byte単位)
	戻り値	：	ui		確保したメモリ領域のID(0ならエラー)
*******************************************************************************/
ui		MA::Create( ui ui_ByteSize )
{
	ui		ui_FreeAreaNo;		//空き領域番号
	ui		ui_SurplusNo;		//空き領域を確保して余った分の番号

	//エラー処理
	if( ui_ByteSize == 0 )
		return 0;

	//空き領域を検索
	G_TM.DisableTS();			//タスクスイッチ禁止
	if( ( ui_FreeAreaNo = SearchFreeArea( ui_ByteSize ) ) == (ui)-1 )
	{
		G_TM.EnableTS();		//タスクスイッチ許可
		return 0;
	}

	//確保した領域リストの設定
	//空き領域と確保する領域のサイズがまったく同じ場合
	if( M_MAL[ui_FreeAreaNo].Mu4_Size == ui_ByteSize )
	{
		M_MAL[ui_FreeAreaNo].M_Flag16.SetFlags( MAL::USAGE_BUSY );
		G_TM.EnableTS();		//タスクスイッチ許可
		return No2ID( ui_FreeAreaNo );
	}
	//空き領域と確保する領域のサイズが同じでない場合
	else
	{
		//配列の空きを検索
		if( ( ui_SurplusNo = SearchEmptyElement() ) == (ui)-1 )
		{
			G_TM.EnableTS();		//タスクスイッチ許可
			return 0;		//空いていない。。。
		}

		M_LM.Join( &M_MAL[ui_SurplusNo], &M_MAL[ui_FreeAreaNo] );		//今回確保する空き領域の後ろに、余った空き領域を作る

		//余り領域の設定
		M_MAL[ui_SurplusNo].Mu4_Base	= M_MAL[ui_FreeAreaNo].Mu4_Base + ui_ByteSize;
		M_MAL[ui_SurplusNo].Mu4_Size	= M_MAL[ui_FreeAreaNo].Mu4_Size - ui_ByteSize;
		M_MAL[ui_SurplusNo].M_Flag16.SetFlags( MAL::USAGE_FREE );


		//確保する領域の設定
		M_MAL[ui_FreeAreaNo].Mu4_Size	= ui_ByteSize;
		M_MAL[ui_FreeAreaNo].M_Flag16.SetFlags( MAL::USAGE_BUSY );

		G_TM.EnableTS();		//タスクスイッチ許可
		return No2ID( ui_FreeAreaNo );
	}

	G_TM.EnableTS();		//タスクスイッチ許可
	return 0;		//ここには来ない。
}


/*******************************************************************************
	概要	：	メモリ領域削除
	説明	：
	Include	：	MM.h
	引数	：	ui ui_ID
	戻り値	：	void
*******************************************************************************/
void	MA::Delete( ui ui_ID )
{
	ui		ui_No = ID2No( ui_ID );

	//エラー処理
	if( ui_No >= HEAP_LIST_MAX )
		return;
	//予約領域は消去不可
	if( M_MAL[ui_No].M_Flag16.CheckFlags( MAL::USAGE_RESERVED ) )
		return;

	//削除するリストを空き領域に設定
	G_TM.DisableTS();			//タスクスイッチ禁止
	M_MAL[ui_No].M_Flag16.SetFlags( MAL::USAGE_FREE );

	//空き領域の結合
	UniteArea( ui_No );
	G_TM.EnableTS();			//タスクスイッチ許可
}


/*******************************************************************************
	概要	：	ベースアドレス取得
	説明	：
	Include	：	MM.h
	引数	：	ui ui_ID
	戻り値	：	void*		ベースアドレス(0ならエラー)
*******************************************************************************/
void*	MA::GetBase( ui ui_ID )
{
	ui		ui_No = ID2No( ui_ID );

	//エラー処理
	if( ui_No >= HEAP_LIST_MAX )
		return 0;

	return (void*)M_MAL[ui_No].Mu4_Base;
}


/*******************************************************************************
	概要	：	サイズ取得
	説明	：
	Include	：	MM.h
	引数	：	ui ui_ID
	戻り値	：	ui		サイズ(0ならエラー)
*******************************************************************************/
ui		MA::GetSize( ui ui_ID )
{
	ui		ui_No = ID2No( ui_ID );

	//エラー処理
	if( ui_No >= HEAP_LIST_MAX )
		return 0;

	return M_MAL[ui_No].Mu4_Size;
}


/*******************************************************************************
	概要	：	メモリ使用量表示
	説明	：	メモリの使用量とパーセンテージを表示します。
	Include	：	MM.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	MA::ShowMemoryUsage( void )
{
	MAL*	P_MAL;				//Memory Area List
	char	c_DebugStr[64];

	ui		ui_NumMemory	= 0;		//メモリ容量
	ui		ui_UsingMemory	= 0;		//メモリ使用量

	G_TM.DisableTS();			//タスクスイッチ禁止
	for( P_MAL = (MAL*)M_LM.GetHeadList(); ; P_MAL = (MAL*)P_MAL->P_Next )
	{
		if( !P_MAL->M_Flag16.CheckFlags( MAL::USAGE_RESERVED ) )
		{
			ui_NumMemory += P_MAL->Mu4_Size;
			if( P_MAL->M_Flag16.CheckFlags( MAL::USAGE_BUSY ) )
				ui_UsingMemory += P_MAL->Mu4_Size;
		}

		//末尾まで表示したら終了
		if( P_MAL == M_LM.GetTailList() )
			break;
	}
	G_TM.EnableTS();			//タスクスイッチ許可

	sprintf( c_DebugStr, "Memory:%#dMiB/%dMiB", ui_UsingMemory >> 20, ui_NumMemory >> 20 );
	G_GUI.DebugPrintString( c_DebugStr, 0, 0, 0xffffffff );
}


/*******************************************************************************
	概要	：	メモリマップ表示
	説明	：	メモリマップを表示させます。(デバッグ用)
	Include	：	MM.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	MA::ShowMemoryMap( void )
{
	MAL*	P_MAL;				//Memory Area List
	ui		ui_CountY = 5;
	char	c_DebugStr[64];

	G_GUI.DebugPrintString( "MemoryMap", 0, ui_CountY++, 0xffffffff );

	G_TM.DisableTS();			//タスクスイッチ禁止
	for( P_MAL = (MAL*)M_LM.GetHeadList(); ; P_MAL = (MAL*)P_MAL->P_Next )
	{
		sprintf( c_DebugStr, "No.%03d Base:%#08x, Size:%#08x, Flags:%#08x", ui_CountY - 6, P_MAL->Mu4_Base, P_MAL->Mu4_Size, P_MAL->M_Flag16.GetFlags() );
		G_GUI.DebugPrintString( c_DebugStr, 0, ui_CountY++, 0xffffffff );

		//末尾まで表示したら終了
		if( P_MAL == M_LM.GetTailList() )
			break;
	}
	G_TM.EnableTS();			//タスクスイッチ許可
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	メモリ割り当てクラス：プロテクティッドメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	メモリの空き領域を検索
	説明	：
	Include	：	MM.h
	引数	：	ui ui_ByteSize	検索する空き領域のサイズ
	戻り値	：	ui		メモリ領域リストの配列番号
						空いていなければ(ui)-1
*******************************************************************************/
ui		MA::SearchFreeArea( ui ui_ByteSize )
{
	MAL*	P_CurrentMAL;

	//確保するメモリのサイズ以上の空きを検索
	for( P_CurrentMAL = (MAL*)M_LM.GetHeadList(); ; P_CurrentMAL = (MAL*)P_CurrentMAL->P_Next )
	{
		//空き領域でないなら次へ。
		if( !P_CurrentMAL->M_Flag16.CheckFlags( MAL::USAGE_FREE ) )
			continue;

		//サイズが要求以上なら検索終了
		if( P_CurrentMAL->Mu4_Size >= ui_ByteSize )
			return ( (u4)P_CurrentMAL - (u4)M_MAL ) / sizeof (MAL);		//アドレスから要素番号を算出

		//末尾まで検索しても無ければエラー。
		if( P_CurrentMAL == (MAL*)M_LM.GetTailList() )
			return (ui)-1;
	}
}


/*******************************************************************************
	概要	：	配列の空きを検索
	説明	：
	Include	：	MM.h
	引数	：	void
	戻り値	：	ui		配列の空き番号
						空いていなければ(ui)-1
*******************************************************************************/
ui		MA::SearchEmptyElement( void )
{
	ui		ui_Count;

	//配列の空きを検索
	for( ui_Count = 0; ui_Count < HEAP_LIST_MAX; ui_Count++ )
		//空きなら検索終了
		if( M_MAL[ui_Count].Mu4_Size == 0 )
			return ui_Count;

	//配列が空いていない
	return (ui)-1;
}


/*******************************************************************************
	概要	：	空き領域結合
	説明	：	空き領域のリストが前後にあれば、一つにまとめます。
	Include	：	MM.h
	引数	：	ui ui_No		結合するリストの配列番号
	戻り値	：	void
*******************************************************************************/
void	MA::UniteArea( ui ui_No )
{
	MAL* const	PC_MAL = &M_MAL[ui_No];		//結合の基準となるリスト
	MAL*	P_Next;
	MAL*	P_Prev;

	//エラー処理
	if( ui_No >= HEAP_LIST_MAX )		//最大要素数以上ならエラー。
		return;
	if( !PC_MAL->M_Flag16.CheckFlags( MAL::USAGE_FREE ) )	//空き領域以外はいじらない。
		return;

	//リストの終端でなく、次が同じ領域である限り続ける
	P_Next = (MAL*)PC_MAL->P_Next;
	while( (List*)PC_MAL != M_LM.GetTailList()
			&& PC_MAL->M_Flag16.GetFlags( MAL::USAGE_MASK ) == P_Next->M_Flag16.GetFlags( MAL::USAGE_MASK ) )
	{
		PC_MAL->Mu4_Size += P_Next->Mu4_Size;	//基準の次のリストのサイズを加算
		M_LM.Split( P_Next );					//基準の次のリストを分離
		P_Next->Clear();						//基準の次のリストをクリアー
		P_Next = (MAL*)PC_MAL->P_Next;
	}

	//リストの先頭でなく、前が同じ領域である限り続ける
	P_Prev = (MAL*)PC_MAL->P_Prev;
	while( (List*)PC_MAL != M_LM.GetHeadList()
			&& PC_MAL->M_Flag16.GetFlags( MAL::USAGE_MASK ) == P_Prev->M_Flag16.GetFlags( MAL::USAGE_MASK ) )
	{
		PC_MAL->Mu4_Base = P_Prev->Mu4_Base;	//基準のベースアドレスを前のリストのベースアドレスに変更
		PC_MAL->Mu4_Size += P_Prev->Mu4_Size;	//基準の前のリストのサイズを加算
		M_LM.Split( P_Prev );					//基準の前のリストを分離
		P_Prev->Clear();						//基準の前のリストをクリアー
		P_Prev = (MAL*)PC_MAL->P_Prev;
	}
}


/*******************************************************************************
	概要	：	配列要素番号からIDへ変換
	説明	：	配列の要素番号をメモリ領域を扱うためのIDへ変換します。
				これは、容易にメモリ領域を削除しないようにするためです。
	Include	：	MM.h
	引数	：	ui ui_No		配列の要素番号
	戻り値	：	ui		メモリ領域を扱うためのID
*******************************************************************************/
ui		MA::No2ID( ui ui_No )
{
	return ui_No ^ ui_No << 11 ^ ui_No << 22 ^ ID_NO_EOR;
}


/*******************************************************************************
	概要	：	IDから配列要素番号へ変換
	説明	：	メモリ領域を扱うためのIDを配列の要素番号へ変換します。
				これは、容易にメモリ領域を削除しないようにするためです。
	Include	：	MM.h
	引数	：	ui ui_ID		メモリ領域を扱うためのID
	戻り値	：	ui		配列の要素番号
*******************************************************************************/
ui		MA::ID2No( ui ui_ID )
{
	return ( ui_ID ^ ID_NO_EOR ) & ( HEAP_LIST_MAX - 1 );
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

