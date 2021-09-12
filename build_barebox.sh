#!/bin/sh

CROSS="/opt/OpenWrt-Toolchain-comcerto2000-for-arm_v7-a-gcc-4.5-linaro_glibc-2.14.1_eabi/bin/arm-openwrt-linux-"
CONFIG_FILE="config_bareboxc2kevm"
STAGING_DIR="staging_dir"
BUILD_TYPE="${1}"
PROG_NAME="`basename ${0}`"

usage()
{
	echo "Usage: [MODEL=STG-XXX] ${PROG_NAME} [degrade_uloader/uloader/nand_uloader]"
	echo "Ex."
	echo "	for STG-540:"
	echo "		> MODEL=STG-540 ./${PROG_NAME}"
	echo "	for STG-520:"
	echo "		> MODEL=STG-520 ./${PROG_NAME}"
	echo "Note:"
	echo "	If \"MODEL\" is not set, use \"STG-540\" as the default value."
}

if [ "${BUILD_TYPE}" = "-h" ] || [ "${BUILD_TYPE}" = "--help" ]; then
	usage
	exit 0
fi

if [ "${MODEL}" = "" ]; then
	echo "*** MODEL is not set, use STG-540 as the default value. ***"
	MODEL="STG-540"
fi

echo "MODEL: ${MODEL}"
if [ "${BUILD_TYPE}" = "uloader" ]; then
	CONFIG_FILE="config_m86203uloader"
elif [ "${BUILD_TYPE}" = "nand_uloader" ]; then
	CONFIG_FILE="config_nandm86203uloader"
elif [ "${BUILD_TYPE}" = "degrade_uloader" ]; then
	CONFIG_FILE="config_m86261uloader"
else
	if [ "${BUILD_TYPE}" = "" ]; then
		BUILD_TYPE="barebox"
	else
		usage
		exit 1
	fi
fi


if [ ! -d ${STAGING_DIR} ]; then
	echo "creating ${STAGING_DIR} ..."
	mkdir ${STAGING_DIR}
fi
export STAGING_DIR="${STAGING_DIR}"

echo "copying ${CONFIG_FILE} to .config ..."
cp ${CONFIG_FILE} .config

echo "change model id for ${MODEL} ..."
rm tmp_config
if [ "${MODEL}" = "STG-540" ]; then
	cat ./arch/arm/boards/comcerto-evm/env/config | sed -e 's/^MODEL_ID=.*/MODEL_ID=B103/g' | sed -e 's/^# MODEL ID for .*/# MODEL ID for STG-540/g' > tmp_config
elif [ "${MODEL}" = "STG-520" ]; then
	cat ./arch/arm/boards/comcerto-evm/env/config | sed -e 's/^MODEL_ID=.*/MODEL_ID=B203/g' | sed -e 's/^# MODEL ID for .*/# MODEL ID for STG-520/g' > tmp_config
else	# default: STG-540
	cat ./arch/arm/boards/comcerto-evm/env/config | sed -e 's/^MODEL_ID=.*/MODEL_ID=B103/g' | sed -e 's/^# MODEL ID for .*/# MODEL ID for STG-540/g' > tmp_config
fi
cp tmp_config ./arch/arm/boards/comcerto-evm/env/config	

echo "start to build ${BUILD_TYPE} ..."
make CROSS_COMPILE=${CROSS}
