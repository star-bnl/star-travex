#!/bin/tcsh

if ( $#argv != 1 ) then
  echo ""
  echo " Usage : $0 [FSET number, 109 or something like that]"
  echo ""
  echo ""
  exit
endif

set template = "embed_template_P10ik.xml"

set FSET = "$1"

star-submit-template -template ${template} -entities FSET=$FSET
