#!/usr/bin/env python

import fileinput
import sys

def update_cmakelist(new_version):
  for line in fileinput.input(files=("CMakeLists.txt",), inplace=True):
    if line.startswith("set(ASTROSTACK_VERSION"):
      sys.stdout.write("set(ASTROSTACK_VERSION %s)\n" % new_version)
    else:
      sys.stdout.write(line)

def update_doxygen(new_version):
  for line in fileinput.input(files=("Doxyfile",), inplace=True):
    if line.startswith("PROJECT_NUMBER"):
      sys.stdout.write("PROJECT_NUMBER         = %s\n" % new_version)
    else:
      sys.stdout.write(line)

def update_changelog(new_version):
  for line in fileinput.input(files=("README.md",), inplace=True):
    sys.stdout.write(line)
    if line.startswith("## Changelog"):
      sys.stdout.write("### %s\n\n" % new_version)

def update_wininstaller(new_version):
  for line in fileinput.input(scriptpath + "/installer/AstroStack.iss", inplace=1):
    if "AppVersion" in line:
      line="AppVersion=" + FullVersionStr + "\n"
    sys.stdout.write(line)

if __name__ == "__main__":
  update_cmakelist(sys.argv[1])
  #update_doxygen(sys.argv[1])
  update_changelog(sys.argv[1])
  update_wininstaller(sys.argv[1])
