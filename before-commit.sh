#!/usr/bin/env bash
echo Styling...
astyle --style=allman --indent=spaces=4 --indent-namespaces --keep-one-line-blocks --indent-preproc-block --indent-preproc-define --mode=c --prefix=none -R *.cpp *.h

echo done.
