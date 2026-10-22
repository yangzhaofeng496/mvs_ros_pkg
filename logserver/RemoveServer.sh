#!/bin/sh
DIRNAME=`dirname $0`
PWD=`pwd`

PIDFILE="/var/run/MvLogServer.pid"

USER_ID=`id -u`
# Check required priviledge
if [ "$USER_ID" != "0" ]; then
echo "LogServer can only be Removed by root user or sudoer"
exit 1
fi

# ARCH=$(uname -m | sed 's/x86_//;s/i[3-6]86/32/')

if [ -f /etc/debian_version ]; then
    OS=Debian  # XXX or Ubuntu??
    VER=$(cat /etc/debian_version)
elif [ -f /etc/redhat-release ]; then
    # TODO add code for Red Hat and CentOS here
    OS=Redhat  # XXX or CentOs??
    VER=$(uname -r)
elif [ -f /etc/kylin-release ]; then
    OS=Kylin
    VER=$(uname -r)
elif [ -f /etc/openEuler-release ]; then
    OS=OpenEuler
    VER=$(uname -r)
else
    # 解析os-release获取系统信息
    if [ -f /etc/os-release ]; then
        OS=$(grep "^ID=" /etc/os-release | awk -F '=' '{print $2}')
        VER=$(uname -r)
        
        # kylin进行特殊处理(不区分大小写)
        if [ echo "$OS" | grep -qi "kylin"]; then
            OS=Kylin
        fi
    else
        OS=$(uname -s)
        VER=$(uname -r)
    fi
fi

service MvLogServer stop  2> /dev/null

if [ "Debian" = "$OS" ]; then
	update-rc.d -f MvLogServer remove 1> /dev/null
elif [ "Redhat" = "$OS" ]; then
	chkconfig MvLogServer off
    chkconfig --del MvLogServer
elif [ "Kylin" = "$OS" ]; then
    # 检测工具并使用对应工具停止服务
    if command -v systemctl &> /dev/null ; then
        systemctl stop MvLogServer
        systemctl disable MvLogServer
    elif command -v service &> /dev/null; then
        service MvLogServer stop
        if [ -x /sbin/chkconfig ] || [ -x /usr/sbin/chkconfig ]; then
            # RedHat/CentOS 系统
            chkconfig MvLogServer off
            chkconfig --del MvLogServer
        elif [ -x /usr/sbin/update-rc.d ]; then
            # RedHat/CentOS 系统
            update-rc.d -f MvLogServer remove
        fi
    elif command -v rc-service &> /dev/null; then
        rc-service MvLogServer stop
        rc-update del MvLogServer default
    else
        echo "No available service manager found, please stop MvLogServer service manually"
    fi    
elif [ "OpenEuler" = "$OS" ]; then
    systemctl stop MvLogServer
    systemctl disable MvLogServer
else
	echo "Can not recognize system"
fi

rm -f /etc/init.d/MvLogServer

rm -f $PIDFILE

