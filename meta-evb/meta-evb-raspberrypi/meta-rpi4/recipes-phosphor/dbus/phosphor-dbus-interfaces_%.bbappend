FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI_append = " file://0028-MCTP-Daemon-D-Bus-interface-definition.patch"
