#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#
#	ファイル名	：	/Include/cStdLib/Makefile
#	概要		：	c言語標準ライブラリの生成規則です。
#	詳細		：
#	責任者		：	佐合 秀昭
#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#



#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#
#	変数定義
#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#
ROOT				= ../../

NAME				= cStdLib

#ライブラリソース
LIB_SOURCE			+= abs.c
LIB_SOURCE			+= atof.c
LIB_SOURCE			+= atoi.c
LIB_SOURCE			+= errno.c
LIB_SOURCE			+= frexp.c
LIB_SOURCE			+= ldexp.c
LIB_SOURCE			+= memchr.c
LIB_SOURCE			+= memcmp.c
LIB_SOURCE			+= memcpy.c
LIB_SOURCE			+= memmove.c
LIB_SOURCE			+= memset.c
LIB_SOURCE			+= qsort.c
LIB_SOURCE			+= rand.c
LIB_SOURCE			+= sprintf.c
LIB_SOURCE			+= strcat.c
LIB_SOURCE			+= strcmp.c
LIB_SOURCE			+= strcpy.c
LIB_SOURCE			+= strcspn.c
LIB_SOURCE			+= strdup.c
LIB_SOURCE			+= strlen.c
LIB_SOURCE			+= strncat.c
LIB_SOURCE			+= strncmp.c
LIB_SOURCE			+= strncpy.c
LIB_SOURCE			+= strpbrk.c
LIB_SOURCE			+= strrchr.c
LIB_SOURCE			+= strsep.c
LIB_SOURCE			+= strspn.c
LIB_SOURCE			+= strstr.c
LIB_SOURCE			+= strtol.c
LIB_SOURCE			+= strtoul.c
LIB_SOURCE			+= strtoul0.c
LIB_SOURCE			+= vsprintf.c



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
