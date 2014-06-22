#!/usr/bin/env sh
# A small script to find a command even with a messed-up PATH.
# Yes, this is appalling; it is intended for brining in tools
# from a host's toolchain when PATH is set for a different target.
# (And why, pray tell, not just source .profile? This is because
# some people include config scripts that change the PATH based
# on the setting of environment variables other than PATH. You,
# alas, probably don't know who you are.)
pie () {
  [-f "${1}" && printf "${1}"
}
printf $(which "${1}"||
         pie "~/bin/${1}"||
         pie "/usr/llvm/bin/${1}"||
         pie "/usr/local/bin/${1}"||
         pie "/opt/local/bin/${1}"||
         pie "/Library/Developer/CommandLineTools/usr/bin/${1}")
