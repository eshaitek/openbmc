1. export TEMPLATECONF=meta-evb/meta-evb-raspberrypi/meta-rpi4/conf/
2. . oe-init-build-env rpi4-build
3. build the obmc image
4. Flashing the image into a SD card: e.g.: sudo dd if=$obmc_build_dir/tmp/deploy/images/raspberrypi4/obmc-phosphor-image-raspberrypi4.rpi-sdimg of=/dev/sdg
