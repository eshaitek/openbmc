SUMMARY = "pldmsensors"
DESCRIPTION = "PLDM Sensor Services Configured from D-Bus"

SRC_URI = "git://github.com/eshaitek/pldmsensors"

SRCREV = "00abf30a446213d9d84f62f450df8ab189bbea14"

PV = "0.1+git${SRCPV}"

LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=86d3f3a95c324c9479bd8986968f4327"

SYSTEMD_SERVICE_${PN} = " xyz.openbmc_project.pldmsensor.service"

DEPENDS = "boost nlohmann-json sdbusplus i2c-tools libgpiod libpldm-intel "
inherit cmake systemd

S = "${WORKDIR}/git"

EXTRA_OECMAKE = "-DYOCTO=1"

FILESEXTRAPATHS_prepend_raspberrypi4 := "${THISDIR}/${PN}:"

SRC_URI += "file://pldm.json "

do_install_append_raspberrypi4() {
    install -d ${D}${sysconfdir}/default
    install -m 0644 ${WORKDIR}/pldm.json ${D}${sysconfdir}/default
}
