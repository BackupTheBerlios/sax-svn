#
# Copyright (c) 1996 S.u.S.E. GmbH Fuerth, Germany.  All rights reserved.
#
# Author: Marcus Schäfer <ms@suse.de>, 1999
#
# Makefile for building the system draft for the SaX
# project
#

all:
	#----------------------#
	# remove the old stuff #
	#----------------------#
	rm -f compiled/sax.dvi
	rm -f index/sax.ilg
	rm -f index/sax.ind
	rm -f index/sax.idx
	rm -f *.idx
	rm -f *.toc
	rm -f *.log

	#-----------------------------------#
	# first touch sax.ind, otherwise    # 
	# we can not tex it                 #
	#-----------------------------------#
	touch index/sax.ind

	#--------------------------#
	# tex document one time    #
	#--------------------------#
	latex sax.tex
	latex sax.tex

	#-------------------------------------#
	# call makeindex for index generation # 
	#-------------------------------------#
	mv sax.idx ./index
	( cd ./index && makeindex -g -s sax.ist sax.idx )

	#---------------------------------#
	# tex it again with the new index #
	#---------------------------------#
	latex sax.tex
	latex sax.tex
	
	#--------------------#
	# remove trash stuff #
	#--------------------#
	rm -f *.idx      
	rm -f *.toc
	rm -f *.log
	rm -f *.bmt
	rm -f *.aux
	rm -f *.mtc*
	rm -f index/sax.ilg
	rm -f index/sax.idx

	#-------------------------------------#
	# move dvi file to compiled directory #
	#-------------------------------------#
	mv sax.dvi ./compiled

	#------------------#
	# let me see :-)   #
	#------------------#
	xdvi ./compiled/sax.dvi


clean:
	rm -f compiled/sax.dvi
	rm -f index/sax.ilg
	rm -f index/sax.ind
	rm -f index/sax.idx
	rm -f *.idx
	rm -f *.toc
	rm -f *.log

