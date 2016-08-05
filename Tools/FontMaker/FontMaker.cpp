/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	Tools/FontMaker/FontMaker.cpp
	概要		：	フォントファイル作成プログラム
	詳細		：	シフトJIS用のビットマップフォントを生成します。
					図形として構成される文字を2進数に変換します。
					サイズに関係なく一文字16*16で、short型16個で一文字を構成します。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/


/*******************************************************************************
	■フォントソースファイル仕様
	コメントは"//"を行コメント、"/*"をブロックコメントとして扱います。
	フォントのサイズは、一番初めの文字の行数で判断します。
	.が0に、@が1に変換されます。
	例：
	......@.........
	......@.........
	......@..@@.....
	...@@@@@@.......
	......@.........
	......@..@......
	......@@@@@@....
	.....@...@..@...
	....@@...@...@..
	...@.@..@.....@.
	..@..@..@.....@.
	..@...@@......@.
	..@..@.@.....@..
	...@@......@@...
	.........@@.....
	................
*******************************************************************************/


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	<iostream>
#include	<sstream>
#include	<fstream>
#include	<sys/stat.h>
#include	"Main.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	関数
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	SJISコードからSJISの余分な部分を詰めた文字コードへ変換
	説明	：	SJISコードを受け取り、SJISの余分な部分を詰めた文字コードへ変換して返却します。
	Include	：	Main.h
	引数	：	u2 u2_CharCode		SJISコード(R)
					有効値：SJISコード
					無効値：-
	戻り値	：	u2	SJIS詰めコード
					正の整数：正常終了
*******************************************************************************/
u2		SJIS2ChinkSJIS( u2 u2_CharCode )
{
	u1		u1_CharCodeH;
	u1		u1_CharCodeL;

	u1_CharCodeH = u2_CharCode >> 8;
	u1_CharCodeL = (u1)u2_CharCode;

	//2バイト文字
	if( u1_CharCodeH )
	{
		u1_CharCodeH &= 0x3f;
		u1_CharCodeL -= 0x40;
	}

	//1バイト文字
	else
	{
		u1_CharCodeL -= ( u1_CharCodeL & 0x80 ) * 0x20;		//u1_CharCodeLが、0x80以上なら0x20引く。
	}

	u2_CharCode = ( u1_CharCodeH << 8 ) + u1_CharCodeL;

	return u2_CharCode;
}


/*******************************************************************************
	概要	：	JISコードからSJISコードへ変換
	説明	：	JISコードを受け取り、SJISコードへ変換して返却します。
	Include	：	Main.h
	引数	：	u2 u2_CharCode		JISコード(R)
					有効値：JISコード
					無効値：-
	戻り値	：	u2	SJISコード
					正の整数：正常終了
*******************************************************************************/
u2		JIS2SJIS( u2 u2_CharCode )
{
	u1		u1_CharCodeH;
	u1		u1_CharCodeL;

	u1_CharCodeH = u2_CharCode >> 8;
	u1_CharCodeL = (u1)u2_CharCode;

	u1_CharCodeL = u1_CharCodeL + 0x1f + ( ( u1_CharCodeH + 1 ) & 1 ) * 0x5e;		//奇数なら0x1f,偶数なら0x1f + 0x5e
	u1_CharCodeH = ( ( u1_CharCodeH + 1 ) >> 1 ) + 0x70;

	if( u1_CharCodeH >= 0xa0 )	//真中で分断されているため。
		u1_CharCodeH = u1_CharCodeH + 0x40;
	if( u1_CharCodeL >= 0x7f )	//なぜか7fの列だけ空いているから。
		u1_CharCodeL = u1_CharCodeL + 1;

	u2_CharCode = ( u1_CharCodeH << 8 ) + u1_CharCodeL;

	return u2_CharCode;
}


