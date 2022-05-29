#! /bin/sh

# build xcode project. Change target to build individual formats
echo "Build translations"
xcodebuild -project AstroStack.xcodeproj -target "AstroStack_lupdate" -configuration Release
echo "Release translations"
xcodebuild -project AstroStack.xcodeproj -target "AstroStack_lrelease" -configuration Release
echo "Release translations"
xcodebuild -project AstroStack.xcodeproj -target "ALL_BUILD" -configuration Release
