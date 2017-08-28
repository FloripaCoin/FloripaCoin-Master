#!/bin/bash
# create multiresolution windows icon
ICON_SRC=../../src/qt/res/icons/floripacoin.png
ICON_DST=../../src/qt/res/icons/floripacoin.ico
convert ${ICON_SRC} -resize 16x16 floripacoin-16.png
convert ${ICON_SRC} -resize 32x32 floripacoin-32.png
convert ${ICON_SRC} -resize 48x48 floripacoin-48.png
convert floripacoin-16.png floripacoin-32.png floripacoin-48.png ${ICON_DST}