/*******************************************************************************
	概要	：	10進数値以外の文字削除
	説明	：	文字列から10進数以外の文字を削除します。
	Include	：	Main.h
	引数	：	std::string s_String		10進数を含む文字列(R)
					有効値：10進数を含む文字列
					無効値：-
	戻り値	：	std::string	10進数文字列
					10進数文字列：正常終了
*******************************************************************************/
std::string	CleanStringDec( std::string s_String )
{
	u4		u4_Count = 0;

	while( s_String.length() > u4_Count )
	{
		//10進数文字なら次へ。
		if( '0' <= s_String[u4_Count] && s_String[u4_Count] <= '9' )
			u4_Count++;

		//10進数文字でなければ削除。
		else
			s_String.erase( u4_Count, 1 );
	}
	return s_String;
}


/*******************************************************************************
	概要	：	16進数値以外の文字削除
	説明	：	文字列から16進数以外の文字を削除します。
	Include	：	Main.h
	引数	：	std::string s_String		16進数を含む文字列(R)
					有効値：16進数を含む文字列
					無効値：-
	戻り値	：	std::string	16進数文字列
					16進数文字列：正常終了
*******************************************************************************/
std::string	CleanStringHex( std::string s_String )
{
	u4		u4_Count = 0;

	while( s_String.length() > u4_Count )
	{
		//16進数文字なら次へ。
		if(
				'0' <= s_String[u4_Count] && s_String[u4_Count] <= '9'
			 || 'A' <= s_String[u4_Count] && s_String[u4_Count] <= 'F'
			 || 'a' <= s_String[u4_Count] && s_String[u4_Count] <= 'f'
		)
			u4_Count++;

		//16進数文字でなければ削除。
		else
			s_String.erase( u4_Count, 1 );
	}
	return s_String;
}


/*******************************************************************************
	概要	：	文字列変換関数 - 10進数
	説明	：	10進数の文字列を数値に変換します。
	Include	：	Main.h
	引数	：	std::string s_StrDex		10進数の文字列(R)
					有効値：10進数の文字列
					無効値：0から9以外の文字が含まれる文字列
	戻り値	：	s4	正の整数値
					0:エラー
					正の整数：正常終了
*******************************************************************************/
u4		Str2Dec( std::string s_StrDec )
{
	u4		u4_Counter = 0;
	u4		u4_Numeric = 0;

	s_StrDec = CleanStringDec( s_StrDec );		//10進数の文字以外削除

	while( s_StrDec.length() > u4_Counter )
	{
		u4_Numeric <<= 4;

		//Asciiコードから数値に変換
		if( '0' <= s_StrDec[u4_Counter] && s_StrDec[u4_Counter] <= '9' )
			u4_Numeric += s_StrDec[u4_Counter] - '0';

		//10進数の文字以外があれば、エラー。
		else
			return 0;

		u4_Counter++;
	}
	return u4_Numeric;
}


/*******************************************************************************
	概要	：	文字列変換関数 - 16進数
	説明	：	16進数の文字列を数値に変換します。
	Include	：	Main.h
	引数	：	std::string s_StrHex		16進数の文字列(R)
					有効値：16進数の文字列
					無効値：0からf以外の文字が含まれる文字列
	戻り値	：	u4	正の整数値
					0:エラー
					正の整数：正常終了
*******************************************************************************/
u4		Str2Hex( std::string s_StrHex )
{
	u4		u4_Counter = 0;
	u4		u4_Numeric = 0;

	s_StrHex = CleanStringHex( s_StrHex );		//16進数の文字以外削除

	while( s_StrHex.length() > u4_Counter )
	{
		u4_Numeric <<= 4;

		//Asciiコードから数値に変換
		if( '0' <= s_StrHex[u4_Counter] && s_StrHex[u4_Counter] <= '9' )
			u4_Numeric += s_StrHex[u4_Counter] - '0';
		else if( 'A' <= s_StrHex[u4_Counter] && s_StrHex[u4_Counter] <= 'F' )
			u4_Numeric += s_StrHex[u4_Counter] - 'A' + 10;
		else if( 'a' <= s_StrHex[u4_Counter] && s_StrHex[u4_Counter] <= 'f' )
			u4_Numeric += s_StrHex[u4_Counter] - 'a' + 10;

		//16進数の文字以外があれば、エラー。
		else
			return 0;

		u4_Counter++;
	}
	return u4_Numeric;
}


