#!/bin/bash
SCRIPTEXEC_PATH=$(cd `dirname $0`; pwd)
SDK_HOME=$(cd ${SCRIPTEXEC_PATH}/../; pwd)
SDK_PATH_OLD=$(cat ${SCRIPTEXEC_PATH}/ScriptServer | grep "^SCRIPT_PATH=" | awk -F '=' '{print $2}')
FIND_PATH=${SDK_PATH_OLD//\//\\\/}
FIND_PATH=${FIND_PATH//\#/\\\#}
REPLACE_PATH=${SCRIPTEXEC_PATH//\//\\\/}
USER_ID=`id -u`

# Check required priviledge
if [ "$USER_ID" != "0" ]; then
echo "ScriptServer can only be installed by root user or sudoer"
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

# 如果不是Debian或者Redhat，都走其他系统
if [ "$OS" != "Debian" ] && [ "$OS" != "Redhat" ]; then
    OS="OtherOS"
fi

# check parameter
if [ "1" = "$1" ]; then

sed -i "/^SCRIPT_PATH=*/s/${FIND_PATH}/${REPLACE_PATH}/" ${SCRIPTEXEC_PATH}/ScriptServer
cp -f ${SCRIPTEXEC_PATH}/ScriptServer /etc/init.d/

chmod 777 /etc/init.d/ScriptServer


if [ "Debian" = "$OS" ]; then
	update-rc.d -f ScriptServer defaults 1> /dev/null
elif [ "Redhat" = "$OS" ]; then
	chkconfig --add ScriptServer
	chkconfig ScriptServer on
else
    # 检测工具并使用对应工具启动服务
    if command -v systemctl &> /dev/null ; then
        systemctl enable ScriptServer
        systemctl start ScriptServer
    elif command -v service &> /dev/null; then
        if [ -x /sbin/chkconfig ] || [ -x /usr/sbin/chkconfig ]; then
            chkconfig --add ScriptServer
            chkconfig ScriptServer on
        elif [ -x /usr/sbin/update-rc.d ]; then
            update-rc.d ScriptServer defaults
        fi
        service ScriptServer start
    elif command -v rc-service &> /dev/null; then
        rc-update add ScriptServer default
        rc-service ScriptServer start
    else
        echo "No available service manager found, please start ScriptServer service manually"
    fi
fi

/etc/init.d/ScriptServer start

elif [ "0" = "$1" ]; then

if [ "Debian" = "$OS" ]; then
        update-rc.d -f ScriptServer remove 1> /dev/null
elif [ "Redhat" = "$OS" ]; then
        chkconfig ScriptServer off
    	chkconfig --del ScriptServer 
else
    # 检测工具并使用对应工具停止服务
    if command -v systemctl &> /dev/null ; then
        systemctl stop ScriptServer
        systemctl disable ScriptServer
    elif command -v service &> /dev/null; then
        service ScriptServer stop
        if [ -x /sbin/chkconfig ] || [ -x /usr/sbin/chkconfig ]; then
            chkconfig ScriptServer off
            chkconfig --del ScriptServer
        elif [ -x /usr/sbin/update-rc.d ]; then
            update-rc.d -f ScriptServer remove
        fi
    elif command -v rc-service &> /dev/null; then
        rc-service ScriptServer stop
        rc-update del ScriptServer default
    else
        echo "No available service manager found, please stop ScriptServer service manually"
    fi    
fi

if [ -f /etc/init.d/ScriptServer ]; then
    rm /etc/init.d/ScriptServer
fi

else
 	echo "wrong parameter"
fi

