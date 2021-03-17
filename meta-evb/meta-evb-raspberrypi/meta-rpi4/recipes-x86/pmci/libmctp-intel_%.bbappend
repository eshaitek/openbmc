FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI := "git://github.com/eshaitek/libmctp.git"
SRCREV := "3d2576fd583229158d0abba4cd38d793b892bd73"

TARGET_CFLAGS += "-DMCTP_HAVE_FILEIO"


