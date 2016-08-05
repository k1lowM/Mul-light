/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Include/Mul_light/Objects/Pack.cpp
	概要		：	Mul light API - パッククラス
	詳細		：	複数の子オブジェクトを持つことができるボックスです。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"MLCommon.h"

#include	"Pack.h"
#include	"Single.h"
#include	"Content.h"

#include	<string.h>



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	パッククラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	パッククラスの初期化
	説明	：
	Include	：	Pack.h
	引数	：	[Aspect Aspect = HORIZONTAL]			縦／横方向
				[Uniformity Uniformity = BALANCED]		均一フラグ
	戻り値	：	void
*******************************************************************************/
void	Pack::Init( Aspect Aspect, Uniformity Uniformity )
{
	Box::Init();
	Object::SetObjectType( Object::PACK );
	Object::SetOffset( 0, 0, H_CENTER, V_CENTER );
	Object::SetSize( 0, 0, WIDTH_MAX, HEIGHT_MAX );

	Box::SetBGColor( 0xffffffff );
	Box::SetPadding();

	memset( MA_ObjList, 0, sizeof MA_ObjList );	//子オブジェクトリスト初期化
	M_ObjLM.Init();								//子オブジェクトリスト管理初期化
	M_Flags.Init( Aspect | Uniformity );		//方向と均一フラグをセット
}


/*******************************************************************************
	概要	：	子オブジェクト追加
	説明	：	パックに子オブジェクトを追加します。
				PartExpandフラグで、広げれる領域まで拡張するか、
				子オブジェクトのサイズまで縮めるかを指定できます。
	Include	：	Pack.h
	引数	：	Object* P_Child							追加する子オブジェクト
//				[PartHExpand PartHExpand = PART_H_EXPAND]	個別拡張フラグ(全体が拡張時のみ有効。)
//				[PartVExpand PartVExpand = PART_V_EXPAND]
	戻り値	：	void
*******************************************************************************/
void	Pack::AddChild( Object* P_Child )
{
	ui		ui_Empty;
	//エラー処理
	if( P_Child == NULL )		//ヌルポ
		return;

	//配列の空きを検索
	for( ui_Empty = 0; MA_ObjList[ui_Empty].MP_Child; ui_Empty++ )
		if( ui_Empty >= NUM_LIST )
			return;

	//配列の空き発見！
	MA_ObjList[ui_Empty].MP_Child = P_Child;			//子オブジェクト設定
	M_ObjLM.JoinTail( MA_ObjList + ui_Empty );			//末尾に連結
	P_Child->SetParent( this );							//子の親を設定
}


/*******************************************************************************
	概要	：	親オブジェクト設定
	説明	：	複数の子オブジェクトの親(this)を設定します。
	Include	：	Pack.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	Pack::SetParent( void )
{
	ObjList*	P_ObjList;

	//子無し。
	if( ( P_ObjList = (ObjList*)M_ObjLM.GetHeadList() ) == NULL )
		return;

	//子オブジェクト全ての親を再帰的に設定
	for( ; ; P_ObjList = (ObjList*)P_ObjList->P_Next )
	{
		P_ObjList->MP_Child->SetParent( this );		//子の親を設定(再帰)

		//すべて設定したら終了
		if( P_ObjList == (ObjList*)M_ObjLM.GetTailList() )
			break;
	}
}


/*******************************************************************************
	概要	：	最小サイズ取得
	説明	：	このオブジェクトの最低限必要とするサイズを返します。
				最低限必要なサイズは、子オブジェクトの内容のサイズを
				　一番下の子まで再帰で調べて算出します。
				パックに含まれる全ての子オブジェクトが計算対象です。
				この関数はシングルクラスのGetMinSize()と連携して動作します。
	Include	：	Pack.h
	引数	：	ui* Pui_Width		最小幅	を返すポインタ
				ui* Pui_Height			高さ
	戻り値	：	void
*******************************************************************************/
void	Pack::GetMinSize( ui* Pui_Width, ui* Pui_Height )
{
	ObjList*	P_ObjList;		//オブジェクト・リスト・ポインタ
	ui		ui_ChWidth = 0;		//子の最小幅のバッファ
	ui		ui_ChHeight = 0;	//子の最小高さのバッファ

	*Pui_Width = *Pui_Height = 0;		//ポインタ引数の戻り値0クリア

	//子無し。
	if( ( P_ObjList = (ObjList*)M_ObjLM.GetHeadList() ) == NULL )
		return;

	//パックに並ぶ子オブジェクトを順番に調べる
	for( ; ; P_ObjList = (ObjList*)P_ObjList->P_Next )
	{
		Object*	P_Child = P_ObjList->MP_Child;		//子オブジェクト

		//子がNULLならエラー
		if( P_Child == NULL )
			return;

		P_Child->Object::GetMinSize( &ui_ChWidth, &ui_ChHeight );

		//水平方向パック
		if( M_Flags.CheckFlags( HORIZONTAL ) )
		{
			*Pui_Width += ui_ChWidth;		//パックの幅を加算
			if( *Pui_Height < ui_ChHeight )	//最大の高さを格納
				*Pui_Height = ui_ChHeight;
		}
		//垂直方向パック
		else
		{
			*Pui_Height += ui_ChHeight;		//パックの高さを加算
			if( *Pui_Width < ui_ChWidth )	//最大の幅を格納
				*Pui_Width = ui_ChWidth;
		}

		//パックを最後まで調べたら抜ける。
		if( P_ObjList == (ObjList*)M_ObjLM.GetTailList() )
			break;
	}
}