/*******************************************************************************
	概要	：	ファイル名を入力する関数です。
	説明	：	フォントのソースファイル名と、出力するファイル名を入力します。
				拡張子を含めたファイル名を入力します。
				ソースファイルが無い場合は、再入力します。
				出力ファイルが存在する場合は、上書き確認をします。
				上書きしない場合は、再入力します。
				ソースファイル、出力ファイルともに未入力の場合は、キャンセルとみなします。
	Include	：	Main.h
	引数	：	std::ifstream* P_InputFile	ソースファイルストリームのポインタ(R/W)
					有効値：ファイルストリームのポインタ
					無効値：NULLポインタ
				std::ofstream* P_OutputFile	出力ファイルストリームのポインタ(R/W)
					有効値：ファイルオブジェクトのポインタ
					無効値：NULLポインタ
	戻り値	：	s4	エラー情報
					1:正常終了
					-1:ソースファイルオープンエラー
					-2:ソースファイル入力キャンセル
					-3:出力ファイルオープンエラー
					-4:出力ファイル入力キャンセル
*******************************************************************************/
s4		InputFileName( std::ifstream* P_InputFile, std::ofstream* P_OutputFile )
{
	std::string		s_InputFileName, s_OutputFileName;
	std::string		s_OverWrite;
	struct stat		st;
	char	c[1];
	c[0] = 0;
	c[1] = 0;


	//ソースファイル名入力
	while( true )
	{
		std::cout << "フォントのソースファイル名：" << std::endl;
		std::cin >> s_InputFileName;

		//入力キャンセル
		if( s_InputFileName == "cancel" )
		{
			std::cout << "入力キャンセル\n" << std::endl;
			return -2;
		}

		//ファイルが存在しない場合、再入力。
		if( stat( s_InputFileName.c_str(), &st ) )
			std::cout << "ソースファイル：" << s_InputFileName << "がみつかりません。。" << std::endl;

		//ファイルが存在する場合は抜ける。
		else
			break;
	}

	//ソースファイルオープン
	P_InputFile->open( s_InputFileName.c_str() );

	//ファイルオープン失敗
	if( !P_InputFile->is_open() )
	{
		std::cout << "ソースファイル：" << s_InputFileName << "が開けませんでした。" << std::endl;
		return -1;
	}

	//-------------------------------------------------------------------------------------------------------

	//出力ファイル名入力
	while( true )
	{
		std::cout << "出力ファイル名：" << std::endl;
		std::cin >> s_OutputFileName;

		//入力キャンセル
		if( s_OutputFileName == "cancel" )
		{
			std::cout << "入力キャンセル\n" << std::endl;
			return -4;
		}

		//ファイルが既に存在する場合上書き確認。
		if( !stat( s_OutputFileName.c_str(), &st ) )
		{
			std::cout << "出力ファイル：" << s_OutputFileName << "がすでに存在しています。" << std::endl;
			std::cout << "上書きしますか？(y,n)" << std::endl;
			std::cin >> s_OverWrite;

			//上書きしない
			if( s_OverWrite == "n" )
				continue;

			//上書きする
			else if( s_OverWrite == "y" )
			{
				std::cout << s_OutputFileName << "に上書きします。" << std::endl;
				break;
			}
		}

		//ファイルが存在しない場合は抜ける。
		else
			break;
	}

	//出力ファイルオープン
	P_OutputFile->open( s_OutputFileName.c_str(), std::ios::out | std::ios::trunc | std::ios::binary );

	//ファイルオープン失敗
	if( !P_OutputFile->is_open() )
	{
		std::cout << "出力ファイル：" << s_OutputFileName << "が開けませんでした。" << std::endl;
		return -3;
	}


	P_OutputFile->seekp( 0x30 * 0x100 * 32 - 1, std::ios::beg );
	P_OutputFile->write( c, 1 );

	return 1;		//正常終了
}


