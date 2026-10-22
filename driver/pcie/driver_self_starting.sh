#!/bin/bash
DRIVER_PATH=$(cd `dirname $0`; pwd)
SDK_HOME=$(cd ${DRIVER_PATH}/../../; pwd)
SDK_PATH_OLD=$(cat ${DRIVER_PATH}/FGDriverServer | grep "^SDK_HOME=" | awk -F '=' '{print $2}')
FIND_PATH=${SDK_PATH_OLD//\//\\\/}
FIND_PATH=${FIND_PATH//\#/\\\#}
REPLACE_PATH=${SDK_HOME//\//\\\/}
USER_ID=`id -u`
PLATFORM=$(uname)
KERNEL_VERSION=$(uname -r)

# Check required priviledge
if [ "$USER_ID" != "0" ]; then
echo "FGDriverServer can only be installed by root user or sudoer"
exit 1
fi

if [ -f /etc/debian_version ]; then
    OS=Debian  # XXX or Ubuntu??
    VER=$(cat /etc/debian_version)
elif [ -f /etc/redhat-release ]; then
    # TODO add code for Red Hat and CentOS here
    OS=Redhat  # XXX or CentOs??
    VER=$(uname -r)
else
    # 解析os-release获取系统信息
    if [ -f /etc/os-release ]; then
        OS=$(grep "^ID=" /etc/os-release | awk -F '=' '{print $2}')
        VER=$(uname -r)
    else
        OS=$(uname -s)
        VER=$(uname -r)
    fi
fi

ARCH=$(uname -m)
if [ ${ARCH} = "aarch64" ]; then
	if grep -q "rk3588" /proc/device-tree/compatible; then
        PLATFORM="rk3588"
	fi
fi

# 如果不是Debian或者Redhat，都走其他系统
if [ "$OS" != "Debian" ] && [ "$OS" != "Redhat" ]; then
    OS="OtherOS"
fi

# check parameter
if [ "1" = "$1" ]; then

sed -i "/^SDK_HOME=*/s/${FIND_PATH}/${REPLACE_PATH}/" ${SDK_HOME}/driver/pcie/FGDriverServer
cp -f ${DRIVER_PATH}/FGDriverServer /etc/init.d/
if [ ${PLATFORM} = "rk3588" ]; then
    mv /etc/init.d/FGDriverServer /etc/init.d/S91FGDriverServer
	chmod 777 /etc/init.d/S91FGDriverServer
else
    chmod 777 /etc/init.d/FGDriverServer
fi

# 记录当前的内核版本
echo "${KERNEL_VERSION}" > ../kernel_version

if [ "Debian" = "$OS" ]; then
	update-rc.d -f FGDriverServer defaults 1> /dev/null
elif [ "Redhat" = "$OS" ]; then
	chkconfig --add FGDriverServer
	chkconfig FGDriverServer on
else
    # 检测工具并使用对应工具启动服务
    if command -v systemctl &> /dev/null ; then
        systemctl enable FGDriverServer
        systemctl start FGDriverServer
    elif command -v service &> /dev/null; then
        if [ -x /sbin/chkconfig ] || [ -x /usr/sbin/chkconfig ]; then
            chkconfig --add FGDriverServer
            chkconfig FGDriverServer on
        elif [ -x /usr/sbin/update-rc.d ]; then
            update-rc.d FGDriverServer defaults
        fi
        service FGDriverServer start
    elif command -v rc-service &> /dev/null; then
        rc-update add FGDriverServer default
        rc-service FGDriverServer start
    else
        echo "No available service manager found, please start FGDriverServer service manually"
    fi
fi

if [ ${PLATFORM} = "rk3588" ]; then
	/etc/init.d/S91FGDriverServer start
else
	/etc/init.d/FGDriverServer start
fi

elif [ "0" = "$1" ]; then

if [ "Debian" = "$OS" ]; then
    update-rc.d -f FGDriverServer remove 1> /dev/null
elif [ "Redhat" = "$OS" ]; then
    chkconfig FGDriverServer off
    chkconfig --del FGDriverServer 
else
    # 检测工具并使用对应工具停止服务
    if command -v systemctl &> /dev/null ; then
        systemctl stop FGDriverServer
        systemctl disable FGDriverServer
    elif command -v service &> /dev/null; then
        service FGDriverServer stop
        if [ -x /sbin/chkconfig ] || [ -x /usr/sbin/chkconfig ]; then
            chkconfig FGDriverServer off
            chkconfig --del FGDriverServer
        elif [ -x /usr/sbin/update-rc.d ]; then
            update-rc.d -f FGDriverServer remove
        fi
    elif command -v rc-service &> /dev/null; then
        rc-service FGDriverServer stop
        rc-update del FGDriverServer default
    else
        echo "No available service manager found, please stop FGDriverServer service manually"
    fi    
fi

rm /etc/init.d/FGDriverServer

else
 	echo "wrong parameter"
fi

