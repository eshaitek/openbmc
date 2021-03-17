FILESEXTRAPATHS_append := ":${THISDIR}/${PN}"

SRC_URI_append = " \
    file://0002-support-pldm-sensors-effecters.patch \
"