/*******************************************************************************
	概要	：	文字コード読み込み
	説明	：	ファイルストリームの現在の位置から、文字コードを読み込みます。
	Include	：	Main.h
	引数	：	std::ifstream* P_InputFile	ソースファイルストリームのポインタ(R)
					有効値：ファイルストリームのポインタ
					無効値：NULLポインタ
				u2* Pu2_CharCode	文字コード変数のポインタ(W)
					有効値：文字コード変数のポインタ
					無効値：NULLポインタ
	戻り値	：	s4	エラー情報
					0:ファイルの終端にたどり着いた
					1:正常読み込み
*******************************************************************************/
s4		ReadCharCode( std::ifstream* P_InputFile, u2* Pu2_CharCode )
{
	//変数宣言
	//----------------------------------------------------------------
	u2		u2_CharCode;		//キャラコード格納用
	u1		u1_CharCodeH;		//キャラコード格納用(上位)
	u1		u1_CharCodeL;		//キャラコード格納用(下位)
	std::string	s_StringWork;
	//----------------------------------------------------------------

	while( true )
	{
		//一行読み込み
		getline( *P_InputFile, s_StringWork );

		//終端なら終了
		if( P_InputFile->eof() )
			return 0;

		//STARTCHARがあれば抜ける。
		if( s_StringWork.find( "STARTCHAR" ) != -1 )
			break;
	}

	//STARTCHARがあれば、文字コードを読み込む。
	u2_CharCode = u1_CharCodeH = u1_CharCodeL = 0;
	s_StringWork.erase(0,9);

	//数値の部分まで文字列を削除
	while( true )
	{
		if(
				'0' <= s_StringWork[0] && s_StringWork[0] <= '9'
			 || 'A' <= s_StringWork[0] && s_StringWork[0] <= 'F'
			 || 'a' <= s_StringWork[0] && s_StringWork[0] <= 'f'
		)
			break;
		else
			s_StringWork.erase(0,1);
	}

	//区点コード
	if( s_StringWork.length() >= 6 )
	{
		s_StringWork.erase( 0, 3 );	//面コードと、三文字目のハイフン消す
		s_StringWork.erase( 4, 1 );	//六文字目のハイフン消す

		//文字列を数値に
		u2_CharCode = Str2Dec( s_StringWork );
		std::cout << "JISコード：" << u2_CharCode << std::endl;
		u2_CharCode = JIS2SJIS( u2_CharCode );
	}

	//Ascii,JISコード
	else
	{
		//文字列を数値に
		u2_CharCode = Str2Hex( s_StringWork );

		//JISコードなら、SJISに変換する。
		if( u2_CharCode & 0xff00 )
		{
			std::cout << "JISコード：" << u2_CharCode << std::endl;
			u2_CharCode = JIS2SJIS( u2_CharCode );
		}
		else
			std::cout << "Asciiコード：" << u2_CharCode << std::endl;
	}

//	std::cout << "SJISコード：" << std::hex << std::showbase << u2_CharCode << std::endl;
	u2_CharCode = SJIS2ChinkSJIS( u2_CharCode );
//	std::cout << "SJIS詰めコード：" << std::hex << std::showbase << u2_CharCode << std::endl;
	*Pu2_CharCode = u2_CharCode;

	return 1;
}


