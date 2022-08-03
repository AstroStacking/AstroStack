#!/usr/bin/env python

import fileinput
import sys

def update_cmakelist(new_version):
  for line in fileinput.input(files=("CMakeLists.txt",), inplace=True):
    if line.startswith("set(ASTROSTACK_VERSION"):
      sys.stdout.write(f"set(ASTROSTACK_VERSION {new_version})\n")
    else:
      sys.stdout.write(line)

def update_doxygen(new_version):
  for line in fileinput.input(files=("Doxyfile",), inplace=True):
    if line.startswith("PROJECT_NUMBER"):
      sys.stdout.write(f"PROJECT_NUMBER         = {new_version}\n")
    else:
      sys.stdout.write(line)

def update_changelog(new_version):
  for line in fileinput.input(files=("README.md",), inplace=True):
    sys.stdout.write(line)
    if line.startswith("## Changelog"):
      sys.stdout.write(f"### {new_version}\n\n")

def update_wininstaller(new_version):
  for line in fileinput.input(scriptpath + "/installer/AstroStack.iss", inplace=1):
    if "AppVersion" in line:
      line="AppVersion=" + FullVersionStr + "\n"
    sys.stdout.write(line)

def update_standalone(new_version, filename):
  for line in fileinput.input(files=(filename,), inplace=True):
    if "QCoreApplication::setApplicationVersion" in line:
      sys.stdout.write(f'QCoreApplication::setApplicationVersion("{new_version}");\n')
    else:
      sys.stdout.write(line)

if __name__ == "__main__":
  import glob

  update_cmakelist(sys.argv[1])
  #update_doxygen(sys.argv[1])
  update_changelog(sys.argv[1])
  #update_wininstaller(sys.argv[1])

  for filename in glob.glob("*/*/Standalone/*.cpp"):
    print(filename)
    update_standalone(sys.argv[1], filename)
