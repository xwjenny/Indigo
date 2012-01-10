#!/bin/sh

if [ -z $1 ]; then
  echo "Please specify bingo version";
  exit;
fi

if [ -z $2 ]; then
  echo "Please specify PostgreSQL version";
  exit;
fi

if [ -z $BINGO_PG_DIR ]; then
echo BINGO_PG_DIR is not specified
exit;
fi

xcodebuild -sdk macosx10.6 -configuration Release10.6 -alltargets
./bingo-release.sh $1 $2 build/Release10.6/bingo_postgres.dylib
