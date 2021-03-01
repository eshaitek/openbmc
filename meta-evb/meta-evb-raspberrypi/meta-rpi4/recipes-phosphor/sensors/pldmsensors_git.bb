SUMMARY = "pldmsensors"
DESCRIPTION = "PLDM Sensor Services Configured from D-Bus"

SRC_URI = "git://github.com/eshaitek/pldmsensors"

SRCREV = "33038f1b027d589304f87754fd6fc09df9a272b9"

PV = "0.1+git${SRCPV}"

LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=86d3f3a95c324c9479bd8986968f4327"

SYSTEMD_SERVICE_${PN} = " xyz.openbmc_project.pldmsensor.service"

DEPENDS = "boost nlohmann-json sdbusplus i2c-tools libgpiod libpldm-intel "
inherit cmake systemd

S = "${WORKDIR}/git"

EXTRA_OECMAKE = "-DYOCTO=1"

FILESEXTRAPATHS_prepend_raspberrypi4 := "${THISDIR}/${PN}:"

SRC_URI += "file://pldmsensors.json "

do_install_append_raspberrypi4() {
    install -d ${D}${sysconfdir}/default
    install -m 0644 ${WORKDIR}/pldmsensors.json ${D}${sysconfdir}/default
}
