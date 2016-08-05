#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#
#	ファイル名	：	/Include/Mul_light/Makefile
#	概要		：	Mul light APIライブラリの生成規則です。
#	詳細		：
#	責任者		：	佐合 秀昭
#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#



#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#
#	変数定義
#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#
ROOT				= ../../

NAME				= Mul_light

#ライブラリソース
LIB_SOURCE			+= DateTime.cpp
#LIB_SOURCE			+= File.cpp
LIB_SOURCE			+= List.cpp
LIB_SOURCE			+= Flag.cpp
LIB_SOURCE			+= Message.cpp
LIB_SOURCE			+= MsgQ.cpp
LIB_SOURCE			+= Queue.cpp
LIB_SOURCE			+= Stack.cpp
LIB_SOURCE			+= Rect.cpp
LIB_SOURCE			+= SC.cpp
LIB_SOURCE			+= MLStdLib.cpp

LIB_SOURCE			+= DCD.cpp
LIB_SOURCE			+= DD.cpp

#オブジェクトソース
LIB_SOURCE			+=	Object.cpp
LIB_SOURCE			+=		Box.cpp
LIB_SOURCE			+=			Single.cpp
LIB_SOURCE			+=				Caption.cpp
LIB_SOURCE			+=					Window.cpp Window_SendMessage.cpp Window_GetMessage.cpp Window_MsgDefProc.cpp
LIB_SOURCE			+=				Button.cpp
LIB_SOURCE			+=			Pack.cpp
LIB_SOURCE			+=		Content.cpp
LIB_SOURCE			+=			Text.cpp


#ディレクトリ情報
DIRECTORY			+= Device
DIRECTORY			+= Objects



#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#
#	インクルード
#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#
include	$(ROOT)Common.mk
-include $(DEPENDS)



#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#
#	コマンド
#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#
#*******************************************************************************
#	デフォルト動作
#*******************************************************************************
PHONY += default
default:
	@make -s _$@
_default: _library


#*******************************************************************************
#	ライブラリファイル作成
#*******************************************************************************
PHONY += library lib _library _lib
library lib:
	@make -s _$@
_library _lib: $(LIB_NAME)


#*******************************************************************************
#	ライブラリファイル再作成
#*******************************************************************************
PHONY += remake relib relibrary _remake _relib _relibrary
remake relib relibrary:
	@make -s _$@
_remake _relib _relibrary: _delete _library



#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#
#	ファイル生成規則
#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#
#*******************************************************************************
#	ライブラリ作成
#*******************************************************************************
$(LIB_NAME): $(OBJECTS)
	@echo -e "\tar\t$@"
	ar rv $@ $^
	@echo -e "\tranlib\t$@"
	ranlib $@



.PHONY: $(PHONY)
#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#
#	End of file
#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#