/*******************************************************************************
	概要	：	文字ビットマップ読み込み
	説明	：	ファイルストリームの現在の位置から、文字ビットマップを読み込みます。
				読み込んだビットマップデータは16進数に変換され、バッファに格納します。
	Include	：	Main.h
	引数	：	std::ifstream* P_InputFile	ソースファイルストリームのポインタ(R)
					有効値：ファイルストリームのポインタ
					無効値：NULLポインタ
				u2 u2_CharCode	文字コード(R)
					有効値：文字コード
					無効値：-
				u2* Pu2_CharBitmapBuffer	文字ビットマップ変数のポインタ(W)
					有効値：文字ビットマップ変数のポインタ
					無効値：NULLポインタ
	戻り値	：	s4	エラー情報
					1:正常読み込み
					0:ファイルの終端
*******************************************************************************/
s4		ReadCharBitmap( std::ifstream* P_InputFile, u2 u2_CharCode, u2* Pu2_CharBitmapBuffer )
{
	u4		u4_LineCount = 0;
	u4		u4_SearchCount = 0;
	u4		u4_HitCount = 0;
	std::string	s_String;

	//.か@が3つ以上あるか検索。
	while( true )
	{
		//一行読み込み
		getline( *P_InputFile, s_String );

		//終端なら終了
		if( P_InputFile->eof() )
			return 0;

		//.か@が3つあれば抜ける。
		for( u4_SearchCount = 0; s_String.length() >= u4_SearchCount; u4_SearchCount++ )
		{
			//.か@のときはヒットカウントを加算
			if( s_String[u4_SearchCount] == '.' || s_String[u4_SearchCount] == '@' )
			{
				u4_HitCount++;

				//3つ以上あればOK
				if( 3 <= u4_HitCount )
					goto SearchEnd;
			}
			//.でも@でもない場合は、ヒットカウントを0に。
			else
				u4_HitCount = 0;
		}
	}

	SearchEnd:

	//最大16行読み込む
	for( u4_LineCount = 0; u4_LineCount < 16; u4_LineCount++ )
	{
		//.と@以外の文字を削除
		for( u4_SearchCount = 0; s_String.length() > u4_SearchCount; u4_SearchCount++ )
		{
			//.か@以外の文字なら削除。
			if( '.' != s_String[u4_SearchCount] && '@' != s_String[u4_SearchCount] )
				s_String.erase( u4_SearchCount, 1 );
		}

		//2バイト文字なら16ビットにリサイズ
		if( u2_CharCode & 0xff00 )
		{
			while( s_String.length() < 16 )
			{
				if( s_String.length() & 1 )
					s_String = '.' + s_String;
				else
					s_String = s_String + '.';
			}
		}
		//1バイト文字なら8ビットにリサイズ
		else
		{
			while( s_String.length() < 8 )
			{
				if( s_String.length() & 1 )
					s_String = '.' + s_String;
				else
					s_String = s_String + '.';
			}
			s_String = s_String + "........";
		}

		//バッファにデータを格納。
		Pu2_CharBitmapBuffer[u4_LineCount] = 0;
		for( u4_SearchCount = 0; u4_SearchCount < 16; u4_SearchCount++ )
		{
			//@なら、指定の位置に1を設定。
			if( '@' == s_String[u4_SearchCount] )
				Pu2_CharBitmapBuffer[u4_LineCount] |= 1 << ( 15 - u4_SearchCount );
		}

		//次のラインを読み込む
		getline( *P_InputFile, s_String );

		//終端なら終了
		if( P_InputFile->eof() )
			return 0;
	}
	return 1;
}


