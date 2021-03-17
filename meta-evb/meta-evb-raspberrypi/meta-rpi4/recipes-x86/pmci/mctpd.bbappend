FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SMBUS_BINDING = "smbus"

SRC_URI = "git://github.com/eshaitek/pmci.git;protocol=ssh"

SRCREV = "ada691a313b6abdfde72a700cebe76bb4ee960d1"

SRC_URI_append = " file://mctp_config.json"
#SRC_URI_append = " file://mctpd-pi.patch"

do_install_append() {
    install -m 0644 -D ${WORKDIR}/mctp_config.json \
        ${D}${datadir}/mctp/mctp_config.json
}