/*******************************************************************************
	概要	：	子オブジェクトを順に取得
	説明	：	複数のオブジェクトを順に返します。
				最初は引数にNULLを渡し、先頭の子オブジェクトを返します。
				返却した子オブジェクトを引数に渡すと、次の子を返します。
				最後まで達するとNULLが返されます。
	Include	：	Pack.h
	引数	：	Object* P_PrevChild
	戻り値	：	Object*
*******************************************************************************/
Object*	Pack::GetChild( Object* P_PrevChild )
{
	ObjList*	P_ObjList;

	//子無し
	if( ( P_ObjList = (ObjList*)M_ObjLM.GetHeadList() ) == NULL )
		return NULL;

	//NULLなら先頭を返す
	if( P_PrevChild == NULL )
		return P_ObjList->MP_Child;

	//引数で指定の子オブジェクトを探す
	for( ; ; P_ObjList = (ObjList*)P_ObjList->P_Next )
	{
		//指定の子オブジェクトを発見したら次の子を返却して終了
		if( P_ObjList->MP_Child == P_PrevChild )
			return ( (ObjList*)P_ObjList->P_Next )->MP_Child;

		//すべて検索したら終了
		if( P_ObjList == (ObjList*)M_ObjLM.GetTailList() )
			return NULL;		//オブジェクトリスト終端のお知らせ。
	}
}


/*******************************************************************************
	概要	：	子領域取得
	説明	：	引数で指定した子オブジェクトの領域を返します。
	Include	：	Pack.h
	引数	：	Object* P_Child		返却する領域の子オブジェクト
	戻り値	：	Rect		子領域
*******************************************************************************/
Rect	Pack::GetChildArea( Object* P_Child )
{
	ObjList*	P_ObjList;
	Rect		ChArea;

	ChArea.Init();			//子領域初期化

	//子無し
	if( ( P_ObjList = (ObjList*)M_ObjLM.GetHeadList() ) == NULL )
		return ChArea;

	Partitioning();			//子オブジェクト領域分配

	//引数で指定の子オブジェクトを探す
	for( ; ; P_ObjList = (ObjList*)P_ObjList->P_Next )
	{
		//指定の子オブジェクトを発見したら領域を返して終了
		if( P_ObjList->MP_Child == P_Child )
			return P_ObjList->M_ChildArea;

		//すべて検索したら終了
		if( P_ObjList == (ObjList*)M_ObjLM.GetTailList() )
			return ChArea;		//対象無し。
	}
}


/*******************************************************************************
	概要	：	先頭の子オブジェクトリスト取得
	説明	：
	Include	：	Window.h
	引数	：	void
	戻り値	：	ObjList*		最初の子オブジェクトリスト
*******************************************************************************/
ObjList*	Pack::GetHeadChildList( void )
{
	return (ObjList*)M_ObjLM.GetHeadList();
}


