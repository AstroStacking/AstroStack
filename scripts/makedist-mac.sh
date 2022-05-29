#! /bin/sh

#shell script to automate Project build, code-signing and packaging on macos

BASEDIR=$(dirname $0)
cd $BASEDIR

# build xcode project. Change target to build individual formats
echo "Build translations"
xcodebuild -project AstroStack.xcodeproj -target "AstroStack_lupdate" -configuration Release 2> ./build-mac.log
