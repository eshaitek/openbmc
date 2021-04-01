FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI := "git://github.com/eshaitek/libmctp.git"
SRCREV := "37e834b9cd04c57eee2f4e396fb0a13dd97286c1"

TARGET_CFLAGS += "-DMCTP_HAVE_FILEIO"


