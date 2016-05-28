#!/usr/bin/env bash
echo Styling...
astyle --quiet --indent=spaces=4 --unpad-paren --pad-header --break-blocks --indent-namespaces --indent-cases --indent-switches --indent-classes --add-one-line-brackets --convert-tabs --style=java --recursive --suffix=none *.cpp *.h

echo done.