/*******************************************************************************
	概要	：	ファイル書き出し
	説明	：	文字コードから算出した位置にフォントデータを書き込みます。
	Include	：	Main.h
	引数	：	std::ofstream* P_OutputFile	出力ファイルストリームのポインタ(R)
					有効値：ファイルストリームのポインタ
					無効値：NULLポインタ
				u2 u2_CharCode	文字コード(R)
					有効値：文字コード
					無効値：-
				u2* Pu2_CharBitmapBuffer	文字ビットマップ変数のポインタ(R)
					有効値：文字ビットマップ変数のポインタ
					無効値：NULLポインタ
	戻り値	：	void
*******************************************************************************/
void	WriteFont( std::ofstream* P_OutputFile, u2 u2_CharCode, u2* Pu2_CharBitmapBuffer )
{
	u4		u4_LineCount;
	char	c[3];
	c[2] = 0;

	P_OutputFile->seekp( u2_CharCode << 5, std::ios::beg );

	//16行書き出し
	for( u4_LineCount = 0; u4_LineCount < 16; u4_LineCount++ )
	{
		c[0] = (char)( Pu2_CharBitmapBuffer[u4_LineCount] );
		c[1] = (char)( Pu2_CharBitmapBuffer[u4_LineCount] >> 8 );
		P_OutputFile->write( c, 2 );
	}
}


/*******************************************************************************
	概要	：	フォント作成関数
	説明	：	ソースファイルからフォントファイルを作成します。
	Include	：	Main.h
	引数	：	std::ifstream* P_InputFile	ソースファイルストリームのポインタ(R)
					有効値：ファイルストリームのポインタ
					無効値：NULLポインタ
				std::ofstream* P_OutputFile	出力ファイルストリームのポインタ(W)
					有効値：ファイルストリームのポインタ
					無効値：NULLポインタ
	戻り値	：	s4	エラー情報
					1:正常終了
					-1:文字コード読み取り中にファイル終端に達した。
					-2:文字ビットマップ読み取り中にファイル終端に達した。
*******************************************************************************/
s4		MakeFont( std::ifstream* P_InputFile, std::ofstream* P_OutputFile )
{
	//変数宣言
	//----------------------------------------------------------------
	u2		u2_CharCode;		//キャラコード格納用
	u2		u2_CharBitmapBuffer[16];	//文字ビットマップのバッファ
	//----------------------------------------------------------------

	//ファイル終端まで実行。
	while( !P_InputFile->eof() )
	{
		//文字コード読み込み
		if( !ReadCharCode( P_InputFile, &u2_CharCode ) )
			return -1;			//ファイルの終端まで実行したら終了。

		//文字ビットマップの読み込み
		if( !ReadCharBitmap( P_InputFile, u2_CharCode, u2_CharBitmapBuffer ) )
			return -2;			//ファイルの終端まで実行したら終了。

		//ファイル書き出し
		WriteFont( P_OutputFile, u2_CharCode, u2_CharBitmapBuffer );
	}

	return 1;
}


/*******************************************************************************
	概要	：	メイン関数です。
	説明	：	メイン関数です。言わずもがな。
	Include	：	Main.h
	引数	：	void
	戻り値	：	int		エラー情報
					0：正常終了
*******************************************************************************/
int		main( void )
{
	std::ifstream	InputFile;			//入力ファイルストリーム
	std::ofstream	OutputFile;			//出力ファイルストリーム

	std::cout << "\n\n■ビットマップフォントメーカー■\n" << std::endl;

	//ファイル名入力
	if( 0 > InputFileName( &InputFile, &OutputFile ) )
		return 0;

	MakeFont( &InputFile, &OutputFile );

	return 0;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*


61 69 75 65 6f 82 a0 82 a2 82 a4 82 a6 82 a8

//1バイト文字(制御コード、ASCIIコード)
if( *P_charcode < 0x80 )
{
	MonaFont16[0][*P_charcode];
	charcode++;
}

//2バイト文字
else if( *P_charcode < 0xa0 || 0xe0 <= *P_charcode )
{
	MonaFont16[*P_charcode & 0x3f][*( P_charcode + 1 ) - 0x40];
	charcode += 2;
}

//1バイト文字(半角カナ他)
else
{
	MonaFont16[*P_charcode & 0x3f][*( P_charcode + 1 )];
	charcode += 2;
}

*/