/*******************************************************************************
	概要	：	末尾の子オブジェクトリスト取得
	説明	：
	Include	：	Window.h
	引数	：	void
	戻り値	：	ObjList*		最後の子オブジェクトリスト
*******************************************************************************/
ObjList*	Pack::GetTailChildList( void )
{
	return (ObjList*)M_ObjLM.GetTailList();
}


/*******************************************************************************
	概要	：	オブジェクト描画(全ての子オブジェクト含む)
	説明	：
	Include	：	Pack.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	Pack::DrawingAll( void )
{
	ObjList*	P_ObjList;

	//子無し
	if( ( P_ObjList = (ObjList*)M_ObjLM.GetHeadList() ) == NULL )
		return;

	//子オブジェクト全て描画
	for( ; ; P_ObjList = (ObjList*)P_ObjList->P_Next )
	{
		P_ObjList->MP_Child->DrawingAll();		//子を描画(再帰)

		//すべて描画したら終了
		if( P_ObjList == (ObjList*)M_ObjLM.GetTailList() )
			break;
	}
}


/*******************************************************************************
	概要	：	子オブジェクト領域分配
	説明	：	複数の子オブジェクトに対する領域を割り当てます。(子領域を設定する)
				割り当てる領域は、拡張フラグ、子オブジェクトサイズ(EXPAND_UNIFORMITY以外)を元に算出されます。
				拡張フラグがCONTRACT_UNIFORMITY以外のときは、全ての子オブジェクトから相対的に割り出されます。
	Include	：	Pack.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	Pack::Partitioning( void )
{
	ObjList*	P_ObjList;					//オブジェクト・リスト・ポインタ
	ui			ui_NumChild;				//子オブジェクト数
	ui			ui_ChCount;					//子オブジェクト数のカウンタ
	ui			ui_Base;					//ベース。子の位置の基準にする値
	Rect		ChArea;						//子領域
	ui			Aui_ChMinWidth[NUM_LIST];	//子の最小幅
	ui			Aui_ChMinHeight[NUM_LIST];	//子の最小高さ
	ui			ui_SumWidth = 0;			//全ての子の合計幅
	ui			ui_SumHeight = 0;			//全ての子の合計高さ

	//エラー処理
	if( ( ui_NumChild = M_ObjLM.GetNumList() ) == 0 )		//子無し
		return;

	UpdateArea();		//オブジェクト領域更新

	//子領域を設定
	ChArea = Object::M_Area;
	ChArea.Contract( M_Padding.u1_Top, M_Padding.u1_Left, M_Padding.u1_Right, M_Padding.u1_Bottom );


	//合計サイズを計算
	for( 	ui_ChCount = 0, P_ObjList = (ObjList*)M_ObjLM.GetHeadList();
			ui_ChCount < ui_NumChild;
			ui_ChCount++, P_ObjList = (ObjList*)P_ObjList->P_Next )
	{
		P_ObjList->MP_Child->GetMinSize( Aui_ChMinWidth + ui_ChCount, Aui_ChMinHeight + ui_ChCount );	//子の最小サイズ取得
		ui_SumWidth		+= Aui_ChMinWidth[ui_ChCount];			//合計幅に加算
		ui_SumHeight	+= Aui_ChMinHeight[ui_ChCount];			//合計高さに加算
	}

	//水平方向
	if( M_Flags.CheckFlags( HORIZONTAL ) )
	{
		ui		ui_ChAreaWidth;			//子領域の幅(キャッシュ)

		ui_ChAreaWidth	= ChArea.GetWidth();
		ui_Base			= ChArea.Msi_Left;

		//バランスのとれたサイズ(子オブジェクトのサイズから相対的に割り当てる)
		//サイズは( chsize * charea / numchsize )で算出。
		if( M_Flags.CheckFlags( BALANCED ) )
		{
			//全ての子領域を設定
			for( 	ui_ChCount = 0, P_ObjList = (ObjList*)M_ObjLM.GetHeadList();
					ui_ChCount < ui_NumChild;
					ui_ChCount++, P_ObjList = (ObjList*)P_ObjList->P_Next )
			{
				//水平なので、TopとBottomは親の子領域に固定
				P_ObjList->M_ChildArea.Msi_Top		= ChArea.Msi_Top;
				P_ObjList->M_ChildArea.Msi_Bottom	= ChArea.Msi_Bottom;

				P_ObjList->M_ChildArea.Msi_Left		= ui_Base;
				ui_Base += ( ui_SumWidth != 0 )? Aui_ChMinWidth[ui_ChCount] * ui_ChAreaWidth / ui_SumWidth: 0;	//0除算回避
//				ui ui_Width	= ( ui_SumWidth != 0 )? Aui_ChMinWidth[ui_ChCount] * ui_ChAreaWidth / ui_SumWidth: 0;	//0除算回避
//				ui_Base		+= ( Aui_ChMinWidth[ui_ChCount] > ui_Width )? Aui_ChMinWidth[ui_ChCount]: ui_Width;		//最小サイズ以下にしない。
				P_ObjList->M_ChildArea.Msi_Right	= ui_Base - 1;
			}
			return;
		}

		//均一のサイズ
		//サイズは( charea / numch )で算出。(サイズは全て同じ)
		if( M_Flags.CheckFlags( UNIFORMITY ) )
		{
			ui_ChAreaWidth /= ui_NumChild;		//子領域全体の幅を個別の幅に変更。
			//全ての子領域を設定
			for( 	ui_ChCount = 0, P_ObjList = (ObjList*)M_ObjLM.GetHeadList();
					ui_ChCount < ui_NumChild;
					ui_ChCount++, P_ObjList = (ObjList*)P_ObjList->P_Next )
			{
				//水平なので、TopとBottomは親の子領域に固定
				P_ObjList->M_ChildArea.Msi_Top		= ChArea.Msi_Top;
				P_ObjList->M_ChildArea.Msi_Bottom	= ChArea.Msi_Bottom;

				P_ObjList->M_ChildArea.Msi_Left		= ui_Base;
				ui_Base								+= ui_ChAreaWidth;
				P_ObjList->M_ChildArea.Msi_Right	= ui_Base - 1;
			}
			return;
		}
	}


	//垂直方向
	if( M_Flags.CheckFlags( VERTICAL ) )
	{
		ui		ui_ChAreaHeight;		//子領域の高さ(キャッシュ)

		ui_ChAreaHeight	= ChArea.GetHeight();		//高さ取得
		ui_Base			= ChArea.Msi_Top;

		//バランスのとれたサイズ(子オブジェクトのサイズから相対的に割り当てる)
		//サイズは( chsize * charea / numchsize )で算出。
		if( M_Flags.CheckFlags( BALANCED ) )
		{
			//全ての子領域を設定
			for( 	ui_ChCount = 0, P_ObjList = (ObjList*)M_ObjLM.GetHeadList();
					ui_ChCount < ui_NumChild;
					ui_ChCount++, P_ObjList = (ObjList*)P_ObjList->P_Next )
			{
				//垂直なので、LeftとRightは親の子領域に固定
				P_ObjList->M_ChildArea.Msi_Left		= ChArea.Msi_Left;
				P_ObjList->M_ChildArea.Msi_Right	= ChArea.Msi_Right;

				P_ObjList->M_ChildArea.Msi_Top		= ui_Base;
				ui_Base += ( ui_SumHeight != 0 )? Aui_ChMinHeight[ui_ChCount] * ui_ChAreaHeight / ui_SumHeight: 0;
				P_ObjList->M_ChildArea.Msi_Bottom	= ui_Base - 1;
			}
			return;
		}

		//均一のサイズ
		//サイズは( charea / numch )で算出。(サイズは全て同じ)
		if( M_Flags.CheckFlags( UNIFORMITY ) )
		{
			ui_ChAreaHeight /= ui_NumChild;		//子領域全体の高さを個別の高さに変更。
			//全ての子領域を設定
			for( 	ui_ChCount = 0, P_ObjList = (ObjList*)M_ObjLM.GetHeadList();
					ui_ChCount < ui_NumChild;
					ui_ChCount++, P_ObjList = (ObjList*)P_ObjList->P_Next )
			{
				//垂直なので、LeftとRightは親の子領域に固定
				P_ObjList->M_ChildArea.Msi_Left		= ChArea.Msi_Left;
				P_ObjList->M_ChildArea.Msi_Right	= ChArea.Msi_Right;

				P_ObjList->M_ChildArea.Msi_Top		= ui_Base;
				ui_Base								+= ui_ChAreaHeight;
				P_ObjList->M_ChildArea.Msi_Bottom	= ui_Base - 1;
			}
			return;
		}
	}
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

