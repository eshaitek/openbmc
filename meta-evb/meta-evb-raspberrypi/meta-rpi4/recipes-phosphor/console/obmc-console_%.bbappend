FILESEXTRAPATHS_prepend_raspberrypi4 := "${THISDIR}/${PN}:"

SRC_URI_append_raspberrypi4 = " file://90-rpi4-sol.rules"

do_install_append_raspberrypi4() {

        install -m 0755 -d ${D}${sysconfdir}/${BPN}
        rm -f ${D}${sysconfdir}/${BPN}/server.ttyVUART0.conf
        install -m 0644 ${WORKDIR}/${BPN}.conf ${D}${sysconfdir}/
        ln -sr ${D}${sysconfdir}/${BPN}.conf ${D}${sysconfdir}/${BPN}/server.ttyUSB0.conf

        install -d ${D}/lib/udev/rules.d
        rm -f ${D}/lib/udev/rules.d/80-obmc-console-uart.rules
        install -m 0644 ${WORKDIR}/90-rpi4-sol.rules ${D}/lib/udev/rules.d
}
