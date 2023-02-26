
pandoc -o doc/AstroStack.pdf README.md */*.md src/*/*/*/*.md -V colorlinks=true \
                                       -V linkcolor=blue \
                                       -V urlcolor=red \
                                       -V toccolor=gray \
                                       --file-scope

